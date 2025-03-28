#pragma once

namespace { // Define platform macros for OSes:

#define HK_PLATFORM_IS(X) (1 == HK_PLATFORM_IS_##X)

// Use #if HK_PLATFORM_IS(X) for platform specific code.
// Do not use #ifdef or the naked macro HK_PLATFORM_IS_X.
// This can help avoid common mistakes like not including "Platform.h" and falling into unwanted
// code block as usage of undefined macro "function" will be blocked by the compiler.

//  - WINDOWS
//    - WIN32
//    - WINUWP
//  - POSIX
//    - LINUX
//      - ANDROID
//      - CHROMEOS
//    - APPLE
//      - IOS
//      - TVOS
//      - MACOS
//    - FUCHSIA
//    - EMSCRIPTEN
#if defined(_WIN32) || defined(_WIN64)
  #include <winapifamily.h>
  #define HK_PLATFORM_IS_WINDOWS 1
  #if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
    #define HK_PLATFORM_IS_WIN32 1
  #elif WINAPI_FAMILY == WINAPI_FAMILY_PC_APP
    #define HK_PLATFORM_IS_WINUWP 1
  #else
    #error "Unsupported Windows platform."
  #endif

#elif defined(__linux__)
  #define HK_PLATFORM_IS_LINUX 1
  #define HK_PLATFORM_IS_POSIX 1
  #if defined(__ANDROID__)
    #define HK_PLATFORM_IS_ANDROID 1
  #elif defined(HK_OS_CHROMEOS)
    #define HK_PLATFORM_IS_CHROMEOS 1
  #else
    #define HK_PLATFORM_IS_LINUX_DESKTOP 1
  #endif

#elif defined(__APPLE__)
  #define HK_PLATFORM_IS_APPLE 1
  #define HK_PLATFORM_IS_POSIX 1
  #include <TargetConditionals.h>
  #if TARGET_OS_IPHONE
    #define HK_PLATFORM_IS_IOS 1
    #if TARGET_OS_TV
      #define HK_PLATFORM_IS_TVOS 1
    #endif
  #elif TARGET_OS_MAC
    #define HK_PLATFORM_IS_MACOS 1
  #else
    #error "Unsupported Apple platform."
  #endif

#elif defined(__Fuchsia__)
  #define HK_PLATFORM_IS_FUCHSIA 1
  #define HK_PLATFORM_IS_POSIX 1

#elif defined(__EMSCRIPTEN__)
  #define HK_PLATFORM_IS_EMSCRIPTEN 1
  #define HK_PLATFORM_IS_POSIX 1
  #include <emscripten/emscripten.h>

#else
  #error "Unsupported platform."
#endif

} // namespace

