#pragma once

#include "lapse_scalar.h"

namespace lapse{

//-----------------------------------------------------------------------------
// f32_obj
//-----------------------------------------------------------------------------

// The terms window and offset were inspired by this article:
//   https://fabiensanglard.net/floating_point_visually_explained/

// f32 enriched with methods
struct f32_obj{
  // this union allows convenient access to the components of an f32
  union{
    float value;
    // a struct with colons (:) is called a bit field,
    //   each number below specifies the sizeof() that struct member in bits
    struct {
      unsigned int offset : f32_mantissa_bits; // mantissa
      unsigned int window : f32_exponent_bits; // exponent
      unsigned int sign   : 1;
    } bits;
  };

  f32_obj(f64 in) { value = f32(in); };
  f32_obj(f32 in) { value = in; };
  f32_obj(u32 in) { value = f32(in); };
  f32_obj(i32 in) { value = f32(in); };

  i32 exponent() {
    return bits.window - f32_exponent_bias;
  }
  f64 window_start();
  f64 window_end();
  // returns percentage of where this number is in its window
  //   so for 3.0f it returns 0.5 because it's in the middle of the 2..4 window
  f64 window_portion() {
    return f64(bits.offset) / f64(f32_mantissa_max);
  }
  // returns the expected error range of this f32_obj
  f64 precision() {
    return (window_end() - window_start()) / f32_mantissa_max;
  }
  // calculates the value of this f32's components as a f64
  f64 to_f64() {
    return window_start() * (1.0 + f64(bits.offset) / f64(f32_mantissa_max));
  }
  bool is_inf() {
    return exponent() == 128 && bits.offset == 0;
  };
  bool if_finite() { return !is_inf(); };
  bool is_nan() { return bits.sign == 0 && exponent() == 128 && bits.offset != 0; };
};

//-----------------------------------------------------------------------------
// float funcs
//-----------------------------------------------------------------------------

// TODO account for negative numbers
// TODO 987'654'321.123456789 comes out as 987'654'336 ... is that unavoidable with f32?

// returns something like 3,140 = 3.14 * 2**3
char* f32_to_c_str_base2(f32_obj in);

char* f32_to_c_str(f32_obj in);

}