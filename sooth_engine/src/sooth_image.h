#pragma once

#include "lapse_scalar.h"
#include "lapse_string.h"

struct image{
  lapse::vec3<>* m_pixels = nullptr;
  lapse::vec2<> m_resolution;
  lapse::str m_path;
  lapse::i32 m_width, m_height, m_bytes_per_pixel;

  image();
  image(lapse::str& path);
  bool load_image(lapse::str* a_path = nullptr);
};
