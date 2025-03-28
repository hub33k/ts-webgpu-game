set(target hub33k)
set(target_dir ${DIR_SOURCE}/${target})

message(STATUS "[Library] ${target}")

# Set API export file and macro
string(MAKE_C_IDENTIFIER ${target} target_id)
string(TOUPPER ${target_id} target_id)
set(feature_file         "include/${target}/${target}_features.h")
set(export_file          "include/${target}/${target}_export.h")
set(template_export_file "include/${target}/${target}_api.h")
set(export_macro         "${target_id}_API")

# Sources
set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

# file(GLOB_RECURSE headers_glob
#   ${source_path}/**.hpp
#   ${source_path}/**.h
#   ${include_path}/**.hpp
#   ${include_path}/**.h
# )
# file(GLOB_RECURSE sources_glob
#   ${source_path}/*.cpp
#   ${source_path}/*.c
# )
# TODO (hub33k): make this work on mac
# if(APPLE)
#   file(GLOB_RECURSE sources_glob
#     ${source_path}/**.mm
#   )
# endif()
# set(headers
#   # ${include_path}/Game.hpp
#   ${headers_glob}
# )
# set(sources
#   # ${source_path}/Game.cpp
#   ${sources_glob}
# )
# set(sources_all
#   ${headers_glob}
#   ${sources_glob}
# )

set(headers
  source/hub33k.hpp
  # source/hub33kPCH.hpp

  source/hub33k/Core/Assert.hpp
  source/hub33k/Core/Base.hpp
  source/hub33k/Core/Defines.hpp
  source/hub33k/Core/Log.hpp
  source/hub33k/Core/PlatformDetection.hpp
  source/hub33k/Core/Timer.hpp
  source/hub33k/Core/Timestep.hpp
  source/hub33k/Core/Utils.hpp

  source/hub33k/Debug/Instrumentor.hpp
)

set(sources
  source/hub33k/Core/Utils.cpp

  source/hub33k/Utils/BumpAllocator.cpp
  source/hub33k/Utils/FileIO.cpp
  source/hub33k/Utils/Math.cpp
)

if(NOT EMSCRIPTEN)
  list(APPEND sources
    source/hub33k/Core/Log.cpp
  )
endif()

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$"
  ${header_group} ${headers}
)
source_group_by_path(${source_path}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$"
  ${source_group} ${sources}
)

# Create executable
add_library(${target} STATIC
  ${headers}
  ${sources}
)
# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})

# Create feature detection header
# Compilers: https://cmake.org/cmake/help/v3.1/variable/CMAKE_LANG_COMPILER_ID.html#variable:CMAKE_%3CLANG%3E_COMPILER_ID
# Feature: https://cmake.org/cmake/help/v3.1/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html

# Create API export header
# TODO (hub33k): generate export header
# generate_export_header(${target}
#   EXPORT_FILE_NAME  ${export_file}
#   EXPORT_MACRO_NAME ${export_macro}
# )
# generate_template_export_header(${target}
#   ${target_id}
#   ${template_export_file}
# )

# Project options
set_target_properties(${target}
  PROPERTIES
  ${DEFAULT_PROJECT_OPTIONS}
  INSTALL_RPATH "${LIBRARY_INSTALL_RPATH}"
  FOLDER "${IDE_FOLDER}"
  VERSION "${META_VERSION}"
  SOVERSION "${META_VERSION_MAJOR}"
)

# Include directories
target_include_directories(${target}
  PRIVATE
  ${PROJECT_BINARY_DIR}/source/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include

  PUBLIC
  ${DEFAULT_INCLUDE_DIRECTORIES}
  ${target_dir}/source # added

  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Libraries
target_link_libraries(${target}
  PRIVATE
  spdlog

  PUBLIC
  ${DEFAULT_LIBRARIES}
)

# Libraries config
# mark_library_as_system(spdlog_header_only)

# Compile definitions
target_compile_definitions(${target}
  PRIVATE

  PUBLIC
  $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${target_id}_STATIC_DEFINE>
  ${DEFAULT_COMPILE_DEFINITIONS}
)

# Compile options
target_compile_options(${target}
  PRIVATE

  PUBLIC
  ${DEFAULT_COMPILE_OPTIONS}
)

# Linker options
target_link_libraries(${target}
  PRIVATE

  PUBLIC
  ${DEFAULT_LINKER_OPTIONS}
)

# PCH - Precompiled Headers
# target_precompile_headers(${target}
#   PRIVATE
#   ${target_dir}/source/hub33kPCH.hpp
# )

# Tests
# add_subdirectory(tests)
