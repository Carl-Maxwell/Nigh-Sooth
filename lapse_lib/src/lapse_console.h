#pragma once

#include "lapse_string.h"

namespace lapse{

struct Console{
  static void h1(str heading);
  static void h2(str heading);
  static void h3(str heading);
  static void log(str a_string);
};


} // end namespace
