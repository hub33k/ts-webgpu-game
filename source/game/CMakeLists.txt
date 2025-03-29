set(target game)
set(target_dir ${DIR_SOURCE}/${target})

message(STATUS "[Executable] ${target}")

# Set subsystem (for Visual Studio and Windows)
# https://discourse.cmake.org/t/controlling-visual-studio-options-from-cmake/6581/3
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

file(GLOB_RECURSE ALL_SOURCES_GLOB "source/**.cpp")
file(GLOB_RECURSE ALL_HEADERS_GLOB "source/*.hpp" "source/*.h")
set(ALL_SOURCES ${ALL_SOURCES_GLOB} ${ALL_HEADERS_GLOB})

set(sources
  source/main.cpp

  source/Platform/WebGPU/WebGPU.cpp
)

if(EMSCRIPTEN)
  list(APPEND sources
    # source/Platform/Emscripten/Emscripten.cpp
  )
else()
  list(APPEND sources
    # source/Platform/WebGPU/WebGPU.cpp
  )
endif()

add_executable(${target}
  # MACOSX_BUNDLE
  ${sources}
)

# Project options
set_target_properties(${target}
  PROPERTIES
  ${DEFAULT_PROJECT_OPTIONS}
  INSTALL_RPATH "${EXECUTABLE_INSTALL_RPATH}"
  FOLDER "${IDE_FOLDER}"
)

# Include directories
target_include_directories(${target}
  PRIVATE
  ${DEFAULT_INCLUDE_DIRECTORIES}
  ${target_dir}/source
)

# Libraries
target_link_libraries(${target}
  PRIVATE
  ${DEFAULT_LIBRARIES}

  entt
  glm
  SDL3
  spdlog_header_only
  stb

  hub33k
)

# Compile definitions
target_compile_definitions(${target}
  PRIVATE
  ${DEFAULT_COMPILE_DEFINITIONS}
)

# Compile options
target_compile_options(${target}
  PRIVATE
  ${DEFAULT_COMPILE_OPTIONS}
)

target_compile_features(${target}
  PRIVATE
)

# Linker options
target_link_libraries(${target}
  PRIVATE
  ${DEFAULT_LINKER_OPTIONS}
)

# PCH - Precompiled Headers
target_precompile_headers(${target}
  PRIVATE
  ${target_dir}/source/GamePCH.hpp
)

# Unity build

# Libs

# Emscripten
if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set_target_properties(${target} PROPERTIES SUFFIX ".html")

  target_link_options(${target} PRIVATE
    -sUSE_WEBGPU=1 # Handle WebGPU symbols
    # -sUSE_GLFW=3
    -sUSE_SDL=3
    -sASYNCIFY # Required by WebGPU-C++
    # -sINITIAL_MEMORY=1GB
    -sALLOW_MEMORY_GROWTH=1
    # -sTOTAL_STACK=512MB
    # -sUSE_PTHREADS=1
    --preload-file "${DIR_DATA}"
  )
endif()

# Not Emscripten
if(NOT EMSCRIPTEN)
  target_link_libraries(${target}
    PRIVATE
    dawn
    sdl3webgpu
  )

  setup_dawn(${target})
endif()
