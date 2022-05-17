#pragma once

#include <lapse_scalar.h>
#include <lapse_string.h>

namespace mui{
  enum class Page {
    main_menu,
    // new_game_menu
  };
};

namespace ephrais{

struct MainMenu{
  mui::Page current_menu = mui::Page::main_menu;
  lapse::i32 frame_count = 0;

  static void main_loop(lapse::f32 delta);
  void start_main_loop();
  void main_menu();
};

} // end namespace
