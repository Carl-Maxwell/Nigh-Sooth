#pragma once

#include "lapse_lib.h"
#include "lapse_array.h"

namespace lapse{

enum class error_code{
  undefined = 0,
  failure,
  success,
  close_app,
  breakpoint
};

class LapseErrorQueue{
public:
  
  // a queue list
  //   array &c depend on exceptions, so we can't use them
  template<typename T>
  class noexcept_list{
  public:
    struct list_node{
      list_node* prev = nullptr; // TODO this can just be singly linked
      list_node* next = nullptr;
      T value{0};
    };

    struct iterator{
      list_node* my_node = nullptr;

      // prefix ++ operator
      iterator& operator++() { my_node = my_node->next; return *this; };
      // unary * operator
      list_node& operator*() { return my_node; };
      // ==
      inline bool operator==(const iterator& right) { 
        return my_node == right.my_node;
      };
    };

    list_node* front = nullptr;
    void push(T that_value) {
      list_node* a_node = new list_node;
      a_node->value = that_value;
      front ->prev = a_node;
      a_node->next = front;
      front = a_node;
    }
    void remove(list_node bad_node) {
      if (bad_node == front && front->next) {
        // bad_node is front & list has elements
        front = front->next;
        front->prev = nullptr;
      } else if (bad_node->prev && bad_node->next) {
        // bad_node is in the middle of the list somewhere
        bad_node->prev->next = bad_node->next;
        bad_node->next->prev = bad_node->prev;
      } else if (bad_node->prev && bad_node->next == nullptr) {
        // bad_node is at the end of the list
        bad_node->prev = nullptr;
      } else if (bad_node == front && 
        (bad_node->prev == nullptr && bad_node->next == nullptr)
      ) {
        // bad_node is the only node
        front = nullptr;
      }

      delete bad_node;
    }
  };
public:
  noexcept_list<error_code> queue;
  noexcept_list<void (*)()> callbacks;

  static LapseErrorQueue& the() {
    static LapseErrorQueue* my_error_queue = nullptr;
    if (!my_error_queue) {
      my_error_queue = new LapseErrorQueue;
    }
    return *my_error_queue;
  };

};

void error(error_code code) {
  LapseErrorQueue::the().queue.push(code);
};

};

