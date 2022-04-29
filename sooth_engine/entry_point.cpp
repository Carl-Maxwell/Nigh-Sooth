
#include "lapse_lib.h"

#include "sooth_input.h"

#include "platform_olc_pixel.h"

#include "sooth_image.h"

using namespace lapse;

struct mouse_obj{
  f64 x = 0;
  f64 y = 0;
  struct button_states{
    bool left = false;
    bool right = false;
  } buttons;
} mouse;

void set_event_callbacks();

namespace minesweeper{

image* image_array = new image[11];

const i32 grid_width  = 18;
const i32 grid_height = 12;

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
  void reveal(bool chance_of_chain = true) {
    if (m_mined) {
      m_tile_state = grid_tile::mined;
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
    std::cout << "chain reaction: ";
    m_coordinates.std_cout();
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

void initialize() {
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

}

int main(void) {

  lapse::LapseErrorQueue::the().register_callback([](lapse::error_code err){
    std::cout << "Lapse Error! " << (int)err;
    switch(err){
      case lapse::error_code::undefined:  __debugbreak(); break;
      case lapse::error_code::failure:    __debugbreak(); break;
      case lapse::error_code::success:    __debugbreak(); break;
      case lapse::error_code::close_app:  platform::close_application(); break;
      case lapse::error_code::breakpoint: __debugbreak(); break;
    }
  });

  lapse::seed_rng(platform::get_application_start_time());

  using minesweeper::grid_height;
  using minesweeper::grid_width;

  // window:
  // u32 WIDTH = 1280/4, HEIGHT = 720/4;
  // u32 WIDTH = 1280, HEIGHT = 720;
  // u32 WIDTH = 1920/4 - 20, HEIGHT = 1080/4 - 20;
  i32 WIDTH = 16*18+20, HEIGHT = 16*12+20;

  // 18 x 12

  minesweeper::tile_obj* grid = new minesweeper::tile_obj[grid_width * grid_height];

  for (i32 y = 0; y < grid_height; y++) {
    for (i32 x = 0; x < grid_width; x++) {
      grid[y*grid_width + x].m_coordinates = {x, y};
      grid[y*grid_width + x].grid = grid;
    }
  }

  // generate mines
  for (u32 n = 0; n < 18; n++) {
    auto x = (i32)rand_integer(grid_width);
    auto y = (i32)rand_integer(grid_height);

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

  platform::set_initialization_callback([](){
    minesweeper::initialize();
  });

  platform::initialize(WIDTH, HEIGHT, false, "Nigh Sooth Engine Test");

  platform::set_main_loop_callback([&grid](f32 delta){
    platform::clear(vec3<>{0, 0, 0});

    lapse::LapseErrorQueue::the().tick();

    for (f32 y = 0; y < platform::get_window_height(); y++) {
      for (f32 x = 0; x < platform::get_window_width(); x++) {
        vec3<> color{1, 1, 1};
        color *= (x / platform::get_window_width());
        platform::plot(vec2<f32>{x, y}, color);
      }
    }

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
      if (mouse_tile && mouse_tile->m_hidden) {
        mouse_tile->reveal();
      }
    } else if (Mouse::right_mouse_hit()) {
      if (mouse_tile && mouse_tile->m_hidden) {
        mouse_tile->m_tile_state = minesweeper::grid_tile::flagged;
        mouse_tile->m_flagged = true;
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
    
  } );

  return 0;
}
