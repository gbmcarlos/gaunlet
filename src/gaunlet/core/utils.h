#pragma once

#include <functional>

#define GL_BIND_CALLBACK_FN(fn) std::bind(&fn, this, std::placeholders::_1)
