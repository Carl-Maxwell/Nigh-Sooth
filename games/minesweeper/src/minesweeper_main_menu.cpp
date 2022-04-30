
#include "minesweeper_main_menu.h"

#include "platform_olc_pixel.h"

#include "lapse_lib.h"

#include "platform_olc_pixel.h"
#include "minesweeper_game_session.h"

#include "mui/button.h"

using namespace lapse;

namespace minesweeper{

void minesweeper_main_menu::start_main_loop() {
  platform::initialize(800, 600, false, "Nigh Sooth - Minesweeper Game - Main Menu");
  platform::start_application();
}

void minesweeper_main_menu::main_loop(f32 delta) {
  for (i32 i = 0; i < 10'000; i++) {
    platform::plot(rand_vec2() * platform::get_window_size(), rand_vec3());
  }

  auto& session = minesweeper_session::the();

  // if (session.run) {
  //   if (mui::button("continue game")) {
  //     session.m_state = session_state::game_run_main_loop;
  //   }
  // }

  if (mui::button("18x12")) {
    session.next_grid_size = new vec2<i32>{18, 12};
    session.m_state = session_state::game_run_startup;
    platform::close_application();
  }

  if (mui::button("27x16")) {
    session.next_grid_size = new vec2<i32>{27, 16};
    session.m_state = session_state::game_run_startup;
    platform::close_application();
  }

  if (mui::button("Exit Minesweeper")) {
    session.m_state = session_state::application_shutdown;
    platform::close_application();
  }
}

}
