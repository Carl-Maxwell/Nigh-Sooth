#include "lapse_arena.h"

#include "lapse_exceptions.h"

namespace lapse{

void Arena::setup_arena(u64 bytes) {
  assert(!m_memory_start && !m_position);
  m_memory_start = new u8[bytes];
  m_size = bytes;
  m_position = m_memory_start;
}

void Arena::destroy_arena() {
  delete m_memory_start;
  m_memory_start = nullptr;
  m_size = 0;
  m_position = nullptr;
}

void* Arena::push(u64 bytes) {
  if (free_space_available() < bytes) {
    clear();
  }
  assert(free_space_available() > bytes);
  void* start_pos = m_position;
  m_position = (void*)((u64)m_position + bytes);
  return start_pos;
}

void Arena::pop(u64 bytes) {
  assert(bytes <= free_space_available());
  m_position = (void*)((u64)m_position - bytes);
}

void Arena::clear() {
  // std::cout << "// Arena::clear() called, bytes used: " << length() << " arena utilization: " << f32(length())/f32(m_size) << "\n";
  m_position = m_memory_start;
}

} // end namespace
