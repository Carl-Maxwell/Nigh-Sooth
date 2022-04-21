#pragma once

#include "lapse_lib.h"

namespace lapse{

template<typename T>
struct array{
  T* elements = nullptr; // contents of array
  u32 size    = 0; // allocated space
  u32 length  = 0; // count of elements inserted

  void push(T elem) {
    elements[length] = elem;
    length++;

    if (length > size) {
      // error
    }
  };

  // TODO operator[]
  // TODO .join()
  // TODO .sort()
};

template<typename T>
struct dynamic_array{
  array<T> elements;

};

};