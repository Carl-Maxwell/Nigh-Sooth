#include <GLFW/glfw3.h>

#include "lapse_lib.h"

// void keyCallback(GLFWwindow* window, int glfw_key, int scancode, int action, int mods);

using namespace lapse;

struct mouse_obj{
  f64 x = 0;
  f64 y = 0;
  struct button_states{
    bool left = false;
    bool right = false;
  } buttons;
} mouse;

int main(void)
{
  GLFWwindow* glfw_window;

  // initialize glfw

  if (!glfwInit())
    return -1;

  // window:
  // int WIDTH = 1280, HEIGHT = 720;
  int WIDTH = 1920, HEIGHT = 1080;
  glfw_window = glfwCreateWindow(WIDTH, HEIGHT, "Sooth Engine Test", NULL, NULL);

  // borderless window:
  // int WIDTH = 1920, HEIGHT = 1080;
  // window = glfwCreateWindow(WIDTH, HEIGHT, "Sooth Engine Test", glfwGetPrimaryMonitor(), NULL);

  if (!glfw_window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(glfw_window);

  glfwSetKeyCallback(glfw_window,
    [](GLFWwindow* window, int glfw_key, int scancode, int action, int mods) {
      std::cout << "keyboard event!" << "\n";
  } );
  glfwSetMouseButtonCallback(glfw_window,
    [](GLFWwindow* glfw_window, int button, int action, int mods) {
      bool pressed = action == GLFW_PRESS;

      switch(button) {
        case GLFW_MOUSE_BUTTON_1: { mouse.buttons.left  = pressed; } break; // left mouse
        case GLFW_MOUSE_BUTTON_2: { mouse.buttons.right = pressed; } break; // right mouse
        default: return;
      }

      switch (action) {
        case GLFW_PRESS: {
          // mouse button released
          ;
          std::cout << "mouse button pressed! \n";
        } break;
        case GLFW_RELEASE: {
          // mouse button released
          std::cout << "mouse button released! \n";
        } break;
      }
  } );

  glfwSetCursorPosCallback(glfw_window, 
    [](GLFWwindow* glfw_window, f64 x_pos, f64 y_pos) {
      mouse.x = x_pos;
      mouse.y = y_pos;
  } );

  while (!glfwWindowShouldClose(glfw_window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(glfw_window);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}