#pragma once

#include <stdint.h>
#include <iostream>

namespace lapse{

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
const u64 i64_max = 9'223'372'036'854'775'808; // 2**64/2

const u64 u8_max  = u8 (-1); // 2**8
const u64 u16_max = u16(-1); // 2**16
const u64 u32_max = u32(-1); // 2**32
const u64 u64_max = u64(-1); // 2**64

// floats

typedef float f16;  // IEEE 754 16-bit half   precision (binary16)
typedef float f32;  // IEEE 754 32-bit single precision (binary32)
typedef double f64; // IEEE 754 64-bit double precision (binary64)

const u64 f32_significant_digits   =  7;
const u64 f32_exponent_max         =  127;
const u64 f32_exponent_min         =  126;
const u64 f32_exponent_max_base_10 =  38;
const i64 f32_exponent_min_base_10 = -37;

const u64 f32_mantissa_max         = 8'388'608; // pow(2, 23)

// 8388608

// f32 memory layout: 1 bit sign, 8 bits exponent, 23 bits mantissa

const u64 f32_exponent_bits        = 8;
const u64 f32_mantissa_bits        = 23;

const u32 f32_sign_bit             = 0b1000'0000'0000'0000'0000'0000'0000'0000;
const u32 f32_bitfield_exponent    = 0b0111'1111'1000'0000'0000'0000'0000'0000;
const u32 f32_bitfield_mantissa    = 0b0000'0000'0111'1111'1111'1111'1111'1111;
// const u32 f32_bitfield_mantissa    = 0b1111'1111'1111'1111'1111'1110'0000'0000;

const u64 f64_significant_digits   =  16;

// this union allows convenient access to the components of an f32
union f32_memory_layout{
  float f;
  // a struct with colons (:) is called a bit field,
  //   each number below specifies the sizeof() that struct member in bits
  struct {
    unsigned int offset : f32_mantissa_bits; // mantissa
    unsigned int window : f32_exponent_bits;  // exponent
    unsigned int sign   : 1;
  } bits;
};

// TODO need to test out these f32 & f64 constants

}; // namespace

#include "lapse_math.h"

#include "lapse_exceptions.h"
#include "lapse_array.h"
#include "lapse_string.h"

#include "lapse_range.h"

// TODO random, rng, etc
// TODO uids

// data structures

// TODO hashmap
// TODO binary_tree
// TODO slot_map (for UIDs)

// math

// TODO vec2, vec3, vec4, mat4x4
// TODO euler, quaternion, etc

// logging utilities

// TODO to_string
