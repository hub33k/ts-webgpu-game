#pragma once

#include <filesystem> // IWYU pragma: exports

// Debug break
// ================================================================

#if HK_ENABLE_ASSERTS
  #if HK_PLATFORM_IS(WINDOWS)
    #define HK_DEBUGBREAK() __debugbreak()
  #elif HK_PLATFORM_IS(LINUX)
    #include <signal.h>
    // __builtin_debugtrap()
    #define HK_DEBUGBREAK() raise(SIGTRAP)
  #elif HK_PLATFORM_IS(MACOS)
    #define HK_DEBUGBREAK() __builtin_trap()
  #elif HK_PLATFORM_IS(EMSCRIPTEN)
    // __asm__("debugger") or emscripten_debugger()
    #define HK_DEBUGBREAK() __asm__("debugger")
  #else
    #error "Platform doesn't support debugbreak yet!"
  #endif
#else
  #define HK_DEBUGBREAK()
#endif

// Asserts
// ================================================================

#if HK_ENABLE_ASSERTS
  // Alternatively we could use the same "default" message for both "WITH_MSG"
  // and "NO_MSG" and provide support for custom formatting by concatenating the
  // formatting string instead of having the format inside the default message
  #define HK_INTERNAL_ASSERT_IMPL(type, check, msg, ...)                                                               \
    {                                                                                                                  \
      if (!(check)) {                                                                                                  \
        HK_LOG##type##ERROR(msg, __VA_ARGS__);                                                                         \
        HK_DEBUGBREAK();                                                                                               \
      }                                                                                                                \
    }
  #define HK_INTERNAL_ASSERT_WITH_MSG(type, check, ...)                                                                \
    HK_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
  #define HK_INTERNAL_ASSERT_NO_MSG(type, check)                                                                       \
    HK_INTERNAL_ASSERT_IMPL(                                                                                           \
      type, check, "Assertion '{0}' failed at {1}:{2}", HK_STRINGIFY_MACRO(check),                                     \
      std::filesystem::path(__FILE__).filename().string(), __LINE__                                                    \
    )

  #define HK_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
  #define HK_INTERNAL_ASSERT_GET_MACRO(...)                                                                            \
    HK_EXPAND_MACRO(                                                                                                   \
      HK_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, HK_INTERNAL_ASSERT_WITH_MSG, HK_INTERNAL_ASSERT_NO_MSG)           \
    )

  // Currently accepts at least the condition and one additional parameter (the message) being optional
  #define HK_ASSERT(...) HK_EXPAND_MACRO(HK_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
  #define HK_CORE_ASSERT(...) HK_EXPAND_MACRO(HK_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
  #define HK_ASSERT(...)
  #define HK_CORE_ASSERT(...)
#endif
