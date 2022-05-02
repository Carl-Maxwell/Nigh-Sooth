
#include "minesweeper_tile.h"
#include "minesweeper_game_session.h"

using namespace lapse;

namespace minesweeper{

// returns the count of how many mines are are in adjacent tiles
i32 tile_obj::calculate_adjacent_mines() {
  auto& session = minesweeper_session::the();
  auto& run = session.run;

  i32 mine_count = 0;
  i32 x = m_coordinates.x;
  i32 y = m_coordinates.y;

  for (auto y2 = max(y-1, 0); y2 < min(y+2, run->grid_height); y2++) {
      for (auto x2 = max(x-1, 0); x2 < min(x+2, run->grid_width); x2++) {
        if (x2 == x && y2 == y) { continue; }
        auto& tile = run->grid[y2 * run->grid_width + x2];
        mine_count += tile.m_mined ? 1 : 0;
      }
  }

  return mine_count;
}

image tile_obj::get_image(bool hovered) {
  auto& session = minesweeper_session::the();

  if (hovered && m_hidden && !m_flagged) {
    return session.image_array[(u32)grid_tile::hovered];
  } else {
    return session.image_array[(u32)m_tile_state];
  }
}

void tile_obj::reveal(bool chance_of_chain) {
  if (m_mined) {
    m_tile_state = grid_tile::mined;
    auto& session = minesweeper_session::the();
    session.restart_run();
  } else if (m_adjacent_mines > 0) {
    m_tile_state = grid_tile(u32(grid_tile::hidden)+m_adjacent_mines);
  } else {
    m_tile_state = grid_tile::empty;
    if (chance_of_chain) { // && die(3) == 1) {
      // set off a chain reaction of revealing empty tiles
      chain_reaction();
    }
  }
  m_hidden = false;
}
void tile_obj::chain_reaction() {
  auto& session = minesweeper_session::the();
  auto& run     = *session.run;

  // std::cout << "chain reaction: ";
  // m_coordinates.std_cout();
  if (!m_hidden || m_mined || m_flagged) { return; }
  reveal(false);
  i32 x = m_coordinates.x;
  i32 y = m_coordinates.y;
  if (m_adjacent_mines == 0) {
    for (auto y2 = max(y-1, 0); y2 < min(y+2, run.grid_height); y2++) {
      for (auto x2 = max(x-1, 0); x2 < min(x+2, run.grid_width); x2++) {
        auto& tile = run.grid[y2*run.grid_width + x2];
        if (tile.m_hidden && !tile.m_mined && !tile.m_flagged) {
          tile.chain_reaction();
        }
      }
    }
  }
}

}; // end namepspace