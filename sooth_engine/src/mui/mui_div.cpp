#include "mui/mui_div.h"

#include "platform_olc_pixel.h"

#include "mui/mui_context.h"
#include "mui/mui_draw.h"

#include <lapse_lib.h>

using namespace lapse;

namespace mui{

void open_div(params args) {
  auto& context = Context::the();
  context.push_container(args);

  if (args.background_color != transparent) {
    mui::fill_rect(args);
  }
  if (!!args.border) {
    mui::draw_rect(args);
  }
}

void close_div() {
  auto& context = Context::the();
  auto& args = context.get_current_container();

  if (args.size.auto_size) {
    auto keep_looping = true;
    for (i32 i = args.index+1; i < context.dom_stack.length() && keep_looping; i++) {
      if (context.dom_stack[i].parent_index == args.index) {
        // TODO here we're assuming each element occupies only one line
        args.size.x = max(args.size.x, context.dom_stack[i].size.x);
        args.size.y = max(args.size.y, context.dom_stack[i].margin_area().bottom_left_point().y);
      } else {
        keep_looping = false;
      }
    }
    args.size += args.padding.to_vec2();
  }

  // move mui position past the div
  context.current_position = args.position() + args.size;
}

} // end namespace
