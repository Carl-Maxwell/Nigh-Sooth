
#include "lapse_exceptions.h"

namespace lapse{

void assert(bool value) {
  if (!value) {
    error(error_code::breakpoint);
    __debugbreak();
  }
}

void error(error_code code) {
  LapseErrorQueue::the().m_queue_of_errors.push(code);
};

void LapseErrorQueue::tick() {
  if (m_callback && !m_queue_of_errors.is_empty()) {
    for (auto err : m_queue_of_errors) {
      m_callback(err->value);
      m_queue_of_errors.remove(err);
      
      // TODO why isn't err an iterator? for some reason it's a list_node*
    }
  }
}

};
