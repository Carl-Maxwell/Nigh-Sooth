#include "mui/mui_params.h"

#include "mui/mui_context.h"

#include <lapse_lib.h>

using namespace lapse;

namespace mui{

params* params::parent() {
  if (parent_index == has_no_parent) { return nullptr; }
  auto& context = Context::the();
  assert(context.dom_stack.last_index() >= parent_index);
  return &context.dom_stack[parent_index];
}

lapse::vec2<> params::position() {
  auto& context = Context::the();

  auto* container = parent();
  if (container) {
    if (index == parent_index+1) {
      // first child
      return container->content_area().top_left_point();
    } else {
      // nth child
      auto i = index-1;
      return context.dom_stack[i].margin_area().bottom_left_point() + margin.top_left();
    }
  } else {
    // TODO should this just return context->current_position?
    return m_position;
  }
}

} // end namespace
