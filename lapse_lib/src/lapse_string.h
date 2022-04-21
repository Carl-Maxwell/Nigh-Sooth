#pragma once

#include "lapse_lib.h"
#include "lapse_array.h"

namespace lapse{

struct str{
  array<u8> characters;

  array<u8> to_a() { return characters; };
};

};