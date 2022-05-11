
#include "olcPixelGameEngine.h"

#include "platform_olc_pixel.h"

#include "lapse_geometry.h"

#include "sooth_image.h"
#include "sooth_input.h"

#include "lapse_lib.h"

namespace{
  lapse_lambda(void, void) game_session_initialization_callback;
  bool game_session_started = false;
  lapse_lambda(void, lapse::f32) main_loop;
}

void force_quit();

class SoothPixelEngineApp : public olc::PixelGameEngine
{
public:
  bool m_should_continue_running = true;
  lapse::u64 frame_count = 0;

  SoothPixelEngineApp(lapse::str& name) {
    sAppName = name.to_c_str();
  }

public:
  bool OnUserCreate() override {
    std::cout << "\n// calling OnUserCreate()\n\n";
    if (game_session_initialization_callback && !game_session_started) {
      game_session_initialization_callback();
      game_session_started = true;
    }

    return true;
  }

  bool OnUserUpdate(float delta) override {
    frame_count++;

    if (main_loop)
      main_loop(delta);

    return m_should_continue_running;
  }

  bool OnUserDestroy() {
    if (this->bForceQuitEvent) {
      force_quit();
    }
    return true;
  }
};

namespace platform{

using namespace lapse;

namespace{
  SoothPixelEngineApp* app;
  olc::rcode status;
  i32 pixel_size = 4;
  f64 application_startup_time = get_timestamp();
};

f64 get_application_start_time() {
  return application_startup_time;
}

i32 get_pixel_size() { return pixel_size; }
f32 get_window_padding() { return 10.0f; };

void initialize(u32 screen_width, u32 screen_height, bool fullscreen, str& window_name, i32 a_pixel_size) {
  // TODO so each time we call this we're creating a new app ... and not deleting the old one
  //   so memory use probably goes up a bit after a few games

  pixel_size = a_pixel_size;

  app = new SoothPixelEngineApp(window_name);
  status = app->Construct(i32(screen_width), i32(screen_height), pixel_size, pixel_size, fullscreen);

  std::cout << "screen resolution: " << i32(screen_width) << "x" << i32(screen_height) << "\n";
  std::cout << "pixel scale: " << pixel_size << "\n";
  std::cout << "initializing olc::pixel game engine as the nigh sooth platform... status: ";
  
  switch(status) {
    case olc::rcode::FAIL:    std::cout << "olc::rcode::FAIL";    break;
    case olc::rcode::OK:      std::cout << "olc::rcode::OK";      break;
    case olc::rcode::NO_FILE: std::cout << "olc::rcode::NO_FILE"; break;
    default: std::cout << "olc::rcode not recognized"; break;
  }
  
  std::cout << "\n";
}

void set_game_session_initialization_callback(lapse_lambda(void, void) arg_initialization_callback) {
  game_session_initialization_callback = arg_initialization_callback;
}

void close_application() {
  app->m_should_continue_running = false;

  // note that this just ends the main loop, causing it to fall back to session::start_session()
}

void finalize_shutdown() {
  delete app;
}

void start_application() {
  std::cout << "\n\n//\n// start of start_application\n//\n\n";
  olc::rcode code = app->Start();
  // TODO look at rcode
  std::cout << "\n\n//\n//end of start_application\n//\n\n";
}

void set_main_loop_callback(lapse_lambda(void, f32) arg_main_loop) {
  main_loop = arg_main_loop;
};

// returns number of frames since game start
u64 get_frame_count() {
  return app->frame_count;
}

u32 get_window_width()  { return app->ScreenWidth();  }
u32 get_window_height() { return app->ScreenHeight(); }
vec2<> get_window_size() {
  return vec2<>{
    (f32)app->ScreenWidth(),
    (f32)app->ScreenHeight()
  };
}

// convert our vec3<> colors to olc pixel vi2d u8 colors
olc::Pixel vec3_to_color(vec3<> color) {
  return olc::Pixel{u8(color.r*255), u8(color.g*255), u8(color.b*255)};
}

void plot(vec2<> screen_coord, vec3<> color) {
  app->Draw(
    screen_coord.x, screen_coord.y,
    olc::Pixel{u8(color.r*255), u8(color.g*255), u8(color.b*255)}
  );
}

void plot(lapse::vec2<> screen_coord, lapse::vec3<u8> color) {
  app->Draw(
    screen_coord.x, screen_coord.y,
    olc::Pixel{color.r, color.g, color.b}
  );
}

void draw_line(vec2<> start, vec2<> end, vec3<> color) {
  f32 distance = (start-end).length(); 
  vec2<> dir = (end-start).normalize();
  for (vec2<> offset = {0, 0}; offset.length() < distance; offset += dir) {
    plot(start+offset, color);
  }
}

void draw_rect(vec2<> start, vec2<> size, vec3<> color) {
  auto top_right_point    = start; top_right_point.x   += size.x;
  auto bottom_left_point  = start; bottom_left_point.y += size.y;
  auto bottom_right_point = start + size;

  draw_line(start, top_right_point);
  draw_line(top_right_point, bottom_right_point);
  draw_line(bottom_right_point, bottom_left_point);
  draw_line(bottom_left_point, start);
}

void draw_rect(rect<> box, vec3<> color) {
  draw_line(box.top_left_point(),     box.top_right_point());
  draw_line(box.top_right_point(),    box.bottom_right_point());
  draw_line(box.bottom_right_point(), box.bottom_left_point());
  draw_line(box.bottom_left_point(),  box.top_left_point());
}

void fill_rect(rect<> box, vec3<> color) {
  vec2<> point = box.top_left_point();
  for (; point.y < box.bottom_right_point().y; point.y++) {
    for (; point.x < box.bottom_right_point().x; point.x++) {
      plot(point, color);
    }
    point.x = box.top_left_point().x;
  }
}

void draw_text(str& text, vec2<> position, vec3<> color, u32 font_size_pixels) {
  // TODO color

  // by default font_size is 8px
  font_size_pixels /= 8;

  app->DrawString(
    olc::vi2d{(i32)position.x, (i32)position.y},
    text.to_c_str(),
    vec3_to_color(color),
    font_size_pixels
  );
}

void draw_bitmap(vec2<>  screen_coord, image& img) {
  u32 image_width  = (u32)img.m_width;
  u32 image_height = (u32)img.m_height;

  auto img_pixels = img.m_u_pixels;

  auto& screen_pixels     = app->pDrawTarget->pColData;
  auto screen_pixel_width = app->pDrawTarget->width;

  auto line_offset = image_width*sizeof(olc::Pixel);

  assert(sizeof(olc::Pixel) == sizeof(vec4<u8>));

  for (i32 y = 0; y < image_height; y++) {
    memcpy(
      (void*)&screen_pixels[(screen_coord.y+y) * screen_pixel_width + screen_coord.x],
      (void*)&img_pixels[y*image_width],
      line_offset
    );
  }

}

void clear(lapse::vec3<> color) {
  color *= 255;
  // app->Clear(olc::Pixel{(u8)color.r, (u8)color.g, (u8)color.b});

  auto& screen_pixels = app->pDrawTarget->pColData;
  auto width          = app->pDrawTarget->width;
  auto height         = app->pDrawTarget->height;

  auto p_color = olc::Pixel{(u8)color.r, (u8)color.g, (u8)color.b};
  i32 clear_color = p_color.n;
  auto line_byte_size = width*sizeof(olc::Pixel);

  for (u32 y = 0; y < height; y++) {
    memset((void*)&screen_pixels[y*width], clear_color, line_byte_size);
  }
}

//-----------------------------------------------------------------------------
// Input functions
//-----------------------------------------------------------------------------

// returns true if mouse hit event happened this frame
bool is_mouse_left_button_hit() {
  return app->GetMouse(olc::Mouse::LEFT).bPressed;
}

// returns true if mouse hit event happened this frame
bool is_mouse_right_button_hit() {
  return app->GetMouse(olc::Mouse::RIGHT).bPressed;
}

vec2<i32> get_mouse_pos() {
  olc::vi2d temp = app->GetMousePos(); //app->GetWindowMouse();
  return vec2<i32>{temp.x*pixel_size, temp.y*pixel_size};
}

olc::Key translate_keycode(keycode code) {
  assert(keycode_list().contains(code));
  switch(code) {
    case keycode::escape : return olc::Key::ESCAPE;
    case keycode::number_1 : return olc::Key::K1;
    case keycode::number_2 : return olc::Key::K2;
  }

  assert(false);
}

KeyState get_key_state(keycode code) {
  KeyState temp{code};

  auto pixel_key = app->GetKey(translate_keycode(code));

  temp.m_key_hit  = pixel_key.bPressed;
  temp.m_key_up   = pixel_key.bReleased;
  temp.m_key_down = pixel_key.bHeld;

  return temp;
}

void poll_key_toggles() {
  const auto& codes = keycode_list();
  for (i32 i = 0; i < codes.length(); i++) {
    auto key_state = key(codes[i]);
    if (key_state.is_hit()) {
      key_state.toggle();
      std::cout << "toggling " << (u32)codes[i] << "\n";
    }
  }
}

}; // end platform namespace
