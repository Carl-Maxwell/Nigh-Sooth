#include "lapse_console.h"

namespace lapse{

void Console::h1(str heading) {
  std::cout
    << "\n\n//-----------------------------------------------------------------------------\n"
    << "// " << heading.to_c_str() << "\n"
    << "//-----------------------------------------------------------------------------\n\n"
  ;
}

void Console::h2(str heading) {
  std::cout
    << "\n\n//\n"
    << "// " << heading.to_c_str() << "\n"
    << "//\n\n"
  ;
}

void Console::h3(str heading) {
    std::cout << "\n\n// " << heading.to_c_str() << "\n";
}

void Console::print(str a_string) {
  std::cout << a_string.to_c_str() << "\n";
}

} // end namespace
