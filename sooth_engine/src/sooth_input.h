#pragma once

#include <lapse_scalar.h>

struct Input{
};

struct Mouse{
  lapse::vec2<lapse::i32> m_pos;
  
  // returns true if left mouse button hit event happened this frame
  static bool left_mouse_hit();
  // returns true if left mouse button hit event happened this frame
  static bool right_mouse_hit();
  static lapse::vec2<> get_mouse_pos();
};

enum class keycode{
  escape = 27
};

struct KeyState{
  bool m_key_hit  = false;
  bool m_key_up   = false;
  bool m_key_down = false;

  // returns true if the key hit event happened this frame
  bool is_hit();
  // returns true if the key up event happened this frame
  bool is_up();
  // returns true if the key is held down
  bool is_down();
};

KeyState key(keycode code);
