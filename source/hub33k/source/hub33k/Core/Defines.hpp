#pragma once

#include <cstddef>
#include <cstdint>

// typedefs, defines, macros, HAL (Hardware Abstraction Layer) etc.

// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/Base.h
// https://github.com/PolyEngineTeam/PolyEngine/blob/master/PolyEngine/Core/Src/Defines.hpp
// https://github.com/travisvroman/kohi/blob/main/kohi.core/src/defines.h
// https://github.com/godotengine/godot/blob/master/core/typedefs.h
// check ElectronicsArt, UnrealEngine, dawn etc.

/**
 * Basic definitions and simple functions to be used everywhere.
 */

namespace hk {

  using u8 = uint8_t;
  using u16 = uint16_t;
  using u32 = uint32_t;
  using u64 = uint64_t;

  using i8 = int8_t;
  using i16 = int16_t;
  using i32 = int32_t;
  using i64 = int64_t;

  using size = size_t;

  using f32 = float;
  using f64 = double;

} // namespace hk
