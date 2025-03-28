#pragma once

// IWYU pragma: begin_exports
#include "hub33k/Core/Compiler.hpp"
#include "hub33k/Core/Platform.hpp"
// #include "hub33k/Core/PlatformDetection.hpp"
// IWYU pragma: end_exports

#include <memory>

#define DATA_DIR PROJECT_DATA_DIR

// Build type
// ================================================================

// Debug define - BUILD_DEBUG, !defined(NDEBUG)
// Release define - BUILD_RELEASE, defined(NDEBUG)
#if defined(BUILD_DEBUG) || !defined(NDEBUG)
  #define HK_BUILD_DEBUG 1
  #define HK_BUILD_RELEASE 0
#elif defined(BUILD_RELEASE) || defined(NDEBUG)
  #define HK_BUILD_DEBUG 0
  #define HK_BUILD_RELEASE 1
#else
  #error "Unknown build type!"
#endif

#if HK_BUILD_DEBUG
  #define HK_ENABLE_ASSERTS 1
#else
  #define HK_ENABLE_ASSERTS 0
#endif

// ================================================================

#define BIT(x) (1 << x)

#define KB(x) ((unsigned long long)1024 * x)
#define MB(x) ((unsigned long long)1024 * KB(x))
#define GB(x) ((unsigned long long)1024 * MB(x))

#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

#define HK_EXPAND_MACRO(x) x
#define HK_STRINGIFY_MACRO(x) #x
#define HK_BIND_EVENT_FN(fn)                                                                                           \
  [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// ================================================================

namespace hub33k {

  template <typename T> using Scope = std::unique_ptr<T>;

  template <typename T, typename... Args> constexpr Scope<T> CreateScope(Args &&...args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  template <typename T> using Ref = std::shared_ptr<T>;

  template <typename T, typename... Args> constexpr Ref<T> CreateRef(Args &&...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

} // namespace hub33k

// IWYU pragma: begin_exports
#include "hub33k/Core/Assert.hpp"
#include "hub33k/Core/Defines.hpp"

#if HK_PLATFORM_IS(EMSCRIPTEN)
  // Core log macros
  #define HK_LOG_CORE_TRACE(...) std::println("[HK_CORE] [TRACE] {}", __VA_ARGS__)
  #define HK_LOG_CORE_INFO(...) std::println("[HK_CORE] [INFO] {}", __VA_ARGS__)
  #define HK_LOG_CORE_WARN(...) std::println("[HK_CORE] [WARN] {}", __VA_ARGS__)
  #define HK_LOG_CORE_ERROR(...) std::println("[HK_CORE] [ERROR] {}", __VA_ARGS__)
  #define HK_LOG_CORE_CRITICAL(...) std::println("[HK_CORE] [CRITICAL] {}", __VA_ARGS__)

  // Client log macros
  #define HK_LOG_TRACE(...) std::println("[HK] [TRACE] {}", __VA_ARGS__)
  #define HK_LOG_INFO(...) std::println("[HK] [INFO] {}", __VA_ARGS__)
  #define HK_LOG_WARN(...) std::println("[HK] [WARN] {}", __VA_ARGS__)
  #define HK_LOG_ERROR(...) std::println("[HK] [ERROR] {}", __VA_ARGS__)
  #define HK_LOG_CRITICAL(...) std::println("[HK] [CRITICAL] {}", __VA_ARGS__)
#else
  #include "hub33k/Core/Log.hpp"
#endif
// IWYU pragma: end_exports
