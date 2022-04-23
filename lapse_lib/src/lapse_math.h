#pragma once

#include "lapse_lib.h"

#include "intrin.h"

namespace lapse{

// returns -1.0f or 1.0f whichever is nearer
f32 sign(f32 a) {
  return a < 0 ? -1.0f : 1.0f;
};

f64 sign(f64 a) {
  return a < 0 ? -1.0 : 1.0;
};

f32 abs(f32 a) {
  return sign(a) * a;
}

f64 abs(f64 a) {
  return sign(a) * a;
}

i32 floor_i(f32 whole) {
  return whole > 0 ? i32(whole) : i32(whole)-1;
};

f32 floor_f(f32 whole) {
  return whole > 0 ? f32(i32(whole)) : f32(i32(whole))-1;
};

i32 ceil_i(f32 whole) {
  return whole > 0 ? f32(abs(f32(i32(whole)))+1) : i32(whole);
};

f32 ceil_f(f32 whole) {
  return whole > 0 ? f32(i32(whole)+1) : f32(i32(whole));
};

// TODO need to account for floats larger than i32 max

f32 round(f32 whole) {
  i32 integer_part = floor_i(abs(whole));
  return sign(whole) * f32( 
    abs(whole) - f32(integer_part) < 0.5 ? integer_part : integer_part+1
  );
};

// TODO I guess everything in this header should be inline and const

f64 pow(f32 base, f32 exponent) {
  if (base == 0) { return 0; }

  f64 temp = 1;
  for (int i = 0; i < exponent; i++) {
    temp *= base;
  }
  return temp;
}

// log base b of big_number
i32 logarithm_i(f32 base, f32 big_number) {
  i32 exponent = 1;
  for ( ; pow(base, exponent) < big_number; exponent++ ) {}
  return exponent;
}

f32 sqrt(f32 big_number) {
  auto y = _mm_sqrt_pd({ big_number, double(0) });
  return y.m128d_f64[0];
}

// hasn't been implemented yet
f32 root(f32 exponent, f32 big_number) {
  f32 base = 0;
  // TODO calculate root
  return base;
}

//-----------------------------------------------------------------------------
// integer funcs
//-----------------------------------------------------------------------------

char* u32_to_c_str(u32 in) {
  char* output;

  if (in == 0) { return (char*)"0"; }

  u32 original_in = in;

  u32 length = 0;
  u32 tens = 1; // which tens place we're looking at
  while (in/tens > 0) {
    tens *= 10;
    length++;
  }

  tens /= 10;

  output = new char[length + 1]; // +1 accounts for null terminator
  u32 i = 0;
  
  while (in > 0) {
    output[i++] = '0' + in/tens;
    in -= in/tens*tens;
    tens /= 10;
  }

  output[i++] = '\0';

  return output;
}

//-----------------------------------------------------------------------------
// float funcs
//-----------------------------------------------------------------------------

// TODO account for negative numbers
// TODO 987'654'321.123456789 comes out as 987'654'336 ... is that unavoidable with f32?

// returns something like 3,140 = 3.14 * 2**3
char* f32_c_str_exp(f32_obj in) {
  char* output = new char[32] {'\0'};

  f64 decimal = f64(in.bits.offset) / f64(f32_mantissa_max);
  auto length = 0;

  for (i32 i = 0; decimal > 0; i++) {
    decimal *= 10.0;
    output[i] = '0' + floor_i(decimal);
    decimal -= floor_i(decimal);
    length++;
  }

  output[length++] = ' ';
  output[length++] = '2';
  output[length++] = '*';
  output[length++] = '*';
  
  auto exp = in.window();
  char* exp_c_str = u32_to_c_str(exp);

  for (int i = 0; exp_c_str[i] != '\0'; i++) {
    output[length++] = exp_c_str[i];
  }

  std::cout << "decimal: " << decimal << "\n";

  __debugbreak();

  return output;
}

char* f32_c_str(f32_obj in) {
  char* output;



  return output;
}

// convert f32 to c_str
char* f32_c_str_old(f32 in) {
  char* temp;

  i32 sign_bump = in < 0 ? 1 : 0;
  in = abs(in);

  u64 integer_part = u64(in);
  in -= integer_part;
  u32 integer_length = logarithm_i(10, integer_part);
  integer_length += sign_bump;

  char* decimal_str = new char[f32_significant_digits];

  // build decimal_str
  u64 decimal_length = 0;
  for (i32 i = 0, tens = 10; in > 0 && i < f32_significant_digits; i++, tens *= 10) {
    u32 digit = in*tens;
    in -= f32(digit)/tens;
    decimal_length++;
    decimal_str[i] = '0' + digit;
  }

  // +1 length for null terminator
  temp = new char[integer_length + decimal_length + 1];

  if (sign_bump) { temp[0] = '-'; }

  // build integer part of string
  for (
    u32 i = sign_bump, tens=pow(10, integer_length-1-sign_bump);
    i < integer_length;
    i++, tens /= 10
  ) {
                                          // so for 321 tens would be 100
    u32 this_part = integer_part/tens;    // so for 321 this would be 3
    integer_part -= this_part*tens;       // 321 - 300
    temp[i] = '0' + this_part;          // calculate ascii code for "3"
  }

  // combine integer and decimal strings
  if (decimal_length > 0) {
    temp[integer_length] = '.';
    for (u32 i=0; i < decimal_length; i++) {
      temp[integer_length + 1 + i] = decimal_str[i];
      // +1 accounts for period '.'
    }
  }

  // +1 accounts for period '.'
  temp[integer_length + decimal_length + 1] = '\0';
  return temp;
};

// convert f64 to c_str
char* f64_c_str(f64 in) {
  char* temp;

  return temp;
}

}
