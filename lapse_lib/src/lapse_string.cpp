
#include "lapse_string.h"

#include "lapse_lib.h"

namespace lapse{

str::str() {}

str::str(const char* c_str) {
  u32 i = 0;
  while (c_str[i] != '\0') { characters.push(c_str[i]); i++; }
};

str::str(char* c_str) {
  u32 i = 0;
  while (c_str[i] != '\0') { characters.push(c_str[i]); i++; }
};

str::str(char c_str) { characters.push(c_str); }

u64 str::as_binary() { return 0; }

u64 str::as_hexadecimal() { return 0; }

u32 str::ascii_code() { return (u32)characters[0]; }

void str::clear() { characters.clear(); }

str& str::concat(const str& other) {
  characters += other.characters;
  return *this;
}

u32 str::length() { return characters.length; }

str str::slice() { return *this;  }

str str::scrub() { return *this; }

array<str> str::split(str sep) {
  for (u32 i = 0; i < length(); i++) {
    __debugbreak();
    if (characters[i] == sep[0]) {
      bool match = true;
      for (u32 i2 = 1; i2 < sep.length(); i2++) {
        match &= characters[i + i2] == sep[i2];
      }
    }
  }

  return array<str>{};
}

array<u8> str::to_array() { return characters; };

char* str::to_c_str() { 
  char* c = new char[characters.length+1];
  for (u32 i = 0; i < characters.length; i++) {
    c[i] = (char)characters[i];
  }
  c[characters.length] = '\0';
  return c;
};

u32 str::to_c_str_estimate_length() {
  return characters.length+1;
}

f32 str::to_f32() { return 0; }

f64 str::to_f64() { return 0; }

template<typename T>
T str::to_integer() {
  T output;

  output = (T)length();

  return output;
}

// explicit templates of to_integer for each integer scalar type
//   never call this func. It's just here to generate templates
void _explicit_instatiation_for_str_to_integer() {
  str noop;

  noop.to_integer<u32>();
}

//

str str::to_str() { return *this; }

yaml str::to_yaml() { return yaml{}; }

str str::trim() { return str{}; }

str& str::operator+(str right_value) {
  str temp_str = *this;
  temp_str += right_value;
  return temp_str;
};

str& str::operator+=(const str right_value) {
  return concat(right_value);
};

str& str::operator=(str right_value) {
  characters = right_value.characters;
  return *this;
}

str& str::operator=(char* right_value) {
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

char str::operator[](u32 index) {
  return (char)characters[index];
}

}
