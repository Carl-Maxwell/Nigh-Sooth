#pragma once

#include <lapse_scalar.h>
#include <lapse_exceptions.h>

#include "mui/mui_params.h"
#include "sooth_input.h"

namespace mui{

struct Context{
  lapse::vec2<> current_position = {0, 0};
  lapse::array<params> dom_stack;
  lapse::array<lapse::i32> container_indexes;
  lapse::i32 current_container_index = params::has_no_parent;

  // returns singleton
  static Context& the() {
    static Context* my_context = nullptr;
    if (!my_context) {
      my_context = new Context;
      my_context->dom_stack.reserve(4); // lapse arrays don't allocate until told to
      my_context->container_indexes.reserve(3);
    }
    return *my_context;
  };

  void reset();
  
  // TODO this should just be called open_container
  void push_container(params& args) {
    args.m_position += current_position;
    if (current_container_index != params::has_no_parent) {
      args.parent_index = current_container_index;
    }

    dom_stack.push(args);
    auto index = dom_stack.last_index();
    current_container_index = index;
    container_indexes.push(index);
    dom_stack[index].index = index;
    args.index = index;

    current_position = args.content_area().top_left_point();
  };

  params& close_container() {
    lapse::assert(dom_stack.last_index() >= current_container_index);

    auto& args = dom_stack[current_container_index];
    container_indexes.pop();
    if (container_indexes.length()) {
      current_container_index = container_indexes.at_reverse(-1);
    } else {
      current_container_index = params::has_no_parent;
    }
    
    args.size(); // calculate current size
    current_position = args.margin_area().bottom_left_point();
    return args;
  };
  
  void open_element(params& args) {
    args.m_position += current_position;
    if (current_container_index != params::has_no_parent) {
      args.parent_index = current_container_index;
    }
    dom_stack.push(args);
    auto index = dom_stack.last_index();
    dom_stack[index].index = index;
    args.index = index;
  };

  void close_element() {
    current_position.y += dom_stack.at_reverse(-1).size().y;
  };

  bool is_hovering() {
    auto mouse_pos = Mouse::get_mouse_pos();
    return dom_stack.at_reverse(-1).box_area().is_point_inside(mouse_pos);
  };

  bool is_active() {
    return is_hovering() && Mouse::left_mouse_hit();
  };
};

} // end namespace
