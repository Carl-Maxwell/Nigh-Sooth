#pragma once

#include "lapse_lib.h"
#include "lapse_array.h"
#include "lapse_yaml.h"

namespace lapse{

struct str{
  array<u8> characters;

  // interprets this string as a binary number
  u64 as_binary();
  // interprets this string as a hexadecimal number
  u64 as_hexadecimal();
  char* c_str() { 
    char* c = new char[characters.length];
    for (u32 i = 0; i < characters.length; i++) {
      c[i] = (char)characters[i];
    }
    return c;
  };
  // returns ascii code of first character
  u32 ascii_code() { return (u32)characters[0]; }
  void clear() { characters.clear(); }
  // TODO length is not a function in array containers, this is an inconsistency in the API
  u32 length() { return characters.length; }
  str slice() {}
  // remove non-ascii characters from string
  str scrub() {}
  str split() {}
  array<u8> to_array() { return characters; };
  f32 to_f32() {}
  f64 to_f64() {}
  template<typename T>
  T to_integer() { return characters;}
  yaml to_yaml() { ; }
  // remove leading and trailing whitespace
  str trim() {}

  // TODO should we support regex?

};

};