
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
  // arrays
  //

  {
    std::cout << "";

    bool good = true;
  
    lapse::LapseErrorQueue::the().register_callback([](lapse::error_code err){
      std::cout << "Error! " << (int)err;
    });

    lapse::fixed_array<lapse::u32> arr(4);
    
    arr.push(5);
    arr.push(3);
    arr.push(6);
    arr.push(1);

    good &= arr[0] == 5 && arr[1] == 3 && arr[2] == 6 && arr[3] == 1;

    // .clear()

    good &= arr.size == 4 && arr.length == 4;

    arr.clear();

    good &= arr.size == 0 && arr.length == 0;

    good &= arr.elements == nullptr;

    // .remove()

    arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    good &= arr[0] == 1 && arr[5] == 6 && arr[9] == 10;

    arr.remove(6);

    good &= arr.size == 10 && arr.length == 9;
    good &= arr[0] == 1 && arr[8] == 10;

    arr.clear();
    arr = {1, 1, 1, 2};
    arr.remove(1);

    // .remove_at()

    arr = {1, 1, 1, 2};
    arr.remove_at(1);
    good &= arr[2] == 2;

    for (lapse::u32 i = 0; i < arr.length; i++) {
      std::cout << "array[" << i << "] " << arr[i] << "\n";
    }

    if (good) {
      std::cout << "test passed\n";
    } else {
      std::cout << "test FAILED\n";
    }
  }

  return 0;
}