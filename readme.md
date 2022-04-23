# Nigh Sooth
Nigh Sooth is a game engine project I'm making from scratch (as much as I can). I'm working on it as a way of learning about game engine programming.

It uses glfw & stb_image, but hopefully won't have other dependencies. It wasn't going to use the standard library (implementing basic functionality is helpful for learning purposes), but has ended up depending on more and more of it (std::function, std::initializer_list, etc). I hope to go back and implement that stuff properly from scratch in the future and I'm trying to limit how far that stuff reaches into the codebase.

All the code in this repo is in the public domain, excepting any submodules, and the premake/ folder which contains the premake exe & license file.

I plan for the project to be broken up into various sub projects:

- **lapse_lib** -- typedefs, vec2, vec3, quaternions, mat4x4, file handling, array, str, sorts, searches, data structures, etc.
- **lapse_test** -- tests for the lapse library, uses std::cout for convenience's sake, wasn't sure how to get console io working without the standard library.
- **sooth_engine** -- game engine
	- *ui::* -- part of sooth, thinking of some sort of immediate mode thing
- **text_choice_game** -- fiction text, choose 1 or 2, etc.
- **minesweeper_game**
