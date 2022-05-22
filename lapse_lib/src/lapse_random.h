#pragma once

#include "lapse_scalar.h"

namespace lapse{

// seed the random number generator
void seed_rng(u32 seed);
// return a random u64 from 0 to u64_max
u64 rand_integer();
// return a random u64 from 0 to end
u64 rand_integer(u64 end);
// returns a number from 0..1
f64 rand_float();
// returns a number in range min..max
f64 rand_range(f64 min, f64 max);
// returns an f32 in range 0..1
f32 rand_f32();
// returns an f32 in range 0..max
f32 rand_f32(f32 max);
// return an f32 in range min..max
f32 rand_f32(f32 min, f32 max);
// roll 1d{sides}
u32 die(u32 sides);
// roll {n}d{sides}
u32 die(u32 n, u32 sides);
// returns a vec2<f32> with random values from 0..1
vec2<> rand_vec2();
// returns a vec2<> with random values from 0..x, 0..y
vec2<> rand_vec2(vec2<> limits);
// returns a vec3<f32> with random values from 0..1
vec3<> rand_vec3();

}
