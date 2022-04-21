#pragma once

#include "lapse_lib.h"

namespace lapse{

template<typename T>
struct func{
  void (*callable)(T);

};

};
