#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "lapse_lib.h"

#include "stb_image_namespace.h"

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

void set_glfw_event_callbacks(GLFWwindow* glfw_window);
void opengl_debug_callback(
  u32 source,
  u32 type,
  u32 id,
  u32 severity,
  i32 length,
  const char *message,
  const void *userParam
);
void checkShaderCompileStatus(u32 shader_id, str name);
void opengl_poll_for_errors();

int main(void)
{
  GLFWwindow* glfw_window;

  glfwSetErrorCallback([](int error, const char* description) {
    std::cout << "glfw error #" << error << ": " << description << "\n";
  } );

  // initialize glfw

  if (!glfwInit())
    return -1;

  // window:
  i32 WIDTH = 1280, HEIGHT = 720;
  // i32 WIDTH = 1920, HEIGHT = 1080;

  glfw_window = glfwCreateWindow(WIDTH, HEIGHT, "Sooth Engine Test", NULL, NULL);

  // borderless window:
  // i32 WIDTH = 1920, HEIGHT = 1080;
  // window = glfwCreateWindow(WIDTH, HEIGHT, "Sooth Engine Test", glfwGetPrimaryMonitor(), NULL);

  // unsigned casts
  u32 WIDTH_U  = u32(WIDTH);
  u32 HEIGHT_U = u32(HEIGHT);

  glfwWindowHint(GLFW_RESIZABLE, true);

  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  if (!glfw_window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(glfw_window);

  //
  // Initialize glew
  //

  {
    GLenum glew_error = glewInit(); // GLenum is just a u32 typedef
    if (glew_error != GLEW_OK) {
      fprintf(stderr, "glew init error: %s\n", glewGetErrorString(glew_error));
    }
    fprintf(stdout, "Status: Using glew %s\n", glewGetString(GLEW_VERSION));
    std::cout << "using opengl version: " << glGetString(GL_VERSION) << "\n";
  }

  //
  //
  //

  set_glfw_event_callbacks(glfw_window);

  //
  // handle opengl errors
  //

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(opengl_debug_callback, NULL);
  
  //
  //
  //

  float positions[] = {
    // triangle one: 
      -1.00f, -1.00f,
       1.00f,  1.00f,
       1.00f, -1.00f,

    // triangle two:
      -1.00f, -1.00f,
      -1.00f,  1.00f,
       1.00f,  1.00f
  };

  u32 buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(f32), positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 2, 0);

  //
  // Shader
  //

  // vertex shader

  auto vertex_shader_text =
    "#version 400"                                       "\n"
    "layout(location = 0) in vec4 position;"             "\n"
    // "uniform mat4 MVP;"                               "\n"
    // "in vec3 vCol;"                                      "\n"
    // "in vec2 vPos;"                                      "\n"
    // "out vec4 color;"                                    "\n"
    "void main(){"                                       "\n"
    // "  gl_Position = vec4(vPos, 0.0, 1.0);"              "\n"
    "  gl_Position = position;"                           "\n"
    // "  color = vec4(1.0f);"                             "\n"
    "}\n";

  u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
  glCompileShader(vertex_shader);

  checkShaderCompileStatus(vertex_shader, "vertex");

  // fragment shader

  char* fragment_shader_text;
  {
    str temp_source = str(
      "#version 400 core"                                     "\n"
      "uniform vec2 u_mousePos;"                              "\n"
      "in vec4 gl_FragCoord;"                                 "\n"
      "out vec4 gl_FragColor;"                                "\n"
      "uniform sampler2D u_texture;"                          "\n"
      
      "void main()"                                           "\n"
      "{"                                                     "\n"
      "  vec2 window_size = vec2(") + 
        str(u32_to_c_str(WIDTH_U)) + str(", ") + str(u32_to_c_str(HEIGHT_U)) + 
      str(");"                  "\n"
      "  vec2 uv_coord = gl_FragCoord.xy/window_size;"        "\n"
      // "  gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);"                          "\n"
      "  gl_FragColor = texture(u_texture, u_mousePos);"        "\n"
      // "  gl_FragColor.rg = u_mousePos.xy/window_size;  "                     "\n"
      "  gl_FragColor.a = 1.0f;"                              "\n"
      // "  gl_FragColor.r = uv_coord.x + u_mousePos.x/window_size.x;  "                     "\n"
      // "  gl_FragColor.g = uv_coord.y + u_mousePos.y/window_size.y;  "                     "\n"
      "};"                                                    "\n");
    fragment_shader_text = temp_source.to_c_str();
  }

  u32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
  glCompileShader(fragment_shader);

  checkShaderCompileStatus(fragment_shader, "fragment");

  // combine shaders into shader program

  u32 shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glValidateProgram(shader_program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  glUseProgram(shader_program);

  //
  // Creating a route to send pixels to opengl
  //

  // pixels

  // unsigned char* pixels = new unsigned char[WIDTH * HEIGHT * 4];
  unsigned char* pixels = nullptr;

  i32 width, height, bytes_per_pixel;

  str path = "test_image.png";

  stb::stbi_set_flip_vertically_on_load(true);

  pixels = stb::stbi_load(path.to_c_str(), &width, &height, &bytes_per_pixel, 4);

  std::cout << path.to_c_str() << " ,"
    << " x: " << width
    << " y: " << height
    << " components: " << bytes_per_pixel
    << "\n";

  // creating opengl texture

  u32 opengl_pixel_buffer_id = 0;

  glGenTextures(1, &opengl_pixel_buffer_id);
  glBindTexture(GL_TEXTURE_2D, opengl_pixel_buffer_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  std::cout << "before:\n";
  opengl_poll_for_errors();

  glTexImage2D(
    GL_TEXTURE_2D,              // GLenum target,
    0,                          // GLint level,
    GL_RGBA,                    // GLint internalFormat,
    width,                      // GLsizei width,
    height,                     // GLsizei height,
    0,                          // GLint border,
    GL_RGBA,                    // GLenum format,
    GL_UNSIGNED_BYTE,           // GLenum type,
    pixels                      // const GLvoid * data);
  );

  std::cout << "after:\n";
  opengl_poll_for_errors();

  glBindTexture(GL_TEXTURE_2D, 0);

  if (!pixels) {
    __debugbreak();
  }

  if (pixels) {
    stb::stbi_image_free(pixels);
    pixels = nullptr;
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, opengl_pixel_buffer_id);

  i32 opengl_pixel_buffer_location = glGetUniformLocation(shader_program, "u_texture");
  std::cout << "opengl_pixel_buffer_location: " << opengl_pixel_buffer_location << "\n"; // this is "1"
  glUniform1i(opengl_pixel_buffer_location, opengl_pixel_buffer_id);

  u32 frame_count = 0;

  while (!glfwWindowShouldClose(glfw_window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    frame_count++;

    glUseProgram(shader_program);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, opengl_pixel_buffer_id);
    glUniform1i(opengl_pixel_buffer_location, opengl_pixel_buffer_id);

    f32 mouse_pos[] = {mouse.x, mouse.y};

    i32 opengl_mouse_pos_location = glGetUniformLocation(shader_program, "u_mousePos");
    glUniform2fv(opengl_mouse_pos_location, 1, mouse_pos);

    glUniform1i(opengl_pixel_buffer_location, opengl_pixel_buffer_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(glfw_window);

    glfwPollEvents();
  }

  glDeleteTextures(1, &opengl_pixel_buffer_id);

  if (pixels) {
    delete[] pixels;
  }

  glDeleteProgram(shader_program);

  glfwDestroyWindow(glfw_window);

  glfwTerminate();
  return 0;
}

void set_glfw_event_callbacks(GLFWwindow* glfw_window) {
  glfwSetKeyCallback(glfw_window,
    [](GLFWwindow* glfw_window, int glfw_key, int scancode, int action, int mods) {
      std::cout << "keyboard event!" << "\n";

      if (glfw_key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(glfw_window, GLFW_TRUE);
      }
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
  
  //
  // Window events
  //

  glfwSetWindowPosCallback (glfw_window, 
    [](GLFWwindow* glfw_window, i32 x_pos, i32 y_pos){
      // std::cout << "Window position event x: " << x_pos << " y:" << y_pos << "\n";
  } );

  glfwSetWindowSizeCallback( glfw_window,
    [](GLFWwindow* glfw_window, i32 width, i32 height){
      std::cout << "window size event! ... this shouldn't realy happen" << "\n";
      // TODO maybe throw an error?
  } );

  glfwSetWindowFocusCallback (glfw_window,
    [](GLFWwindow* glfw_window, i32 focused){
      std::cout
        << "Event: window focus callback ... focused: " 
        << (focused ? "focus gained!" : "focus lost : (") 
        << "\n";
  } );

  glfwSetWindowRefreshCallback(glfw_window, [](GLFWwindow* glfw_window){
    std::cout << "Event: window refresh event triggered\n";
  });

};

void opengl_debug_callback(
  u32 source,
  u32 type,
  u32 id,
  u32 severity,
  i32 length,
  const char *message,
  const void *userParam
) {
  if (id == 131'185 && severity == 33'387) { return; }
  std::cout 
    << "opengl error: {"              "\n\t" 
    << "source " << source << " "     "\n\t"
    << "type " << type <<             "\n\t"
    << "#" << id <<                   "\n\t"
    << "severity: " << severity <<    "\n\t"
    << "length: " << length <<        "\n\t"
    << message <<                     "\n\t"
    << "user param: " << userParam << "\n"
    << "}\n";

    __debugbreak();
}

void checkShaderCompileStatus(u32 shader_id, str name) {
  i32 shader_compile_status;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compile_status);

  if (shader_compile_status == GL_FALSE) {

    std::cout
      << "validating " << name.to_c_str() << " shader ... {\n\t"
      << "shader program compile status: " << shader_compile_status << "\n\t";

    i32 length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
    char* info_log = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(shader_id, length, &length, info_log);

    std::cout 
      << "\t info_log (length " << length << "): " << info_log
      << "\n}\n";
  } else {
    std::cout << "opengl " << name.to_c_str() << " shader compiled successully (we hope)\n";
  }

}

void opengl_poll_for_errors() {
  GLenum err = 1;
  while (err != GL_NO_ERROR) {
    err = glGetError();
    str err_string = "";

    switch (err) {
      case GL_INVALID_ENUM:                   err_string = "GL_INVALID_ENUM";                   break;
      case GL_INVALID_VALUE:                  err_string = "GL_INVALID_VALUE";                  break;
      case GL_INVALID_OPERATION:              err_string = "GL_INVALID_OPERATION";              break;
      case GL_STACK_OVERFLOW:                 err_string = "GL_STACK_OVERFLOW";                 break;
      case GL_STACK_UNDERFLOW:                err_string = "GL_STACK_UNDERFLOW";                break;
      case GL_OUT_OF_MEMORY:                  err_string = "GL_OUT_OF_MEMORY";                  break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:  err_string = "GL_INVALID_FRAMEBUFFER_OPERATION";  break;
      default: break;
    }

    if (err_string.length()) {
      std::cout << "opengl error: " << err_string.to_c_str() << "\n";

    }
  }
}