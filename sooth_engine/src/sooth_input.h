#pragma once

#include <lapse_scalar.h>

struct Input{
  struct Mouse{
    lapse::vec2<lapse::i32> m_pos;

    static lapse::vec2<> get_mouse_pos();
  };
};
