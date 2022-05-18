#include "ephrais_session.h"

#include <platform_olc_pixel.h>

#include <iostream>

#include "ephrais_run.h"

#include "sooth.h"

#include "lapse_lib.h"

using namespace lapse;

namespace ephrais{

// activate a new session state
void Session::advance_phase() {
  // this function is called to start the application, and loops until the application closes
  // it directs execution to the menu, run, etc, objs to do their things.
  // session.m_state (the session state) determines where it goes

  // We only hit this func if:
    // a run starts/ends, the main menu opens/closes, the application opens/closes
  // Otherwise see the main loop funcs

  while (m_state != SessionState::application_shutdown) {
    switch(m_state) {
      case SessionState::application_startup:
        // initialize engine
        sooth::initialize_engine();
        // load images & whatnot
        initialize_game_session();
        m_state = SessionState::main_menu;
      break;
      case SessionState::main_menu:
        main_menu->start_main_loop();
      break;
      case SessionState::game_run_startup:
        std::cout << "\n\n//\n// starting up a new game run ...\n//\n\n";
        // if (run) delete run;
        run = new Run();
        if (next_map_settings) {
          run->initialize_run(*next_map_settings);
        } else {
          run->initialize_run();
        }
        m_state = SessionState::game_run;
      // break; // fall through
      case SessionState::game_run:
        std::cout << "\n\n//\n// entering main loop\n//\n\n";
        run->start_main_loop();
        if (m_should_end_run) { end_run(); }
        m_should_end_run = true;
        std::cout << "\n\n//\n// exiting main loop\n//\n\n";
      break;
      case SessionState::application_shutdown:
        // platform::close_application();
      break;
      default: std::cout << "Error! Bad game session state: " << (u32)m_state;
    }
  }
}

void Session::change_phase(SessionState new_phase) {
  assert(m_state != new_phase);

  switch(m_state) {
    default: break;
  }

  m_state = new_phase;

  platform::close_application();
  platform::set_main_loop_callback(&ephrais::Session::main_loop);
}

void Session::main_loop(f32 delta) {
  auto& session = the();
  switch(session.m_state) {
    case SessionState::game_run:
      platform::set_main_loop_callback(&Run::main_loop);
    break;
    case SessionState::main_menu:
      platform::set_main_loop_callback(&MainMenu::main_loop);
    break;
    case SessionState::application_shutdown : return; break;
    default:
      std::cout << "\n\nError! Bad ephrais Session::main_loop() call \n\n";
      __debugbreak();
  }

  // TODO should we then call that new main loop function from here?
  //   if we don't then there's a frame dropped ...
  //   althogh that frame doesn't render/clear, so it probably goes by quick
}

// Initialize graphics (called once on application startup)
void Session::initialize_game_session() {
  main_menu = new MainMenu;

  // TODO grep resources/assets folder for *.sooth_asset

  // TODO sooth::Asset
    // should contain
    // a void pointer to the data in memory
    // a file path to where to get the data from (if it's not loaded yet)
    // and have a way to serialize/deserialize to a file

  // deserialize all assets
  // load resource data for all assets

  str image_paths[] = {
    "resources/images/Sprite-0001.png",
    "resources/images/reticle_001.png",
    "resources/images/bullet_001.png",
    "resources/images/ground_001.png",
    ""
  };

  {
    auto image_count = 0;
    while (image_paths[image_count++].length()) {}
    image_array = new image[image_count-1];
  }

  auto* image_path = &image_paths[0];
  auto i = 0;
  while ((*image_path).length()) {
    image_array[i].load_image(image_path);

    i++;
    image_path = &image_paths[i];
  }

}

void Session::end_run() {
  delete run;
}

void Session::restart_run() {
  change_phase(SessionState::game_run_startup);
}

void Session::continue_run() {
  assert(run);
  // m_state = SessionState::game_run;
  change_phase(SessionState::game_run);
}

} // end namespace

// fully close out everything
//   TODO this func should be part of session object
//   but currently it has to be accessed from the platform code (which doesn't have access to the session)
void force_quit() {
  auto& session = ephrais::Session::the();
  session.m_state = ephrais::SessionState::application_shutdown;
  platform::close_application();
}
