#include "button.h"

#include "lapse_lib.h"

#include "platform_olc_pixel.h"

#include "mui/mui_context.h"

using namespace lapse;

namespace mui{

bool button(lapse::str label) {
  auto& context = Context::the();

  f32 padding = 10.0f;

  vec2<> drawing_area_start = {10, 10};
  vec2<> drawing_area_size  = {padding*2 + label.length() * 8, 20};

  drawing_area_start += context.current_position;
  // drawing_area_end   += context.current_position;

  auto mouse_pos = Mouse::get_mouse_pos();
  
  bool hovering =
    mouse_pos > drawing_area_start && 
    mouse_pos < (drawing_area_start + drawing_area_size);
  bool active = hovering && Mouse::left_mouse_hit();

  if (!hovering) {
    platform::draw_rect(
      drawing_area_start,
      drawing_area_size
    );
  }
  platform::draw_text(label, vec2<>{10 + padding, 20} + context.current_position);

  context.current_position.y += 30.0f;

  if (active) {
    return true;
  }

  return false;
}

} // end namespace
