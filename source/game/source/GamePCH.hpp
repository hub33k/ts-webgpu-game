#pragma once

// IWYU pragma: begin_exports

// C Standard Library header files
// https://en.cppreference.com/w/c/header
// ================================================================

// C++ Standard Library headers
// https://en.cppreference.com/w/cpp/header
// ================================================================

// Utilities library
// ================================

#include <functional>
#include <utility>

// Dynamic memory management
#include <memory>

// Strings library
// ================================

#include <string>

// Containers library
// ================================

#include <array>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Algorithms library
// ================================

#include <algorithm>

// Input/output library
// ================================

#include <fstream>
#include <iostream>
#include <print>
#include <sstream>

// Internal
// ================================================================

#include "hub33k/Core/Base.hpp"
#include "hub33k/Core/Compiler.hpp"
#include "hub33k/Core/Platform.hpp"
#include "hub33k/Debug/Instrumentor.hpp"

// Platform specific
// ================================================================

#include <SDL3/SDL.h>
#include <webgpu/webgpu_cpp.h>

// IWYU pragma: end_exports
