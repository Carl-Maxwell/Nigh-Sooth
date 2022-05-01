#pragma once

#include "lapse_scalar.h"

namespace lapse{

// TODO should rect have a template for which types the vec2s are?

struct rect{
  vec2<> position;
  vec2<> size;

  vec2<> top_left_point()     { return position; }
  vec2<> top_right_point()    {
    vec2 temp = position;
    temp.x += size.x;
    return temp;
  }
  vec2<> bottom_left_point()  {
    vec2 temp = position;
    temp.y += size.y;
    return temp;
  }
  vec2<> bottom_right_point() { return position + size; }
};

}
