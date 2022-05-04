#pragma once

#include "lapse_scalar.h"
#include "lapse_random.h"

namespace lapse{

struct uid{
  u64 id;
  uid() { id = rand_integer(); };
  // TODO less than operator
  // TODo == operator
};

} // end namespace
