#pragma once

#include "lapse_string.h"
#include "lapse_lambda.h"

#include <fstream>

namespace lapse{

class Path{
public:
  array<str> my_path;
  Path(str input_path) {
    my_path += input_path.split('/');
  }
  str to_str() { my_path.join("/"); }
  char* to_c_str() { to_str().to_c_str(); }
};

class File{
public:
  std::fstream my_file;
  File(Path file_path) : my_file(file_path.to_c_str()) { ; };
  bool operator!() const { return !my_file.is_open(); };
  // call callback on each line of file
  void lines(lapse_lambda(void, str) callback);
};

};