#pragma once

#include "lapse_geometry.h"

// mui's layout is based on the css box model,
//   though it uses the box-sizing: border-box idea

namespace mui{

const lapse::f32 mui_auto = 3.4028235e+38f; // almost inf, used for optional params
// transparent here actually means "no color", as in, don't draw this thing
const lapse::vec3<> transparent = {mui_auto, mui_auto, mui_auto}; 

struct mui_size{
  lapse::vec2<> actual_size = {0, 0};
  bool auto_size     = true;
};

struct box_property{
  lapse::vec4<> prop = {0, 0, 0, 0};

  box_property(lapse::f32 one) {
    prop = {one, one, one, one};
  };
  box_property(lapse::f32 top_bottom, lapse::f32 left_right) {
    prop = {top_bottom, left_right, top_bottom, left_right};
  };
  box_property(lapse::f32 a_top, lapse::f32 a_right, lapse::f32 a_bottom, lapse::f32 a_left) {
    prop = {a_top, a_right, a_bottom, a_left};
  };

  lapse::f32& top()    { return prop[0]; };
  lapse::f32& right()  { return prop[1]; };
  lapse::f32& bottom() { return prop[2]; };
  lapse::f32& left()   { return prop[3]; };

  lapse::vec2<> top_left()     { return lapse::vec2<>{top(),    left() }; };
  lapse::vec2<> bottom_right() { return lapse::vec2<>{bottom(), right()}; };

  lapse::vec2<> to_vec2() { return lapse::vec2<>{top() + bottom(), right() + left()}; };

  inline bool operator!() const { return !((prop.x + prop.y + prop.z + prop.w) > 0); };
};

struct params{
  lapse::vec2<> position = {0, 0};
  lapse::vec2<> size     = {0, 0}; // width & height of entire box

  box_property  margin{0};  // white space outside the box
  box_property  padding{0}; // white space inside the box (reduces content_area)
  box_property  border{0};  // thickness of border at box's edge

  lapse::vec3<> background_color = transparent;

  lapse::rect box_area() { return lapse::rect{position, size}; };
  lapse::rect content_area() {
    return lapse::rect{
      position,
      size - padding.to_vec2() - border.to_vec2()
    };
  };
};

}; // end namespace
