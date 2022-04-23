#pragma once

#include "lapse_lib.h"

#include "intrin.h"

namespace lapse{

// returns -1.0f or 1.0f whichever is nearer
f32 sign(f32 a) {
  return a < 0 ? -1.0f : 1.0f;
};

f64 sign(f64 a) {
  return a < 0 ? -1.0 : 1.0;
};

f32 abs(f32 a) {
  return sign(a) * a;
}

f64 abs(f64 a) {
  return sign(a) * a;
}

i32 floor_i(f32 whole) {
  return whole > 0 ? i32(whole) : i32(whole)-1;
};

f32 floor_f(f32 whole) {
  return whole > 0 ? f32(i32(whole)) : f32(i32(whole))-1;
};

i32 ceil_i(f32 whole) {
  return whole > 0 ? f32(abs(f32(i32(whole)))+1) : i32(whole);
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

f64 pow(f32 base, f32 exponent) {
  if (base == 0) { return 0; }

  f64 temp = 1;
  for (int i = 0; i < exponent; i++) {
    temp *= base;
  }
  return temp;
}

// log base b of big_number
//   "log base 2 of 8 is asking, 2 to the what equals 8?"
//   base**? = big_number
i32 logarithm_i(f32 base, f32 big_number) {
  i32 exponent = 1;
  for ( ; pow(base, exponent) < big_number; exponent++ ) {}
  return exponent;
}

f32 sqrt(f32 big_number) {
  auto y = _mm_sqrt_pd({ big_number, double(0) });
  return y.m128d_f64[0];
}

// hasn't been implemented yet
f32 root(f32 exponent, f32 big_number) {
  f32 base = 0;
  // TODO calculate root
  return base;
}

}
