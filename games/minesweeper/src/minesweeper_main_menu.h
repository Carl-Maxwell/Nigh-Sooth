#pragma once

#include "lapse_scalar.h"

namespace mui{
  enum class page {
    main_menu,
    new_game_menu
  };
};

namespace minesweeper{

struct minesweeper_main_menu{
  mui::page current_menu = mui::page::main_menu;
  lapse::i32 frame_count = 0;

  void main_loop(lapse::f32 delta);
  void start_main_loop();
  void main_menu();
  void new_game_menu();
};

};
