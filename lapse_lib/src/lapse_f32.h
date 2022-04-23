#pragma once

namespace lapse{

//-----------------------------------------------------------------------------
// f32_obj
//-----------------------------------------------------------------------------

// f32 enriched with methods
struct f32_obj{
  // this union allows convenient access to the components of an f32
  union{
    float value;
    // a struct with colons (:) is called a bit field,
    //   each number below specifies the sizeof() that struct member in bits
    struct {
      unsigned int offset : f32_mantissa_bits; // mantissa
      unsigned int window : f32_exponent_bits;  // exponent
      unsigned int sign   : 1;
    } bits;
  };

  f32_obj(f64 in) { value = f32(in); };
  f32_obj(f32 in) { value = in; };
  f32_obj(u32 in) { value = f32(in); };
  f32_obj(i32 in) { value = f32(in); };

  i32 exponent() {
    return bits.window - (f32_exponent_min+1);
  }
  f64 window_start() {
    return pow(2, f32(exponent()));
  };
  f64 window_end() {
    return pow(2, f32(1 + exponent()));
  };
  // returns percentage of where this number is in its window
  //   so for 3.0f it returns 0.5 because it's in the middle of the 2..4 window
  f64 window_portion() {
    return f64(bits.offset) / f64(f32_mantissa_max);
  }
  // returns the expected error range of this f32_obj
  f64 precision() {
    return (window_end() - window_start()) / f32_mantissa_max;
  }
  // calculates the value of this f32's components as a f64
  f64 to_f64() {
    return window_start() * (1.0 + f64(bits.offset) / f64(f32_mantissa_max));
  }
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

}