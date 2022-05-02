#pragma once

#include <lapse_scalar.h>

#include "minesweeper_tile.h"

namespace minesweeper{

struct minesweeper_run{
  lapse::i32 grid_width  = 18;
  lapse::i32 grid_height = 12;
  minesweeper::tile_obj* grid = nullptr;
  const lapse::i32 grid_size = 16; // size in pixels of each grid img
  const lapse::i32 window_padding = 10.0f; // pixels of padding around the window edge
  bool m_first_click = true;

  void initialize_run(lapse::i32 n_width = 18, lapse::i32 n_height = 12);
  void start_main_loop();
  void main_loop(lapse::f32 delta);

  // dimensions for next window's width & height
  lapse::vec2<lapse::i32> next_window_size = {16*18+20, 16*12+20};
};

}