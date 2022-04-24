#pragma once

namespace lapse{

//-----------------------------------------------------------------------------
// integer funcs
//-----------------------------------------------------------------------------

// takes in a number 0..9 and returns it as a char
char u32_to_ascii(u32 i);

// convert u32 to c_str 123 -> "123\0"
char* u32_to_c_str(u32 in);

}