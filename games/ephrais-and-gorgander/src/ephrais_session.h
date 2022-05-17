#pragma once

#include <sooth_image.h>

#include "ephrais_run.h"
#include "ephrais_main_menu.h"
#include "ephrais_next_map_settings.h"

#include "lapse_scalar.h"

namespace ephrais{

enum class SessionState{
  application_shutdown = 0, // entire program is shutting down
  application_startup,      // application is starting up
  main_menu,                // game's main menu
  game_run_startup,         // loading level 1, setting up the game
  game_run,                 // main loop of the game
};

struct Session{
  SessionState m_state = SessionState::application_startup;
  image* image_array;
  Run* run = nullptr;
  MainMenu* main_menu = nullptr;
  NextMapSettings* next_map_settings = nullptr;
  // whether the run should be ended when the main loop exits
  bool m_should_end_run = true;
  
  ~Session() {
    std::cout << "~ephrais session()\n";
  }

  // returns singleton
  static Session& the() {
    static Session* my_session = nullptr;
    if (!my_session) {
      my_session = new Session;
    }
    return *my_session;
  };

  void advance_phase();
  void change_phase(SessionState new_phase);
  void initialize_game_session();
  static void main_loop(lapse::f32 delta);
  void end_run();
  void restart_run();
  // this func resumes an ongoing run that was paused (for example, to open a menu)
  void continue_run();
};

}
