# Nigh Sooth
Nigh Sooth is a game engine project. I'm working on it as a way of learning about game engine programming.

It uses various libraries: javidx9's pixel game engine, stb_image, the C++ standard library, etc. It used to use glfw & glew, but I ran into troubles debugging opengl so I switched over to the pixel engine, there are still submodules for those libs.

I hope to gradually remove all the libraries and re-implement everything from scratch, but I didn't have the time or knowhow to do that initially.

This project is in the public domain, I've included license.txt files in each of the source code folders specifying that. The libraries (the /vendor/ folders, premake folder, etc) are obviously their own various licenses.

I plan for the project to be broken up into various sub projects:

- **lapse_lib** -- typedefs (u64, f32, etc), vec2, vec3, quaternions, mat4x4, file handling, array, str, sorts, searches, data structures, etc.
- **lapse_test** -- tests for the lapse library, uses std::cout for convenience's sake, wasn't sure how to get console io working without the standard library.
- **sooth_engine** -- game engine
  - *mui::* -- part of sooth, I'm trying to combine the CSS box model with immediate mode graphical user interface ideas, though I wouldn't say they're synergizing well.
- **games**
  - **text_choice_game** -- interactive fiction sortof thing, type 1 or 2 to choose this path or that path, etc. Simple game for testing out the engine's basic string stuff.
  - **minesweeper_game** -- fairly functional minesweeper game
