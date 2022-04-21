
#include <iostream>

#include "lapse_lib.h"

int main() {

  std::cout << "Running lapse_lib tests\n";

  //
  // typedefs
  //

  {
    std::cout << "is bitwise right shift working?\n";
    int a = 2 >> 1;
    if ((2 >> 1) == 1) {
      std::cout << "test passed\n";
    }
  }

  {
    // are typedefs there?

    lapse::u64 u64_max = lapse::u64(-1);
    int good_bit = 1;

    for (int i = 0 ; i < sizeof(u64_max); i++) {
      good_bit = good_bit & u64_max;
      u64_max = u64_max >> 1;
    }

    std::cout << "18446744073709551615 == \n" << lapse::u64(-1) << "\n";

    if (good_bit == 1) {
      std::cout << "u64 type working\n";
      std::cout << "test passed\n";
    }
  }

  //
  // exceptions
  //

  {
    std::cout << "exceptions defaulting to undefined code?\n";
    lapse::error_code err{};

    if (err == lapse::error_code::undefined) {
      std::cout << "test passed\n";
    }
  }

  {
    std::cout << "does the error queue noexcept_list work?\n";

    lapse::LapseErrorQueue::noexcept_list<int> a_list;

    bool good = true;

    a_list.push(8);

    good = a_list.front->value == 8;

    a_list.remove(a_list.front);

    good &= a_list.is_empty();

    a_list.push(1);
    a_list.push(2);
    a_list.push(3);
    a_list.push(4);

    good &= a_list.front->next->next->next->value == 1;

    a_list.remove(a_list.front->next->next->next); // remove 1

    good &= a_list.front->next->next->next == nullptr;

    a_list.remove(a_list.front->next); // remove 3

    good &= a_list.front->next->value == 2;

    a_list.remove(a_list.front); // remove 4

    good &= a_list.front->value == 2;

    good &= !a_list.is_empty();

    if (good) {
      std::cout << "test passed\n";
    } else {
      std::cout << "test FAILED\n";
    }
  }

  // registering error callbacks, throwing errors
  
  {
    bool good = true;

    std::cout << "registering error callbacks, throwing errors\n";

    lapse::error_code expected_code{};

    // allocate space for two pointers
    void* captures = malloc(sizeof(bool*) + sizeof(lapse::error_code*));

    *(bool**)captures = &good;

    // move up 8 bytes (one pointer sizeof in bytes)
    void* capture_point = sizeof(bool*) + (char*)(void*)captures;

    lapse::LapseErrorQueue::the().register_callback(
      [&good, &expected_code](lapse::error_code err) {
        bool pre_good = good;
        good &= err == expected_code;
        if (pre_good != good) {
          std::cout << "Failed on error code #: " << (int)expected_code << "\n";
        }
      }
    );

    expected_code = lapse::error_code::undefined;
    lapse::error(expected_code);
    lapse::LapseErrorQueue::the().tick();
    expected_code = lapse::error_code::failure;
    lapse::error(expected_code);
    lapse::LapseErrorQueue::the().tick();
    expected_code = lapse::error_code::success;
    lapse::error(expected_code);
    lapse::LapseErrorQueue::the().tick();
    expected_code = lapse::error_code::close_app;
    lapse::error(expected_code);
    lapse::LapseErrorQueue::the().tick();
    expected_code = lapse::error_code::breakpoint;
    lapse::error(expected_code);
    lapse::LapseErrorQueue::the().tick();

    if (good) {
      std::cout << "test passed\n";
    } else {
      std::cout << "test FAILED\n";
    }
  }

  //
  // 
  //

  return 0;
}