#pragma once

#include "lapse_lib.h"
#include "lapse_array.h"

#include "iostream"

// TODO should this be called errors or something instead of exceptions?
//   not sure about the definitions of these terms

namespace lapse{

enum class error_code{
  undefined = 0,
  failure,
  success,
  close_app,
  breakpoint = 244
};

struct error_queue_callback{
  void (*callback)(error_code);
  void* captures;
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

    list_node* front = nullptr;
    list_node* last_node  = nullptr;

    list_node* last() {
      if (last_node) { return last_node; }
      list_node* temp = front;
      while (temp->next != nullptr) {
        temp = temp->next;
      }
      last_node = temp;
      return last_node;
    };
    void push(T that_value) {
      list_node* a_node = new list_node;
      a_node->value = that_value;

      if (!front) {
        front = a_node;
      } else {
        front ->prev = a_node;
        a_node->next = front;
        front = a_node;
      }
    }
    void remove(list_node* bad_node) {
      // TODO edge case: bad_node isn't in list

      last_node = nullptr; // break cache
      // TODO only need to break cache if bad_node == last_node I think?

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
        bad_node->prev->next = nullptr;
        bad_node->prev = nullptr;
      } else if (bad_node == front &&
        (bad_node->prev == nullptr && bad_node->next == nullptr)
      ) {
        // bad_node is the only node
        front = nullptr;
      }

      bad_node = nullptr;

      bad_node = nullptr;

      delete bad_node;
    }
    inline bool is_empty() {
      return front == nullptr;
    }

    // iterator

    struct iterator{
      list_node* my_node = nullptr;

      // prefix ++ operator
      iterator& operator++() { my_node = my_node->next; return *this; };
      // unary * operator
      list_node* operator*() { return my_node; };
      // ==
      inline bool operator==(iterator& right) {
        return my_node == right.my_node;
      };
    };

    iterator begin() { return iterator(front ); };
    iterator end()   { return iterator{nullptr}; };

  };
public:
  noexcept_list<error_code> queue_of_errors;

  noexcept_list<error_queue_callback> callbacks;

  static LapseErrorQueue& the() {
    static LapseErrorQueue* my_error_queue = nullptr;
    if (!my_error_queue) {
      my_error_queue = new LapseErrorQueue;
    }
    return *my_error_queue;
  };

  void register_callback(error_queue_callback a_callback) {
    callbacks.push(a_callback);
  };

  void tick() {
    if (!callbacks.is_empty() && !queue_of_errors.is_empty()) {
      for (auto err : queue_of_errors) {
        for (auto callback : callbacks) {
          callback->value.callback(err->value);
        }
      }
    }
  }

};

void error(error_code code) {
  LapseErrorQueue::the().queue_of_errors.push(code);
};

};

