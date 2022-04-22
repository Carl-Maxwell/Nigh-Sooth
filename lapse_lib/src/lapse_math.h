#pragma once

#include "lapse_lib.h"

namespace lapse{

// returns -1.0f or 1.0f whichever is nearer
f32 sign(f32 a) {
  return a < 0 ? -1.0f : 1.0f;
};

f32 abs(f32 a) {
  return sign(a) * a;
}

i32 floor_i(f32 whole) {
  return whole > 0 ? i32(whole) : i32(whole)-1;
};

f32 floor_f(f32 whole) {
  return whole > 0 ? f32(i32(whole)) : f32(i32(whole))-1;
};

i32 ceil_i(f32 whole) {
  return whole > 0 ? f32(abs(i32(whole))+1) : i32(whole);
};

f32 ceil_f(f32 whole) {
  return whole > 0 ? f32(i32(whole)+1) : f32(i32(whole));
};

// TODO need to account for floats larger than i32 max

f32 round(f32 whole) {
  i32 integer_part = floor_i(abs(whole));
  return sign(whole) * f32( 
    abs(whole) - f32(integer_part) < 0.5 ? integer_part : integer_part+1
  );
};

// TODO I guess everything in this header should be inline and const

}
