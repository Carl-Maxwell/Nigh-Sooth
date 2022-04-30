#include "lapse_lib.h"

#include "minesweeper_game_session.h"
#include "minesweeper_game_run.h"

using namespace lapse;

namespace minesweeper{

void minesweeper_run::start_main_loop() {
  platform::initialize(next_window_size.x, next_window_size.y, false, "Nigh Sooth - Minesweeper Game");
  platform::start_application();
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

  // generate mines
  for (u32 n = 0; n < 18; n++) {
    auto x = (i32)rand_integer(grid_width);
    auto y = (i32)rand_integer(grid_height);

    // if it's already mined, find a fresh spot
    while (grid[y*grid_width + x].m_mined) {
      x = (i32)rand_integer(grid_width);
      y = (i32)rand_integer(grid_height);
    }

    grid[y*grid_width + x].m_mined = true;

    // update adjacent mine count in adjacent squares
    for (auto y2 = max(y-1, 0); y2 < min(y+2, grid_height); y2++) {
      for (auto x2 = max(x-1, 0); x2 < min(x+2, grid_width); x2++) {
        if (x2 == x && y2 == y) { continue; }
        grid[y2*grid_width + x2].m_adjacent_mines++;
        assert(grid[y2*grid_width + x2].m_adjacent_mines <= 5);
      }
    }
  }
}

} // end namespace