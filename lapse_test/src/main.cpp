
#include <iostream>

#include "lapse_lib.h"

using namespace lapse;

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
  // math
  //

  {
    std::cout << "testing math library (round, floor, etc) \n";

    lapse::LapseErrorQueue::the().register_callback([](lapse::error_code err){
      std::cout << "Error! " << (int)err;
    });

    bool good = true;

    {
      // test round, ceil, floor, etc

      good &= lapse::round( 0.1) ==  0 && lapse::round( 0.9) ==  1;
      good &= lapse::round(-3.2) == -3 && lapse::round(-3.9) == -4;

      good &= lapse::floor_i( 1.9f) ==  1 && lapse::floor_f( 1.9f) ==  1;
      good &= lapse::floor_i(-1.9f) == -2 && lapse::floor_f(-1.9f) == -2;
      good &= lapse::floor_i( 1.1f) ==  1 && lapse::floor_f( 1.1f) ==  1;
      good &= lapse::floor_i(-1.1f) == -2 && lapse::floor_f(-1.1f) == -2;

      good &= lapse::ceil_i( 1.1f) ==  2 && lapse::ceil_f( 1.1f) ==  2;
      good &= lapse::ceil_i(-1.1f) == -1 && lapse::ceil_f(-1.1f) == -1;
      good &= lapse::ceil_i( 1.9f) ==  2 && lapse::ceil_f( 1.9f) ==  2;
      good &= lapse::ceil_i(-1.9f) == -1 && lapse::ceil_f(-1.9f) == -1;
    }

    {
      // testing pow(), sqrt(), etc

      good &= lapse::pow(2, 0) == 1 && lapse::pow(2, 1) == 2;
      good &= lapse::pow(2, 2) == 4 && lapse::pow(2, 5) == 32;

      std::cout << "37'822'859'361\n" << lapse::pow(21, 8) << "\n";

      std::cout << "u32 1234567 to c_str: " << u32_to_c_str(1234567) << "\n";

      {
        // test f32_to_c_str_base2()

        for (int i = 3; i < 100; i++) {
          f32_obj num {i};
          // std::cout << i << ".0f == (1.0 + " << num.window_portion() << ") * 2**" << num.exponent() << " == " << lapse::f32_to_c_str_base2(i) << "\n";
        }

      }

      {
        // testing f32_to_c_str()

        std::cout << 10'000'000'000.0f << " f32_to_c_str() == " << f32_to_c_str(10'000'000'000.0f) << "\n";

        for (int i = 3; i < 100; i++) {
          f32_obj num {i};
          // std::cout << i << " == " << f32_to_c_str(i) << "\n";
        }

        // std::cout << "321.0f   : " << lapse::f32_c_str(321.0f) << "\n";
        // std::cout << "123.0f   : " << lapse::f32_c_str(123.0f) << "\n";
        // std::cout << "-123.0f   : " << lapse::f32_c_str(-123.0f) << "\n";
        // std::cout << "321.123f : " << lapse::f32_c_str(321.123f) << "\n";
        // std::cout << "-321.123f : " << lapse::f32_c_str(-321.123f) << "\n";
        // std::cout << "987654321.123456789 : " << 987654321.123456789 << "\n";
        // std::cout << "987654321.123456789 : " << lapse::f32_c_str(987654321.123456789f) << "\n";
        // std::cout << "12'000'000'000'000'000'000'000'000'000'000'000'000.0f : " << lapse::f32_c_str(12'000'000'000'000'000'000'000'000'000'000'000'000.0f) << "\n";

      }

      {
        // testing f64_c_str()

        // std::cout << "987654321.123456789 : " << lapse::f64_c_str(987654321.123456789) << "\n";
      }

      // lapse::f32_c_str(987654321.123456789);
      // lapse::f32_c_str(lapse::pow(21, 8));

      std::cout << lapse::u32_max << "\n" << lapse::pow(2, 32)-1 << "\n";

      good &= (lapse::pow(2, 32)-1) == lapse::u32_max && lapse::pow(21, 8) == 37'822'859'361;

      // if (!good) { std::cout << "test passed\n"; }

      lapse::fixed_array<lapse::i32> powers_of_two;
      powers_of_two = lapse::range(1, 10).map([](lapse::i32 a){ return pow(2, a); });

      good &= powers_of_two[powers_of_two.length-1] && lapse::pow(2, 10);

      std::cout << "log base 4 of 29: " << lapse::logarithm_i(4, 29) << "\n";
      std::cout << "log base 2 of 17: " << lapse::logarithm_i(2, 29) << "\n";
      for (int i = 0; i < 100; i++) {
        good &= lapse::sqrt(lapse::pow(2, (f32)i)) == i;
        good &= powers_of_two.contains( lapse::pow(2, (f32)lapse::logarithm_i(2, (f32)i)) );
        // std::cout << "lapse::pow(2, lapse::logarithm_i(2, " << i << ")))" << lapse::pow(2, lapse::logarithm_i(2, i)) << "\n";
      }

      good &= lapse::sqrt(4) == 2 && lapse::round(lapse::sqrt(lapse::u32_max)) == 65'536;

    }

    if (good) {
      std::cout << "test passed\n";
    } else {
      std::cout << "test FAILED\n";
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
    std::cout << "Testing fixed_array\n";

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

  {
    std::cout << "testing dynamic array\n";

    lapse::LapseErrorQueue::the().register_callback([](lapse::error_code err){
      std::cout << "Error! " << (int)err;
    });

    bool good = true;

    if (good) {
      std::cout << "test passed\n";
    } else {
      std::cout << "test FAILED\n";
    }
  }

  return 0;
}