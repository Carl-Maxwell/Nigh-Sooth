#include "lapse_lib.h"

#include "minesweeper_game_session.h"
#include "minesweeper_game_run.h"
#include "sooth_input.h"

using namespace lapse;

namespace minesweeper{

bool minesweeper_run::start_main_loop() {
  auto pixel_size = 4;

  // TODO fetch the actual monitor resolution from platform

  vec2<i32> monitor_resolution = {1920, 1080};
  monitor_resolution *= 0.8f;
  monitor_resolution = monitor_resolution / next_window_size;
  pixel_size = min(monitor_resolution.x, monitor_resolution.y);

  std::cout << "pixel size: " << pixel_size;

  pixel_size = max(1, pixel_size);

  static str str_application_name = "Nigh Sooth - Minesweeper Game";

  platform::initialize(next_window_size.x, next_window_size.y, false, str_application_name, pixel_size);
  return platform::start_application();
}

// main loop of gameplay
void minesweeper_run::main_loop(f32 delta) {
  auto& session = minesweeper_session::the();
  auto& run = *session.run;
  platform::clear(vec3<>{0, 0, 0});

  auto& game_zoom      = run.game_zoom;
  auto& game_state     = run.game_state;
  auto  grid           = run.grid;
  auto& tile_size      = run.tile_size;
  auto& grid_width     = run.grid_width;
  auto& grid_height    = run.grid_height;
  auto& panning_offset = run.panning_offset;
  auto& m_first_click  = run.m_first_click;
  auto& safe_spaces    = run.safe_spaces;

  platform::poll_key_toggles();

  lapse::LapseErrorQueue::the().tick();

  i32 mouse_x = static_cast<i32>(Mouse::get_mouse_pos().x);
  i32 mouse_y = static_cast<i32>(Mouse::get_mouse_pos().y);

  mouse_x = static_cast<i32>(lapse::round(f32(mouse_x)/game_zoom - panning_offset.x));
  mouse_y = static_cast<i32>(lapse::round(f32(mouse_y)/game_zoom - panning_offset.y));

  mouse_x /= tile_size;
  mouse_y /= tile_size;
  minesweeper::tile_obj* mouse_tile = nullptr;

  if (
       mouse_x >= 0 && mouse_y >= 0
    && mouse_x < grid_width && mouse_y < grid_height
  ) {
    mouse_tile = &grid[mouse_y * grid_width + mouse_x];
  }

  switch (game_state){
  case game_state_enum::in_progress:
    if (Mouse::left_mouse_hit()) {
      if (mouse_tile && mouse_tile->m_hidden && !mouse_tile->m_flagged) {
        if (m_first_click) {
          m_first_click = false;
          run.generate_safe_spaces(mouse_tile);
          run.generate_mines();
          for (u32 i = 0; i < safe_spaces.length(); i++) {
            auto& safe_tile = grid[safe_spaces[i].y * grid_width + safe_spaces[i].x];
            safe_tile.reveal();
          }
        } else {
          mouse_tile->reveal();
        }
      }
      if (run.is_game_won()) { game_state = game_state_enum::won; }
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
      if (run.is_game_won()) { game_state = game_state_enum::won; }
    }
  break;
  case game_state_enum::won:
  case game_state_enum::lost:
    if (Mouse::left_mouse_hit()) {
      session.restart_run();
    }
  break;
  }

  //
  // Draw tiles
  //

  {
    auto window_width  = static_cast<f32>(platform::get_window_width() );
    auto window_height = static_cast<f32>(platform::get_window_height());

    auto current_grid_width = window_width   / (f32(tile_size)*game_zoom) + 1.0f;
    auto current_grid_height = window_height / (f32(tile_size)*game_zoom) + 1.0f;

    // calc current topleft tile
    auto grid_topleft_x = lapse::floor_f(-panning_offset.x/f32(tile_size));
    auto grid_topleft_y = lapse::floor_f(-panning_offset.y/f32(tile_size));

    grid_topleft_x = max(0.0f, grid_topleft_x);
    grid_topleft_y = max(0.0f, grid_topleft_y);

    if (key(keycode::number_2).is_hit()) {
      __debugbreak();
    }

    current_grid_width  += grid_topleft_x;
    current_grid_height += grid_topleft_y;

    current_grid_width  = min(f32(grid_width), current_grid_width);
    current_grid_height = min(f32(grid_height), current_grid_height);

    auto tile_count = 0;

    for (f32 y = grid_topleft_y; y < current_grid_height; y++) {
      for (f32 x = grid_topleft_x; x < current_grid_width; x++) {
        tile_count++;
        vec2<> screen_pos{
          x*tile_size,
          y*tile_size
        };

        auto& tile = grid[u32(y*grid_width + x)];
        bool hovering = mouse_tile && mouse_tile == &tile && game_state == game_state_enum::in_progress;
        image& img = tile.get_image(game_state, hovering);

        screen_pos += panning_offset;
        // TODO multiply screen_pos here, not in draw_bitmap
        platform::draw_bitmap_scaled(screen_pos, img, game_zoom);
      }
    }
  }

  if (game_state == game_state_enum::won) {
    auto center = platform::get_window_size()/2.0f - session.victory_image.m_resolution/2.0f;
    platform::draw_bitmap(center, session.victory_image);
  }

  //
  // Player input
  //

  // open main menu if esc is hit
  if (key(keycode::escape).is_hit()) {
    session.m_should_end_run = false;
    session.change_phase(session_state::main_menu);
  }

  // debugging stuff:

  if (key(keycode::number_1).is_toggled()) {
    for (u32 i = 0; i < safe_spaces.length(); i++) {
      platform::draw_rect(
        vec2<>{f32(safe_spaces[i].x), f32(safe_spaces[i].y)} * run.tile_size_vec2(),
        run.tile_size_vec2(),
        vec3<>{0, 0.75, 0}
      );
    }
  }

  //
  // zoom & pan
  //

  if (platform::get_mouse_wheel_delta()) {
    auto old_zoom = game_zoom;

    if (game_zoom >= 1.0f) {
      game_zoom += platform::get_mouse_wheel_delta() > 0 ? 0.1f : -0.1f;
    } else {
      if (game_zoom > 0.01f) {
        game_zoom *= platform::get_mouse_wheel_delta() > 0 ? 1.1f : 0.9f;
      } else {
        game_zoom  = platform::get_mouse_wheel_delta() > 0 ? 0.02f : 0.01f;
      }
    }

    // game_zoom vs old_zoom tells us how much of a change we're dealing with,
    //   adding half the difference of them has us zooming in on screen center
    auto new_screen_size = platform::get_window_size();
    auto old_screen_size = platform::get_window_size();
    new_screen_size /= game_zoom;
    old_screen_size /= old_zoom;
    panning_offset.x -= lapse::round((old_screen_size-new_screen_size).x/2);
    panning_offset.y -= lapse::round((old_screen_size-new_screen_size).y/2);

    // zooming_in: -1 if zooming out and +1 if zooming in
    f32 zooming_in = sign(platform::get_mouse_wheel_delta());

    // offset towards the mouse position so it zooms in on wherever the mouse is hovering
    auto mouse_offset = (platform::get_window_size()/2) - Mouse::get_mouse_pos();
    panning_offset += mouse_offset * (game_zoom-old_zoom) / game_zoom;
    // have to multiply it by the magnitude of the change so it doesn't jitter all over the place
  }

  if (key(keycode::d).is_down()) {
    panning_offset.x -= 12.0f * delta;
  }
  if (key(keycode::s).is_down()) {
    panning_offset.y -= 12.0f * delta;
  }
  if (key(keycode::a).is_down()) {
    panning_offset.x += 12.0f * delta;
  }
  if (key(keycode::w).is_down()) {
    panning_offset.y += 12.0f * delta;
  }

  // panning the view by moving the mouse
  if (key(keycode::space_bar).is_down() || Mouse::middle_mouse_down()) {
    panning_offset -= Mouse::get_mouse_delta()/game_zoom;
  }

  // empty out temp arena
  arenas::temp.clear();
}

// called each time we need to setup a new map to play
void minesweeper_run::initialize_run(i32 n_width, i32 n_height) {
  std::cout << "intiailizing run ... \n";
  grid_width  = n_width;
  grid_height = n_height;

  next_window_size = {
    tile_size*grid_width  + window_padding*2, // window width
    tile_size*grid_height + window_padding*2  // window height
  };

  // initialize grid
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
    array<tile_obj> adjacents(8);
    adjacents = start_tile->adjacent_tiles(adjacents);
    for (u32 i = 0; i < adjacents.length(); i++) {
      safe_spaces.push(adjacents[i].m_coordinates);
    }
  }

