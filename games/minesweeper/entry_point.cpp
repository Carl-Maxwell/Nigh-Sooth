
#include "sooth.h"

#include "lapse_lib.h"

#include "sooth_input.h"

#include "platform_olc_pixel.h"

#include "sooth_image.h"

using namespace lapse;

namespace minesweeper{

image* image_array = new image[11];

const i32 grid_width  = 18;
const i32 grid_height = 12;

// TODO those variables should be allocated inside the game session initialization callback

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
  u32 m_adjacent_mines = 0;
  vec2<i32> m_coordinates = {0, 0};
  bool m_mined   = false;
  bool m_hidden  = true;
  bool m_flagged = false;
  grid_tile m_tile_state = minesweeper::grid_tile::hidden;
  minesweeper::tile_obj* grid;

  image get_image(bool hovered = false) {
    if (hovered && m_hidden && !m_flagged){
      return image_array[(u32)grid_tile::hovered];
    } else {
      return image_array[(u32)m_tile_state];
    }
  }
  void reset() {
    m_adjacent_mines = 0;
    m_coordinates = {0, 0};
    m_mined   = false;
    m_hidden  = true;
    m_flagged = false;
    m_tile_state = minesweeper::grid_tile::hidden;
  }
  void reveal(bool chance_of_chain = true) {
    if (m_mined) {
      m_tile_state = grid_tile::mined;
      platform::close_application();
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
  void chain_reaction() {
    // std::cout << "chain reaction: ";
    // m_coordinates.std_cout();
    if (!m_hidden || m_mined || m_flagged) { return; }
    reveal(false);
    i32 x = m_coordinates.x;
    i32 y = m_coordinates.y;
    if (m_adjacent_mines == 0) {
      for (auto y2 = max(y-1, 0); y2 < min(y+2, grid_height); y2++) {
        for (auto x2 = max(x-1, 0); x2 < min(x+2, grid_width); x2++) {
          auto& tile = grid[y2*grid_width + x2];
          if (tile.m_hidden && !tile.m_mined && !tile.m_flagged) {
            tile.chain_reaction();
          }
        }
      }
    }
  }
};

minesweeper::tile_obj* grid = new minesweeper::tile_obj[grid_width * grid_height];

// Initialize graphics (called once on application startup)
void initialize_game_session() {
  image_array[ (u32)grid_tile::number_1 ] = image("resources/number_1.png");
  image_array[ (u32)grid_tile::number_2 ] = image("resources/number_2.png");
  image_array[ (u32)grid_tile::number_3 ] = image("resources/number_3.png");
  image_array[ (u32)grid_tile::number_4 ] = image("resources/number_4.png");
  image_array[ (u32)grid_tile::number_5 ] = image("resources/number_5.png");
  image_array[ (u32)grid_tile::hidden ]   = image("resources/grid.png");
  image_array[ (u32)grid_tile::empty ]    = image("resources/grid_empty.png");
  image_array[ (u32)grid_tile::hovered ]  = image("resources/grid_highlighted.png");
  image_array[ (u32)grid_tile::mined ]    = image("resources/bomb.png");
  image_array[ (u32)grid_tile::flagged ]  = image("resources/flag.png");
}

// called each time we need to setup a new map to play
void initialize_game_run() {
  // 18 x 12

  for (i32 y = 0; y < grid_height; y++) {
    for (i32 x = 0; x < grid_width; x++) {
      grid[y*grid_width + x].reset();
      grid[y*grid_width + x].m_coordinates = {x, y};
      grid[y*grid_width + x].grid = grid; // TODO stop copying this pointer around
    }
  }

  // generate mines
  for (u32 n = 0; n < 18; n++) {
    auto x = (i32)rand_integer(grid_width);
    auto y = (i32)rand_integer(grid_height);

    // if it's already mined, find a fresh spot
    while (grid[y*grid_width + x].m_mined) {
      x = (i32)rand_integer(grid_width);
      y = (i32)rand_integer(grid_height);
    }

    grid[y*grid_width + x].m_mined = true;

    // update adjacent mine count in adjacent squares
    for (auto y2 = max(y-1, 0); y2 < min(y+2, grid_height); y2++) {
      for (auto x2 = max(x-1, 0); x2 < min(x+2, grid_width); x2++) {
        if (x2 == x && y2 == y) { continue; }
        grid[y2*grid_width + x2].m_adjacent_mines++;
        assert(grid[y2*grid_width + x2].m_adjacent_mines <= 5);
      }
    }
  }
}

void main_loop_callback() {
  platform::clear(vec3<>{0, 0, 0});

  lapse::LapseErrorQueue::the().tick();

  f32 grid_size = 16.0f;
  f32 padding = platform::get_window_padding();

  i32 mouse_x = (i32)Mouse::get_mouse_pos().x - padding;
  i32 mouse_y = (i32)Mouse::get_mouse_pos().y - padding;
  mouse_x /= grid_size;
  mouse_y /= grid_size;
  minesweeper::tile_obj* mouse_tile = nullptr;

  if (mouse_x >= 0 && mouse_y >= 0 && mouse_x < grid_width && mouse_y < grid_height) {
    mouse_tile = &grid[mouse_y * grid_width + mouse_x];
  }

  if (Mouse::left_mouse_hit()) {
    if (mouse_tile && mouse_tile->m_hidden && !mouse_tile->m_flagged) {
      mouse_tile->reveal();
    }
  } else if (Mouse::right_mouse_hit()) {
    if (mouse_tile && mouse_tile->m_hidden) {
      if (!mouse_tile->m_flagged) {
        mouse_tile->m_tile_state = minesweeper::grid_tile::flagged;
        mouse_tile->m_flagged = true;
      } else {
        mouse_tile->m_tile_state = minesweeper::grid_tile::hidden;
        mouse_tile->m_flagged = false;
      }
    }
  }

  for (f32 y = 0; y < grid_height; y++) {
    for (f32 x = 0; x < grid_width; x++) {
      vec2<> screen_pos{x*grid_size + padding, y*grid_size + padding};
      auto& tile = grid[u32(y*grid_width + x)];
      bool hovering = mouse_tile && mouse_tile == &tile;
      image img = tile.get_image(hovering);

      platform::draw_bitmap(screen_pos, img);
    }
  }
}

} // end minesweeper namespace

int main() {
  sooth::initialize_engine();

  using minesweeper::grid_height;
  using minesweeper::grid_width;

  // window:
  // u32 WIDTH = 1280/4, HEIGHT = 720/4;
  // u32 WIDTH = 1280, HEIGHT = 720;
  // u32 WIDTH = 1920/4 - 20, HEIGHT = 1080/4 - 20;
  i32 WIDTH = 16*18+20, HEIGHT = 16*12+20;

  platform::set_game_session_initialization_callback([](){
    minesweeper::initialize_game_session();
  });

  platform::set_main_loop_callback([](f32 delta){
    minesweeper::main_loop_callback();
  } );

  //
  // Start the game
  //

  enum class game_session_state{
    application_shutdown = 0, // entire program is shutting down
    application_startup,      // application is starting up
    main_menu,                // game's main menu
    game_run_startup,         // loading level 1, setting up the game
    game_run_main_loop,       // main loop of the game
  };

  auto game_session = game_session_state::game_run_startup; // TODO should be game_session_state::application_startup

  // game session loop
  while (game_session != game_session_state::application_shutdown) {
    switch(game_session) {
      // case game_session_state::application_startup:
      //   game_session = game_session_state::game_run_main_loop;
      // break;
      case game_session_state::game_run_startup:
        minesweeper::initialize_game_run();
        game_session = game_session_state::game_run_main_loop;
      break;
      case game_session_state::game_run_main_loop:
        platform::initialize(WIDTH, HEIGHT, false, "Nigh Sooth Engine Test");
        platform::start_application();
        // if the main loop has ended, then:
        game_session = game_session_state::game_run_startup;
      break;
      // case game_session_state::main_menu:
      //   game_session = game_session_state::game_run_main_loop;
      // break;
      // default: std::cout << "Error! Bad game session state: " << (u32)game_session;
    }
  }

  return 0;
}

