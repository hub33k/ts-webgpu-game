#pragma once

// dawn/utils/compiler.h
namespace {

// HK_COMPILER_IS(CLANG|GCC|MSVC): Compiler detection
//
// Note: clang masquerades as GCC on POSIX and as MSVC on Windows. It must be checked first.
#if defined(__clang__)
  #define HK_COMPILER_IS_CLANG 1
  #define HK_COMPILER_IS_GCC 0
  #define HK_COMPILER_IS_MSVC 0
#elif defined(__GNUC__)
  #define HK_COMPILER_IS_CLANG 0
  #define HK_COMPILER_IS_GCC 1
  #define HK_COMPILER_IS_MSVC 0
#elif defined(_MSC_VER)
  #define HK_COMPILER_IS_CLANG 0
  #define HK_COMPILER_IS_GCC 0
  #define HK_COMPILER_IS_MSVC 1
#else
  #error "Unsupported compiler"
#endif

// Use #if HK_COMPILER_IS(XXX) for compiler specific code.
// Do not use #ifdef or the naked macro HK_COMPILER_IS_XXX.
// This can help avoid common mistakes like not including "compiler.h" and falling into unwanted
// code block as usage of undefined macro "function" will be blocked by the compiler.
#define HK_COMPILER_IS(X) (1 == HK_COMPILER_IS_##X)

// HK_HAS_ATTRIBUTE
//
// A wrapper around `__has_attribute`. This test whether its operand is recognized by the compiler.
#if defined(__has_attribute)
  #define HK_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
  #define HK_HAS_ATTRIBUTE(x) 0
#endif

// HK_HAS_CPP_ATTRIBUTE
//
// A wrapper around `__has_cpp_attribute`. This test whether its operand is recognized by the
// compiler.
#if defined(__has_cpp_attribute)
  #define HK_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
  #define HK_HAS_CPP_ATTRIBUTE(x) 0
#endif

// HK_BUILTIN_UNREACHABLE()
//
// Hints the compiler that a code path is unreachable.
#if HK_COMPILER_IS(MSVC)
  #define HK_BUILTIN_UNREACHABLE() __assume(false)
#else
  #define HK_BUILTIN_UNREACHABLE() __builtin_unreachable()
#endif

// HK_LIKELY(EXPR)
//
// Where available, hints the compiler that the expression will be true to help it generate code
// that leads to better branch prediction.
#if HK_COMPILER_IS(GCC) || HK_COMPILER_IS(CLANG)
  #define HK_LIKELY(x) __builtin_expect(!!(x), 1)
#else
  #define HK_LIKELY(x) (x)
#endif

// HK_UNLIKELY(EXPR)
//
// Where available, hints the compiler that the expression will be false to help it generate code
// that leads to better branch prediction.
#if HK_COMPILER_IS(GCC) || HK_COMPILER_IS(CLANG)
  #define HK_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
  #define HK_UNLIKELY(x) (x)
#endif

// HK_ASAN_ENABLED()
//
// Checks whether ASan is enabled.
#if HK_COMPILER_IS(CLANG)
  #define HK_ASAN_ENABLED() __has_feature(address_sanitizer)
#elif HK_COMPILER_IS(GCC) || HK_COMPILER_IS(MSVC)
  #define HK_ASAN_ENABLED() defined(__SANITIZE_ADDRESS__)
#endif

// HK_NO_SANITIZE(instrumentation)
//
// Annotate a function or a global variable declaration to specify that a particular instrumentation
// or set of instrumentations should not be applied.
#if HK_HAS_ATTRIBUTE(no_sanitize)
  #define HK_NO_SANITIZE(instrumentation) __attribute__((no_sanitize(instrumentation)))
#else
  #define HK_NO_SANITIZE(instrumentation)
#endif

} // namespace

