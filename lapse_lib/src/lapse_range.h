#pragma once

// #include "lapse_scalar.h"

// #include "lapse_array.h"

namespace lapse{

// returns an array filled with i32s from start to end
//   so (1, 3) returns [1, 2, 3]
//   note that the array is allocated as temporary
fixed_array<i32>& range(i32 start, i32 end);

};
