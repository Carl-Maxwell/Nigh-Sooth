#pragma once

#include "lapse_arena.h"

// temporary allocation arena
//   it's lifetime is defined by the application, but is intended to end at end of frame

namespace arenas{
  extern lapse::Arena temp;
}

namespace lapse{

template<typename T>
struct temp_ptr{
  T* ptr;

  void allocate();
  void deallocate();

  // reallocate this thing in longer lived memory (make it not be temporary anymore)
  T* promote();
};

} // end namespace
