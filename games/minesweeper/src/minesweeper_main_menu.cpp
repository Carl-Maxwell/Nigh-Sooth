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

// wrapper for mui::open_div() that adds this game's style to it
void open_menu_content() {
  mui::params div_params;

  div_params.border = {1};
  div_params.padding = {10};
  div_params.background_color = {0.3607843137254902f, 0.4470588235294118, 0.792156862745098f};

  mui::open_div(div_params);
}

// wrapper func for mui::button() that adds this game's style to it
bool menu_button(str& label, mui::params args = {}) {
  // args.font_color = minesweeper_font_color;

  args.background_color = {0.7254901960784314, 0.8274509803921569, 0.8666666666666667};
  return mui::button(label, args);
}

void minesweeper_main_menu::main_menu() {
  auto& session = minesweeper_session::the();

  open_menu_content();

  if (session.run) {
    if (menu_button(str_continue_game)) {
      session.m_state = session_state::game_run_main_loop;
    }
  }

  if (menu_button(str_new_game)) {
    current_menu = mui::page::new_game_menu;
  }

  if (menu_button(str_test_menu)) {
    //current_menu = mui::page::test_menu;
  }

  if (menu_button(str_exit)) {
    session.m_state = session_state::application_shutdown;
    platform::close_application();
  }

  mui::close_div();
}

str str_quick_game  = "quick game";
str str_normal_game = "normal game";
str str_hard_game   = "hard game";
str str_huge_game   = "huge game";
str str_back        = "back";

void minesweeper_main_menu::new_game_menu() {
  auto& session = minesweeper_session::the();

  open_menu_content();

  if (menu_button(str_quick_game)) {
    session.next_grid_size = new vec2<i32>{10, 10};
    session.m_state = session_state::game_run_startup;
    platform::close_application();
    current_menu = mui::page::main_menu;
  }

  if (menu_button(str_normal_game)) {
    session.next_grid_size = new vec2<i32>{18, 12};
    session.m_state = session_state::game_run_startup;
    platform::close_application();
    current_menu = mui::page::main_menu;
  }

  if (menu_button(str_hard_game)) {
    session.next_grid_size = new vec2<i32>{27, 16};
    session.m_state = session_state::game_run_startup;
    platform::close_application();
    current_menu = mui::page::main_menu;
  }

  if (menu_button(str_huge_game)) {
    session.next_grid_size = new vec2<i32>{i32(1920*0.8f), i32(1080*0.8f)};
    *session.next_grid_size /= session.run->grid_size;
    session.m_state = session_state::game_run_startup;
    platform::close_application();
    current_menu = mui::page::main_menu;
  }

  if (menu_button(str_back)) {
    current_menu = mui::page::main_menu;
  }
  mui::close_div();
}

}
