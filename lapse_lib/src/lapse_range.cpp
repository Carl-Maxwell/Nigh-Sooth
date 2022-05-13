
#include "lapse_array.h"
#include "lapse_range.h"
#include "lapse_temp.h"

namespace lapse{

fixed_array<i32>& range(i32 start, i32 end){
  u32 size = 1+end-start;
  fixed_array<i32>& arr = *(fixed_array<i32>*)arenas::temp.push(size * sizeof(i32));
  memset(&arr, 0, size * sizeof(i32));
  arr.reserve(size);

  for (i32 i = start; i <= end; i++) {
    arr.push(i);
    // this loops forever?
  }

  return arr;
}

};
