#pragma once

#include "lapse_scalar.h"
#include "mui/mui_params.h"

namespace mui{

struct Context{
  lapse::vec2<> current_position = {0, 0};
  lapse::array<params> dom_stack;
  // TODO active element? Is it just the end of dom_stack?

  // returns singleton
  static Context& the() {
    static Context* my_context = nullptr;
    if (!my_context) {
      my_context = new Context;
      my_context->dom_stack.reserve(4); // lapse::arrays don't allocate until told to
    }
    return *my_context;
  };

  void reset() {
    current_position = {0, 0};
  }
  void push_container(params args) {
    dom_stack.push(args);
    current_position += args.padding.top_left();
    // TODO margins
  };
  params& pop_container() {
    auto& args = dom_stack.pop();
    current_position -= args.padding.bottom_right();
    // TODO margins
    return args;
  };

  // TODO push_element? push_node?
  void push_element(lapse::rect box) {
    current_position.y += box.size.y;
    // TODO margins
    for (lapse::i32 i = dom_stack.last_index(); i >= 0; i--) {
      // if (dom_stack[i].size.x == mui_auto) {
        // if (dom_stack[i].content_area()) {

        // }
      // }
      // if (dom_stack[i].size.y == mui_auto) {

      // }
    }
  };
};

} // end namespace
