#pragma once

#include "lapse_scalar.h"
#include "lapse_math.h"
#include "lapse_lambda.h"
#include "lapse_exceptions.h"
#include "lapse_random.h"
#include "lapse_temp.h"

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
    reserve((u32)i_list.size());
    for (T elem : i_list) {
      push(elem);
    }
  };

  ~fixed_array() {
    std::cout << "~fixed_array()\n";
    if (m_elements) delete[] m_elements;
    // __debugbreak();
    // if (m_elements && sizeof(T) <= 8) {
      // __debugbreak();
      // std::cout << *(u64*)(void*)&m_elements[0] << "\n";
    // }
      
  }

  // fixed_array(const fixed_array& other) {

  // }

  // returns elem, with safety check
  inline T& at(i32 i) const {
    assert(m_size && i < i32(m_size) && i >= 0);
    return m_elements[i];
  };
  // returns elem at index = length+i
  //   so you use this if you're sending in a negative number
  inline T& at_reverse(i32 i) const {
    assert(m_size && m_length + i >= 0 && m_length + i < m_size);
    return m_elements[m_length + i];
  };
  // removes all elements
  void clear() {
    // TODO rename this to .dealloc()
    assert(m_size);
    delete[] m_elements;
    m_elements = nullptr;
    m_size = 0;
    m_length = 0;
  };
  // removes all elements (but keeps allocation)
  void clear_no_dealloc() {
    // TODO rename this to .clear()
    m_length = 0;
  };
  bool contains(T elem) const {
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

    output = (char*)arenas::temp.push(length_sum);

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
  i32 last_index() { return m_length-1; }
  u32 length() const { return m_length; }
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
    // TODO memcpy?
    for (u32 i = removal_i; i < m_length-1; i++) {
      m_elements[i] = m_elements[i+1];
    }
    m_length--;
  }
  // allocate space for the array
  void reserve(u32 better_size) {
    assert(m_length == 0);
    m_elements = new T[better_size];
    m_size = better_size;
  }
  // returns n random elements
  T& sample(u64 n=1) {
    assert(m_length > u32(0));
    assert(n == u64(1)); // TODO implement loop
    u32 r = (u32)rand_integer(m_length-u64(1));
    assert(r < length()-u32(1) && r >= u32(0));
    return m_elements[r];
  }

  inline T& operator[](i32 i) const {
    return at(i);
    // TODO if this operator is used to add elements to an array .length() is not updated
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

  ~array() {
    std::cout << "~array()";
  }

  T& pop() {
    assert(this->m_length > 0);
    this->m_length -= 1;
    return this->m_elements[this->m_length];
  };
  void push(T elem) {
    assert(this->m_size);

    // note: you need to call reserve() or use the constructor before pushing elements
    //   otherwise you get an exception here

    this->m_elements[this->m_length] = elem;
    
    this->m_length++;

    // TODO currently this grows the array allocation when you hit the size ...
    //   shouldn't it instead grow only if you try to push past the size?
    //   Otherwise you'll always be allocating more than you actually use.

    if (this->m_length == this->m_size) {
      reserve(u32(ceil_i(f32(this->m_size) * 1.5f)));
    }
  };
  // allocate space for the array, copying existing elems into new allocation
  void reserve(u32 better_size) {
    assert(better_size);
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

      delete[] old_elements;
      this->m_size = better_size;
    }
  }
  array& do_unique(bool is_sorted = false) {
    if (is_sorted) {
      // TODO remove non-unique elements. The array is sorted so that's pretty easy.
      //   for (elem in arr)
      //     if elem == previous elem, then remove elem.
      // ... should probably use a sortof 'tombstone' system, .remove() isn't really the most efficient implementation at the moment
    } else {
      assert(false);
      // TODO remove non-unique elements in place without sorting ... maybe with a hash?
      //   or just make a sorted array and use that for the unique checks?
      //  ... though that wouldn't be an in place algorithm.
    }
  }

  array& operator+=(const array& other) {
    u32 total_length = this->m_length + other.m_length;
    reserve(total_length);

    for (u32 i = this->m_length, other_i = 0; i < total_length; i++) {
      this->m_elements[i] = other[other_i++];
    }

    this->m_length = total_length;
    return *this;
  };

  // TODO .join()
  // TODO .insert(index, elem)
  // TODO .sort()
  // TODO .sort(lambda) -- comparison lambda
  // TODO .search(elem), .search(lambda)
  // TODO .count(elem), .count(lambda)
  // TODO iterator, range based for loop compatibility
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

// a temporarily allocated array
template<class T>
class temp_array : public array<T>{
public:
  // allocate space for the array, copying existing elems into new allocation
  void reserve(u32 better_size) {
    assert(better_size);
    if (this->m_length == 0) {
      this->m_elements = (T*)arenas::temp.push(better_size);
      this->m_size = better_size;
    } else {
      T* old_elements = this->m_elements;
      this->m_elements = (T*)arenas::temp.push(better_size);
      for (u32 i = 0; i < this->m_length; i++) {
        this->m_elements[i] = old_elements[i];
      }

      // TODO callback for reference stability?

      this->m_size = better_size;
    }
  }
};

}; // end namespace

// TODO temporarily putting this here:
#include "lapse_range.h"
