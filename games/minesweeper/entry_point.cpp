
#include "sooth.h"

#include "lapse_lib.h"

#include "sooth_input.h"

#include "platform_olc_pixel.h"

#include <sooth_image.h>
#include <mui/mui_context.h>

#include "minesweeper_game_session.h"
#include "minesweeper_game_run.h"

#include "minesweeper_tile.h"

using namespace lapse;

namespace minesweeper{

void main_loop_callback(f32 delta) {
  auto& session = minesweeper_session::the();

  platform::clear(vec3<>{0, 0, 0});
  mui::Context::the().reset();

  lapse::LapseErrorQueue::the().tick();

  session.main_loop(delta);
}

} // end minesweeper namespace

int main() {
  sooth::initialize_engine();

  platform::set_game_session_initialization_callback([](){
    auto& session = minesweeper::minesweeper_session::the();
    session.initialize_game_session();
  });

  platform::set_main_loop_callback([](f32 delta){
    minesweeper::main_loop_callback(delta);
  } );

  //
  // Start the game
  //

  auto& session = minesweeper::minesweeper_session::the();
  session.start_session();

  return 0;
}

