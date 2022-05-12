#pragma once

#include "lapse_scalar.h"

namespace lapse{

template<typename T=lapse::f32>
struct rect{
  vec2<T> position;
  vec2<T> size;

  vec2<T> top_left_point()     { return position; }
  vec2<T> top_right_point()    {
    vec2 temp = position;
    temp.x += size.x;
    return temp;
  }
  vec2<T> bottom_left_point()  {
    vec2 temp = position;
    temp.y += size.y;
    return temp;
  }
  vec2<T> bottom_right_point() { return position + size; }

  bool is_point_inside(vec2<> point) {
    return point > position &&
      point < (position + size);
  }
  // scale rect
  rect& operator*=(f32 factor) {
    position *= factor;
    size *= factor;
    return *this;
  }
};

}
