
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

    lapse::error_code expected_code{lapse::error_code::breakpoint};

    // allocate space for two pointers
    void* captures = malloc(sizeof(bool*) + sizeof(lapse::error_code*));

    *(bool**)captures = &good;

    // move up 8 bytes (one pointer sizeof in bytes)
    void* capture_point = sizeof(bool*) + (char*)(void*)captures;

    *(lapse::error_code**)capture_point = &expected_code;

    lapse::LapseErrorQueue::the().register_callback(
      lapse::error_queue_callback{
        [](lapse::error_code err) {
          std::cout << "inside the callback!\n";
          int i = 493;
          i += 2348092;
          if (err == lapse::error_code::breakpoint) {
            std::cout << "breakpoint called for \n";
            __debugbreak();
          }
        },
        captures
      }
    );

    lapse::error(expected_code);

    lapse::LapseErrorQueue::the().tick();

    // lapse::LapseErrorQueue::the().register_callback(
    //   [&good, expected_code](lapse::error_code err) {
    //   good &= err == expected_code;
    // } );

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