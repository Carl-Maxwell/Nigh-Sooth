#include "minesweeper_main_menu.h"

#include "platform_olc_pixel.h"

#include <lapse_lib.h>

#include "minesweeper_game_session.h"

#include "mui/mui_div.h"
#include "mui/button.h"
#include "mui/mui_draw.h"
#include "mui/mui_context.h"

using namespace lapse;

namespace minesweeper{

void minesweeper_main_menu::start_main_loop() {
  static str str_application_name = "Nigh Sooth - Minesweeper Game - Main Menu";
  platform::initialize(1920/5, 1080/5, false, str_application_name);
  platform::start_application();
}

void minesweeper_main_menu::main_loop(f32 delta) {
  auto& session      = minesweeper_session::the();
  auto& main_menu    = *session.main_menu;
  auto& current_menu = main_menu.current_menu;

  // std::cout << "main menu start of frame " << frame_count << "\n";
  platform::clear(vec3<>{0, 0, 0});
  mui::Context::the().reset();
  platform::poll_key_toggles();
  lapse::LapseErrorQueue::the().tick();

  switch (current_menu) {
    case mui::page::main_menu:     main_menu.main_menu();     break;
    case mui::page::new_game_menu: main_menu.new_game_menu(); break;
  }

  mui::draw();

  if (key(keycode::number_2).is_hit()) {
    __debugbreak();
  }

  if (key(keycode::escape).is_hit()) {
    if (session.run) {
      session.continue_run();
    }
  }

  // std::cout << "main menu end of frame " << frame_count << "\n";
  main_menu.frame_count++;

  // empty out temp arena
  arenas::temp.clear();
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

  args.background_color = {0.7254901960784314f, 0.8274509803921569f, 0.8666666666666667f};
  return mui::button(label, args);
}

// wrapper func for mui::button() that adds this game's style to it
bool menu_button_nth(str& label, mui::params args = {}) {
  // args.font_color = minesweeper_font_color;

  args.margin.bottom() = 10;
  return menu_button(label, args);
}

void minesweeper_main_menu::main_menu() {
  auto& session = minesweeper_session::the();

  open_menu_content();

  if (session.run) {
    if (menu_button_nth(str_continue_game)) {
      session.continue_run();
    }
  }

  if (menu_button_nth(str_new_game)) {
    current_menu = mui::page::new_game_menu;
  }

  if (menu_button_nth(str_test_menu)) {
    //current_menu = mui::page::test_menu;
  }

  if (menu_button(str_exit)) {
    session.change_phase(session_state::application_shutdown);
  }

  mui::close_div();
}

str str_quick_game  = "quick game";
str str_normal_game = "normal game";
str str_hard_game   = "hard game";
str str_huge_game   = "huge game";
str str_2x_game     = "impossibly big game";
str str_back        = "back";

void minesweeper_main_menu::new_game_menu() {
  auto& session = minesweeper_session::the();

  open_menu_content();

  if (menu_button_nth(str_quick_game)) {
    session.next_grid_size = new vec2<i32>{10, 10};
    session.change_phase(session_state::game_run_startup);
    current_menu = mui::page::main_menu;
  }

  if (menu_button_nth(str_normal_game)) {
    session.next_grid_size = new vec2<i32>{18, 12};
    session.change_phase(session_state::game_run_startup);
    current_menu = mui::page::main_menu;
  }

  if (menu_button_nth(str_hard_game)) {
    session.next_grid_size = new vec2<i32>{27, 16};
    session.change_phase(session_state::game_run_startup);
    current_menu = mui::page::main_menu;
  }

  if (menu_button_nth(str_huge_game)) {
    session.next_grid_size = new vec2<i32>{i32(1920*0.8f), i32(1080*0.8f)};
    *session.next_grid_size /= 16;//session.run->grid_size;
    session.change_phase(session_state::game_run_startup);
    current_menu = mui::page::main_menu;
  }

  if (menu_button_nth(str_2x_game)) {
    session.next_grid_size = new vec2<i32>{i32(1920*2.0f), i32(1080*2.0f)};
    *session.next_grid_size /= 16;//session.run->grid_size;
    session.change_phase(session_state::game_run_startup);
    current_menu = mui::page::main_menu;
  }

  if (menu_button(str_back)) {
    current_menu = mui::page::main_menu;
  }
  mui::close_div();
}

}
