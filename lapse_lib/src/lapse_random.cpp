#include "lapse_random.h"

#include <random>

namespace lapse{

class Random{
public:
  std::default_random_engine m_engine;
  std::uniform_int_distribution<u64> m_dist;

 static Random& the() {
    static Random* my_rng = nullptr;
    if (!my_rng) {
      my_rng = new Random;
    }
    return *my_rng;
  };
};

u64 rand_integer() {
  return Random::the().m_dist(Random::the().m_engine);
}

f64 rand_float() {
  return (double( rand_integer()%10'000'000'000'000'001 ) / 10'000'000'000'000'000.0);
  // +1 to the u64 is because of the mod operator, this way it's rand*x/x = 0..1 
  //   inclusive of the number 1
}

f64 rand_range(f64 min, f64 max) {
  f64 length = max-min;
  return f64(rand_float() * length + min);
}

u32 die(u32 sides) {
  return die(1, sides);
}

u32 die(u32 n, u32 sides) {
  u32 sum = 0;
  for (u32 i = 0; i < n; i++) {
    sum += (rand() % sides) + 1;
  }
  return sum;
}

}