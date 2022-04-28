#pragma once

#include <stdint.h>

#include <iostream>

namespace lapse{

//-----------------------------------------------------------------------------
// Basic Scalar types (ints & floats)
//-----------------------------------------------------------------------------

// signed integers

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// unsigned integers

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// integer limits

const u64 i8_max  = 128;                       // 2**8/2
const u64 i16_max = 32'768;                    // 2**16/2
const u64 i32_max = 2'147'483'648;             // 2**32/2
const u64 i64_max = 9'223'372'036'854'775'808; // 2**64/2 // TODO don't these need -1s?

const u64 u8_max  = u8 (-1); // 2**8
const u64 u16_max = u16(-1); // 2**16
const u64 u32_max = u32(-1); // 2**32
const u64 u64_max = u64(-1); // 2**64

// floats

// typedef float  f16;  // IEEE 754 16-bit half precision (binary16)
typedef float  f32;  // IEEE 754 32-bit single precision (binary32)
typedef double f64; // IEEE 754 64-bit double precision (binary64)

// float constants:

const u64 f32_significant_digits   =  7;
const u64 f32_exponent_max         =  127;
const u64 f32_exponent_min         =  126;
const u64 f32_exponent_bias        =  127;
const u64 f32_exponent_max_base_10 =  38;
const i64 f32_exponent_min_base_10 = -37;

const u64 f32_mantissa_max         = 8'388'608; // 2**23

// f32 memory layout: 1 bit sign, 8 bits exponent, 23 bits mantissa

const u64 f32_exponent_bits        = 8;
const u64 f32_mantissa_bits        = 23;

const u32 f32_bitmask_sign_bit     = 0b1000'0000'0000'0000'0000'0000'0000'0000;
const u32 f32_bitmask_exponent     = 0b0111'1111'1000'0000'0000'0000'0000'0000;
const u32 f32_bitmask_mantissa     = 0b0000'0000'0111'1111'1111'1111'1111'1111;

// f64 constants

const u64 f64_significant_digits   =  16; // 15 to 17
const u64 f64_exponent_max         =  1023;
const u64 f64_exponent_min         = -1022;
const u64 f64_exponent_bias        =  1023;
const u64 f64_exponent_max_base_10 =  38; // 307
const i64 f64_exponent_min_base_10 = -37; // 307

// TODO need to test out these f32 & f64 constants

//-----------------------------------------------------------------------------
// Algebraic vectors, also used for specifying colors
//-----------------------------------------------------------------------------

template<typename T=f32>
struct vec2{
  union { T x, r; };
  union { T y, g; };

  vec2  operator+(vec2 right_value) {
    vec2 temp;

    temp.x = x + right_value.x;
    temp.y = y + right_value.y;

    return temp;
    // TODO check for lifetime issues
  }
};

template<typename T=f32>
struct vec3{
  union { T x, r; };
  union { T y, g; };
  union { T z, b; };

  template<typename DisplayType=f32>
  void std_cout() {
    std::cout << " vec3 {\n"
      << "\tx: " << (DisplayType)x << "\n"
      << "\ty: " << (DisplayType)y << "\n"
      << "\tz: " << (DisplayType)z << "\n"
      << "}\n";
  }
};

template<typename T=f32>
struct vec4{
  union { T x, r; };
  union { T y, g; };
  union { T z, b; };
  union { T w, a; };
};

// TODO need vector math funcs

//-----------------------------------------------------------------------------
// Angles
//-----------------------------------------------------------------------------

// describes a euler angle
struct euler{
  f32 pitch;
  f32 yaw;
  f32 roll;
  // TODO euler funcs 
};

// used for 3D rotations
struct quaternion{
  vec4<> components;
  // TODO quaternion funcs
};

//-----------------------------------------------------------------------------
// Matrices
//-----------------------------------------------------------------------------

// TODO mat4x4

};
