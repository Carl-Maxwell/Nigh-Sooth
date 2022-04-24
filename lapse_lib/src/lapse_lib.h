#pragma once

#include <iostream>

#include "lapse_scalar.h"

namespace lapse{

// takes in a number 0..9 and returns it as a char
char u32_to_ascii(u32 i) { return '0' + i; }

};

#include "lapse_lambda.h"

#include "lapse_math.h"

#include "lapse_integer.h"
#include "lapse_f32.h"

#include "lapse_exceptions.h"
#include "lapse_array.h"
#include "lapse_string.h"

// #include "lapse_range.h"

#include "lapse_random.h"

// TODO uids

// #include "lapse_file.h"

// data structures

// TODO hashmap
// TODO binary_tree
// TODO slot_map (for UIDs)

// various math types

// TODO vec2, vec3, vec4, mat4x4
// TODO euler, quaternion, etc

// logging utilities

// TODO to_string
