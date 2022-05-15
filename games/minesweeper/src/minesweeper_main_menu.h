#pragma once

#include <lapse_scalar.h>
#include <lapse_string.h>

namespace mui{
  enum class page {
    main_menu,
    new_game_menu
  };
};

namespace minesweeper{

struct minesweeper_main_menu{
  mui::page current_menu       = mui::page::main_menu;
  lapse::i32 frame_count       = 0;
  lapse::str str_continue_game = "Continue Game";
  lapse::str str_new_game      = "New Game";
  lapse::str str_test_menu     = "test menu";
  lapse::str str_exit          = "Exit Minesweeper";

  static void main_loop(lapse::f32 delta);
  void start_main_loop();
  void main_menu();
  void new_game_menu();
};

};
