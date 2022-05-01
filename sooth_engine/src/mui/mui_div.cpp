#include "mui/mui_div.h"

#include "platform_olc_pixel.h"

#include "mui/mui_context.h"

namespace mui{

void open_div(params args) {
  auto& context = Context::the();
  context.push_container(args);

  if (args.background_color != transparent) {
    platform::fill_rect(args.box_area(), args.background_color);
  }
  if (!!args.border) {
    platform::draw_rect(args.position, args.size);
  }
}

void close_div() {
  auto& context = Context::the();
  auto& openning_args = context.pop_container();
  context.current_position = openning_args.position + openning_args.size;
}

} // end namespace
