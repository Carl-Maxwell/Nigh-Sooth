
#include "sooth_image.h"

#include "stb_image_namespace.h"

#include "lapse_lib.h"

using namespace lapse;

image::image() {
  m_path = "";
}

image::image(str& path) {
  m_path = path;
  load_image();
}

bool image::load_image(str* a_path) {
  if (a_path) std::cout << "caliing load_image(" << a_path->to_c_str() << ")\n";
  if (a_path->length()) { m_path = *a_path; }

  // generate UID, because in future we probably won't have constructors on this class
  this->id.generate_id();

  std::cout
    << " a_path (" << a_path->to_c_str()
    << ") m_path (" << m_path.to_c_str()
    << ")\n"
  ;

  unsigned char* raw_pixels = stb::stbi_load(
    m_path.to_c_str(),
    &m_width,
    &m_height,
    &m_bytes_per_pixel,
    4
  );

  if (!raw_pixels) {
    std::cout << "failed to load image: \"" << m_path.to_c_str() << "\"\n";
    assert(raw_pixels);
    return false;
  }

  // m_pixels = new vec3<>[m_width * m_height];

  // for (u32 i = 0; i < m_width * m_height; i++) {
  //   m_pixels[i] = vec3<>{
  //     f32(raw_pixels[m_bytes_per_pixel*i+0])/255.0f,
  //     f32(raw_pixels[m_bytes_per_pixel*i+1])/255.0f,
  //     f32(raw_pixels[m_bytes_per_pixel*i+2])/255.0f
  //   };
  // }

  m_u_pixels = new vec4<u8>[m_width * m_height];
  for (i32 i = 0; i < m_width * m_height; i++) {
    m_u_pixels[i] = vec4<u8>{
      raw_pixels[m_bytes_per_pixel*i+0],
      raw_pixels[m_bytes_per_pixel*i+1],
      raw_pixels[m_bytes_per_pixel*i+2],
      255
    };
  }

  m_resolution = {(f32)m_width, (f32)m_height};

  std::cout << "loaded image: " << m_path.to_c_str() << " ,"
    << " x: " << m_width
    << " y: " << m_height
    << " components: " << m_bytes_per_pixel
    << "\n";

  return true;
}
