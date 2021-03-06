#pragma once

#include <lapse_scalar.h>
#include <lapse_array.h>

struct Input{
};

struct Mouse{
  lapse::vec2<lapse::i32> m_pos;
  lapse::vec2<lapse::i32> m_old_pos;
  lapse::vec2<lapse::i32> m_pos_delta;

  
  static Mouse& the();

  // returns true if left mouse button hit event happened this frame
  static bool left_mouse_hit();
  // returns true if left mouse button hit event happened this frame
  static bool right_mouse_hit();
  // returns true if middle mouse button is held down this frame
  static bool middle_mouse_down();
  // returns the mouse position
  static lapse::vec2<> get_mouse_pos();
  // returns how far the mouse has moved during this frame
  static lapse::vec2<> get_mouse_delta();
};

// for whatever reason I've decided to set this enum equal to the javascript keycodes

enum class keycode{
  none      = 0,
  escape    = 27,
  space_bar = 32,
  number_1 = 48,
  number_2 = 49,
  number_3 = 50,
  a = 65,
  d = 68,
  s = 83,
  w = 87,
  // note: if you add something here you need to add it to keycode_list() in cpp
  //   and in platform cpp translate_keycode()
};

// returns a list of all keycodes
const lapse::fixed_array<keycode>& keycode_list();

struct KeyState{
  keycode code = keycode::none;
  bool m_key_hit  = false;
  bool m_key_up   = false;
  bool m_key_down = false;

  // returns true if the key hit event happened this frame
  bool is_hit();
  // returns true if the key up event happened this frame
  bool is_up();
  // returns true if the key is held down
  bool is_down();
  // returns true if key has been hit an odd number of times
  bool is_toggled();
  // 
  void toggle();
};

KeyState key(keycode code);
