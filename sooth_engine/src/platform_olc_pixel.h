#pragma once

#include "lapse_scalar.h"
#include "lapse_string.h"

namespace platform{

void initialize(lapse::u32 screen_width, lapse::u32 screen_height, bool fullscreen, lapse::str window_name);

void set_initialization_callback(lapse_lambda(void, void) arg_initialization_callback);

void set_main_loop_callback(lapse_lambda(void, lapse::f32) main_loop);

lapse::u64 get_frame_count();

lapse::u32 get_window_width();
lapse::u32 get_window_height();

void plot(lapse::vec2<> screen_coord, lapse::vec3<> color);
void plot(lapse::vec2<> screen_coord, lapse::vec3<lapse::u8> color);

void draw_bitmap(
  lapse::vec2<>  screen_coord,
  lapse::vec2<>  image_size,
  lapse::vec3<lapse::u8>* pixels
);

}
