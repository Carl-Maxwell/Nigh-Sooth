#include "button.h"

#include <lapse_lib.h>

#include "platform_olc_pixel.h"
#include "mui/mui_context.h"
#include "mui/mui_draw.h"

using namespace lapse;

namespace mui{

bool button(lapse::str label) {
  auto& context = Context::the();

  f32 padding = 10.0f;

  params box = {{0, 0}, {padding*2 + label.length() * 8, 20}};
  box.padding = {10};

  context.open_element(box);

  if (!context.is_hovering()) {
    mui::draw_rect(box);
  }
  mui::draw_text(label, box);

  bool active = context.is_active();

  context.close_element();

  return active;
}

} // end namespace
