
#include "lapse_scalar.h"
#include "lapse_math.h"

namespace lapse{

char u32_to_ascii(u32 i) { return '0' + i; };

char* u32_to_c_str(u32 in) {
  char* output;

  if (in == 0) { return (char*)"0"; }

  u32 length = logarithm_i(10, f32(in));

  u32 str_pos = 0;
  u32 tens = 1; // which tens place we're looking at
  while (str_pos < length) {
    tens *= 10;
    str_pos++;
  }

  length += 1;

  output = new char[length + 1]; // +1 accounts for null terminator
  str_pos = 0;
  
  while (str_pos < length) {
    output[str_pos++] = '0' + in/tens;
    in -= in/tens*tens;
    tens /= 10;
  }

  output[str_pos++] = '\0';

  return output;
};

};
