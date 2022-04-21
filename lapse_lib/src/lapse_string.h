#pragma once

// TODO should this just be called str?

#include "lapse_lib.h"
#include "lapse_array.h"

namespace lapse{

struct string{
  array<u8> characters;

  array<u8> to_a() { return characters; };
};

};