namespace { // Define platform macros for CPU architectures:

//  - X86
//    - I386
//    - X86_64
//  - ARM
//    - ARM32
//    - ARM64
//  - LOONGARCH
//    - LOONGARCH32
//    - LOONGARCH64
//  - RISCV
//    - RISCV32
//    - RISCV64
//  - MIPS
//    - MIPS32
//    - MIPS64
//  - S390
//  - S390X
//  - PPC
//  - PPC64
#if defined(__i386__) || defined(_M_IX86)
  #define HK_PLATFORM_IS_X86 1
  #define HK_PLATFORM_IS_I386 1
#elif defined(__x86_64__) || defined(_M_X64)
  #define HK_PLATFORM_IS_X86 1
  #define HK_PLATFORM_IS_X86_64 1

#elif defined(__arm__) || defined(_M_ARM)
  #define HK_PLATFORM_IS_ARM 1
  #define HK_PLATFORM_IS_ARM32 1
#elif defined(__aarch64__) || defined(_M_ARM64)
  #define HK_PLATFORM_IS_ARM 1
  #define HK_PLATFORM_IS_ARM64 1

#elif defined(__loongarch__)
  #define HK_PLATFORM_IS_LOONGARCH 1
  #if __loongarch_grlen == 64
    #define HK_PLATFORM_IS_LOONGARCH64 1
  #else
    #define HK_PLATFORM_IS_LOONGARCH32 1
  #endif

#elif defined(__riscv)
  #define HK_PLATFORM_IS_RISCV 1
  #if __riscv_xlen == 32
    #define HK_PLATFORM_IS_RISCV32 1
  #else
    #define HK_PLATFORM_IS_RISCV64 1
  #endif

#elif defined(__mips__)
  #define HK_PLATFORM_IS_MIPS 1
  #if _MIPS_SIM == _ABIO32
    #define HK_PLATFORM_IS_MIPS32 1
  #else
    #define HK_PLATFORM_IS_MIPS64 1
  #endif

#elif defined(__s390__)
  #define HK_PLATFORM_IS_S390 1
#elif defined(__s390x__)
  #define HK_PLATFORM_IS_S390X 1

#elif defined(__PPC__)
  #define HK_PLATFORM_IS_PPC 1
#elif defined(__PPC64__)
  #define HK_PLATFORM_IS_PPC64 1

#elif defined(__wasm32__)
  #define HK_PLATFORM_IS_WASM32 1
#elif defined(__wasm64__)
  #define HK_PLATFORM_IS_WASM64 1

#else
  #error "Unsupported platform."
#endif

} // namespace

namespace { // Define platform macros for pointer width:

//  - 64_BIT
//  - 32_BIT
#if defined(HK_PLATFORM_IS_X86_64) || defined(HK_PLATFORM_IS_ARM64) || defined(HK_PLATFORM_IS_RISCV64) ||              \
  defined(HK_PLATFORM_IS_MIPS64) || defined(HK_PLATFORM_IS_S390X) || defined(HK_PLATFORM_IS_PPC64) ||                  \
  defined(HK_PLATFORM_IS_LOONGARCH64) || defined(HK_PLATFORM_IS_WASM64)
  #define HK_PLATFORM_IS_64_BIT 1
  static_assert(sizeof(sizeof(char)) == 8, "Expect sizeof(size_t) == 8");
#elif defined(HK_PLATFORM_IS_I386) || defined(HK_PLATFORM_IS_ARM32) || defined(HK_PLATFORM_IS_RISCV32) ||              \
  defined(HK_PLATFORM_IS_MIPS32) || defined(HK_PLATFORM_IS_S390) || defined(HK_PLATFORM_IS_PPC32) ||                   \
  defined(HK_PLATFORM_IS_LOONGARCH32) || defined(HK_PLATFORM_IS_WASM32)
  #define HK_PLATFORM_IS_32_BIT 1
  static_assert(sizeof(sizeof(char)) == 4, "Expect sizeof(size_t) == 4");
#else
  #error "Unsupported platform"
#endif

} // namespace

