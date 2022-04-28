
#include "olcPixelGameEngine.h"

#include <lapse_scalar.h> 

#include "platform_olc_pixel.h"

#include "sooth_input.h"

using namespace lapse;

namespace{
  Input _input;
  Input::Mouse _mouse;
};

vec2<> Input::Mouse::get_mouse_pos() {
  _mouse.m_pos = platform::get_mouse_pos();

  return vec2<>{(f32)_mouse.m_pos.x, (f32)_mouse.m_pos.y} / platform::get_pixel_size();
}
