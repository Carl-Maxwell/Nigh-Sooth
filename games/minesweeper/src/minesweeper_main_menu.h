#pragma once

#include "lapse_scalar.h"

namespace minesweeper{

struct minesweeper_main_menu{
  enum class menu_location {
    main_menu,
    new_game_menu
  };
  menu_location current_menu = menu_location::main_menu;

  void main_loop(lapse::f32 delta);
  void start_main_loop();
  void main_menu();
  void new_game_menu();
};

};
