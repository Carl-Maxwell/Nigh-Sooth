#include "button.h"

#include <lapse_lib.h>

#include "platform_olc_pixel.h"
#include "mui/mui_context.h"
#include "mui/mui_draw.h"
#include "mui/mui_div.h"

using namespace lapse;

namespace mui{

mui_size text_size(str label) {
  return {f32(label.length() * 8), 10.0f};
}

bool button(lapse::str label) {
  auto& context = Context::the();

  params box;
  box.padding = {10};
  box.border  = {1};

  mui::open_div(box);

  params text = {text_size(label)};
  text.padding = {1, 0, 0, 0};
  text.border = {1};

  context.open_element(text);
  mui::draw_text(label, text);
  context.close_element();

  if (context.is_hovering()) {
    // TODO highlight button or underline text or whatever
    mui::draw_rect(text);
  }

  bool active = context.is_active();

  mui::close_div();

  return active;
}

} // end namespace
