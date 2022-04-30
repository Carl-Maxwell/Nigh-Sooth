#pragma once

#include "lapse_scalar.h"

namespace mui{

struct Context{
  lapse::vec2<> current_position = {0, 0};
  // TODO active element

  // returns singleton
  static Context& the() {
    static Context* my_context = nullptr;
    if (!my_context) {
      my_context = new Context;
    }
    return *my_context;
  };

  void reset() {
    current_position = {0, 0};
  }
};

} // end namespace
