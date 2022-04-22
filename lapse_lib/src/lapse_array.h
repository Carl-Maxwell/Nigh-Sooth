#pragma once

#include "lapse_lib.h"

namespace lapse{

// TODO need a way of enabling/disabling safety checks

// a dynamic-sized array, like to std::vector
template<typename T>
struct array{
  T* elements = nullptr; // contents of array
  u32 size    = 0; // allocated space
  u32 length  = 0; // count of elements inserted

  array(u32 n) { reserve(n); };
  // returns elem, with safety check
  inline T& at(u32 i) const {
    if (!size || i > size-1) {
      error(error_code::breakpoint);
      return *(T*)nullptr;
    } else {
      return elements[i];
    }
  };
  // removes all elements
  void clear() { delete elements; elements = nullptr; size = 0; length = 0; };
  void push(T elem) {
    elements[length] = elem;
    length++;

    if (length > size) {
      error(error_code::breakpoint);
    }
  };
  // remove all elements equal to argument,
  //   compacts array with each removal
  void remove(T elem) {
    for (u32 i = 0; i < length; i++) {
      if (elements[i] == elem) {
        remove_at(i);
        return remove(elem);
        // TODO recursion here might hit a stack overflow
        //   when removing all elems from large arrays
      }
    }
  }
  void remove_at(u32 removal_i) {
    for (u32 i = removal_i; i < length-1; i++) {
      elements[i] = elements[i+1];
    }
    length--;
  }
  void reserve(u32 better_size) {
    if (length == 0) {
      elements = new T[better_size];
      size = better_size;
    } else {

    }
  }

  inline T& operator[](u32 i) const {
    return at(i);
  };

  // TODO initializer list

  // TODO operator[]
  // TODO .join()
  // TODO .sort()
  // TODO .sort(lambda) -- comparison lambda
  // TODO .search()
  // TODO .count(elem)
  // TODO .count(lambda)
  // TODO iterator, range based for loop compatibility
  // TODO .sample(n) -- returns n random elements
  // TODO .slice() 
  // TODO .select(lambda) -- returns a sub array where each lambda(elem) returns true
  // TODO .shuffle(start), .shuffle(start, length)
  // TODO .map(lambda) -- each elem = lambda(elem)
  // TODO .unique() -- removes duplicate elems
  // TODO .sum(lambda), .sum(init, lambda) -- like map_reduce
  // TODO .transpose() -- [[1, 2], [3, 4]] -> [[1, 3], [2, 4]]

  // TODO .union(arr)
  // TODO .difference(arr)
};

// a fixed-size array, like default arrays in C++
template<typename T>
struct fixed_array{
  T* elements = nullptr; // contents of array
  u32 size    = 0; // allocated space
  u32 length  = 0; // count of elements inserted

  fixed_array() {};
  fixed_array(u32 n) { reserve(n); };
  // returns elem, with safety check
  inline T& at(u32 i) const {
    if (!size || i > size-1) {
      error(error_code::breakpoint);
      return *(T*)nullptr;
    } else {
      return elements[i];
    }
  };
  // removes all elements
  void clear() { delete elements; elements = nullptr; size = 0; length = 0; };
  // like .push_back(), add an element to the end of the array
  void push(T elem) {
    at(length) = elem;
    length++;

    if (length > size) {
      error(error_code::breakpoint);
    }
  };
  // remove all elements equal to argument,
  //   compacts array with each removal
  void remove(T elem) {
    for (u32 i = 0; i < length; i++) {
      if (at(i) == elem) {
        remove_at(i);
        return remove(elem);
        // TODO recursion here might hit a stack overflow
        //   when removing all elems from large arrays
      }
    }
  }
  void remove_at(u32 removal_i) {
    for (u32 i = removal_i; i < length-1; i++) {
      elements[i] = elements[i+1];
    }
    length--;
  }
  void reserve(u32 better_size) {
    if (length == 0) {
      elements = new T[better_size];
      size = better_size;
    } else {
      T* old_elements = elements;
      elements = new T[better_size];
      for (u32 i = 0; i < length; i++) {
        elements[i] = old_elements[i];
      }
      delete elements;
      size = better_size;
    }
  }

  inline T& operator[](u32 i) const {
    return at(i);
  };
};

};