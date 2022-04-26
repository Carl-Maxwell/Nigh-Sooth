#pragma once

#include "lapse_scalar.h"

namespace lapse{

// returns -1.0f or 1.0f whichever is nearer
f32 sign(f32 a);

f64 sign(f64 a);

f32 abs(f32 a);

f64 abs(f64 a);

i64 floor_i(f64 whole);

i32 floor_i(f32 whole);

f32 floor_f(f32 whole);

i32 ceil_i(f32 whole);

f32 ceil_f(f32 whole);

// TODO need to account for floats larger than i32 max

f64 round(f64 whole);

f32 round(f32 whole);

// TODO I guess everything in this header should be inline and const

f64 pow(f32 base, f32 exponent);

// log base b of big_number
//   "log base 2 of 8 is asking, 2 to the what equals 8?"
//   base**? = big_number
i32 logarithm_i(f32 base, f32 big_number);

f64 sqrt(f64 big_number);

// hasn't been implemented yet
f32 root(f32 exponent, f32 big_number);

}
