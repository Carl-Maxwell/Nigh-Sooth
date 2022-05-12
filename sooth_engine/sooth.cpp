
#include "lapse_lib.h"
#include "sooth_input.h"
#include "platform_olc_pixel.h"
#include "sooth_image.h"

using namespace lapse;

namespace sooth{

void initialize_engine() {
  lapse::LapseErrorQueue::the().register_callback([](lapse::error_code err){
    std::cout << "Lapse Error! " << (int)err;
    switch(err){
      case lapse::error_code::undefined:  __debugbreak(); break;
      case lapse::error_code::failure:    __debugbreak(); break;
      case lapse::error_code::success:    __debugbreak(); break;
      case lapse::error_code::close_app:  platform::close_application(); break;
      case lapse::error_code::breakpoint: __debugbreak(); break;
    }
  });

  lapse::seed_rng((u32)platform::get_application_start_time());
}

}
