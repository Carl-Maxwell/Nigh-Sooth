#include "lapse_lib.h"

#include "sooth_input.h"

#include "platform_olc_pixel.h"

#include <sooth_image.h>

#include "minesweeper_game_session.h"
#include "minesweeper_game_run.h"

#include "minesweeper_tile.h"

using namespace lapse;

int main() {
  //
  // Allocate memory
  //

  arenas::temp.reserve(3'000);
  platform::allocate_image_cache();

  //
  // Set up main loop callback
  //

  platform::set_main_loop_callback(&minesweeper::minesweeper_session::main_loop);

  //
  // Start the game
  //

  minesweeper::minesweeper_session::the().advance_phase();

  return 0;
}

