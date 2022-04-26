
#include "lapse_string.h"

#include "lapse_lib.h"

namespace lapse{

str::str() : m_characters(1) {}

str::str(const char* c_str) {
  u32 c_str_length = 0;
  while (c_str[c_str_length++] != '\0') { }
  m_characters.reserve(c_str_length);

  u32 i = 0;
  while (c_str[i] != '\0') { m_characters.push(c_str[i]); i++; }
};

str::str(char* c_str) {
  u32 c_str_length = 0;
  while (c_str[c_str_length++] != '\0') { }
  m_characters.reserve(c_str_length);

  u32 i = 0;
  while (c_str[i] != '\0') { m_characters.push(c_str[i]); i++; }
};

str::str(char c_str) { m_characters.reserve(3); m_characters.push(c_str); }

u64 str::as_binary() { return 0; }

u64 str::as_hexadecimal() { return 0; }

u32 str::ascii_code() { return (u32)m_characters[0]; }

void str::clear() { m_characters.clear(); }

str& str::concat(const str& other) {
  m_characters += other.m_characters;
  return *this;
}

u32 str::length() { return m_characters.length(); }

str str::slice() { return *this; }

str str::scrub() { return *this; }

// not implemented yet
array<str> str::split(str sep) {
  for (u32 i = 0; i < length(); i++) {
    __debugbreak();
    if (m_characters[i] == sep[0]) {
      bool match = true;
      for (u32 i2 = 1; i2 < sep.length(); i2++) {
        match &= m_characters[i + i2] == sep[i2];
      }
    }
  }

  return array<str>{};
}

array<u8> str::to_array() { return m_characters; };

char* str::to_c_str() {
  char* c = new char[m_characters.length()+1];
  for (u32 i = 0; i < m_characters.length(); i++) {
    c[i] = (char)m_characters[i];
  }
  c[m_characters.length()] = '\0';
  return c;
};

u32 str::to_c_str_estimate_length() {
  return m_characters.length() + 1;
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

str str::operator+(str right_value) {
  str temp_str = *this;
  temp_str += right_value;
  return temp_str;
};

str& str::operator+=(const str right_value) {
  return concat(right_value);
};

str& str::operator=(str right_value) {
  m_characters = right_value.m_characters;
  return *this;
}

str& str::operator=(char* right_value) {
  clear();
  u32 better_length = 0;
  {
    u32 i = 0;
    while (right_value[i++] != '\0') { better_length++; }
  }

  m_characters.reserve(better_length);

  u32 i = 0;
  while (right_value[i] != '\0') { m_characters.push(right_value[i]); i++; }

  return *this;
}

char str::operator[](u32 index) {
  return (char)m_characters[index];
}

}