  // find the "frontier" -- the tiles adjacent to the 3x3 safe area
  array<tile_obj> frontier(5*5 - 3*3 + 1);

  auto x    = safe_3x3.top_left_point().x;
  auto y    = safe_3x3.top_left_point().y;
  auto br_x = safe_3x3.bottom_right_point().x;
  auto br_y = safe_3x3.bottom_right_point().y;

  for (auto y2 = max(y-1, 0); y2 < min(br_y+1, grid_height); y2++) {
    for (auto x2 = max(x-1, 0); x2 < min(br_x+1, grid_width); x2++) {
      if (x2 >= x && y2 >= y && x2 < br_x && y2 < br_y) { continue; }
      frontier.push(grid[y2*grid_width + x2]);
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

  {
    array<tile_obj> adjacents(8);
    for (u32 i = 0; i < extra_spaces; i++) {
      auto& tile = frontier.sample();
      frontier.remove(tile);
      safe_spaces.push(tile.m_coordinates);
      adjacents = tile.adjacent_tiles_cardinal(adjacents);
      for (u32 i2 = 0; i2 < adjacents.length(); i2++) {
        auto& adj_tile = adjacents[i2];
        if (safe_spaces.contains(adj_tile.m_coordinates)) { continue; }
        if (frontier.contains(adj_tile)) { continue; }
        frontier.push(adj_tile);
      }
      adjacents.clear_no_dealloc();
    }
  }
}

// place mines on the map
void minesweeper_run::generate_mines(i32 num_mines) {
  if (!num_mines) {
    num_mines = (grid_width * grid_height)/4;
  }

  {
    array<tile_obj> adjacents(8);
    i32 x;
    i32 y;
    bool bad_spot = true;
    
    for (i32 n = 0; n < num_mines; n++) {
      bad_spot = true;

      // if it's already mined, find a fresh spot
      while (bad_spot) {
        x = (i32)rand_integer(grid_width);
        y = (i32)rand_integer(grid_height);

        bad_spot = grid[y*grid_width + x].m_mined;
        bad_spot = bad_spot || grid[y*grid_width + x].calculate_adjacent_mines() >= 5;
        bad_spot = bad_spot || safe_spaces.contains(vec2<i32>{x,y});

        adjacents = grid[y*grid_width + x].adjacent_tiles(adjacents);
        for (u32 i = 0; i < adjacents.length(); i++) {
          bad_spot = bad_spot || adjacents[i].calculate_adjacent_mines() >= 5;
        }
        adjacents.clear_no_dealloc();
      }

      auto& tile = grid[y*grid_width + x];
      tile.m_mined = true;
      tile.m_adjacent_mines = tile.calculate_adjacent_mines();
    }
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
