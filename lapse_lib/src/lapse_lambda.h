#pragma once

#include <functional>

namespace lapse{

#define lapse_lambda(return_type, ...) std::function<return_type(__VA_ARGS__)>

}