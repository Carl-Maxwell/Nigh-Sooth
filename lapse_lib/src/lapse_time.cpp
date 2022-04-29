
#include "lapse_time.h"

#include <chrono>

namespace lapse{

f64 get_timestamp() {
  std::chrono::duration<f64, std::milli> chronotime = std::chrono::high_resolution_clock::now().time_since_epoch();

  f64 integer_time = std::chrono::duration< f64 >(chronotime).count();

  return integer_time;
}

}
