#include "ephrais_run.h"

#include "lapse_lib.h"

#include "platform_olc_pixel.h"
#include "sooth_input.h"

#include "ephrais_session.h"

using namespace lapse;

namespace ephrais{
// TODO is there any point to having namespaces like ephrais and minesweeper?

bool Run::start_main_loop() {
  auto pixel_size = 4;

  // TODO fetch the actual monitor resolution from platform

  vec2<i32> monitor_resolution = {1920, 1080};
  monitor_resolution = (monitor_resolution/5)*4;
  monitor_resolution = monitor_resolution / next_window_size;
  pixel_size = min(monitor_resolution.x, monitor_resolution.y);

  std::cout << "pixel size: " << pixel_size;

  pixel_size = max(1, pixel_size);

  static str str_application_name = "Nigh Sooth - Top Down Shooter Game";

  platform::initialize(
    next_window_size.x,
    next_window_size.y,
    false,
    str_application_name,
    pixel_size
  );
  return platform::start_application();
}

void Run::main_loop(f32 delta) {
  auto& session = Session::the();
  auto& run = *session.run;
  platform::clear(vec3<>{0, 0, 0});

  auto& game_zoom      = run.game_zoom;
  auto& panning_offset = run.panning_offset;

  platform::poll_key_toggles();
  lapse::LapseErrorQueue::the().tick();

  auto mouse_pos = Mouse::get_mouse_pos();

  //
  // Draw stuff
  //

  // screen_pos += panning_offset;
  platform::draw_bitmap_scaled(run.player_position, session.image_array[0], game_zoom);

  //
  // Player input
  //

  // open main menu if esc is hit
  if (key(keycode::escape).is_hit()) {
    session.m_should_end_run = false;
    session.change_phase(SessionState::main_menu);
  }

  // debug
  if (key(keycode::number_2).is_hit()) {
    auto window = platform::get_window_size();
    auto pixel_siez = platform::get_pixel_size();
    __debugbreak();
  }

  // wasd player movement
  if (key(keycode::d).is_down()) {
    run.player_position.x += 12.0f * delta;
  }
  if (key(keycode::s).is_down()) {
    run.player_position.y += 12.0f * delta;
  }
  if (key(keycode::a).is_down()) {
    run.player_position.x -= 12.0f * delta;
  }
  if (key(keycode::w).is_down()) {
    run.player_position.y -= 12.0f * delta;
  }

  //
  // Zoom & Pan
  //

  if (platform::get_mouse_wheel_delta()) {

  }

  // panning the view by moving the mouse
  if (key(keycode::space_bar).is_down() || Mouse::middle_mouse_down()) {
    panning_offset -= Mouse::get_mouse_delta()/game_zoom;
  }

  // empty out temp arena
  arenas::temp.clear();
}

// called each time we need to setup a new map to play
void Run::initialize_run(NextMapSettings next_map) {

}

}
