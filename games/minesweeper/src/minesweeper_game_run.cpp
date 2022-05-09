#include "lapse_lib.h"

#include "minesweeper_game_session.h"
#include "minesweeper_game_run.h"
#include "sooth_input.h"

using namespace lapse;

namespace minesweeper{

void minesweeper_run::start_main_loop() {
  auto pixel_size = 4;

  // TODO fetch the actual monitor resolution from platform

  vec2<i32> monitor_resolution = {1920, 1080};
  monitor_resolution *= 0.8;
  monitor_resolution = monitor_resolution / next_window_size;
  pixel_size = min(monitor_resolution.x, monitor_resolution.y);

  std::cout << "pixel size: " << pixel_size;

  pixel_size = max(1, pixel_size);

  static str str_application_name = "Nigh Sooth - Minesweeper Game";

  platform::initialize(next_window_size.x, next_window_size.y, false, str_application_name, pixel_size);
  platform::start_application();
}

// main loop of gameplay
void minesweeper_run::main_loop(f32 delta) {
  auto& session = minesweeper_session::the();
  platform::clear(vec3<>{0, 0, 0});

  platform::poll_key_toggles();

  lapse::LapseErrorQueue::the().tick();

  i32 mouse_x = (i32)Mouse::get_mouse_pos().x - window_padding;
  i32 mouse_y = (i32)Mouse::get_mouse_pos().y - window_padding;
  mouse_x /= grid_size;
  mouse_y /= grid_size;
  minesweeper::tile_obj* mouse_tile = nullptr;

  if (
       mouse_x >= 0 && mouse_y >= 0
    && mouse_x < grid_width && mouse_y < grid_height
    && Mouse::get_mouse_pos() > (f32)window_padding
  ) {
    mouse_tile = &grid[mouse_y * grid_width + mouse_x];
  }

  switch (game_state){
  case game_state_enum::in_progress:
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
      if (is_game_won()) { game_state = game_state_enum::won; }
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
      if (is_game_won()) { game_state = game_state_enum::won; }
    }
  break;
  case game_state_enum::won:
  case game_state_enum::lost:
    if (Mouse::left_mouse_hit()) {
      session.restart_run();
    }
  break;
  }

  for (f32 y = 0; y < grid_height; y++) {
    for (f32 x = 0; x < grid_width; x++) {
      vec2<> screen_pos{
        x*grid_size + window_padding,
        y*grid_size + window_padding
      };

      auto& tile = grid[u32(y*grid_width + x)];
      bool hovering = mouse_tile && mouse_tile == &tile && game_state == game_state_enum::in_progress;
      image& img = tile.get_image(game_state, hovering);

      platform::draw_bitmap(screen_pos, img);
    }
  }

  if (game_state == game_state_enum::won) {
    auto center = platform::get_window_size()/2.0f - session.victory_image.m_resolution/2.0f;
    platform::draw_bitmap(center, session.victory_image);
  }

  // open main menu if esc is hit
  if (key(keycode::escape).is_hit()) {
    session.m_state = session_state::main_menu;
  }

  // debugging stuff:

  if (key(keycode::number_1).is_toggled()) {
    for (i32 i = 0; i < safe_spaces.length(); i++) {
      platform::draw_rect(
        vec2<>{f32(safe_spaces[i].x), f32(safe_spaces[i].y)} * grid_size_vec2() + (f32)window_padding,
        grid_size_vec2(),
        vec3<>{0, 0.75, 0}
      );
    }
  }

  if (key(keycode::number_2).is_toggled()) {
    // TODO reveal all tiles
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
    }
  }
}

bool minesweeper_run::is_game_won() {
  for (i32 y = 0; y < grid_height; y++) {
    for (i32 x = 0; x < grid_width; x++) {
      if (grid[y*grid_width + x].m_mined && !grid[y*grid_width + x].m_flagged) {
        return false;
      } else if (grid[y*grid_width + x].m_flagged && !grid[y*grid_width + x].m_mined) {
        return false;
      } else if (grid[y*grid_width + x].m_hidden && !grid[y*grid_width + x].m_flagged) {
        return false;
      }
    }
  }
  std::cout << "\n\n//\n// victory is yours!\n//\n\n";
  return true;
}

void minesweeper_run::win_game() {}

void minesweeper_run::lose_game() {
  auto& session = minesweeper_session::the();

  // TODO start animation
  game_state = game_state_enum::lost;
}

