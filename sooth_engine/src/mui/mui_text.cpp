#include "mui/mui_text.h"

#include <lapse_lib.h>

#include "platform_olc_pixel.h"
#include "mui/mui_context.h"

using namespace lapse;

namespace mui{

u32 font_size() {
  u32 size = 8;
  size = platform::get_window_size().x / 60; // aim for 60 columns

  // integer rounding (has to be an integer multiple of 8 ... so either 8 or 16, etc ... )
  size /= 8;
  size *= 8;
  size *= ContextScale::the().scale();
  size = max(8u, size); // the only size less than 8 is 0

  return size;
}

mui_size text_box_size(str& label) {
  return {f32(label.length() * font_size()), f32(font_size() + 2u)};
}

} // end namespace
