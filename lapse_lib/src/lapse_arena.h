#pragma once

#include "lapse_scalar.h"

namespace lapse{

struct Arena{
  void* m_memory_start = nullptr;
  // size of allocated memory in bytes
  u64 m_size = 0;
  // arena is claimed from memory_start to position
  void* m_position = nullptr;

  // setup the arena (aka constructor), allocate space for it
  void reserve(u64 bytes);
  // destroy the arena (aka destructor)
  void destroy_arena();

  // get how much memory the arena has access to (in bytes)
  u64 size()       { return m_size; }
  // how much memory within the arena has been used (in bytes)
  u64 length()     { return (u64)m_position - (u64)m_memory_start; }
  // how much space is left in the arena (in bytes)
  u64 free_space_available() { return m_size - length(); }

  // take x number of bytes
  void* push(u64 bytes);
  // take x number of bytes, and zero them out
  void* push_zeroes(u64 bytes);
  // remove x number of bytes
  void pop(u64 bytes);
  // erase everything in the arena, free up all the space
  void clear();
};

} // end namespace
