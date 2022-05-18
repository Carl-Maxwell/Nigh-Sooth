#include "lapse_lib.h"

#include "platform_olc_pixel.h"

#include "ephrais_session.h"

using namespace lapse;

int main() {
  //
  // Allocate memory
  //

  arenas::temp.reserve(10'000'000);
  platform::allocate_image_cache();

  //
  // Set up main loop callback
  //

  platform::set_main_loop_callback(&ephrais::Session::main_loop);

  //
  // Start the game
  //

  ephrais::Session::the().advance_phase();

  return 0;
}

