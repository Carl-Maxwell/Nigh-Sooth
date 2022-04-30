
#include "minesweeper_main_menu.h"

#include "platform_olc_pixel.h"

#include "lapse_lib.h"

#include "platform_olc_pixel.h"

using namespace lapse;

namespace minesweeper{

void minesweeper_main_menu::main_loop(f32 delta) {
  for (i32 i = 0; i < 10'000; i++) {
    platform::plot(rand_vec2() * platform::get_window_size(), rand_vec3());
  }
}

}
