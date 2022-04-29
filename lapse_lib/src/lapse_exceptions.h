#pragma once

#include <functional>

#include "iostream"

// TODO should this be called errors or something instead of exceptions?
//   not sure about the definitions of these terms

namespace lapse{

void assert(bool);

enum class error_code{
  undefined = 0,
  failure,
  success,
  close_app,
  breakpoint = 244
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

    list_node* m_front = nullptr;
    list_node* m_last_node  = nullptr;

    list_node* last() {
      if (m_last_node) { return m_last_node; }
      list_node* temp = m_front;
      while (temp->next != nullptr) {
        temp = temp->next;
      }
      m_last_node = temp;
      return m_last_node;
    };
    void push(T that_value) {
      list_node* a_node = new list_node;
      a_node->value = that_value;

      if (!m_front) {
        m_front = a_node;
      } else {
        m_front ->prev = a_node;
        a_node->next = m_front;
        m_front = a_node;
      }
    }
    void remove(list_node* bad_node) {
      // TODO edge case: bad_node isn't in list

      m_last_node = nullptr; // break cache
      // TODO only need to break cache if bad_node == last_node I think?

      if (bad_node == m_front && m_front->next) {
        // bad_node is front & list has elements
        m_front = m_front->next;
        m_front->prev = nullptr;
      } else if (bad_node->prev && bad_node->next) {
        // bad_node is in the middle of the list somewhere
        bad_node->prev->next = bad_node->next;
        bad_node->next->prev = bad_node->prev;
      } else if (bad_node->prev && bad_node->next == nullptr) {
        // bad_node is at the end of the list
        bad_node->prev->next = nullptr;
        bad_node->prev = nullptr;
      } else if (bad_node == m_front &&
        (bad_node->prev == nullptr && bad_node->next == nullptr)
      ) {
        // bad_node is the only node
        m_front = nullptr;
      }

      bad_node = nullptr; // TODO is this the right way to do this?

      delete bad_node;
    }
    inline bool is_empty() {
      return m_front == nullptr;
    }

    // iterator

    struct iterator{
      list_node* m_node = nullptr;

      // prefix ++ operator
      iterator& operator++() { m_node = m_node->next; return *this; };
      // unary * operator
      list_node* operator*() { return m_node; };
      // ==
      inline bool operator==(iterator& right) {
        return m_node == right.m_node;
      };
    };

    iterator begin() { return iterator(m_front ); };
    iterator end()   { return iterator{nullptr}; };

  };
public:
  noexcept_list<error_code> m_queue_of_errors;

  std::function<void(error_code)> m_callback;

  static LapseErrorQueue& the() {
    static LapseErrorQueue* my_error_queue = nullptr;
    if (!my_error_queue) {
      my_error_queue = new LapseErrorQueue;
    }
    return *my_error_queue;
  };

  void register_callback(std::function<void(error_code)> a_callback) {
    m_callback = a_callback;
    // TODO crash if this is called twice
  };

  void tick();
};

// TODO need to send in an error_struct like
//   error({error_code::close_app, "Array overflow!"})

void error(error_code code);

};

