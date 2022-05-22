#include "lapse_random.h"

#include "lapse_scalar.h"

#include <random>

namespace lapse{

class Random{
public:
  std::default_random_engine m_engine;
  std::uniform_int_distribution<u64> m_dist;

 static Random& the(u32 rngseed = 0) {
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

void seed_rng(u32 seed) {
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

f32 rand_f32() {
  return static_cast<f32>(rand_float());
}

f32 rand_f32(f32 max) {
  return static_cast<f32>(rand_float()) * max;
}

f32 rand_f32(f32 min, f32 max) {
  f32 length = max-min;
  return f32(rand_f32() * length + min);
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

vec2<> rand_vec2() {
  return vec2<>{rand_f32(), rand_f32()};
}

vec2<> rand_vec2(vec2<> limits) {
  return vec2<>{rand_f32(limits.x), rand_f32(limits.y)};
}

vec3<> rand_vec3() {
  return vec3<f32>{(f32)rand_float(), (f32)rand_float(), (f32)rand_float()};
}

}