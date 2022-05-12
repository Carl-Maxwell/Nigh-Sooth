#pragma once

#include <lapse_scalar.h>

#include "minesweeper_tile.h"

namespace minesweeper{

enum class game_state_enum{
  lost,
  in_progress,
  won
};

struct minesweeper_run{
  lapse::i32 grid_width  = 18;
  lapse::i32 grid_height = 12;
  minesweeper::tile_obj* grid = nullptr;
  // Spaces where mines are not allowed during lvl generation
  lapse::array<lapse::vec2<lapse::i32>> safe_spaces;
  const lapse::i32 grid_size = 16; // size in pixels of each grid img
  const lapse::i32 window_padding = 10; // pixels of padding around the window edge
  bool m_first_click = true;

  game_state_enum game_state{game_state_enum::in_progress};

  ~minesweeper_run() {
    std::cout << "~minesweeper_run\n";
    if (grid) delete[] grid;
    platform::finalize_shutdown();
  }

  lapse::vec2<> grid_size_vec2() {
    return lapse::vec2<>{(lapse::f32)grid_size, (lapse::f32)grid_size};
  }

  void initialize_run(lapse::i32 n_width = 18, lapse::i32 n_height = 12);
  void generate_mines(lapse::i32 num_mines = 0);
  void generate_safe_spaces(tile_obj* start_tile);
  void start_main_loop();
  void main_loop(lapse::f32 delta);
  bool is_game_won();
  void win_game();
  void lose_game();

  // dimensions for next window's width & height
  lapse::vec2<lapse::i32> next_window_size = {16*18+20, 16*12+20};
};

}