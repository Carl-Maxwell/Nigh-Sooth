
#include "lapse_scalar.h"
#include "lapse_string.h"

namespace platform{

void initialize(lapse::u32 screen_width, lapse::u32 screen_height, bool fullscreen, lapse::str window_name);

void set_initialization_callback(lapse_lambda(void, void) arg_initialization_callback);

void set_main_loop_callback(lapse_lambda(void, lapse::f32) main_loop);

void plot(lapse::vec2<lapse::f32> screen_coord, lapse::vec3<lapse::f32> color);

}
