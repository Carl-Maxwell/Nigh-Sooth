
#include "sooth_image.h"

#include "stb_image_namespace.h"

#include "lapse_lib.h"

using namespace lapse;

image::image(str path) {
  m_path = path;
  load_image();
}

bool image::load_image() {
  unsigned char* raw_pixels = stb::stbi_load(
    m_path.to_c_str(),
    &m_width,
    &m_height,
    &m_bytes_per_pixel,
    4
  );

  if (!raw_pixels) return false;

  m_pixels = new vec3<>[m_width * m_height];

  for (u32 i = 0; i < m_width * m_height; i++) {
    m_pixels[i] = vec3<>{
      f32(raw_pixels[m_bytes_per_pixel*i+0])/255.0f,
      f32(raw_pixels[m_bytes_per_pixel*i+1])/255.0f,
      f32(raw_pixels[m_bytes_per_pixel*i+2])/255.0f
    };
  }

  m_resolution = {(f32)m_width, (f32)m_height};

  std::cout << m_path.to_c_str() << " ,"
    << " x: " << m_width
    << " y: " << m_height
    << " components: " << m_bytes_per_pixel
    << "\n";

  return true;
}
