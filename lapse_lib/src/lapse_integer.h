#pragma once

namespace lapse{

//-----------------------------------------------------------------------------
// integer funcs
//-----------------------------------------------------------------------------

char* u32_to_c_str(u32 in) {
  char* output;

  if (in == 0) { return (char*)"0"; }

  u32 length = 0;
  u32 tens = 1; // which tens place we're looking at
  while (in/tens > 0) {
    tens *= 10;
    length++;
  }

  tens /= 10;

  output = new char[length + 1]; // +1 accounts for null terminator
  u32 i = 0;
  
  while (in > 0) {
    output[i++] = '0' + in/tens;
    in -= in/tens*tens;
    tens /= 10;
  }

  output[i++] = '\0';

  return output;
}

}