// dawn/common/Compiler.h
namespace { // Defines macros for compiler-specific functionality

// HK_UNUSED_FUNC
//
// Prevents unused variable/expression warnings on FUNC.
#define HK_UNUSED_FUNC(FUNC) static_assert(sizeof(&FUNC) == sizeof(void (*)()))

// HK_FORCE_INLINE
//
// Annotate a function indicating it should really never be inline, even in debug mode.
#if HK_COMPILER_IS(CLANG) && defined(NDEBUG) && HK_HAS_CPP_ATTRIBUTE(clang::always_inline)
  #define HK_FORCE_INLINE [[clang::always_inline]] inline
#elif HK_COMPILER_IS(GCC) && defined(NDEBUG) && HK_HAS_ATTRIBUTE(always_inline)
  #define HK_FORCE_INLINE inline __attribute__((always_inline))
#elif HK_COMPILER_IS(MSVC) && defined(NDEBUG)
  #define HK_FORCE_INLINE __forceinline
#else
  #define HK_FORCE_INLINE inline
#endif

// HK_TRIVIAL_ABI
//
// Marks a type as being eligible for the "trivial" ABI despite having a
// non-trivial destructor or copy/move constructor. Such types can be relocated
// after construction by simply copying their memory, which makes them eligible
// to be passed in registers. The canonical example is std::unique_ptr.
//
// Use with caution; this has some subtle effects on constructor/destructor
// ordering and will be very incorrect if the type relies on its address
// remaining constant. When used as a function argument (by value), the value
// may be constructed in the caller's stack frame, passed in a register, and
// then used and destructed in the callee's stack frame. A similar thing can
// occur when values are returned.
//
// TRIVIAL_ABI is not needed for types which have a trivial destructor and
// copy/move constructors, such as base::TimeTicks and other POD.
//
// It is also not likely to be effective on types too large to be passed in one
// or two registers on typical target ABIs.
//
// See also:
//   https://clang.llvm.org/docs/AttributeReference.html#trivial-abi
//   https://libcxx.llvm.org/docs/DesignDocs/UniquePtrTrivialAbi.html
#if HK_COMPILER_IS(CLANG) && HK_HAS_ATTRIBUTE(trivial_abi)
  #define HK_TRIVIAL_ABI [[clang::trivial_abi]]
#else
  #define HK_TRIVIAL_ABI
#endif

// HK_CONSTINIT
//
// Requires constant initialization. See constinit in C++20. Allows to rely on a variable being
// initialized before execution, and not requiring a global constructor.
#if HK_HAS_ATTRIBUTE(require_constant_initialization)
  #define HK_CONSTINIT __attribute__((require_constant_initialization))
#else
  #define HK_CONSTINIT
#endif

// HK_GSL_POINTER
//
// In [lifetime safety], If you write a custom class wrapping a pointer, the [[gsl::Owner]] and
// [[gsl::Pointer]] can help the compiler to know if it acquired ownership over the pointee, or not.
// The compiler is then able to emit useful warning.
//
// [lifetime safety]: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1179r0.pdf
#if HK_COMPILER_IS(CLANG)
  #define HK_GSL_POINTER [[gsl::Pointer]]
#else
  #define HK_GSL_POINTER
#endif

// HK_CONSTEXPR_DTOR
//
// Constexpr destructors were introduced in C++20. Dawn's minimum supported C++ version is C++17.
#if defined(__cpp_constexpr) && __cpp_constexpr >= 201907L
  #define HK_CONSTEXPR_DTOR constexpr
#else
  #define HK_CONSTEXPR_DTOR
#endif

// HK_ATTRIBUTE_RETURNS_NONNULL
//
// Tells the compiler that a function never returns a null pointer. Sourced from Abseil's
// `attributes.h`.
#if HK_HAS_ATTRIBUTE(returns_nonnull)
  #define HK_ATTRIBUTE_RETURNS_NONNULL __attribute__((returns_nonnull))
#else
  #define HK_ATTRIBUTE_RETURNS_NONNULL
#endif

// HK_ENABLE_STRUCT_PADDING_WARNINGS
//
// Tells the compiler to emit a warning if the structure has any padding.
// This is helpful to avoid uninitialized bits in cache keys or other similar structures.
#if HK_COMPILER_IS(CLANG)
  #define HK_ENABLE_STRUCT_PADDING_WARNINGS                                                                            \
    _Pragma("clang diagnostic push") _Pragma("clang diagnostic error \"-Wpadded\"")
  #define HK_DISABLE_STRUCT_PADDING_WARNINGS _Pragma("clang diagnostic pop")
#elif HK_COMPILER_IS(GCC)
  #define HK_ENABLE_STRUCT_PADDING_WARNINGS _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic error \"-Wpadded\"")
  #define HK_DISABLE_STRUCT_PADDING_WARNINGS _Pragma("GCC diagnostic pop")
#elif HK_COMPILER_IS(MSVC)
  #define HK_ENABLE_STRUCT_PADDING_WARNINGS __pragma(warning(push)) __pragma(warning(error : 4820))
  #define HK_DISABLE_STRUCT_PADDING_WARNINGS __pragma(warning(pop))
#else
  #define HK_ENABLE_STRUCT_PADDING_WARNINGS
  #define HK_DISABLE_STRUCT_PADDING_WARNINGS
#endif

} // namespace
