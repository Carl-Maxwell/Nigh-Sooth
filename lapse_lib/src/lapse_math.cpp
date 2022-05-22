#include "lapse_scalar.h"

#include "intrin.h"

#include "lapse_f32.h"
#include "lapse_assert.h"

namespace lapse{

f32 pi = 3.14159265359;

// return -1 or 1
i32 sign(i32 a) {
  return i32(reinterpret_cast<u32&>(a) >> 31)*-2 + 1;
  // return a < 0 ? -1.0f : 1.0f;
};
// return -1 or 1
u32 sign(u32 a) {
  return u32(1);
  // return a < 0 ? -1.0f : 1.0f;
};
// returns -1.0f or +1.0f whichever is nearer
inline f32 sign(f32 a) {
  /*
  auto negative_a = -a;

  auto correct = a < 0 ? -1.0f : 1.0f;
  auto thing = f32(reinterpret_cast<u32&>(a) >> 31)*-2.0f + 1.0f;
  auto negative_thing = f32(reinterpret_cast<u32&>(negative_a) >> 31)*-2.0f + 1.0f;

  if (thing != correct) {
    f32 shift = reinterpret_cast<u32&>(a) >> 31;

    __debugbreak();
  }
  */

  return f32(reinterpret_cast<u32&>(a) >> 31)*-2.0f + 1.0f;
  // return a < 0 ? -1.0f : 1.0f;
};
// returns -1.0 or +1.0
f64 sign(f64 a) {
  return f64(reinterpret_cast<u64&>(a) >> 63)*-2.0f + 1.0f;
  // return a < 0 ? -1.0 : 1.0;
};

i32 abs(i32 a) {
  return sign(a) * a;
}
inline f32 abs(f32 a) {
  return sign(a) * a;
}
f64 abs(f64 a) {
  return sign(a) * a;
}

i64 floor_i(f64 whole) {
    return whole > 0 ? i64(whole) : i64(whole) - 1;
};
i32 floor_i(f32 whole) {
  return whole >= 0 ? i32(whole) : i32(whole)-1;
};
f32 floor_f(f32 whole) {
  return f32(i32(
    whole - 
    static_cast<f32>(reinterpret_cast<u32&>(whole) >> 31) // returns sign bit (either 0 or 1)
  ));
};
// branchless floor_f, only works with whole >= 0
f32 floor_f_positive(f32 whole) {
  return f32(i32(whole));
}

i32 ceil_i(f32 whole) {
  return whole > 0 ? i32(f32(abs(f32(i32(whole)))+1)) : i32(whole);
};
f32 ceil_f(f32 whole) {
  return whole > 0 ? f32(i32(whole)+1) : f32(i32(whole));
};

i32 max(i32 a, i32 b) {
  return a > b ? a : b;
  // assert that this doesn't go past i32_min or i32_max
  assert(a > i32_min/2 && a < i32_max/2);
  assert(b > i32_min/2 && b < i32_max/2);
  // TODO D'oh! This breaks if a == b
  return b * abs(((a-b)>>31)) + a * abs((b-a)>>31);
}
u32 max(u32 a, u32 b) {
  return a > b ? a : b;
}
f32 max(f32 a, f32 b) {
  return a > b ? a : b;
}

i32 min(i32 a, i32 b) {
  return a < b ? a : b;
}
u32 min(u32 a, u32 b) {
  return a < b ? a : b;
}
f32 min(f32 a, f32 b) {
  return a < b ? a : b;
}

// TODO need to account for floats larger than i32 max

f64 round(f64 whole) {
    i64 integer_part = (i64)floor_i(abs(whole));
    return sign(whole) * f64(
        abs(whole) - f64(integer_part) < 0.5 ? integer_part : integer_part + 1
    );
};

f32 round(f32 whole) {
  f32 abs_whole = abs(whole);
  f32 integer_part = floor_f(abs_whole);

  // f32 arr[] = {integer_part, 0, integer_part+1};
  // return sign(whole) * arr[ static_cast<i32>(sign(abs_whole - f32(integer_part) - 0.5) )+1 ];

  return sign(whole) * 
    (abs_whole - integer_part < 0.5 ? integer_part : integer_part+1.0f);
};

f32 old_round(f32 whole) {
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
    temp *= f64(base);
    if(f32_obj(temp).is_inf()) { __debugbreak(); }
  }
  return temp;
}

// log base b of big_number
//   "log base 2 of 8 is asking, 2 to the what equals 8?"
//   base**? = big_number
i32 logarithm_i(f32 base, f32 big_number) {
  i32 exponent    = 1;
  f64 range_start = base;
  f64 range_end   = base*base;

  while (true) {
    if (range_start <= big_number && big_number < range_end) {
      return exponent;
    }
    exponent++;
    range_start = pow(base, f32(exponent));
    range_end   = pow(base, f32(exponent+1));

    if (f32_obj(range_start).is_inf() || f32_obj(range_end).is_inf()) {
      __debugbreak();
    }

    // TODO should probably do some asserts above to avoid infinite loops here
  }

  return i32_max;
}

f64 sqrt(f64 big_number) {
  auto y = _mm_sqrt_pd({ big_number, double(0) });
  return y.m128d_f64[0];
}

// hasn't been implemented yet
f32 root(f32 exponent, f32 big_number) {
  f32 base = 0;
  // TODO calculate root
  return base;
}

};
