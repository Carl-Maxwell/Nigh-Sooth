#pragma once

#include "lapse_scalar.h"
#include "lapse_math.h"
#include "lapse_lambda.h"
#include "lapse_exceptions.h"

#include <initializer_list>

namespace lapse{

// TODO need a way of enabling/disabling safety checks

// a fixed-size array, like default arrays in C++
template<typename T>
class fixed_array{
public:
  T* m_elements = nullptr; // contents of array
  u32 m_size    = 0; // allocated space
  u32 m_length  = 0; // count of elements inserted

  fixed_array() {};
  fixed_array(u32 n) { reserve(n); };
  fixed_array(std::initializer_list<T> i_list) {
    reserve(i_list.size());
    for (auto elem : i_list) {
      push(elem);
    }
  };
  // returns elem, with safety check
  inline T& at(u32 i) const {
    if (!m_size || i > m_size-1) {
      error(error_code::breakpoint);
      return *(T*)nullptr;
    } else {
      return m_elements[i];
    }
  };
  // removes all elements
  void clear() { delete m_elements; m_elements = nullptr; m_size = 0; m_length = 0; };
  bool contains(T elem) {
    for (u32 i = 0; i < m_length; i++) {
      if (elem == m_elements[i]) {
        return true;
      }
    }
    return false;
  };
  // transforms array to str with sep between elems
  char* join(const char* sep = ", ") {
    char* output;
    u32 length_sum = 0;
    for (u32 i = 0; i < m_length; i++) {
      length_sum += m_elements[i].to_c_str_estimate_length();
    }

    auto sep_length = 0;
    while (sep[sep_length++] != '\0') {}
    length_sum += m_length-1 * sep_length;

    output = new char[length_sum];

    u32 str_pos = 0;

    for (u32 i = 0; i < m_length; i++) {
      char* sub_str = m_elements[i].to_c_str();
      u32 i2 = 0;
      while (sub_str[i2] != '\0') { output[str_pos++] = sub_str[i2]; }
      if (i != m_length) { 
        for (auto sep_pos = 0; sep_pos < sep_length; sep_pos++) {
          output[str_pos++] = sep[sep_pos];
        }
      }
    }
    return output;
  };
  /*
  template<typename T_STR>
  // transforms array to str with sep between elems
  T_STR join(T_STR sep = ", ") {
    // TODO is there a more efficient way to do this? With a c_str for example?
    //   maybe each obj should have a estimate_str_len func? then we could reserve(sum)
    T_STR output = "";
    for (u32 i = 0; i < m_length; i++) {
     output += m_elements[i].to_str();
     if (i != m_length) { output += sep; }
    }
    return output;
  };
  */
  u32 length() { return m_length; }
  // transform each elem with lambda : elem = lambda(elem)
  fixed_array& map(lapse_lambda(T, T) callback) {
    for (u32 i = 0; i < m_length; i++) {
      m_elements[i] = callback(m_elements[i]);
    }

    return *this;
  }
  // like .push_back(), add an element to the end of the array
  void push(T elem) {
    at(m_length) = elem;
    m_length++;

    if (m_length > m_size) {
      error(error_code::breakpoint);
    }
  };
  // remove all elements equal to argument,
  //   compacts array with each removal
  void remove(T elem) {
    for (u32 i = 0; i < m_length; i++) {
      if (at(i) == elem) {
        remove_at(i);
        return remove(elem);
        // TODO recursion here might hit a stack overflow
        //   when removing all elems from large arrays
      }
    }
  }
  void remove_at(u32 removal_i) {
    for (u32 i = removal_i; i < m_length-1; i++) {
      m_elements[i] = m_elements[i+1];
    }
    m_length--;
  }
  void reserve(u32 better_size) {
    if (m_length == 0) {
      m_elements = new T[better_size];
      m_size = better_size;
    } else {
      error(error_code::close_app);
    }
  }

  inline T& operator[](u32 i) const {
    return at(i);
  };
  fixed_array<T>& operator=(std::initializer_list<T> i_list) {
    if (m_size) {
      clear();
    }
    reserve(static_cast<u32>(i_list.size()));
    for (auto elem : i_list) {
      push(elem);
    }
    return *this;
  };

};

// a dynamic-sized array, similar to std::vector
template<class T>
class array : public fixed_array<T>{
public:
  // T* m_elements = nullptr; // contents of array
  // u32 m_size    = 0; // allocated space
  // u32 m_length  = 0; // count of elements inserted

  array() {};
  array(u32 start_size) { reserve(start_size); };

  void push(T elem) {
    this->m_elements[this->m_length] = elem;
    this->m_length++;

    if (this->m_length == this->m_size) {
      reserve(u32(ceil_i(f32(this->m_size) * 1.5f)));
    }
  };
  void reserve(u32 better_size) {
    if (this->m_length == 0) {
      this->m_elements = new T[better_size];
      this->m_size = better_size;
    } else {
      T* old_elements = this->m_elements;
      this->m_elements = new T[better_size];
      for (u32 i = 0; i < this->m_length; i++) {
        this->m_elements[i] = old_elements[i];
      }

      // TODO callback for reference stability?

      delete old_elements;
      this->m_size = better_size;
    }
  }

  array& operator+=(const array other) {
    u32 total_length = this->m_length + other.m_length;
    reserve(total_length);
    
    for (u32 i = this->m_length; i < total_length; i++) {
      this->m_elements[i] = other[i];
    }
    return *this;
  };

  // TODO .join()
  // TODO .insert(index, elem)
  // TODO .sort()
  // TODO .sort(lambda) -- comparison lambda
  // TODO .search(elem), .search(lambda)
  // TODO .count(elem), .count(lambda)
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

};

// temporarily putting this here

#include "lapse_range.h"
