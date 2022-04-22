#pragma once

#include "lapse_lib.h"
#include "lapse_array.h"

namespace lapse{

struct str{
  array<u8> characters;

  array<u8> to_a() { return characters; };
  char* c_str() { 
    char* c = new char[characters.length];
    for (u32 i = 0; i < characters.length; i++) {
      c[i] = (char)characters[i];
    }
    return c;
  };
};

};