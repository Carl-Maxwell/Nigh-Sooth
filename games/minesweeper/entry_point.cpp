
#include "sooth.h"

#include "lapse_lib.h"

#include "sooth_input.h"

#include "platform_olc_pixel.h"

#include <sooth_image.h>

#include "minesweeper_game_session.h"
#include "minesweeper_game_run.h"

#include "minesweeper_tile.h"

using namespace lapse;

namespace minesweeper{

void main_loop_callback(f32 delta) {
  auto& session = minesweeper_session::the();
  auto& run     = *session.run;

  assert(run.grid != nullptr);

  platform::clear(vec3<>{0, 0, 0});

  lapse::LapseErrorQueue::the().tick();

  f32 grid_size = run.grid_size;
  f32 padding = platform::get_window_padding();

  i32 mouse_x = (i32)Mouse::get_mouse_pos().x - padding;
  i32 mouse_y = (i32)Mouse::get_mouse_pos().y - padding;
  mouse_x /= grid_size;
  mouse_y /= grid_size;
  minesweeper::tile_obj* mouse_tile = nullptr;

  if (mouse_x >= 0 && mouse_y >= 0 && mouse_x < run.grid_width && mouse_y < run.grid_height) {
    mouse_tile = &run.grid[mouse_y * run.grid_width + mouse_x];
  }

  if (Mouse::left_mouse_hit()) {
    if (mouse_tile && mouse_tile->m_hidden && !mouse_tile->m_flagged) {
      mouse_tile->reveal();
    }
  } else if (Mouse::right_mouse_hit()) {
    if (mouse_tile && mouse_tile->m_hidden) {
      if (!mouse_tile->m_flagged) {
        mouse_tile->m_tile_state = minesweeper::grid_tile::flagged;
        mouse_tile->m_flagged = true;
      } else {
        mouse_tile->m_tile_state = minesweeper::grid_tile::hidden;
        mouse_tile->m_flagged = false;
      }
    }
  }

  for (f32 y = 0; y < run.grid_height; y++) {
    for (f32 x = 0; x < run.grid_width; x++) {
      vec2<> screen_pos{x*grid_size + padding, y*grid_size + padding};
      auto& tile = run.grid[u32(y*run.grid_width + x)];
      bool hovering = mouse_tile && mouse_tile == &tile;
      image img = tile.get_image(hovering);

      platform::draw_bitmap(screen_pos, img);
    }
  }
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

