#pragma once

#include <sooth_image.h>
#include "minesweeper_game_run.h"
#include "minesweeper_main_menu.h"
#include "lapse_scalar.h"

namespace minesweeper{

enum class session_state{
  application_shutdown = 0, // entire program is shutting down
  application_startup,      // application is starting up
  main_menu,                // game's main menu
  game_run_startup,         // loading level 1, setting up the game
  game_run_main_loop,       // main loop of the game
};

struct minesweeper_session{
  // TODO default state should be application_startup
  session_state m_state = session_state::game_run_startup;
  image* image_array = new image[(lapse::u32)grid_tile::count];
  image victory_image;
  minesweeper_run* run = nullptr;
  minesweeper_main_menu* main_menu = nullptr;
  lapse::vec2<lapse::i32>* next_grid_size = nullptr;
  
  // returns singleton
  static minesweeper_session& the() {
    static minesweeper_session* my_session = nullptr;
    if (!my_session) {
      my_session = new minesweeper_session;
    }
    return *my_session;
  };

  void start_session();
  void initialize_game_session();
  void main_loop(lapse::f32 delta);
  void restart_run();
};

}
