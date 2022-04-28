
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
    return true;
  }
};

namespace platform{

using namespace lapse;

namespace{
  PixelEngineApp* app;
};

void initialize(u32 screen_width, u32 screen_height, bool fullscreen, str window_name) {
  i32 pixel_size = 4;

  app = new PixelEngineApp(window_name);
  if (app->Construct(i32(screen_width), i32(screen_height), pixel_size, pixel_size, fullscreen))
    app->Start();
}

void set_initialization_callback(lapse_lambda(void, void) arg_initialization_callback) {
  initialization_callback = arg_initialization_callback;
}

void set_main_loop_callback(lapse_lambda(void, f32) arg_main_loop) {
  main_loop = arg_main_loop;
}

void plot(vec2<f32> screen_coord, vec3<f32> color) {
  app->Draw(
    screen_coord.x, screen_coord.y,
    olc::Pixel(color.r*255, color.g*255, color.b*255)
  );
}

};
