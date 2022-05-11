#include "minesweeper_game_session.h"

#include <platform_olc_pixel.h>

#include <iostream>

#include "minesweeper_tile.h"
#include "minesweeper_game_run.h"

#include "lapse_lib.h"

using namespace lapse;

namespace minesweeper{

// 
void minesweeper_session::start_session() {
  // TODO this func needs a better name
  
  // maybe something like "advance session to next phase?"

  // activate game session
  while (m_state != session_state::application_shutdown) {
    switch(m_state) {
      case session_state::application_startup:
        // TODO load images & whatnot
        m_state = session_state::main_menu;
      break;
      case session_state::main_menu:
        // TODO this line is never hit
        main_menu->start_main_loop();
      break;
      case session_state::game_run_startup:
        std::cout << "\n\n//\n// starting up a new game run ...\n//\n\n";
        // if (run) delete run;
        run = new minesweeper_run();
        if (next_grid_size) {
          run->initialize_run(next_grid_size->x, next_grid_size->y);
        } else {
          run->initialize_run();
        }
        m_state = session_state::game_run_main_loop;
      // break; // fall through
      case session_state::game_run_main_loop:
        std::cout << "\n\n//\n// entering main loop\n//\n\n";
        run->start_main_loop();
        delete run;
        std::cout << "\n\n//\n// exiting main loop\n//\n\n";
      break;
      case session_state::application_shutdown:
        // platform::close_application();
      break;
      default: std::cout << "Error! Bad game session state: " << (u32)m_state;
    }
  }
}

void minesweeper_session::main_loop(f32 delta) {
  switch(m_state) {
    case session_state::game_run_main_loop: run->main_loop(delta);       break;
    case session_state::main_menu:          main_menu->main_loop(delta); break;
    case session_state::application_shutdown : return; break;
    default:
      std::cout << "\n\nError! Bad minesweeper_session::main_loop() call \n\n";
      __debugbreak();
  }
  arenas::temp.clear();
}

// Initialize graphics (called once on application startup)
void minesweeper_session::initialize_game_session() {
  main_menu = new minesweeper_main_menu;

  str paths[] = {
    "resources/number_1.png",
    "resources/number_2.png",
    "resources/number_3.png",
    "resources/number_4.png",
    "resources/number_5.png",
    "resources/grid.png",
    "resources/grid_empty.png",
    "resources/grid_highlighted.png",
    "resources/bomb.png",
    "resources/flag.png",
    "resources/bad_flag.png",
    "resources/victory.png",
  };

  i32 i = 0;

  image_array[ (u32)grid_tile::number_1 ] .load_image(&paths[i++]);
  image_array[ (u32)grid_tile::number_2 ] .load_image(&paths[i++]);
  image_array[ (u32)grid_tile::number_3 ] .load_image(&paths[i++]);
  image_array[ (u32)grid_tile::number_4 ] .load_image(&paths[i++]);
  image_array[ (u32)grid_tile::number_5 ] .load_image(&paths[i++]);
  image_array[ (u32)grid_tile::hidden ]   .load_image(&paths[i++]);
  image_array[ (u32)grid_tile::empty ]    .load_image(&paths[i++]);
  image_array[ (u32)grid_tile::hovered ]  .load_image(&paths[i++]);
  image_array[ (u32)grid_tile::mined ]    .load_image(&paths[i++]);
  image_array[ (u32)grid_tile::flagged ]  .load_image(&paths[i++]);
  image_array[ (u32)grid_tile::bad_flag ] .load_image(&paths[i++]);

  victory_image .load_image(&paths[i++]);
}

void minesweeper_session::restart_run() {
  m_state = session_state::game_run_startup;
  platform::close_application();
}

} // end namespace

// fully close out everything
//   TODO this func should be part of session object
//   but currently it has to be accessed from the platform code (which doesn't have access to the session)
void force_quit() {
  auto& session = minesweeper::minesweeper_session::the();
  session.m_state = minesweeper::session_state::application_shutdown;
  platform::close_application();
}
