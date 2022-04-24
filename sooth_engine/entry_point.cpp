
#include <GLFW/glfw3.h>

#include <iostream>

// #include <windows.h>

#include <lapse_lib.h>

using namespace lapse;

int main() {
  std::cout << "Hello World\n";

  u64 timer;

  u64 l;

  while (timer++ < 2'000'000) { l ^= timer; }
  

  return 0;
}
