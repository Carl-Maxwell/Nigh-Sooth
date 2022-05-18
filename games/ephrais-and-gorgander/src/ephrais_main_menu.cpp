#include "ephrais_main_menu.h"

#include <lapse_lib.h>

#include "platform_olc_pixel.h"

#include "mui/mui_div.h"
#include "mui/button.h"
#include "mui/mui_draw.h"
#include "mui/mui_context.h"

#include "ephrais_session.h"

using namespace lapse;

namespace ephrais{

void MainMenu::start_main_loop() {
  static str str_application_name = "Nigh Sooth - Top Down Shooter Game - Main Menu";
  platform::initialize(1920/5, 1080/5, false, str_application_name);
  platform::start_application();
}

void MainMenu::main_loop(f32 delta) {
  auto& session      = Session::the();
  auto& main_menu    = *session.main_menu;
  auto& current_menu = main_menu.current_menu;

  // std::cout << "main menu start of frame " << main_menu.frame_count << "\n";
  platform::clear(vec3<>{0, 0, 0});
  mui::Context::the().reset();
  platform::poll_key_toggles();
  lapse::LapseErrorQueue::the().tick();

  switch (current_menu) {
    case mui::Page::main_menu:     main_menu.main_menu();     break;
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

void MainMenu::main_menu() {
  auto& session = Session::the();

  mui::params div_container = {};

  mui::open_div(div_container);

  static str str_continue_game = "Continue Game";
  static str str_new_game = "New Game";
  static str str_exit = "Exit Game"; 

  if (session.run) {
    if (mui::button(str_continue_game)) {
      session.continue_run();
    }
  }

  if (mui::button(str_new_game)) {
    session.next_map_settings = new NextMapSettings{};
    session.change_phase(SessionState::game_run_startup);
  }

  if (mui::button(str_exit)) {
    session.change_phase(SessionState::application_shutdown);
  }

  mui::close_div();
}

} // end namespace
