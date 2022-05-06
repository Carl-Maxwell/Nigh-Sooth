#pragma once

#include <lapse_scalar.h>
#include <lapse_geometry.h>

#include <sooth_image.h>

#include <platform_olc_pixel.h>

namespace minesweeper{

enum class game_state_enum;

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

  lapse::array<tile_obj>* adjacent_tiles();
  lapse::array<tile_obj>* adjacent_tiles_cardinal();
  lapse::i32 calculate_adjacent_mines();
  image get_image(game_state_enum game_state, bool hovered = false);
  void reset() {
    m_adjacent_mines = 0;
    m_coordinates    = {0, 0};
    m_mined          = false;
    m_hidden         = true;
    m_flagged        = false;
    m_tile_state     = minesweeper::grid_tile::hidden;
  }
  void reveal(bool chance_of_chain = true);
  void chain_reaction();
  lapse::rect<lapse::i32> to_rect();
  bool operator==(tile_obj right) {
    return m_coordinates == right.m_coordinates;
  }
};

} // end namespace