//
void minesweeper_run::generate_safe_spaces(tile_obj* start_tile) {
  safe_spaces.reserve(9 + 6);
  safe_spaces.push(start_tile->m_coordinates);

  // grid space rectangular coords of safe area
  rect<i32> safe_3x3{
    start_tile->m_coordinates - vec2<i32>{1, 1},
    {3, 3}
  };

  //
  // keep the safe_3x3 within the game board
  //

  // if position is off the edge of the game board, snap it back on, and reduce size accordingly
  if (safe_3x3.top_left_point().x < 0) {
    safe_3x3.position.x += 1;
    safe_3x3.size.x -= 1;
  }
  if (safe_3x3.top_left_point().y < 0) {
    safe_3x3.position.y += 1;
    safe_3x3.size.y -= 1;
  }

  // if size would push it past the edge of the game board, snap it back
  if (safe_3x3.bottom_right_point().x > grid_width) {
    safe_3x3.size.x -= 1;
  }
  if (safe_3x3.bottom_right_point().y > grid_height) {
    safe_3x3.size.y -= 1;
  }

  {
    // add adjacent tiles
    array<tile_obj>* adjacents = start_tile->adjacent_tiles();
    for (i32 i = 0; i < adjacents->length(); i++) {
      safe_spaces.push((*adjacents)[i].m_coordinates);
    }
    delete adjacents;
  }

  // find the "frontier" -- the tiles adjacent to the 3x3 safe area
  array<tile_obj>* frontier = new array<tile_obj>(5*5 - 3*3 + 1);

  auto x    = safe_3x3.top_left_point().x;
  auto y    = safe_3x3.top_left_point().y;
  auto br_x = safe_3x3.bottom_right_point().x;
  auto br_y = safe_3x3.bottom_right_point().y;

  for (auto y2 = max(y-1, 0); y2 < min(br_y+1, grid_height); y2++) {
    for (auto x2 = max(x-1, 0); x2 < min(br_x+1, grid_width); x2++) {
      if (x2 >= x && y2 >= y && x2 < br_x && y2 < br_y) { continue; }
      frontier->push(grid[y2*grid_width + x2]);
    }
  }

  // TODO should this be called "frontier" ? Could call it boundary, bounds, outer_edge,
  //   border, borderline, hedge, fence, limit, partition, rim, verge, marches,

  // At this point the frontier is like this:
  // 1 1 1 1 1
  // 1 0 0 0 1
  // 1 0 0 0 1
  // 1 0 0 0 1
  // 1 1 1 1 1
  // Where the 0s are the 3x3 of safe spaces

  // starting frontier is calculated,
  //   now add 1d6 random tiles to safe spaces from the frontier
  //   expanding the frontier each time to include the new adjacent tiles
  //   where those new tiles are not already safe or frontier

  auto extra_spaces = die(1, 6);

  for (auto i = 0; i < extra_spaces; i++) {
    auto& tile = frontier->sample();
    frontier->remove(tile);
    safe_spaces.push(tile.m_coordinates);
    auto* adjacents = tile.adjacent_tiles_cardinal();
    for (auto i = 0; i < adjacents->length(); i++) {
      auto& adj_tile = (*adjacents)[i]; 
      if (safe_spaces.contains(adj_tile.m_coordinates)) { continue; }
      if (frontier->contains(adj_tile)) { continue; }
      frontier->push(adj_tile);
    }
  }

  delete frontier;
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
      bad_spot = bad_spot || grid[y*grid_width + x].calculate_adjacent_mines() >= 5;
      bad_spot = bad_spot || safe_spaces.contains(vec2<i32>{x,y});

      auto* adjacents = grid[y*grid_width + x].adjacent_tiles();
      for (i32 i = 0; i < adjacents->length(); i++) {
        bad_spot = bad_spot || (*adjacents)[i].calculate_adjacent_mines() >= 5;
      }
      delete adjacents;
    }

    auto& tile = grid[y*grid_width + x];
    tile.m_mined = true;
    tile.m_adjacent_mines = tile.calculate_adjacent_mines();
  }

  for (i32 y = 0; y < grid_height; y++) {
    for (i32 x = 0; x < grid_width; x++) {
      auto& tile = grid[y*grid_width + x];
      tile.m_adjacent_mines = tile.calculate_adjacent_mines();
      assert(tile.m_adjacent_mines <= 5);
    }
  }
}

} // end namespace
