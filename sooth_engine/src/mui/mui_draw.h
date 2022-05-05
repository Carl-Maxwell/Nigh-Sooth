#pragma once

#include "lapse_scalar.h"
#include "lapse_geometry.h"
#include "lapse_string.h"
#include "lapse_lambda.h"
#include "mui/mui_params.h"

namespace mui{

// Draw the stuff that mui has set up to draw
void draw();

struct DrawCall{
  // lapse::rect<> box;
  // lapse::vec3<> color = mui::transparent;
  lapse::i32 element_index;
  lapse_lambda(void, DrawCall&) draw;
  lapse::str text;
};

struct DrawCallArray{
  lapse::array<DrawCall> m_calls;

  // returns singleton
  static DrawCallArray& the() {
    static DrawCallArray* myself = nullptr;
    if (!myself) {
      myself = new DrawCallArray;
      myself->m_calls.reserve(2);
      std::cout << "sizeof(DrawCall) " << sizeof(DrawCall) << " bytes\n";
    }
    return *myself;
  };

  static inline void push(DrawCall call) {
    the().m_calls.push(call);
  }
  static inline void clear() {
    if (the().m_calls.length()) {
      the().m_calls.clear();
      the().m_calls.reserve(4);
    }
  }

};

void draw_rect(params args);
void fill_rect(params args);

void draw_text(lapse::str text, params args);

}