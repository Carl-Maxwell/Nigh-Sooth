
#include "lapse_file.h"

#include <fstream>
#include <string>

namespace lapse{

void File::lines(lapse_lambda(void, str) callback) {
  std::string c_str_line;
  while (!!this && std::getline(my_file, c_str_line)) {
    callback(str(c_str_line.c_str()));
  }
}

};