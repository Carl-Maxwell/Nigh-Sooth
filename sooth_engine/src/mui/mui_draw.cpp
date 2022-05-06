#include "mui/mui_draw.h"

#include <lapse_lib.h>
#include "mui/mui_params.h"
#include "mui/mui_context.h"
#include "platform_olc_pixel.h"

using namespace lapse;

namespace mui{

void draw_rect(params args) {
  DrawCallArray::push(DrawCall{
    args.index,
    [](DrawCall& call){
      auto& context = Context::the();
      auto& args = context.dom_stack[call.element_index];
      // TODO border_color
      platform::draw_rect(args.box_area());
    }
  });
}

void fill_rect(params args) {
  DrawCallArray::push(DrawCall{
    args.index,
    [](DrawCall& call){
      auto& context = Context::the();
      auto& args = context.dom_stack[call.element_index];
      if (args.background_color != mui::default_color) {
        platform::fill_rect(args.box_area(), args.background_color);
      } else {
        platform::fill_rect(args.box_area());
      }
    }
  });
}

void draw_text(str text, params args) {
  DrawCallArray::push(DrawCall{
    args.index,
    [](DrawCall& call){
      auto& context = Context::the();
      auto args = context.dom_stack[call.element_index];
      if (args.font_color != default_color) {
        platform::draw_text(call.text, args.content_area().top_left_point(), args.font_color);
      } else {
        platform::draw_text(call.text, args.content_area().top_left_point());
      }


      // TODO args.font_color
    },
    text
  });
}

void draw() {
  auto& draw_call_array = DrawCallArray::the();

  for (i32 i = 0; i < draw_call_array.m_calls.length(); i++) {
    auto& call = draw_call_array.m_calls[i];
    call.draw(call);
  }

  draw_call_array.clear();
}

} // end namespace