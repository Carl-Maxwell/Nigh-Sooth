
#include "lapse_array.h"
#include "lapse_range.h"

namespace lapse{

fixed_array<i32> range(i32 start, i32 end){
  fixed_array<i32> arr(1+end-start);

  for (i32 i = start; i <= end; i++) {
    arr.push(i);
  }

  return arr;
}

};
