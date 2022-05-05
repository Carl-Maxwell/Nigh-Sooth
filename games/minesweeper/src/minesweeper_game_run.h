#pragma once

#include <lapse_scalar.h>

#include "minesweeper_tile.h"

namespace minesweeper{

struct minesweeper_run{
  lapse::i32 grid_width  = 18;
  lapse::i32 grid_height = 12;
  minesweeper::tile_obj* grid = nullptr;
  // Spaces where mines are not allowed during lvl generation
  lapse::array<lapse::vec2<lapse::i32>> safe_spaces;
  const lapse::i32 grid_size = 16; // size in pixels of each grid img
  const lapse::i32 window_padding = 10.0f; // pixels of padding around the window edge
  bool m_first_click = true;

  lapse::vec2<> grid_size_vec2() {
    return lapse::vec2<>{(lapse::f32)grid_size, (lapse::f32)grid_size};
  }

  void initialize_run(lapse::i32 n_width = 18, lapse::i32 n_height = 12);
  void generate_mines(lapse::i32 num_mines = 0);
  void generate_safe_spaces(tile_obj* start_tile);
  void start_main_loop();
  void main_loop(lapse::f32 delta);

  // dimensions for next window's width & height
  lapse::vec2<lapse::i32> next_window_size = {16*18+20, 16*12+20};
};

}