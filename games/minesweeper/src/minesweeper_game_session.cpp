#include "minesweeper_game_session.h"

#include <platform_olc_pixel.h>

#include <iostream>

#include "minesweeper_tile.h"
#include "minesweeper_game_run.h"

using namespace lapse;

namespace minesweeper{

void minesweeper_session::start_session() {
  // game session loop
  while (m_state != session_state::application_shutdown) {
    switch(m_state) {
      // case session_state::application_startup:
      // break;
      case session_state::game_run_startup:
        std::cout << "\n\n//\n// starting up a new game run ...\n//\n\n";
        if (run) delete run;
        run = new minesweeper_run();
        run->initialize_run();
        m_state = session_state::game_run_main_loop;
      break;
      case session_state::game_run_main_loop:
        run->start_main_loop();
        // if the main loop has ended, then:
        m_state = session_state::game_run_startup;
      break;
      // case session_state::main_menu:
      // break;
      default: std::cout << "Error! Bad game session state: " << (u32)m_state;
    }
  }
}

// Initialize graphics (called once on application startup)
void minesweeper_session::initialize_game_session() {
  image_array[ (u32)grid_tile::number_1 ] = image("resources/number_1.png");
  image_array[ (u32)grid_tile::number_2 ] = image("resources/number_2.png");
  image_array[ (u32)grid_tile::number_3 ] = image("resources/number_3.png");
  image_array[ (u32)grid_tile::number_4 ] = image("resources/number_4.png");
  image_array[ (u32)grid_tile::number_5 ] = image("resources/number_5.png");
  image_array[ (u32)grid_tile::hidden ]   = image("resources/grid.png");
  image_array[ (u32)grid_tile::empty ]    = image("resources/grid_empty.png");
  image_array[ (u32)grid_tile::hovered ]  = image("resources/grid_highlighted.png");
  image_array[ (u32)grid_tile::mined ]    = image("resources/bomb.png");
  image_array[ (u32)grid_tile::flagged ]  = image("resources/flag.png");
}

} // end namespace
