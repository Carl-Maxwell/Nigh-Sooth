
#include "olcPixelGameEngine.h"

#include <lapse_scalar.h> 

#include "platform_olc_pixel.h"

#include "sooth_input.h"

using namespace lapse;

namespace{
  Input _input;
  Mouse _mouse;
};

//
// Mouse
//

bool Mouse::left_mouse_hit() {
  return platform::is_mouse_left_button_hit();
}

bool Mouse::right_mouse_hit() {
  return platform::is_mouse_right_button_hit();
}

vec2<> Mouse::get_mouse_pos() {
  _mouse.m_pos = platform::get_mouse_pos();

  return vec2<>{(f32)_mouse.m_pos.x, (f32)_mouse.m_pos.y} / platform::get_pixel_size();
}

//
// Keyboard
//

bool KeyState::is_hit() { return m_key_hit; }
bool KeyState::is_up() { return m_key_up; }
bool KeyState::is_down() { return m_key_down; }

KeyState key(keycode code) {
  return platform::get_key_state(code);
}
