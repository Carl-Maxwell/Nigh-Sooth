
#include "lapse_exceptions.h"

namespace lapse{

void error(error_code code) {
  LapseErrorQueue::the().m_queue_of_errors.push(code);
};

};
