
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
  flagged  = 7
};

}

int main(void) {

  // window:
  // u32 WIDTH = 1280/4, HEIGHT = 720/4;
  // u32 WIDTH = 1280, HEIGHT = 720;
  // u32 WIDTH = 1920/4 - 20, HEIGHT = 1080/4 - 20;
  i32 WIDTH = 16*18+20, HEIGHT = 16*12+20;

  platform::initialize(WIDTH, HEIGHT, false, "Nigh Sooth Engine Test");

  const i32 grid_width  = 18;
  const i32 grid_height = 12;

  // 18 x 12

  minesweeper::grid_tile* grid = new minesweeper::grid_tile[grid_width * grid_height];

  for (u32 y = 0; y < grid_height; y++) {
    for (u32 x = 0; x < grid_width; x++) {
      grid[y * grid_width + x] = minesweeper::grid_tile::hidden;
    }
  }

  for (u32 n = 0; n < 18; n++) {
    auto x = (i32)rand_integer(grid_width);
    auto y = (i32)rand_integer(grid_height);

    grid[y * grid_width + x] = minesweeper::grid_tile::mined;

    for (auto y2 = max(y-1, 0); y2 <= min(y+1, grid_height); y2++) {
      for (auto x2 = max(x-1, 0); x2 <= min(x+1, grid_width); x2++) {
        // if (x2 < 0 || y2 < 0 || x2 > grid_width || y2 > grid_height) {
        //   continue;
        // }
        if (x2 == x && y2 == y) { continue; }
        if ((u32)grid[y2*grid_width + x2] <= (u32)minesweeper::grid_tile::number_5) {
          // it's already adjacent to a mine or mines
          reinterpret_cast<u32&>(grid[y2*grid_width + x2]) += 1;
        } else {
          grid[y2*grid_width + x2] = minesweeper::grid_tile::number_1;
        }
        assert((u32)grid[y2*grid_width + x2] <= (u32)minesweeper::grid_tile::number_5);
      }
    }
  }

  image* image_array = new image[10];

  image_array[ (u32)minesweeper::grid_tile::number_1 ] = image("resources/number_1.png");
  image_array[ (u32)minesweeper::grid_tile::number_2 ] = image("resources/number_2.png");
  image_array[ (u32)minesweeper::grid_tile::number_3 ] = image("resources/number_3.png");
  image_array[ (u32)minesweeper::grid_tile::number_4 ] = image("resources/number_4.png");
  image_array[ (u32)minesweeper::grid_tile::number_5 ] = image("resources/number_5.png");
  image_array[ (u32)minesweeper::grid_tile::hidden ]   = image("resources/grid.png");
  image_array[ (u32)minesweeper::grid_tile::hovered ]  = image("resources/grid_highlighted.png");
  image_array[ (u32)minesweeper::grid_tile::mined ]    = image("resources/bomb.png");
  image_array[ (u32)minesweeper::grid_tile::flagged ]  = image("resources/flag.png");

  platform::set_main_loop_callback([&image_array, &grid, grid_width, grid_height](f32 delta){
    platform::clear(vec3<>{0, 0, 0});

    for (f32 y = 0; y < platform::get_window_height(); y++) {
      for (f32 x = 0; x < platform::get_window_width(); x++) {
        vec3<> color{1, 1, 1};
        // platform::plot(vec2<f32>{x, y}, rand_vec3());
        color *= (x / platform::get_window_width());
        platform::plot(vec2<f32>{x, y}, color);
      }
    }

    f32 grid_size = 16.0f;
    f32 padding = platform::get_window_padding();

    for (f32 y = 0; y < grid_height; y++) {
      for (f32 x = 0; x < grid_width; x++) {
        vec2<> screen_pos{x*grid_size + padding, y*grid_size + padding};
        minesweeper::grid_tile tile = grid[u32(y*grid_width + x)];
        bool hovering =
          Input::Mouse::get_mouse_pos() > screen_pos &&
          Input::Mouse::get_mouse_pos() < screen_pos+vec2<>{grid_size, grid_size};
        image img = image_array[(u32)tile];
        if (hovering && tile == minesweeper::grid_tile::hidden) {
          img = image_array[(u32)minesweeper::grid_tile::hovered];
        }

        platform::draw_bitmap(screen_pos, img);
      }
    }
    
  } );

  return 0;
}

void set_event_callbacks() {
  // glfwSetKeyCallback(glfw_window,
  //   [](GLFWwindow* glfw_window, int glfw_key, int scancode, int action, int mods) {
  //     std::cout << "keyboard event!" << "\n";

  //     if (glfw_key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
  //       glfwSetWindowShouldClose(glfw_window, GLFW_TRUE);
  //     }
  // } );

  // glfwSetMouseButtonCallback(glfw_window,
  //   [](GLFWwindow* glfw_window, int button, int action, int mods) {
  //     bool pressed = action == GLFW_PRESS;

  //     switch(button) {
  //       case GLFW_MOUSE_BUTTON_1: { mouse.buttons.left  = pressed; } break; // left mouse
  //       case GLFW_MOUSE_BUTTON_2: { mouse.buttons.right = pressed; } break; // right mouse
  //       default: return;
  //     }

  //     switch (action) {
  //       case GLFW_PRESS: {
  //         // mouse button released
  //         ;
  //         std::cout << "mouse button pressed! \n";
  //       } break;
  //       case GLFW_RELEASE: {
  //         // mouse button released
  //         std::cout << "mouse button released! \n";
  //       } break;
  //     }
  // } );

  // glfwSetCursorPosCallback(glfw_window, 
  //   [](GLFWwindow* glfw_window, f64 x_pos, f64 y_pos) {
  //     mouse.x = x_pos;
  //     mouse.y = y_pos;
  // } );
  
  // //
  // // Window events
  // //

  // glfwSetWindowPosCallback (glfw_window, 
  //   [](GLFWwindow* glfw_window, i32 x_pos, i32 y_pos){
  //     // std::cout << "Window position event x: " << x_pos << " y:" << y_pos << "\n";
  // } );

  // glfwSetWindowSizeCallback( glfw_window,
  //   [](GLFWwindow* glfw_window, i32 width, i32 height){
  //     std::cout << "window size event! ... this shouldn't realy happen" << "\n";
  //     // TODO maybe throw an error?
  // } );

  // glfwSetWindowFocusCallback (glfw_window,
  //   [](GLFWwindow* glfw_window, i32 focused){
  //     std::cout
  //       << "Event: window focus callback ... focused: " 
  //       << (focused ? "focus gained!" : "focus lost : (") 
  //       << "\n";
  // } );

  // glfwSetWindowRefreshCallback(glfw_window, [](GLFWwindow* glfw_window){
  //   std::cout << "Event: window refresh event triggered\n";
  // });

};
