#include "minesweeper_main_menu.h"

#include "platform_olc_pixel.h"

#include <lapse_lib.h>

#include "platform_olc_pixel.h"
#include "minesweeper_game_session.h"

#include "mui/mui_div.h"
#include "mui/button.h"
#include "mui/mui_draw.h"

using namespace lapse;

namespace minesweeper{

void minesweeper_main_menu::start_main_loop() {
  platform::initialize(800, 600, false, "Nigh Sooth - Minesweeper Game - Main Menu");
  platform::start_application();
}

void minesweeper_main_menu::main_loop(f32 delta) {
  // std::cout << "main menu start of frame " << frame_count << "\n";

  // for (i32 i = 0; i < 10'000; i++) {
    // platform::plot(rand_vec2() * platform::get_window_size(), rand_vec3());
  // }

  switch (current_menu) {
    case mui::page::main_menu: main_menu(); break;
    case mui::page::new_game_menu: new_game_menu(); break;
  }

  mui::draw();

  // std::cout << "main menu end of frame " << frame_count << "\n";
  frame_count++;
}

void minesweeper_main_menu::main_menu() {
  auto& session = minesweeper_session::the();

  {
    mui::params div_params;

    div_params.border = {1};
    div_params.padding = {10};
    div_params.background_color = {0, 0, 255};

    mui::open_div(div_params);
  }

  if (session.run) {
    if (mui::button("continue game")) {
      session.m_state = session_state::game_run_main_loop;
    }
  }

  if (mui::button("New Game")) {
    current_menu = mui::page::new_game_menu;
  }

  if (mui::button("Exit Minesweeper")) {
    session.m_state = session_state::application_shutdown;
    platform::close_application();
  }

  mui::close_div();
}

void minesweeper_main_menu::new_game_menu() {
  auto& session = minesweeper_session::the();

  if (mui::button("normal game")) {
    session.next_grid_size = new vec2<i32>{18, 12};
    session.m_state = session_state::game_run_startup;
    platform::close_application();
    current_menu = mui::page::main_menu;
  }

  if (mui::button("hard game")) {
    session.next_grid_size = new vec2<i32>{27, 16};
    session.m_state = session_state::game_run_startup;
    platform::close_application();
    current_menu = mui::page::main_menu;
  }

  if (mui::button("huge game")) {
    session.next_grid_size = new vec2<i32>{i32(1920*0.8f), i32(1080*0.8f)};
    *session.next_grid_size /= session.run->grid_size;
    session.m_state = session_state::game_run_startup;
    platform::close_application();
    current_menu = mui::page::main_menu;
  }

  if (mui::button("back")) {
    current_menu = mui::page::main_menu;
  }
}

}
