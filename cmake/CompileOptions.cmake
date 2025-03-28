# clang: https://clang.llvm.org/docs/CommandGuide/clang.html
# clang-cl: https://clang.llvm.org/docs/UsersManual.html #id11

# Get upper case system name
string(TOUPPER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_UPPER)

# Project options
set(DEFAULT_PROJECT_OPTIONS
  DEBUG_POSTFIX             "d"

  C_STANDARD_REQUIRED       ON
  C_STANDARD                17

  CXX_STANDARD_REQUIRED     ON
  CXX_STANDARD              23
  CXX_EXTENSIONS            OFF # We use -std=c++17 instead of -std=gnu++17 in macOS
  #CXX_VISIBILITY_PRESET     "hidden"

  LINKER_LANGUAGE           "CXX"
  POSITION_INDEPENDENT_CODE ON
  #COMPILE_WARNING_AS_ERROR  ON
)

# Include directories
set(DEFAULT_INCLUDE_DIRECTORIES)

# Libraries
set(DEFAULT_LIBRARIES)

# Compile definitions
set(DEFAULT_COMPILE_DEFINITIONS
  SYSTEM_${SYSTEM_NAME_UPPER}
)
# Variables in CPP
set(DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS}
  -DPROJECT_NAME="${PROJECT_NAME}"
)
# Build type
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS}
    -DBUILD_DEBUG
    -DPROJECT_ROOT_DIR="${DIR_ROOT}/"
    -DPROJECT_DATA_DIR="${DIR_DATA}/"
  )
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  set(DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS}
    -DBUILD_RELEASE
    -DPROJECT_ROOT_DIR="/"
    -DPROJECT_DATA_DIR="data/"
  )
endif()

# MSVC compiler options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
  set(DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS}
    _SCL_SECURE_NO_WARNINGS  # Calling any one of the potentially unsafe methods in the Standard C++ Library
    _CRT_SECURE_NO_WARNINGS  # Calling any one of the potentially unsafe methods in the CRT Library
  )
endif()

# Compile options
set(DEFAULT_COMPILE_OPTIONS)

# MSVC compiler options
# https://learn.microsoft.com/en-us/visualstudio/debugger/hot-reload?view=vs-2022
# https://devblogs.microsoft.com/cppblog/improvements-for-c-edit-and-continue-in-visual-studio-2015-update-1/
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
  set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
    PRIVATE
      /MP           # -> build with multiple processes
      /W4           # -> warning level 4

      /MP           # -> build with multiple processes
      /W4           # -> warning level 4
      # /WX         # -> treat warnings as errors

      $<$<CONFIG:Debug>:
        /ZI
        /INCREMENTAL
        # /RTCc         # -> value is assigned to a smaller data type and results in a data loss
      >

      $<$<CONFIG:Release>:
        /Gw           # -> whole program global optimization
        /GS-          # -> buffer security check: no
        /GL           # -> whole program optimization: enable link-time code generation (disables Zi)
        /GF           # -> enable string pooling
      >

      # No manual c++11 enable for MSVC as all supported MSVC versions for cmake-init have C++11 implicitly enabled (MSVC >=2013)

    PUBLIC
      /wd4251       # -> disable warning: 'identifier': class 'type' needs to have dll-interface to be used by clients of class 'type2'
      /wd4592       # -> disable warning: 'identifier': symbol will be dynamically initialized (implementation limitation)
      # /wd4201     # -> disable warning: nonstandard extension used: nameless struct/union (caused by GLM)
      # /wd4127     # -> disable warning: conditional expression is constant (caused by Qt)

      # Fix for: `static_assert failed: 'Unicode support requires compiling with /utf-8'`
      # https://github.com/gabime/spdlog/issues/3251#issuecomment-2503026347
      # https://github.com/gabime/spdlog/issues/3264
      /utf-8
  )
endif()

# GCC and Clang compiler options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
    PRIVATE
      -Wall
      -Wextra
      -Wunused

      -Wreorder
      -Wignored-qualifiers
      -Wmissing-braces
      -Wreturn-type
      -Wswitch
      -Wswitch-default
      -Wuninitialized
      -Wmissing-field-initializers

      # disable specific warnings
      #-Wno-unused-macros
      -Wno-covered-switch-default
      -Wno-unsafe-buffer-usage
      -Wno-switch-enum

      $<$<CXX_COMPILER_ID:GNU>:
        -Wmaybe-uninitialized

        $<$<VERSION_GREATER:$<CXX_COMPILER_VERSION>,4.8>:
          -Wpedantic
          -Wreturn-local-addr
        >
      >

      $<$<CXX_COMPILER_ID:Clang>:
        -Wpedantic
        # -Wreturn-stack-address # gives false positives

        # Disable c++98-compat warnings
        # https://www.emmtrix.com/wiki/Clang:Flag/-Wc%2B%2B98-compat
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic

        # https://www.emmtrix.com/wiki/Clang:Flag/-Wc%2B%2B20-extensions
        -Wno-c++20-extensions
        -Wno-c++20-designator

        -Wno-c23-extensions
      >

      $<$<BOOL:${OPTION_COVERAGE_ENABLED}>:
        -fprofile-arcs
        -ftest-coverage
      >

    PUBLIC
      $<$<PLATFORM_ID:Darwin>:
        -pthread
      >
  )
endif()

# CMake PCH + CCache options
# https://discourse.cmake.org/t/ccache-clang-and-fno-pch-timestamp/7253
# https://github.com/ccache/ccache/discussions/1218
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_SIMULATE_ID MATCHES "MSVC")
  list(APPEND CMAKE_CXX_COMPILE_OPTIONS_CREATE_PCH -Xclang -fno-pch-timestamp)
endif()

# Linker options
set(DEFAULT_LINKER_OPTIONS)

# Use pthreads on mingw and linux
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  set(DEFAULT_LINKER_OPTIONS
    PUBLIC
      ${DEFAULT_LINKER_OPTIONS}
      -pthread
  )

  if(${OPTION_COVERAGE_ENABLED})
    set(DEFAULT_LINKER_OPTIONS
      PUBLIC
        ${DEFAULT_LINKER_OPTIONS}
        -fprofile-arcs
        -ftest-coverage
    )
  endif()
endif()
