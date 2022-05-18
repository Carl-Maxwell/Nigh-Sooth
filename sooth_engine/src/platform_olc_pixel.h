#pragma once

#include "lapse_scalar.h"
#include "lapse_string.h"
#include "lapse_geometry.h"
#include "sooth_image.h"
#include "sooth_input.h"

namespace platform{

lapse::f64 get_application_start_time();

void allocate_image_cache();

lapse::i32 get_pixel_size();

void initialize(lapse::i32 screen_width, lapse::i32 screen_height, bool fullscreen, lapse::str& window_name, lapse::i32 a_pixel_size = 4);

void close_application();
void finalize_shutdown();
bool start_application();

void set_main_loop_callback(lapse_lambda(void, lapse::f32) main_loop);

lapse::u64 get_frame_count();

lapse::u32 get_window_width();
lapse::u32 get_window_height();
// gets window resolution, after dividing it by pixel_size
//   so you'd multiply this by pixel_size if you want the resolution in monitor pixels
lapse::vec2<> get_window_size();

lapse::f32 get_window_padding();

void plot(lapse::vec2<> screen_coord, lapse::vec3<> color);
void plot(lapse::vec2<> screen_coord, lapse::vec3<lapse::u8> color);

void draw_line(lapse::vec2<> start, lapse::vec2<> end, lapse::vec3<> color = {1.0, 1.0, 1.0});
void draw_text(lapse::str& text, lapse::vec2<> position, lapse::vec3<> color = {1.0, 1.0, 1.0},  lapse::u32 font_size_pixels = 8);
void draw_rect(lapse::vec2<> start, lapse::vec2<> size, lapse::vec3<> color = {1.0, 1.0, 1.0});
void draw_rect(lapse::rect<> box, lapse::vec3<> color = {0.0f, 0.0f, 0.0f});
void fill_rect(lapse::rect<> box, lapse::vec3<> color = {0.0f, 0.0f, 0.0f});

void draw_bitmap(lapse::vec2<> screen_coord, image& img);
// draw bitmap, and scale it
void draw_bitmap_scaled(lapse::vec2<> screen_coord, image& img, lapse::f32 scale = 1.0f);
// draw bitmap with rotation and scale
void draw_bitmap_rs(lapse::vec2<> screen_coord, image& img, lapse::f32 scale, lapse::f32 rotation);
// draw bitmap with rotation
void draw_bitmap_rotated(lapse::vec2<> screen_coord, image& img, lapse::f32 rotation);

void clear(lapse::vec3<> color);

//
// input functions
//

bool is_mouse_left_button_hit();
bool is_mouse_right_button_hit();
lapse::vec2<lapse::i32> get_mouse_pos();
// get how much the mouse wheel was rolled this frame
lapse::i32 get_mouse_wheel_delta();

void poll_key_toggles();
KeyState get_key_state(keycode code);

} // end namespace
