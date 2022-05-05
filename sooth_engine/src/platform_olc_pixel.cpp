
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

class PixelEngineApp : public olc::PixelGameEngine
{
public:
  bool m_should_continue_running = true;
  lapse::u64 frame_count = 0;

  PixelEngineApp(lapse::str name) {
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
};

namespace platform{

using namespace lapse;

namespace{
  PixelEngineApp* app;
  olc::rcode status;
  i32 pixel_size = 4;
  f64 application_startup_time = get_timestamp();
};

f64 get_application_start_time() {
  return application_startup_time;
}

i32 get_pixel_size() { return pixel_size; }
f32 get_window_padding() { return 10.0f; };

void initialize(u32 screen_width, u32 screen_height, bool fullscreen, str window_name) {
  // TODO so each time we call this we're creating a new app ... and not deleting the old one
  //   so memory use probably goes up a bit after a few games
  app = new PixelEngineApp(window_name);
  status = app->Construct(i32(screen_width), i32(screen_height), pixel_size, pixel_size, fullscreen);

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

void draw_text(str text, vec2<> position, vec3<> color) {
  // TODO font size
  // TODO color

  app->DrawString(
    olc::vi2d{(i32)position.x, (i32)position.y},
    text.to_c_str(),
    olc::WHITE
  );
}

void draw_bitmap(
  vec2<>  screen_coord,
  vec2<>  image_size,
  vec3<u8>* pixels
) {
  vec2<> current_coord{screen_coord};
  vec2<> end_coord = screen_coord + image_size;

  u32 image_width = (u32)image_size.x;

  u32 image_x = 0;
  u32 image_y = 0;

  while (current_coord.y < end_coord.y) {
    while(current_coord.x < end_coord.x) {
      plot(
        current_coord,
        pixels[image_y*image_width + image_x]
      );

      image_x++;
      current_coord.x++;
    }
    current_coord.x = screen_coord.x;
    image_x = 0;

    image_y++;
    current_coord.y++;
  }
}

void draw_bitmap(
  vec2<>  screen_coord,
  vec2<>  image_size,
  vec3<>* pixels
) {
  vec2<> current_coord{screen_coord};
  vec2<> end_coord = screen_coord + image_size;

  u32 image_width = (u32)image_size.x;

  u32 image_x = 0;
  u32 image_y = 0;

  while (current_coord.y < end_coord.y) {
    while(current_coord.x < end_coord.x) {
      plot(
        current_coord,
        pixels[image_y*image_width + image_x]
      );

      image_x++;
      current_coord.x++;
    }
    current_coord.x = screen_coord.x;
    image_x = 0;

    image_y++;
    current_coord.y++;
  }
}

void draw_bitmap(vec2<>  screen_coord, image img) {
  vec2<> current_coord{screen_coord};
  vec2<> end_coord = screen_coord + img.m_resolution;

  u32 image_width = (u32)img.m_resolution.x;

  u32 image_x = 0;
  u32 image_y = 0;

  while (current_coord.y < end_coord.y) {
    while(current_coord.x < end_coord.x) {
      plot(
        current_coord,
        img.m_pixels[image_y*image_width + image_x]
      );

      image_x++;
      current_coord.x++;
    }
    current_coord.x = screen_coord.x;
    image_x = 0;

    image_y++;
    current_coord.y++;
  }
}

void clear(lapse::vec3<> color) {
  color *= 255;
  app->Clear(olc::Pixel{(u8)color.r, (u8)color.g, (u8)color.b});
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
  olc::vi2d temp = app->GetWindowMouse();
  return vec2<i32>{temp.x, temp.y};
}

olc::Key translate_keycode(keycode code) {
  assert(keycode_list().contains(code));
  switch(code) {
    case keycode::escape : return olc::Key::ESCAPE;
    case keycode::number_1 : return olc::Key::K1;
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
  auto codes = keycode_list();
  for (i32 i = 0; i < codes.length(); i++) {
    auto key_state = key(codes[i]);
    if (key_state.is_hit()) {
      key_state.toggle();
      std::cout << "toggling " << (u32)codes[i] << "\n";
    }
  }
}

}; // end platform namespace
