#pragma once

#include "lapse_lib.h"
#include "lapse_array.h"
#include "lapse_yaml.h"

namespace lapse{

struct str{
  array<u8> characters;

  str() {}
  str(const char* c_str) {
    u32 i = 0;
    while (c_str[i] != '\0') { characters.push(c_str[i]); i++; }
  };
  str(char* c_str) {
    u32 i = 0;
    while (c_str[i] != '\0') { characters.push(c_str[i]); i++; }
  };
  str(char c_str) { characters.push(c_str); }

  // interprets this string as a binary number
  u64 as_binary();
  // interprets this string as a hexadecimal number
  u64 as_hexadecimal();
  // returns ascii code of first character
  u32 ascii_code() { return (u32)characters[0]; }
  void clear() { characters.clear(); }
  // add str onto the end of this str
  str& concat(const str& other) {
    characters += other.characters;
    return *this;
  }
  // TODO length is not a function in array containers, this is an inconsistency in the API
  u32 length() { return characters.length; }
  str slice() {}
  // remove non-ascii characters from string
  str scrub() {}
  // break str into array of strings
  array<str> split(str sep) {
    for (u32 i = 0; i < length(); i++) {
      __debugbreak();
      if (characters[i] == sep[0]) {
        bool match = true;
        for (u32 i2 = 1; i2 < sep.length(); i2++) {
          match &= characters[i + i2] == sep[i2];
        }
      }
    }
  }
  array<u8> to_array() { return characters; };
  char* to_c_str() { 
    char* c = new char[characters.length+1];
    for (u32 i = 0; i < characters.length; i++) {
      c[i] = (char)characters[i];
    }
    c[characters.length] = '\0';
    return c;
  };
  u32 to_c_str_estimate_length() {
    return characters.length+1;
  }
  f32 to_f32() {}
  f64 to_f64() {}
  template<typename T>
  T to_integer() { return characters;}
  str to_str() { return *this; }
  yaml to_yaml() { ; }
  // remove leading and trailing whitespace
  str trim() {}

  str& operator+(str right_value) {
    str temp_str = *this;
    temp_str += right_value;
    return temp_str;
  };
  str& operator+=(const str right_value) {
    return concat(right_value);
  };
  str& operator=(str right_value) {
    characters = right_value.characters;
    return *this;
  }
  str& operator=(char* right_value) {
    clear();
    u32 n_length = 0;
    {
      u32 i = 0;
      while (right_value[i++] != '\0') { n_length++; }
    }

    characters.reserve(n_length);

    u32 i = 0;
    while (right_value[i] != '\0') { characters.push(right_value[i]); i++; }

    return *this;
  }
  char operator[](u32 index) {
    return (char)characters[index];
  }

  // TODO should we support regex?

};

};