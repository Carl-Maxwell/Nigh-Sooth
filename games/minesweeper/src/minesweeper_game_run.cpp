#include "lapse_lib.h"

#include "minesweeper_game_session.h"
#include "minesweeper_game_run.h"
#include "sooth_input.h"

using namespace lapse;

namespace minesweeper{

void minesweeper_run::start_main_loop() {
  platform::initialize(next_window_size.x, next_window_size.y, false, "Nigh Sooth - Minesweeper Game");
  platform::start_application();
}

void minesweeper_run::main_loop(f32 delta) {
  platform::clear(vec3<>{0, 0, 0});

  lapse::LapseErrorQueue::the().tick();

  i32 mouse_x = (i32)Mouse::get_mouse_pos().x - window_padding;
  i32 mouse_y = (i32)Mouse::get_mouse_pos().y - window_padding;
  mouse_x /= grid_size;
  mouse_y /= grid_size;
  minesweeper::tile_obj* mouse_tile = nullptr;

  if (mouse_x >= 0 && mouse_y >= 0 && mouse_x < grid_width && mouse_y < grid_height) {
    mouse_tile = &grid[mouse_y * grid_width + mouse_x];
  }

  if (Mouse::left_mouse_hit()) {
    if (mouse_tile && mouse_tile->m_hidden && !mouse_tile->m_flagged) {
      if (m_first_click) {
        m_first_click = false;
        generate_safe_spaces(mouse_tile);
        generate_mines();
        for (i32 i = 0; i < safe_spaces.length(); i++) {
          auto& safe_tile = grid[safe_spaces[i].y * grid_width + safe_spaces[i].x];
          safe_tile.reveal();
        }
      } else {
        mouse_tile->reveal();
      }
    }
  } else if (Mouse::right_mouse_hit()) {
    if (mouse_tile && mouse_tile->m_hidden) {
      if (!mouse_tile->m_flagged) {
        mouse_tile->m_tile_state = minesweeper::grid_tile::flagged;
        mouse_tile->m_flagged = true;
      } else {
        mouse_tile->m_tile_state = minesweeper::grid_tile::hidden;
        mouse_tile->m_flagged = false;
      }
    }
  }

  for (f32 y = 0; y < grid_height; y++) {
    for (f32 x = 0; x < grid_width; x++) {
      vec2<> screen_pos{
        x*grid_size + window_padding,
        y*grid_size + window_padding
      };
      auto& tile = grid[u32(y*grid_width + x)];
      bool hovering = mouse_tile && mouse_tile == &tile;
      image img = tile.get_image(hovering);

      platform::draw_bitmap(screen_pos, img);
    }
  }

  if (key(keycode::escape).is_hit()) {
    auto& session = minesweeper_session::the();
    session.m_state = session_state::main_menu;
  }
}

// called each time we need to setup a new map to play
void minesweeper_run::initialize_run(i32 n_width, i32 n_height) {
  std::cout << "intiailizing run ... \n";
  grid_width  = n_width;
  grid_height = n_height;

  next_window_size = {
    grid_size*grid_width  + window_padding*2, // window width
    grid_size*grid_height + window_padding*2  // window height
  };

  grid = new minesweeper::tile_obj[grid_width * grid_height];

  for (i32 y = 0; y < grid_height; y++) {
    for (i32 x = 0; x < grid_width; x++) {
      grid[y*grid_width + x].reset();
      grid[y*grid_width + x].m_coordinates = {x, y};
      // grid[y*grid_width + x].grid = grid; // TODO stop copying this pointer around
    }
  }
}

//
void minesweeper_run::generate_safe_spaces(tile_obj* start_tile) {
  safe_spaces.reserve(9 + 6);
  safe_spaces.push(start_tile->m_coordinates);

  // add adjacent tiles
  array<tile_obj>* adjacents = start_tile->adjacent_tiles();
  for (i32 i = 0; i < adjacents->length(); i++) {
    safe_spaces.push((*adjacents)[i].m_coordinates);
  }
  
  // TODO: add 1d6 adjacent tiles into the safe area
  //   finding tiles that are adjacent to the 3x3 safe area 

  // find the "frontier" -- the tiles adjacent to the 3x3 safe area
  // array<tile_obj>* frontier = new array<tile_obj>(3*3*8);
  // for (i32 i = 0; i < adjacents->length(); i++) {
  //   frontier->push((*adjacents)[i]);
  //   array<tile_obj>* sub_adjacents = (*adjacents)[i].adjacent_tiles();
  //   *frontier += *sub_adjacents; // push each elem onto frontier
  //   delete sub_adjacents;
  // }
  // frontier->do_sort();
  // frontier->do_unique(true);
  // TODO foreach over frontier and remove safe spaces from it
  // TODO then the frontier is finally calculated.
  //   So, sample 1d6 elems from it and add them to safe_spaces

  // simpler approach: just loop from x-1, y-1, to x+1, y+1 from the safe area's top left to bottom left...
  //   then just skip if the coord is inside the 3x3 area.
  // That gives us the starting frontier, so then just sample it...
  //   but then you have to grow the frontier to tiles adjacent to that new space.

  // delete frontier;
  delete adjacents;
}

// place mines on the map
void minesweeper_run::generate_mines(i32 num_mines) {
  if (!num_mines) {
    num_mines = (grid_width * grid_height)/4;
  }

  for (u32 n = 0; n < num_mines; n++) {
    i32 x;
    i32 y;
    bool bad_spot = true;

    // if it's already mined, find a fresh spot
    while (bad_spot) {
      x = (i32)rand_integer(grid_width);
      y = (i32)rand_integer(grid_height);

      bad_spot = grid[y*grid_width + x].m_mined;
      bad_spot = bad_spot || grid[y*grid_width + x].calculate_adjacent_mines() > 5;
      bad_spot = bad_spot || safe_spaces.contains(vec2<i32>{x,y});
    }

    auto& tile = grid[y*grid_width + x];
    tile.m_mined = true;
    tile.m_adjacent_mines = tile.calculate_adjacent_mines();
  }

  for (i32 y = 0; y < grid_height; y++) {
    for (i32 x = 0; x < grid_width; x++) {
      auto& tile = grid[y*grid_width + x];
      tile.m_adjacent_mines = tile.calculate_adjacent_mines();
    }
  }
}

} // end namespace