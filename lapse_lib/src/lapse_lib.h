#pragma once

#include <stdint.h>

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

typedef float f32;
typedef double f64;

}; // namespace

#include "lapse_math.h"

#include "lapse_exceptions.h"
#include "lapse_array.h"
#include "lapse_string.h"

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
