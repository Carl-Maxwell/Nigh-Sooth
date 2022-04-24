#pragma once

#include "lapse_lib.h"

namespace lapse{

// return a random u64 from 0 to u64_max
u64 rand_integer();
// returns a number from 0..1
f64 rand_float();
// returns a number in range min..max
f64 rand_range(f64 min, f64 max);
// roll 1d{sides}
u32 die(u32 sides);
// roll {n}d{sides}
u32 die(u32 n, u32 sides);

}