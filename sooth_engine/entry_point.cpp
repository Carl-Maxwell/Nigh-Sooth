
#include "lapse_lib.h"

#include "sooth_input.h"

#include "platform_olc_pixel.h"

#include "stb_image_namespace.h"

using namespace lapse;

struct mouse_obj{
  f64 x = 0;
  f64 y = 0;
  struct button_states{
    bool left = false;
    bool right = false;
  } buttons;
} mouse;

void set_event_callbacks();

int main(void) {

  // window:
  u32 WIDTH = 1280/4, HEIGHT = 720/4;
  // u32 WIDTH = 1280, HEIGHT = 720;
  // u32 WIDTH = 1920, HEIGHT = 1080;

  // borderless window:
  // u32 WIDTH = 1920, HEIGHT = 1080;

  platform::initialize(WIDTH, HEIGHT, false, "Nigh Sooth Engine Test");

  str path = "test_image_small.png";
  i32 img_width, img_height, bytes_per_pixel;

  unsigned char* raw_pixels = stb::stbi_load(
    path.to_c_str(),
    &img_width,
    &img_height,
    &bytes_per_pixel,
    4
  );

  vec3<u8>* pixels = new vec3<u8>[img_width * img_height];

  for (u32 i = 0; i < img_width * img_height; i++) {
    pixels[i] = vec3<u8>{
      raw_pixels[bytes_per_pixel*i+0],
      raw_pixels[bytes_per_pixel*i+1],
      raw_pixels[bytes_per_pixel*i+2]
    };
  }

  std::cout << path.to_c_str() << " ,"
    << " x: " << img_width
    << " y: " << img_height
    << " components: " << bytes_per_pixel
    << "\n";

  platform::set_main_loop_callback([img_width, img_height, pixels](f32 delta){
    platform::clear(vec3<>{0, 0, 0});

    for (f32 y = 0; y < platform::get_window_height(); y++) {
      for (f32 x = 0; x < platform::get_window_width(); x++) {
        vec3<> color{1, 1, 1};
        // platform::plot(vec2<f32>{x, y}, rand_vec3());
        color *= (x / platform::get_window_width());
        platform::plot(vec2<f32>{x, y}, color);
      }
    }

    platform::draw_bitmap(vec2<>{10, 10}, vec2<>{(f32)img_width, (f32)img_height}, pixels);

    platform::plot(Input::Mouse::get_mouse_pos(), rand_vec3());
  } );

  return 0;
}

void set_event_callbacks() {
  // glfwSetKeyCallback(glfw_window,
  //   [](GLFWwindow* glfw_window, int glfw_key, int scancode, int action, int mods) {
  //     std::cout << "keyboard event!" << "\n";

  //     if (glfw_key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
  //       glfwSetWindowShouldClose(glfw_window, GLFW_TRUE);
  //     }
  // } );

  // glfwSetMouseButtonCallback(glfw_window,
  //   [](GLFWwindow* glfw_window, int button, int action, int mods) {
  //     bool pressed = action == GLFW_PRESS;

  //     switch(button) {
  //       case GLFW_MOUSE_BUTTON_1: { mouse.buttons.left  = pressed; } break; // left mouse
  //       case GLFW_MOUSE_BUTTON_2: { mouse.buttons.right = pressed; } break; // right mouse
  //       default: return;
  //     }

  //     switch (action) {
  //       case GLFW_PRESS: {
  //         // mouse button released
  //         ;
  //         std::cout << "mouse button pressed! \n";
  //       } break;
  //       case GLFW_RELEASE: {
  //         // mouse button released
  //         std::cout << "mouse button released! \n";
  //       } break;
  //     }
  // } );

  // glfwSetCursorPosCallback(glfw_window, 
  //   [](GLFWwindow* glfw_window, f64 x_pos, f64 y_pos) {
  //     mouse.x = x_pos;
  //     mouse.y = y_pos;
  // } );
  
  // //
  // // Window events
  // //

  // glfwSetWindowPosCallback (glfw_window, 
  //   [](GLFWwindow* glfw_window, i32 x_pos, i32 y_pos){
  //     // std::cout << "Window position event x: " << x_pos << " y:" << y_pos << "\n";
  // } );

  // glfwSetWindowSizeCallback( glfw_window,
  //   [](GLFWwindow* glfw_window, i32 width, i32 height){
  //     std::cout << "window size event! ... this shouldn't realy happen" << "\n";
  //     // TODO maybe throw an error?
  // } );

  // glfwSetWindowFocusCallback (glfw_window,
  //   [](GLFWwindow* glfw_window, i32 focused){
  //     std::cout
  //       << "Event: window focus callback ... focused: " 
  //       << (focused ? "focus gained!" : "focus lost : (") 
  //       << "\n";
  // } );

  // glfwSetWindowRefreshCallback(glfw_window, [](GLFWwindow* glfw_window){
  //   std::cout << "Event: window refresh event triggered\n";
  // });

};
