#pragma once

#include <lapse_scalar.h>

#include <sooth_image.h>

#include <platform_olc_pixel.h>

namespace minesweeper{

enum class grid_tile{
  number_1 = 1,
  number_2 = 2,
  number_3 = 3,
  number_4 = 4,
  number_5 = 5,
  hidden   = 0,
  hovered  = 9,
  revealed = 6,
  mined    = 8,
  flagged  = 7,
  empty    = 10
};

struct tile_obj{
  lapse::u32 m_adjacent_mines = 0;
  lapse::vec2<lapse::i32> m_coordinates = {0, 0};
  bool m_mined   = false;
  bool m_hidden  = true;
  bool m_flagged = false;
  grid_tile m_tile_state = minesweeper::grid_tile::hidden;
  // minesweeper::tile_obj* grid;

  image get_image(bool hovered = false);
  void reset() {
    m_adjacent_mines = 0;
    m_coordinates = {0, 0};
    m_mined   = false;
    m_hidden  = true;
    m_flagged = false;
    m_tile_state = minesweeper::grid_tile::hidden;
  }
  void reveal(bool chance_of_chain = true);
  void chain_reaction();
};

} // end namespace
