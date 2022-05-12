#include "mui/mui_draw.h"

#include <lapse_lib.h>
#include "mui/mui_params.h"
#include "mui/mui_context.h"
#include "mui/mui_text.h"
#include "platform_olc_pixel.h"

using namespace lapse;

namespace mui{

void draw_rect(params args) {
  DrawCallArray::push(DrawCall{
    args.index,
    [](DrawCall& call){
      auto& context = Context::the();
      auto& args = context.dom_stack[call.element_index];

      auto box = args.box_area();
      box *= ContextScale::the().scale();

      assert(
        box.position.x >= 0 && box.position.y >= 0
        && box.bottom_right_point() < platform::get_window_size()
      );

      // TODO border_color
      platform::draw_rect(box);
    }
  });
}

void fill_rect(params args) {
  DrawCallArray::push(DrawCall{
    args.index,
    [](DrawCall& call){
      auto& context = Context::the();
      auto& args = context.dom_stack[call.element_index];

      auto box = args.box_area();
      box *= ContextScale::the().scale();

      assert(
        box.position.x >= 0 || box.position.y >= 0
        && box.bottom_right_point() < platform::get_window_size()
      );

      if (args.background_color != mui::default_color) {
        platform::fill_rect(box, args.background_color);
      } else {
        platform::fill_rect(box);
      }
    }
  });
}

void draw_text(str& text, params args) {
  DrawCallArray::push(DrawCall{
    args.index,
    [&text](DrawCall& call){
      auto& context = Context::the();
      auto& args = context.dom_stack[call.element_index];
      auto position = args.content_area().top_left_point();
      position *= ContextScale::the().scale();
      if (args.font_color != default_color) {
        platform::draw_text(text, position, args.font_color, font_size());
      } else {
        platform::draw_text(text, position, vec3<>{1.0, 1.0, 1.0}, font_size());
      }

      // TODO args.font_color
    }
  });
}

void draw() {
  auto& draw_call_array = DrawCallArray::the();

  for (i32 i = 0; i < draw_call_array.m_calls.length(); i++) {
    auto& call = draw_call_array.m_calls[i];
    call.draw(call);
  }

  draw_call_array.clear();

  std::cout << "mui scale: " << ContextScale::the().scale() << "\n";
}

} // end namespace
