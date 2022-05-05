#include "mui/mui_div.h"

#include "platform_olc_pixel.h"

#include "mui/mui_context.h"
#include "mui/mui_draw.h"

#include <lapse_lib.h>

using namespace lapse;

namespace mui{

void open_div(params& args) {
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
  auto& args = context.close_container();
}

} // end namespace
