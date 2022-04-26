
#include "lapse_f32.h"

#include "lapse_math.h"
#include "lapse_integer.h"

namespace lapse{

f64 f32_obj::window_start() {
  return pow(2, f32(exponent()));
};
f64 f32_obj::window_end() {
  return pow(2, f32(1 + exponent()));
};


//-----------------------------------------------------------------------------
// float funcs
//-----------------------------------------------------------------------------

// TODO account for negative numbers
// TODO 987'654'321.123456789 comes out as 987'654'336 ... is that unavoidable with f32?

// returns something like 3,140 = 3.14 * 2**3
char* f32_to_c_str_base2(f32_obj in) {
  char* output = new char[64] {'\0'};

  // TODO handle edge cases, like 0, inf, NaN, etc

  f64 decimal = in.window_portion();
  auto length = 0;

  output[length++] = '1';
  output[length++] = '.';

  while (decimal > 0) {
    decimal *= 10.0;
    auto integer_part = floor_i(decimal);
    output[length++] = '0' + (char)integer_part;
    decimal -= integer_part;
  }

  output[length++] = ' ';
  output[length++] = '*';
  output[length++] = ' ';
  output[length++] = '2';
  output[length++] = '*';
  output[length++] = '*';
  
  auto exp = in.exponent();
  char* exp_c_str = u32_to_c_str(exp);

  for (int i = 0; exp_c_str[i] != '\0'; i++) {
    output[length++] = exp_c_str[i];
  }

  output[length++] = '\0';

  auto portion = in.window_portion();

  return output;
}

char* f32_to_c_str(f32_obj in) {
  char* output;

  // TODO handle edge cases, like 0, inf, NaN, etc
  // TODO handle large numbers
  // TODO handle signed numbers
  // TODO handle very small numbers

  f64 decimal = in.to_f64();
  f64 basis_form = decimal;
  {
    while (basis_form > 9.999) { basis_form /= 10.0; }
    while (basis_form < 0.0 ) { basis_form *= 10.0; }
  }
  u64 integer_form = u64(decimal);
  if (decimal > f64(integer_form)) {
    integer_form = 0;
    f64 f_int = basis_form;
    while (f_int - f64(integer_form) > 0) {
      f_int *= 10;
      integer_form = u64(f_int);
    }
  }

  auto decimal_place = pow(2, f32(in.exponent()));
  decimal_place = logarithm_i(10, f32(decimal_place));

  u32 length = 0;
  u32 strpos = 0;
  u64 tens = 1; // which tens place we're looking at
  while (integer_form/tens > 0) {
    tens *= 10;
    length++;
  }

  tens /= 10;

  // +1 for decimal point, +1 for null terminator
  output = new char[length+1+1] {'\0'};

  while (integer_form > 0) {
    output[strpos++] = '0' + (char)(integer_form/tens);
    integer_form -= integer_form/tens*tens;
    tens /= 10;

    if (strpos == 1+decimal_place) { output[strpos++] = '.'; }
  }
  
  // check in.precision() and round if we're getting too small

  output[strpos++] = '\0';

  return output;
}


};
