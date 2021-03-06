#pragma once

#include <lapse_scalar.h>
#include <lapse_geometry.h>

#include <sooth_image.h>

#include <platform_olc_pixel.h>

namespace minesweeper{

enum class game_state_enum;

enum class grid_tile{
  hidden  ,
  number_1,
  number_2,
  number_3,
  number_4,
  number_5,
  hovered ,
  revealed,
  mined   ,
  flagged ,
  empty   ,
  bad_flag,
  count // used for figuring out how many values there are in the enum (for declaring arrays)
};

struct tile_obj{
  lapse::u32 m_adjacent_mines = 0;
  lapse::vec2<lapse::i32> m_coordinates = {0, 0};
  bool m_mined   = false;
  bool m_hidden  = true;
  bool m_flagged = false;
  grid_tile m_tile_state = minesweeper::grid_tile::hidden;
  // minesweeper::tile_obj* grid;

  lapse::array<tile_obj>& adjacent_tiles(lapse::array<tile_obj>& adjacents);
  lapse::array<tile_obj>& adjacent_tiles_cardinal(lapse::array<tile_obj>& adjacents);
  lapse::i32 calculate_adjacent_mines();
  image& get_image(game_state_enum game_state, bool hovered = false);
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
