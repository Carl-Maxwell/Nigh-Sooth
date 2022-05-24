#include "ephrais_run.h"

#include "lapse_lib.h"

#include "platform_olc_pixel.h"
#include "sooth_input.h"

#include "ephrais_session.h"

#include <mui/mui_text.h>

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

  vec2<> reticle_offset = (mouse_pos - run.player_position - (tile_size/2)).normalize()*tile_size;
  
  //
  // Draw the player sprite
  //
  
  // screen_pos += panning_offset;
  platform::draw_bitmap_scaled(run.player_position, session.image_array[0], game_zoom);
  auto reticle_rotation = atan2(reticle_offset.y, reticle_offset.x);
  platform::draw_bitmap_rotated(run.player_position + reticle_offset, session.image_array[1], reticle_rotation);

  //
  // Enemies
  //

  struct Transform{
    vec2<> m_pos;
    f32 m_rotation;

    vec2<> forward_vector() {
      vec2<> forward = {0.0f, 0.0f};
      forward.x = cos(m_rotation);
      forward.y = sin(m_rotation);
      return forward;
    }
  };
  struct EnemySprite{
    f32 m_scale;
    i32 m_sprite_index;

    image& sprite() {
      auto& session = Session::the();
      return session.image_array[m_sprite_index];
    }
    rect<> box() {
      auto& l_sprite = sprite();
      return {{0, 0}, {l_sprite.m_width*m_scale, l_sprite.m_height*m_scale}};
    }
    vec2<> size() {
      return sprite().m_resolution * m_scale;
    }
  };
  struct EnemyAttackTimer{
    f64 m_last_fire_time;
    f64 m_refire;
  };

  static array<Transform>        enemy_transforms;
  static array<EnemySprite>      enemy_sprites;
  static array<vec2<>>           enemy_velocities;
  static array<vec2<>>           enemy_target_pos;
  static array<f32>              enemy_hp;
  static array<EnemyAttackTimer> enemy_attack_time;

  // Enemy projectile transforms
  static array<Transform>   e_bullet_transforms;
  static array<EnemySprite> e_bullet_sprites;
  static array<f64>         e_bullet_lifetimes;

  if (!enemy_transforms.m_size)      { enemy_transforms   .reserve(100); }
  if (!enemy_sprites.m_size)         { enemy_sprites      .reserve(100); }
  if (!enemy_velocities.m_size)      { enemy_velocities   .reserve(100); }
  if (!enemy_target_pos.m_size)      { enemy_target_pos   .reserve(100); }
  if (!enemy_hp.m_size)              { enemy_hp           .reserve(100); }
  if (!enemy_attack_time.m_size)     { enemy_attack_time  .reserve(100); }

  if (!e_bullet_transforms.m_size)   { e_bullet_transforms.reserve(100); }
  if (!e_bullet_sprites.m_size)      { e_bullet_sprites   .reserve(100); }
  if (!e_bullet_lifetimes.m_size)    { e_bullet_lifetimes .reserve(100); }

  if (enemy_transforms.length() == 0) {
    auto spawn_count = die(3, 6);
    for (decltype(spawn_count) i = 0; i < spawn_count; i++) {
      enemy_transforms.push( {
        rand_vec2(platform::get_window_size()),
        rand_f32(pi * 2.0f)
      } );
      enemy_sprites.push( { 1.0f, 4 } );
      enemy_velocities.push( { rand_vec2() } );
      enemy_target_pos.push( { 0, 0 } );
      enemy_hp.push( { 3 } );
      enemy_attack_time.push( { get_timestamp(), rand_range(0.6f, 1.4f) } );
    }
  }

  // Draw enemies
  for (auto i = 0; i < enemy_transforms.length(); i++) {
    auto& position = enemy_transforms[i].m_pos;
    auto& image = enemy_sprites[i].sprite();
    platform::draw_bitmap_scaled(position, image, game_zoom);
  }

  // Draw enemy HP
  for (auto i = 0; i < enemy_transforms.length(); i++) {
    auto position = enemy_transforms[i].m_pos;
    auto hp = enemy_hp[i];
    str str_hp = str(f32_to_c_str(hp));
    position.y -= mui::text_box_size(str_hp).y;
    platform::draw_text(str_hp, position);
  }

  // Update enemy postion & velocity
  for (auto i = 0; i < enemy_transforms.length(); i++) {
    auto& position   = enemy_transforms[i].m_pos;
    auto& velocity   = enemy_velocities[i];
    auto& target_pos = enemy_target_pos[i];

    // Update target position
    auto distance = (target_pos - position).length();
    if (distance < tile_size) {
      target_pos = (run.player_position - position).normalize() * 5 + position;
      if ((target_pos - run.player_position).length() < 10.0f * tile_size) {
        target_pos = (position - run.player_position).normalize() * 7.0f * tile_size;
        target_pos += run.player_position;
        target_pos += (rand_vec2() - 0.5f) * 2.0f * 3.0f * tile_size;
      }
    }

    // Draw target position (for debug purposes)
    if (
      platform::get_window_rect().is_point_inside(target_pos) &&
      platform::get_window_rect().is_point_inside(position)
    ) {
      platform::draw_line(position, target_pos);
    }

    // Update position
    velocity = (target_pos - position).normalize() * tile_size * 1.5f;
    position += velocity * delta;
  }

  // Enemy attacks (spawning bullets)
  {
    auto now = get_timestamp();
    for (u32 i = 0; i < enemy_attack_time.length(); i++) {
      auto attack = enemy_attack_time[i];
      if (now > attack.m_last_fire_time + attack.m_refire) {
        // Spawn an enemy bullet
        auto move = (run.player_position - enemy_transforms[i].m_pos).normalize();
        auto position = enemy_transforms[i].m_pos + (move * 1.0f * tile_size);
        auto rotation = atan2(move.y, move.x);

        e_bullet_transforms.push( { position, rotation } );
        e_bullet_sprites   .push( { 1.0f, 5 } );
        e_bullet_lifetimes .push( { get_timestamp() + 3.0f } );

        // Update attack last fire time
        enemy_attack_time[i].m_last_fire_time = now;
      }
    }
  }

  // Draw enemy bullets
  for (u32 i = 0; i < e_bullet_transforms.length(); i++) {
    auto position     = e_bullet_transforms[i].m_pos;
    auto rotation     = e_bullet_transforms[i].m_rotation;
    auto sprite_index = e_bullet_sprites[i].m_sprite_index;

    platform::draw_bitmap_rotated(position, session.image_array[sprite_index], rotation);
  }

  // Update enemy bullet positions
  for (u32 i = 0; i < e_bullet_transforms.length(); i++) {
    auto dir = e_bullet_transforms[i].forward_vector();
    e_bullet_transforms[i].m_pos += dir * delta * 1.0f * tile_size;
  }

  // Delete old enemy bullets
  {
    auto now = get_timestamp();
    for (u32 i = 0; i < e_bullet_lifetimes.length(); i++) {
      if (now > e_bullet_lifetimes[i]) {
        e_bullet_transforms.remove_at(i);
        e_bullet_sprites   .remove_at(i);
        e_bullet_lifetimes .remove_at(i);
        i--;
      }
    }
  }

  //
  // Draw friendly (player) bullets
  //

  static array<fixed_array<vec2<>>> bullets;
  if (!bullets.m_size) { bullets.reserve(100); }

  for (auto i = 0; i < bullets.length(); i++) {
    auto& bullet_move = bullets[i][1];
    auto rotation = atan2(bullet_move.normalize().y, bullet_move.normalize().x);
    platform::draw_bitmap_rotated(bullets[i][0], session.image_array[2], rotation);
    // platform::draw_bitmap_rs(bullets[i][0], session.image_array[2], game_zoom, rotation);
    // Move bullets forward
    bullets[i][0] += bullet_move * delta;
    if (
      bullets[i][0].greater_than_or(platform::get_window_size()) ||
      bullets[i][0].less_than_or(0)
      ) {
        bullets.remove_at(i);
        // Have to decrement to hit the next bullet
        //   (since length() just went down by 1)
        i--; 
      }
  }

  // Check for collisions between friendly bullets and enemies
  for (u32 i = 0; i < bullets.length(); i++) {
    auto bullet_rect = rect<>{bullets[i][0], session.image_array[2].m_resolution};
    auto bullet_collided = false;
    for (u32 e_i = 0; e_i < enemy_hp.length(); e_i++) {
      auto enemy_top_left     = enemy_transforms[e_i].m_pos;
      auto enemy_bottom_right = enemy_sprites[e_i].sprite().m_resolution + enemy_top_left;
      if (
        bullet_rect.is_point_inside(enemy_top_left) ||
        bullet_rect.is_point_inside(enemy_bottom_right)
      ) {
        enemy_hp[e_i] -= 1;

        if (enemy_hp[e_i] <= 0) {
          // Delete one enemy ...
          enemy_transforms .remove_at(e_i);
          enemy_sprites    .remove_at(e_i);
          enemy_velocities .remove_at(e_i);
          enemy_target_pos .remove_at(e_i);
          enemy_hp         .remove_at(e_i);
          enemy_attack_time.remove_at(e_i);
        }
        
        // And mark this bullet for deletion ...
        bullet_collided = true;
      }
    }

    if (bullet_collided) {
      bullets.remove_at(i);
      // Have to decrement to hit the next bullet
      //   (since length() just went down by 1)
      i--; 
      
    }
  }

  //
  // Player input
  //

  static auto last_bullet_fired = lapse::get_timestamp();
  static vec2<> player_velocity = {0, 0};

  // Fire bullet if left mouse is hit
  if (Mouse::left_mouse_down() && get_timestamp() > last_bullet_fired + 0.2f) {
    std::cout << player_velocity.length() << "\n";
    auto player_push = player_velocity.length() > 0 ? player_velocity.normalize().dot(reticle_offset.normalize()) : 0.0f;
    std::cout << "player_push dot product: " << player_push << "\n";

    bullets.push({
      run.player_position + reticle_offset,
      reticle_offset.normalize() * tile_size * 1.5f // + (player_velocity * player_push)
    });
    // TODO add player velocity onto bullet velocity
    last_bullet_fired = lapse::get_timestamp();
  }

  // Open main menu if esc is hit
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

  // Have to normalize velocity after adding each input direction to avoid faster diagonal movement
  player_velocity = player_velocity.length() ? player_velocity.normalize()*4*tile_size : player_velocity;
  run.player_position += player_velocity * delta;

  //
  // Zoom & Pan
  //

  if (platform::get_mouse_wheel_delta()) {

  }

  // Panning the view by moving the mouse
  if (key(keycode::space_bar).is_down() || Mouse::middle_mouse_down()) {
    panning_offset -= Mouse::get_mouse_delta()/game_zoom;
  }

  // Empty out temp arena
  arenas::temp.clear();
}

// Called each time we need to setup a new map to play
void Run::initialize_run(NextMapSettings next_map) {

}

}
