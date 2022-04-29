#include "lapse_random.h"

#include "lapse_scalar.h"

#include <random>

namespace lapse{

class Random{
public:
  std::default_random_engine m_engine;
  std::uniform_int_distribution<u64> m_dist;

 static Random& the(u64 rngseed = 0) {
    static Random* my_rng = nullptr;
    if (!my_rng) {
      my_rng = new Random;
    }
    if (rngseed) {
      my_rng->m_engine.seed(rngseed);
    }
    return *my_rng;
  };
};

void seed_rng(u64 seed) {
  Random::the(seed);
}

u64 rand_integer(u64 end) {
  return (u64)rand_range(0, (f64)end);
}

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

vec3<> rand_vec3() {
  return vec3<f32>{(f32)rand_float(), (f32)rand_float(), (f32)rand_float()};
}

}