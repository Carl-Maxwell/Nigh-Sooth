/*
#include "lapse_lib.h"

using namespace lapse;

bool string_test(){
  std::cout << "testing strings\n";

  LapseErrorQueue::the().register_callback([](lapse::error_code err){
    std::cout << "Error! " << (int)err;
  });

  bool good = true;

  str number = "1";
  std::cout << "number: " << number.to_c_str() << " == " << number.to_integer<u32>();

  if (good) {
    std::cout << "test passed\n";
  } else {
    std::cout << "test FAILED\n";
  }

  return good;
}
*/
