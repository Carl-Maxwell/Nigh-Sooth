
#include "olcPixelGameEngine.h"

#include "platform_olc_pixel.h"

#include "lapse_geometry.h"

#include "sooth_image.h"
#include "sooth_input.h"

#include "lapse_lib.h"

namespace{
  lapse_lambda(void, lapse::f32) main_loop;
}

namespace arenas{
  lapse::Arena image_cache;
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
    return true;
  }

  bool OnUserUpdate(float delta) override {
    frame_count++;

    main_loop(delta);

    return m_should_continue_running;
  }

  bool OnUserDestroy() {
    if (this->bForceQuitEvent) {
      // user has clicked the window X or otherwise told the app to close immediately
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

i32 get_pixel_size() { return pixel_size; };
f32 get_window_padding() { return 10.0f; };

void initialize(i32 screen_width, i32 screen_height, bool fullscreen, str& window_name, i32 a_pixel_size) {
  pixel_size = a_pixel_size;
  // pixel_size = 1;

  app = new SoothPixelEngineApp(window_name);
  status = app->Construct(screen_width, screen_height, pixel_size, pixel_size, fullscreen);

  std::cout << "screen resolution: " << screen_width << "x" << screen_height << "\n";
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

void close_application() {
  app->m_should_continue_running = false;

  // note that this just ends the main loop, causing it to fall back to session::advance_phase()
}

void finalize_shutdown() {
  delete app;
}

bool start_application() {
  std::cout << "\n\n//\n// start of start_application\n//\n\n";
  olc::rcode code = app->Start();
  // TODO look at rcode
  std::cout << "\n\n//\n//end of start_application\n//\n\n";
  return true;
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
    i32(screen_coord.x), i32(screen_coord.y),
    olc::Pixel{u8(color.r*255), u8(color.g*255), u8(color.b*255)}
  );
}

void plot(lapse::vec2<> screen_coord, lapse::vec3<u8> color) {
  app->Draw(
    i32(screen_coord.x), i32(screen_coord.y),
    olc::Pixel{color.r, color.g, color.b}
  );
}

void draw_line(vec2<> start, vec2<> end, vec3<> color) {
  auto& screen_pixels = app->pDrawTarget->pColData;
  auto screen_width   = app->pDrawTarget->width;
  auto screen_height  = app->pDrawTarget->height;

  auto p_color = olc::Pixel{u8(color.r*255), u8(color.g*255), u8(color.b*255)};

  f32 distance = (start-end).length();
  vec2<> dir = (end-start).normalize();
  vec2<> pos;
  for (vec2<> offset = {0, 0}; offset.length() < distance; offset += dir) {
    pos = start+offset;
    assert(pos.x >= 0 && pos.x < screen_width);
    assert(pos.y >= 0 && pos.y < screen_height);
    screen_pixels[u32(pos.y)*u32(screen_width) + u32(pos.x)] = p_color;
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
  u32 rect_width  = u32(box.size.x);
  u32 rect_height = u32(box.size.y);
  u32 screen_x = u32(box.top_left_point().x);
  u32 screen_y = u32(box.top_left_point().y);

  auto& screen_pixels = app->pDrawTarget->pColData;
  auto screen_width   = (u32)app->pDrawTarget->width;
  auto screen_height  = (u32)app->pDrawTarget->height;

  assert(screen_x >= 0 && screen_x + rect_width  < screen_width);
  assert(screen_y >= 0 && screen_y + rect_height < screen_height);

  auto p_color = olc::Pixel{u8(color.r*255), u8(color.g*255), u8(color.b*255)};
  i32 bg_color = p_color.n;
  auto line_size_in_bytes = rect_width*sizeof(olc::Pixel);

  auto line_of_pixels = arenas::temp.push(line_size_in_bytes);
  // auto line_of_pixels = alloca(line_size_in_bytes);
  for (u32 x = 0; x < rect_width; x++) {
    ((i32*)line_of_pixels)[x] = bg_color;
  }

  for (u32 y = 0; y < rect_height; y++) {
    memcpy(
      (void*)&screen_pixels[(u32(screen_y)+y)*u32(screen_width) + u32(screen_x)],
      line_of_pixels,
      line_size_in_bytes
    );
  }
}

void draw_text(str& text, vec2<> position, vec3<> color, u32 font_size_pixels) {
  // by default font_size is 8px
  font_size_pixels /= 8;

  app->DrawString(
    olc::vi2d{(i32)position.x, (i32)position.y},
    text.to_c_str(),
    vec3_to_color(color),
    font_size_pixels
  );
}

void draw_bitmap(vec2<> screen_coord, image& img) {
  u32 image_width  = (u32)img.m_width;
  u32 image_height = (u32)img.m_height;

  auto img_pixels = img.m_u_pixels;

  auto& screen_pixels      = app->pDrawTarget->pColData;
  auto screen_pixel_width  = app->pDrawTarget->width;
  auto screen_pixel_height = app->pDrawTarget->height;

  assert(screen_coord.x >= 0 && screen_coord.x + image_width  < screen_pixel_width) ;
  assert(screen_coord.y >= 0 && screen_coord.y + image_height < screen_pixel_height);

  auto line_offset = image_width*sizeof(olc::Pixel);

  assert(sizeof(olc::Pixel) == sizeof(vec4<u8>));

  for (u32 y = 0; y < image_height; y++) {
    memcpy(
      (void*)&screen_pixels[(u32(screen_coord.y)+y) * u32(screen_pixel_width) + u32(screen_coord.x)],
      (void*)&img_pixels[y*image_width],
      line_offset
    );
  }
}

// forward declare image cache funcs

vec4<u8>* get_scaled_image(uid imd_id, f32 scale);
vec4<u8>* create_scaled_image(uid img_id, f32 scale, u32 size_in_bytes);

void draw_bitmap_scaled(vec2<> screen_coord, image& img, f32 scale) {
  u32 image_width  = (u32)img.m_width;
  u32 image_height = (u32)img.m_height;

  // the 'target' is the rect in the img we're drawing
  f32 f_target_width  = lapse::round(static_cast<f32>(image_width)  * scale);
  f32 f_target_height = lapse::round(static_cast<f32>(image_height) * scale);

  // record original target size before we clip it
  u32 u_target_full_width  = static_cast<u32>(f_target_width );
  u32 u_target_full_height = static_cast<u32>(f_target_height);

  screen_coord *= scale;

  screen_coord.x = lapse::round(screen_coord.x);
  screen_coord.y = lapse::round(screen_coord.y);

  vec2<> f_target_pos_offset;

  // fetch draw target info (note the two levels of indirection here)
  auto& draw_target   = *app->pDrawTarget;
  auto screen_width   = draw_target.width ; // screen width  in pixels
  auto screen_height  = draw_target.height; // screen height in pixels

  if (
    screen_coord.x >= screen_width || screen_coord.y >= screen_height
    ||
    screen_coord.x + f_target_width < 0 || screen_coord.y + f_target_height < 0
    // ||
    // screen_coord.x < 0 || screen_coord.y < 0
  ) {
    return;
  }

  {
    // clip target into screen size

    // clip target position inside window
    f_target_pos_offset.x = lapse::max(-screen_coord.x, 0.0f);
    f_target_pos_offset.y = lapse::max(-screen_coord.y, 0.0f);

    // clip target size to window topleft edge
    f_target_width  -= f_target_pos_offset.x;
    f_target_height -= f_target_pos_offset.y;

    // clip size inside window bottom right edge
    f_target_width  = lapse::min(f_target_width , static_cast<f32>(screen_width)  - screen_coord.x - 1.0f);
    f_target_height = lapse::min(f_target_height, static_cast<f32>(screen_height) - screen_coord.y - 1.0f);
  }

  //
  // convert to integers for memcpy()
  //

  i32 screen_x = static_cast<i32>(screen_coord.x + f_target_pos_offset.x);
  i32 screen_y = static_cast<i32>(screen_coord.y + f_target_pos_offset.y);

  u32 u_target_width  = (u32)lapse::round(f_target_width );
  u32 u_target_height = (u32)lapse::round(f_target_height);

  u32 u_screen_x = (u32)screen_x;
  u32 u_screen_y = (u32)screen_y;

  {
    assert(screen_x >= 0 && u_screen_x + u_target_width  < u32(screen_width) );
    assert(screen_y >= 0 && u_screen_y + u_target_height < u32(screen_height));
    assert(sizeof(olc::Pixel) == sizeof(vec4<u8>));
  }

  auto line_offset = u_target_width*sizeof(olc::Pixel);

  //
  // Construct the scaled image
  //

  auto& screen_pixels = draw_target.pColData;
  auto img_pixels = img.m_u_pixels;
  auto scaled_img_pixels = get_scaled_image(img.id, scale);

  if (!scaled_img_pixels) {
    scaled_img_pixels = create_scaled_image(img.id, scale, u_target_full_width * u_target_full_height * sizeof(olc::Pixel));
    // auto* scaled_img_pixels = (vec4<u8>*)arenas::temp.push(line_offset * u_target_height);

    // scale the image
    u32 img_y, img_x;
    for (u32 y = 0; y < u_target_full_height; y++) {
      img_y = static_cast<u32>(lapse::floor_f_positive(f32(y) / scale));
      for (u32 x = 0; x < u_target_full_width; x++) {
        img_x = static_cast<u32>(lapse::floor_f_positive(f32(x) / scale));
        scaled_img_pixels[y * u_target_full_width + x] = img_pixels[img_y*image_width + img_x];
      }
    }
  }

  //
  // Render image onto screen draw target
  //

  // need to know where in the image we start drawing from if it was clipped
  u32 img_topleft_x = static_cast<u32>(f_target_pos_offset.x);
  u32 img_topleft_y = static_cast<u32>(f_target_pos_offset.y);

  for (u32 y = 0; y < u_target_height; y++) {
    memcpy(
      (void*)&screen_pixels[(u_screen_y+y) * u32(screen_width) + u_screen_x],
      (void*)&scaled_img_pixels[(y+img_topleft_y)*u_target_full_width + img_topleft_x],
      line_offset
    );
  }
}

void draw_bitmap_rs(vec2<> screen_coord, image& img, f32 scale, f32 rotation) {
  u32 image_width  = (u32)img.m_width;
  u32 image_height = (u32)img.m_height;

  // the 'target' is the rect in the img we're drawing
  f32 f_target_width  = lapse::round(static_cast<f32>(image_width)  * scale);
  f32 f_target_height = lapse::round(static_cast<f32>(image_height) * scale);

  // record original target size before we clip it
  u32 u_target_full_width  = static_cast<u32>(f_target_width );
  u32 u_target_full_height = static_cast<u32>(f_target_height);

  screen_coord *= scale;

  screen_coord.x = lapse::round(screen_coord.x);
  screen_coord.y = lapse::round(screen_coord.y);

  vec2<> f_target_pos_offset;

  // fetch draw target info (note the two levels of indirection here)
  auto& draw_target   = *app->pDrawTarget;
  auto screen_width   = draw_target.width ; // screen width  in pixels
  auto screen_height  = draw_target.height; // screen height in pixels

  if (
    screen_coord.x >= screen_width || screen_coord.y >= screen_height
    ||
    screen_coord.x + f_target_width < 0 || screen_coord.y + f_target_height < 0
    // ||
    // screen_coord.x < 0 || screen_coord.y < 0
  ) {
    return;
  }

  {
    // clip target into screen size

    // clip target position inside window
    f_target_pos_offset.x = lapse::max(-screen_coord.x, 0.0f);
    f_target_pos_offset.y = lapse::max(-screen_coord.y, 0.0f);

    // clip target size to window topleft edge
    f_target_width  -= f_target_pos_offset.x;
    f_target_height -= f_target_pos_offset.y;

    // clip size inside window bottom right edge
    f_target_width  = lapse::min(f_target_width , static_cast<f32>(screen_width)  - screen_coord.x - 1.0f);
    f_target_height = lapse::min(f_target_height, static_cast<f32>(screen_height) - screen_coord.y - 1.0f);
  }

  //
  // convert to integers for memcpy()
  //

  i32 screen_x = static_cast<i32>(screen_coord.x + f_target_pos_offset.x);
  i32 screen_y = static_cast<i32>(screen_coord.y + f_target_pos_offset.y);

  u32 u_target_width  = (u32)lapse::round(f_target_width );
  u32 u_target_height = (u32)lapse::round(f_target_height);

  u32 u_screen_x = (u32)screen_x;
  u32 u_screen_y = (u32)screen_y;

  {
    assert(screen_x >= 0 && u_screen_x + u_target_width  < u32(screen_width) );
    assert(screen_y >= 0 && u_screen_y + u_target_height < u32(screen_height));
    assert(sizeof(olc::Pixel) == sizeof(vec4<u8>));
  }

  // this is the size in bytes of one line of the image target
  auto line_offset = u_target_width*sizeof(olc::Pixel);
  // TODO I think this is actually called "pitch" ... or maybe "stride" ?

  //
  // Construct the scaled image
  //

  auto& screen_pixels = draw_target.pColData;
  auto img_pixels = img.m_u_pixels;
  // auto scaled_img_pixels = get_scaled_image(img.id, scale);

  // if (!scaled_img_pixels) {
    // scaled_img_pixels = create_scaled_image(img.id, scale, u_target_full_width * u_target_full_height * sizeof(olc::Pixel));
    auto scaled_img_pixels = (vec4<u8>*)arenas::temp.push(u_target_full_width * u_target_full_height * sizeof(olc::Pixel));

    // scale the image
    u32 img_y, img_x;
    for (u32 y = 0; y < u_target_full_height; y++) {
      img_y = static_cast<u32>(lapse::floor_f_positive(f32(y) / scale));
      for (u32 x = 0; x < u_target_full_width; x++) {
        img_x = static_cast<u32>(lapse::floor_f_positive(f32(x) / scale));
        scaled_img_pixels[y * u_target_full_width + x] = img_pixels[img_y*image_width + img_x];
      }
    }
  // }

  //
  // Render image onto screen draw target
  //

  // need to know where in the image we start drawing from if it was clipped
  u32 img_topleft_x = static_cast<u32>(f_target_pos_offset.x);
  u32 img_topleft_y = static_cast<u32>(f_target_pos_offset.y);

  for (u32 y = 0; y < u_target_height; y++) {
    memcpy(
      (void*)&screen_pixels[(u_screen_y+y) * u32(screen_width) + u_screen_x],
      (void*)&scaled_img_pixels[(y+img_topleft_y)*u_target_full_width + img_topleft_x],
      line_offset
    );
  }
}

olc::Pixel translate_color(vec4<u8> our_color) {
  return {our_color.x, our_color.y, our_color.z, our_color.w};
}

olc::Pixel& blend_pixels(olc::Pixel fg_color, olc::Pixel& bg_color) {
  auto blend_factor = static_cast<f32>(fg_color.a)/255.0f;
  fg_color *= blend_factor;
  bg_color = fg_color + bg_color * (1.0f - blend_factor);
  return bg_color;
}

void draw_bitmap_rotated(vec2<> screen_coord, image& img, f32 rotation) {
  vec2 img_size = vec2<>{f32(img.m_width), f32(img.m_height)};

  vec2<> current_coord{screen_coord};
  vec2<> end_coord = screen_coord + img_size;

  u32 image_width  = (u32)img_size.x;
  u32 image_height = (u32)img_size.y;

  u32 image_x = 0;
  u32 image_y = 0;

  auto& draw_target   = *app->pDrawTarget;
  auto& screen_pixels = draw_target.pColData;
  auto screen_width   = draw_target.width ; // screen width  in pixels
  auto screen_height  = draw_target.height; // screen height in pixels
  auto screen_size    = vec2<>{f32(screen_width), f32(screen_height)};

  if (
    screen_coord.less_than_or(0.0f) || screen_coord.greater_than_or(screen_size)
    ||
    end_coord.less_than_or(0.0f) || end_coord.greater_than_or(screen_size)
  ) {
    return;
  }

  while (current_coord.y < end_coord.y) {
    while(current_coord.x < end_coord.x) {
      // rotated x & y within the image coords
      u32 rot_image_x;
      u32 rot_image_y;

      // rotated x & y within screen coords
      u32 rot_screen_x;
      u32 rot_screen_y;
      // rotated x & y within screen coords as floats
      f32 f_rot_screen_x;
      f32 f_rot_screen_y;
      {
        // calculate rotated x & y
        auto f_rot_image_x = static_cast<f32>(image_x) - img_size.x/2.0f;
        auto f_rot_image_y = static_cast<f32>(image_y) - img_size.y/2.0f;

        auto old_x = f_rot_image_x;
        auto old_y = f_rot_image_y;

        f_rot_image_x = cos(rotation) * old_x + sin(rotation) * old_y;
        f_rot_image_y = cos(rotation) * old_y - sin(rotation) * old_x;

        f_rot_image_x += img_size.x/2.0f;
        f_rot_image_y += img_size.y/2.0f;

        rot_image_x = static_cast<u32>(f_rot_image_x);
        rot_image_y = static_cast<u32>(f_rot_image_y);

        rot_image_x = u32(f_rot_image_x < 0 ? image_width  + f_rot_image_x : f_rot_image_x);
        rot_image_y = u32(f_rot_image_y < 0 ? image_height + f_rot_image_y : f_rot_image_y);

        rot_image_x = rot_image_x >= image_width  ? rot_image_x - image_width  : rot_image_x;
        rot_image_y = rot_image_y >= image_height ? rot_image_y - image_height : rot_image_y;

        // rot_screen_x = u32(current_coord.x + f_rot_image_x) - image_x;
        // rot_screen_y = u32(current_coord.y + f_rot_image_y) - image_y;
      }

      rot_image_x = image_x;
      rot_image_y = image_y;

      {
        // rotate screen coords
        
        // we have to move the coords so that the center of the image is at 0,0
        //   this makes it so the image rotates about its center
        auto f_rot_x = current_coord.x - screen_coord.x - img_size.x/2.0f;
        auto f_rot_y = current_coord.y - screen_coord.y - img_size.y/2.0f;

        {
          auto old_x = f_rot_x;
          auto old_y = f_rot_y;

          f_rot_x =  cos(rotation) * old_x + sin(rotation) * old_y;
          f_rot_y = -cos(rotation) * old_y + sin(rotation) * old_x;
        }

        // translate coords back away from 0,0
        f_rot_x += screen_coord.x + img_size.x/2.0f;
        f_rot_y += screen_coord.y + img_size.y/2.0f;

        f_rot_screen_x = f_rot_x;
        f_rot_screen_y = f_rot_y;

        rot_screen_x = u32(f_rot_x);
        rot_screen_y = u32(f_rot_y);
      }

      // rot_screen_x = u32(current_coord.x);
      // rot_screen_y = u32(current_coord.y);

      if (
        rot_image_x >= 0 && rot_image_x < image_width &&
        rot_image_y >= 0 && rot_image_y < image_height
        &&
        rot_screen_x >= 0 && rot_screen_x < screen_width &&
        rot_screen_y >= 0 && rot_screen_y < screen_height
        &&
        img.m_u_pixels[rot_image_y*image_width + rot_image_x].a != 0
      ) {
        auto our_color = img.m_u_pixels[rot_image_y*image_width + rot_image_x];
        olc::Pixel their_color = {our_color.x, our_color.y, our_color.z, our_color.w};
        // olc::Pixel their_color = {(u8)rot_image_x, (u8)rot_image_y, 0, our_color.w}; // this'll show the coordinates
        screen_pixels[rot_screen_y * u32(screen_width) + rot_screen_x] = their_color;

        //
        // try to fix "missing pixels"
        //
        if (
          f_rot_screen_x > 1 && f_rot_screen_x < screen_width -2 &&
          f_rot_screen_y > 1 && f_rot_screen_y < screen_height-2
        ){
          auto x_diff = f_rot_screen_x;
          auto y_diff = f_rot_screen_y;

          // fract()
          x_diff -= static_cast<f32>(static_cast<i32>(x_diff));
          y_diff -= static_cast<f32>(static_cast<i32>(y_diff));

          auto abs_x_diff = lapse::abs(x_diff);
          auto abs_y_diff = lapse::abs(y_diff);

          u32 u_x_diff = static_cast<u32>(lapse::round(x_diff));
          u32 u_y_diff = static_cast<u32>(lapse::round(y_diff));

          olc::Pixel alarm_color = {255, 255, 0, our_color.w};

          if (x_diff != 0.0f) {
            olc::Pixel avg_color = translate_color(
              (
                (img.m_u_pixels[rot_image_y*image_width + rot_image_x] * (1.0f-abs_y_diff)) +
                (img.m_u_pixels[rot_image_y*image_width + rot_image_x + u_x_diff] * abs_y_diff)
              )
            );
            auto& screen_pixel = screen_pixels[(rot_screen_y) * u32(screen_width) + (rot_screen_x + u_x_diff)];
            screen_pixel = blend_pixels(avg_color, screen_pixel);
            // screen_pixels[(rot_screen_y) * u32(screen_width) + (rot_screen_x + u_x_diff)] = avg_color;
          }
          if (y_diff != 0.0f) {
            olc::Pixel avg_color = translate_color(
              (
                (img.m_u_pixels[rot_image_y*image_width + rot_image_x] * (1.0f-abs_x_diff)) +
                (img.m_u_pixels[(rot_image_y + u_y_diff)*image_width + rot_image_x] * abs_x_diff)
              )
            );
            auto& screen_pixel = screen_pixels[(rot_screen_y + u_y_diff) * u32(screen_width) + rot_screen_x];
            screen_pixel = blend_pixels(avg_color, screen_pixel);
          }
          if (x_diff != 0.0f && y_diff != 0.0f) {
            auto avg_abs_diff = (abs_x_diff + abs_y_diff)/2.0f;
            olc::Pixel avg_color = translate_color(
              (
                (img.m_u_pixels[rot_image_y*image_width + rot_image_x] * (1.0f-avg_abs_diff)) +
                (img.m_u_pixels[(rot_image_y + u_y_diff)*image_width + rot_image_x + u_x_diff] * avg_abs_diff)
              )
            );
            auto& screen_pixel = screen_pixels[(rot_screen_y + u_y_diff) * u32(screen_width) + rot_screen_x + u_x_diff];
            screen_pixel = blend_pixels(avg_color, screen_pixel);
          }
        }
      } else {
        // __debugbreak();
      }

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
  // app->Clear(olc::Pixel{(u8)color.r, (u8)color.g, (u8)color.b});

  auto& screen_pixels = app->pDrawTarget->pColData;
  auto width          = app->pDrawTarget->width;
  auto height         = app->pDrawTarget->height;

  auto p_color = olc::Pixel{(u8)color.r, (u8)color.g, (u8)color.b};
  i32 clear_color = p_color.n;
  auto line_byte_size = width*(i32)sizeof(olc::Pixel);

  for (i32 y = 0; y < height; y++) {
    memset((void*)&screen_pixels[y*width], clear_color, line_byte_size);
  }
}

//-----------------------------------------------------------------------------
// Image cache (for scaled images)
//-----------------------------------------------------------------------------

void allocate_image_cache() {
  arenas::image_cache.reserve(10'000'000);
}

struct ImageCacheRecord{
  uid img_id;
  vec4<u8>* scaled_pixels;
  // u32 size_in_bytes;
};

// limit of how many images can be stored in cache
inline u32 get_image_cache_max_length() {
  return 100;
}

ImageCacheRecord* get_image_cache_table() {
  return (ImageCacheRecord*)((u8*)(arenas::image_cache.m_memory_start)+4);
}

vec4<u8>* get_scaled_image(uid img_id, f32 scale) {
  vec4<u8>* output_img = nullptr;

  auto maximum_possible_images = get_image_cache_max_length();

  f32& arena_scale = *(f32*)arenas::image_cache.m_memory_start;
  if (arena_scale != scale) {
    arenas::image_cache.clear();
    arenas::image_cache.push(4);
    arena_scale = scale;

    // initialize image cache table
    arenas::image_cache.push_zeroes(maximum_possible_images * sizeof(ImageCacheRecord));
  } else {
    // try to get scaled image from cache
    // foreach through cache records to find this img_id
    auto record = get_image_cache_table();
    auto i = 0;
    while(record->img_id != img_id) {
      record++;
      i++;
      // if record is null we've gone past the length of the table
      if (record->scaled_pixels == nullptr) { return nullptr; }
      // if we hit 100 records we've overrun the table
      if (i >= maximum_possible_images) { return nullptr; }
    }
    output_img = record->scaled_pixels;
  }

  return output_img;
}

vec4<u8>* create_scaled_image(uid img_id, f32 scale, u32 size_in_bytes) {
  // allocate space for the cached image
  vec4<u8>* cached_img = (vec4<u8>*)arenas::image_cache.push(size_in_bytes);

  // what we want to do is:
  //    image_cache_table.push(ImageCacheRecord{ ... })
  // so we have to find the index for that .push()
  auto table = get_image_cache_table();
  auto record = table;
  while (record->scaled_pixels != nullptr) { record++; }
  assert(record < table+get_image_cache_max_length());

  // set the record at that index
  *record = ImageCacheRecord{
    img_id,
    cached_img,
    // size_in_bytes
  };

  return cached_img;
}

//-----------------------------------------------------------------------------
// Input functions
//-----------------------------------------------------------------------------

// returns true if mouse hit event happened this frame
bool is_mouse_left_button_hit() {
  return app->GetMouse(olc::Mouse::LEFT).bPressed;
}

// returns true every frame while left mouse button is down
bool is_mouse_left_button_down() {
  return app->GetMouse(olc::Mouse::LEFT).bHeld;
}

// returns true if mouse hit event happened this frame
bool is_mouse_right_button_hit() {
  return app->GetMouse(olc::Mouse::RIGHT).bPressed;
}

// returns true each frame while button is held down
bool is_mouse_middle_button_down() {
  return app->GetMouse(olc::Mouse::MIDDLE).bHeld;
}

vec2<i32> get_mouse_pos() {
  olc::vi2d temp = app->GetMousePos(); //app->GetWindowMouse();
  return vec2<i32>{temp.x*pixel_size, temp.y*pixel_size};
}

i32 get_mouse_wheel_delta() {
  return app->GetMouseWheel();
}

olc::Key translate_keycode(keycode code) {
  assert(keycode_list().contains(code));
  switch(code) {
    case keycode::escape    : return olc::Key::ESCAPE;
    case keycode::space_bar : return olc::Key::SPACE;
    case keycode::number_1  : return olc::Key::K1;
    case keycode::number_2  : return olc::Key::K2;
    case keycode::number_3  : return olc::Key::K3;
    case keycode::a         : return olc::Key::A;
    case keycode::d         : return olc::Key::D;
    case keycode::s         : return olc::Key::S;
    case keycode::w         : return olc::Key::W;
    default: return olc::Key::NONE;
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
  for (u32 i = 0; i < codes.length(); i++) {
    auto key_state = key(codes[i]);
    if (key_state.is_hit()) {
      key_state.toggle();
      std::cout << "toggling " << (u32)codes[i] << "\n";
    }
  }

  // poll mouse stuff
  auto& mouse = Mouse::the();
  mouse.m_pos = platform::get_mouse_pos();
  mouse.m_pos_delta = mouse.m_old_pos - mouse.m_pos;
  mouse.m_old_pos = mouse.m_pos;
}

}; // end platform namespace
