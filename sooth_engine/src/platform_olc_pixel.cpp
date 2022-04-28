
#include "olcPixelGameEngine.h"

#include "platform_olc_pixel.h"

#include "lapse_lib.h"

namespace{
  lapse_lambda(void, void) initialization_callback;
  lapse_lambda(void, lapse::f32) main_loop;
}

class PixelEngineApp : public olc::PixelGameEngine
{
public:
  bool m_should_continue_running = true;
  PixelEngineApp(lapse::str name) {
    sAppName = name.to_c_str();
  }

public:
  bool OnUserCreate() override {
    if (initialization_callback)
      initialization_callback();
    return true;
  }

  bool OnUserUpdate(float delta) override {
    if (main_loop)
      main_loop(delta);

    for (int x = 0; x < ScreenWidth(); x++)
      for (int y = 0; y < ScreenHeight(); y++)
        Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
    return m_should_continue_running;
  }
};

namespace platform{

using namespace lapse;

namespace{
  PixelEngineApp* app;
  olc::rcode status;
};

void initialize(u32 screen_width, u32 screen_height, bool fullscreen, str window_name) {
  i32 pixel_size = 4;

  app = new PixelEngineApp(window_name);
  status = app->Construct(i32(screen_width), i32(screen_height), pixel_size, pixel_size, fullscreen);

  std::cout << "initializing olc::pixel game engine as the nigh sooth platform... status: ";
  
  switch(status) {
    case olc::rcode::FAIL:    std::cout << "olc::rcode::FAIL";    break;
    case olc::rcode::OK:      std::cout << "olc::rcode::OK";      break;
    case olc::rcode::NO_FILE: std::cout << "olc::rcode::NO_FILE"; break;
    default: std::cout << "olc::rode not recognized"; break;
  }
  
  std::cout << "\n";
}

void set_initialization_callback(lapse_lambda(void, void) arg_initialization_callback) {
  initialization_callback = arg_initialization_callback;
}

void set_main_loop_callback(lapse_lambda(void, f32) arg_main_loop) {
  main_loop = arg_main_loop;
  app->Start();
};

u32 get_frame_count() {
  return app->nFrameCount;
}

void plot(vec2<> screen_coord, vec3<> color) {
  app->Draw(
    screen_coord.x, screen_coord.y,
    olc::Pixel(color.r*255, color.g*255, color.b*255)
  );
}

void draw_bitmap(
  vec2<>  screen_coord,
  vec2<>  image_size,
  vec3<>* pixels
) {
  vec2<> current_coord{0, 0};
  vec2<> end_coord = screen_coord + image_size;
  for (; current_coord.y < end_coord.y; current_coord.y++) {
    for(; current_coord.x < end_coord.x; current_coord.x++) {
      plot(
        current_coord,
        pixels[
          i32(current_coord.x) + i32(current_coord.y)*app->ScreenWidth()
        ]
      );
    }
  }

}

};
