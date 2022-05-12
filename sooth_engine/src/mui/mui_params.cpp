#include "mui/mui_params.h"

#include "mui/mui_context.h"

#include <lapse_lib.h>

#include "platform_olc_pixel.h"

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
    bool is_first_child = index == parent_index+1;

    // find sibling just above this one
    //   note that the element at dom_stack[index-1] might be inside of a different container,
    //   where that container itself is actually the sibling
    auto i = index-1;
    while (context.dom_stack[i].parent_index != parent_index && i > parent_index && i >= 0) { 
      i--;
    }
    // if we didn't find a sibling
    if (i < 0 || i == parent_index) { is_first_child = true; }

    if (is_first_child) {
      // first child
      return container->content_area().top_left_point() + margin.top_left();
    } else {
      // nth child
      return 
        context.dom_stack[i].margin_area().bottom_left_point()
        + margin.top_left()
        // + vec2<>{0, 1}
      ;
    }
  } else {
    return m_position;
  }
  std::cout << "\n";
}

// calculate the size of this element
mui_size params::size() {
  auto& context = Context::the();

  if (m_size.auto_size) {
    auto keep_looping = true;
    m_position = position();

    for (u32 i = index+1; i < context.dom_stack.length() && keep_looping; i++) {
      if (context.dom_stack[i].parent_index == index) {
        // TODO here we're assuming each element occupies only one line
        auto child_width =
            context.dom_stack[i].m_size.x
          + padding.left()
          + border.left();
        
        m_size.x = max(m_size.x, child_width);

        auto child_bottom_left_y =
            context.dom_stack[i].margin_area().bottom_left_point().y
          - m_position.y;
        
        m_size.y = max(m_size.y, child_bottom_left_y);
      }
    }
    m_size += padding.bottom_right();
    m_size += border.bottom_right();
  }

  return m_size;
}


} // end namespace