namespace { // This section define other platform macros to 0 to avoid undefined macro usage error.

#if !defined(HK_PLATFORM_IS_WINDOWS)
  #define HK_PLATFORM_IS_WINDOWS 0
#endif
#if !defined(HK_PLATFORM_IS_WIN32)
  #define HK_PLATFORM_IS_WIN32 0
#endif
#if !defined(HK_PLATFORM_IS_WINUWP)
  #define HK_PLATFORM_IS_WINUWP 0
#endif

#if !defined(HK_PLATFORM_IS_POSIX)
  #define HK_PLATFORM_IS_POSIX 0
#endif

#if !defined(HK_PLATFORM_IS_LINUX)
  #define HK_PLATFORM_IS_LINUX 0
#endif
#if !defined(HK_PLATFORM_IS_ANDROID)
  #define HK_PLATFORM_IS_ANDROID 0
#endif
#if !defined(HK_PLATFORM_IS_CHROMEOS)
  #define HK_PLATFORM_IS_CHROMEOS 0
#endif
#if !defined(HK_PLATFORM_IS_LINUX_DESKTOP)
  #define HK_PLATFORM_IS_LINUX_DESKTOP 0
#endif

#if !defined(HK_PLATFORM_IS_APPLE)
  #define HK_PLATFORM_IS_APPLE 0
#endif
#if !defined(HK_PLATFORM_IS_IOS)
  #define HK_PLATFORM_IS_IOS 0
#endif
#if !defined(HK_PLATFORM_IS_MACOS)
  #define HK_PLATFORM_IS_MACOS 0
#endif

#if !defined(HK_PLATFORM_IS_FUCHSIA)
  #define HK_PLATFORM_IS_FUCHSIA 0
#endif
#if !defined(HK_PLATFORM_IS_EMSCRIPTEN)
  #define HK_PLATFORM_IS_EMSCRIPTEN 0
#endif

#if !defined(HK_PLATFORM_IS_X86)
  #define HK_PLATFORM_IS_X86 0
#endif
#if !defined(HK_PLATFORM_IS_I386)
  #define HK_PLATFORM_IS_I386 0
#endif
#if !defined(HK_PLATFORM_IS_X86_64)
  #define HK_PLATFORM_IS_X86_64 0
#endif

#if !defined(HK_PLATFORM_IS_ARM)
  #define HK_PLATFORM_IS_ARM 0
#endif
#if !defined(HK_PLATFORM_IS_ARM32)
  #define HK_PLATFORM_IS_ARM32 0
#endif
#if !defined(HK_PLATFORM_IS_ARM64)
  #define HK_PLATFORM_IS_ARM64 0
#endif

#if !defined(HK_PLATFORM_IS_LOONGARCH)
  #define HK_PLATFORM_IS_LOONGARCH 0
#endif
#if !defined(HK_PLATFORM_IS_LOONGARCH32)
  #define HK_PLATFORM_IS_LOONGARCH32 0
#endif
#if !defined(HK_PLATFORM_IS_LOONGARCH64)
  #define HK_PLATFORM_IS_LOONGARCH64 0
#endif

#if !defined(HK_PLATFORM_IS_RISCV)
  #define HK_PLATFORM_IS_RISCV 0
#endif
#if !defined(HK_PLATFORM_IS_RISCV32)
  #define HK_PLATFORM_IS_RISCV32 0
#endif
#if !defined(HK_PLATFORM_IS_RISCV64)
  #define HK_PLATFORM_IS_RISCV64 0
#endif

#if !defined(HK_PLATFORM_IS_MIPS)
  #define HK_PLATFORM_IS_MIPS 0
#endif
#if !defined(HK_PLATFORM_IS_MIPS32)
  #define HK_PLATFORM_IS_MIPS32 0
#endif
#if !defined(HK_PLATFORM_IS_MIPS64)
  #define HK_PLATFORM_IS_MIPS64 0
#endif

#if !defined(HK_PLATFORM_IS_S390)
  #define HK_PLATFORM_IS_S390 0
#endif
#if !defined(HK_PLATFORM_IS_S390X)
  #define HK_PLATFORM_IS_S390X 0
#endif

#if !defined(HK_PLATFORM_IS_PPC)
  #define HK_PLATFORM_IS_PPC 0
#endif
#if !defined(HK_PLATFORM_IS_PPC64)
  #define HK_PLATFORM_IS_PPC64 0
#endif

#if !defined(HK_PLATFORM_IS_WASM32)
  #define HK_PLATFORM_IS_WASM32 0
#endif
#if !defined(HK_PLATFORM_IS_WASM64)
  #define HK_PLATFORM_IS_WASM64 0
#endif

#if !defined(HK_PLATFORM_IS_64_BIT)
  #define HK_PLATFORM_IS_64_BIT 0
#endif
#if !defined(HK_PLATFORM_IS_32_BIT)
  #define HK_PLATFORM_IS_32_BIT 0
#endif

} // namespace
