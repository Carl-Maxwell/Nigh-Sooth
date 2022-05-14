#include "lapse_lib.h"

#include "sooth_input.h"

#include "platform_olc_pixel.h"

#include <sooth_image.h>

#include "minesweeper_game_session.h"
#include "minesweeper_game_run.h"

#include "minesweeper_tile.h"

using namespace lapse;

int main() {
  arenas::temp.reserve(10'000);
  platform::allocate_image_cache();

  platform::set_main_loop_callback([](f32 delta){
    auto& session = minesweeper::minesweeper_session::the();
    session.main_loop(delta);
  } );

  //
  // Start the game
  //

  auto& session = minesweeper::minesweeper_session::the();
  session.advance_phase();

  return 0;
}

