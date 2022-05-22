#include "ephrais_run.h"

#include "lapse_lib.h"

#include "platform_olc_pixel.h"
#include "sooth_input.h"

#include "ephrais_session.h"

using namespace lapse;

namespace ephrais{
// TODO is there any point to having namespaces like ephrais and minesweeper?

bool Run::start_main_loop() {
  auto pixel_size = 8;

  // TODO fetch the actual monitor resolution from platform

  // next_window_size /= pixel_size;

  vec2<i32> monitor_resolution = {1920, 1080};
  monitor_resolution = (monitor_resolution/10)*9;
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

  u32 tile_size = 32;

  // The width of the board in game tiles
  auto board_width  = platform::get_window_width() /tile_size;
  auto board_height = platform::get_window_height()/tile_size;

  // Draw the ground
  for (u32 y = 0; y < board_height; y++) {
    for (u32 x = 0; x < board_width; x++) {
      vec2<> screen_pos = {f32(x*tile_size), f32(y*tile_size)};
      platform::draw_bitmap_scaled(screen_pos, session.image_array[3], game_zoom);
    }
  }

  static array<fixed_array<vec2<>>> bullets;
  if (!bullets.m_size) { bullets.reserve(100); }

  vec2<> reticle_offset = (mouse_pos - run.player_position - (tile_size/2)).normalize()*tile_size;

  // screen_pos += panning_offset;
  platform::draw_bitmap_scaled(run.player_position, session.image_array[0], game_zoom);
  auto reticle_rotation = atan2(reticle_offset.y, reticle_offset.x);
  platform::draw_bitmap_rotated(run.player_position + reticle_offset, session.image_array[1], reticle_rotation);

  for (auto i = 0; i < bullets.length(); i++) {
    auto& bullet_dir = bullets[i][1];
    auto rotation = atan2(bullet_dir.y, bullet_dir.x);
    platform::draw_bitmap_rotated(bullets[i][0], session.image_array[2], rotation);
    // std::cout << "// rotation:" << rotation << "\n";
    // platform::draw_bitmap_rs(bullets[i][0], session.image_array[2], game_zoom, rotation);
    bullets[i][0] += bullet_dir * delta;
    if (
      bullets[i][0].greater_than_or(platform::get_window_size()) ||
      bullets[i][0].less_than_or(0)
      ) {
        bullets.remove_at(i);
        i--; // have to decrement to hit the next bullet (since length() just went down by 1)
      }
  }

  //
  // Player input
  //

  static auto last_bullet_fired = lapse::get_timestamp();
  static vec2<> player_velocity = {0, 0};

  // fire bullet if left mouse is hit
  if (Mouse::left_mouse_down() && get_timestamp() > last_bullet_fired + 0.2f) {
    std::cout << player_velocity.length() << "\n";
    auto player_push = player_velocity.length() > 0 ? player_velocity.normalize().dot(reticle_offset.normalize()) : 0.0f;
    std::cout << "player_push dot product: " << player_push << "\n";

    bullets.push({
      run.player_position + reticle_offset,
      reticle_offset.normalize() * (tile_size/2) // + (player_velocity * player_push)
    });
    // TODO add player velocity onto bullet velocity
    last_bullet_fired = lapse::get_timestamp();
  }

  // open main menu if esc is hit
  if (key(keycode::escape).is_hit()) {
    session.m_should_end_run = false;
    session.change_phase(SessionState::main_menu);
  }

  // debug
  if (key(keycode::number_2).is_hit()) {
    auto window = platform::get_window_size();
    auto pixel_size = platform::get_pixel_size();
    __debugbreak();
  }

  player_velocity = {0, 0};

  // wasd player movement
  if (key(keycode::d).is_down()) {
    player_velocity.x += 1.0f;
  }
  if (key(keycode::s).is_down()) {
    player_velocity.y += 1.0f;
  }
  if (key(keycode::a).is_down()) {
    player_velocity.x -= 1.0f;
  }
  if (key(keycode::w).is_down()) {
    player_velocity.y -= 1.0f;
  }

  // have to normalize velocity after adding each input direction to avoid faster diagonal movement
  player_velocity = player_velocity.length() ? player_velocity.normalize()*4*tile_size : player_velocity;
  run.player_position += player_velocity * delta;

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
