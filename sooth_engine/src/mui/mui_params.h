#pragma once

#include "lapse_scalar.h"
#include "lapse_geometry.h"

// mui's layout is based on the css box model,
//   though it uses the box-sizing: border-box idea

#include <initializer_list>

namespace mui{

const lapse::f32 mui_auto = lapse::f32_almost_inf; // used for optional params
// transparent here actually means "no color", as in, don't draw this thing
const lapse::vec3<> transparent = {mui_auto, mui_auto, mui_auto};
const lapse::vec3<> default_color = {lapse::f32_almost_inf-1, lapse::f32_almost_inf-1, lapse::f32_almost_inf-1};

struct mui_size : public lapse::vec2<>{
  mui_size() { x = 0; y = 0; };
  mui_size(std::initializer_list<lapse::f32> i_list) {
    auto_size = false;
    lapse::i32 i = 0;
    for (auto elem : i_list) {
      if (i == 0) {
        x = elem;
      } else if (i == 1){
        y = elem;
      } else {
        return;
      }
      i++;
    }

    // TODO for some reason you can't just do:
    // x = i_list[0];
    // y = i_list[1];
    // with std::initializer_list
    // Dunno if there's some more sane way of getting two elements out of an iterator.
  };

  bool auto_size = true;
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

  lapse::vec2<> top_left()     { return lapse::vec2<>{left(),    top() }; };
  lapse::vec2<> bottom_right() { return lapse::vec2<>{right(), bottom()}; };

  // returns the sum of the properties, so for {margin: 10px;} this would be 20px, 20px
  lapse::vec2<> to_vec2() { return lapse::vec2<>{right() + left(), top() + bottom()}; };

  inline bool operator!() const { return !((prop.x + prop.y + prop.z + prop.w) > 0); };
};

struct params{
  mui_size m_size; // width & height of entire box

  lapse::vec2<> m_position = {0, 0}; // context.current_position at params creation time

  box_property margin{0};  // white space outside the box
  box_property padding{0}; // white space inside the box (reduces content_area)
  box_property border{0};  // thickness of border at box's edge

  lapse::vec3<> background_color = transparent;
  const static lapse::i32 has_no_parent = lapse::i32_min;
  lapse::i32 parent_index = has_no_parent;
  const static lapse::i32 has_no_index = lapse::i32_min;
  lapse::i32 index = has_no_index;

  lapse::vec3<> font_color = default_color;

  params* parent();
  lapse::vec2<> position();
  mui_size size();

  lapse::rect<> box_area() { return lapse::rect<>{position(), m_size}; };

  lapse::rect<> margin_area() {
    auto output = box_area();
    output.position -= margin.top_left();
    output.size += margin.to_vec2();

    return output;
  };

  lapse::rect<> content_area() {
    return lapse::rect<>{
      position() + padding.top_left() + border.top_left(),
      m_size - padding.to_vec2() - border.to_vec2()
    };
  };
};

}; // end namespace
