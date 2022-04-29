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

