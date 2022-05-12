#include "mui/mui_context.h"

#include <lapse_lib.h>
#include "mui_draw.h"

using namespace lapse;

namespace mui{

void Context::reset() {
  current_position = {0, 0};
  mui::DrawCallArray::clear();
  if (dom_stack.length()) {
    dom_stack.clear();
    dom_stack.reserve(4);
  }
  current_container_index = params::has_no_parent;
  auto& scale = ContextScale::the();
  scale.old_scale = scale.scale();
  scale.m_size_needed = platform::get_window_size();
}

} // end namespace
