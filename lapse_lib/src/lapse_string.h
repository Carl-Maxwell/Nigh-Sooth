#pragma once

#include "lapse_scalar.h"
#include "lapse_array.h"
#include "lapse_yaml.h"

namespace lapse{

struct str{
  array<u8> m_characters;

  str();
  str(const char* c_str);
  str(char* c_str);
  str(char c_str);

  str(const str& other) {
    std::cout << "creating a copy of str(" << other.to_c_str() << ")\n";
    *this = other;
  }

  str(str& other) {
    std::cout << "creating a copy of str(" << other.to_c_str() << ")\n";
    *this = other;
  }

  ~str() {
    std::cout << "~str(" << to_c_str() << ")\n";
  }
  // inline ~str() noexcept {}

  // interprets this string as a binary number
  u64 as_binary();
  // interprets this string as a hexadecimal number
  u64 as_hexadecimal();
  // returns ascii code of first character
  u32 ascii_code();
  void clear();
  // add str onto the end of this str
  str& concat(const str& other);
  u32 length() const;
  str slice();
  // remove non-ascii characters from string
  str scrub();
  // break str into array of strings
  array<str> split(str sep);
  array<u8> to_array();
  char* to_c_str() const;
  u32 to_c_str_estimate_length();
  f32 to_f32();
  f64 to_f64();
  template<typename T>
  T to_integer();
  str to_str();
  yaml to_yaml();
  // remove leading and trailing whitespace
  str trim();

  str  operator+(str& right_value);
  str& operator+=(const str& right_value);
  str& operator=(str& right_value);
  str& operator=(const str& right_value);
  str& operator=(char* right_value);
  str& operator=(const char* right_value);
  char operator[](u32 index) const;

  // TODO should we support regex?

};

};
