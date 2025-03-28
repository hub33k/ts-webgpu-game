# Based on https://github.com/cginternals/cmake-init

cmake_minimum_required(VERSION 3.31 FATAL_ERROR)
message(STATUS "[PROJECT] Configuring ts-webgpu-game")

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake
  ${CMAKE_SOURCE_DIR}/cmake/vendor
)

# Setup paths
get_filename_component(DIR_ROOT . ABSOLUTE)
set(DIR_CMAKE ${DIR_ROOT}/cmake)
set(DIR_DATA ${DIR_ROOT}/data)
set(DIR_SCRIPTS ${DIR_ROOT}/scripts)
set(DIR_SOURCE ${DIR_ROOT}/source)
set(DIR_TESTS ${DIR_ROOT}/tests)
set(DIR_VENDOR ${DIR_ROOT}/vendor)
set(DIR_BUILD_VENDOR ${DIR_ROOT}/build-vendor)
set(DIR_BUILD ${CMAKE_BINARY_DIR})

# Project description and (meta) information
# ================================================================

# Get git revision
execute_process(
  COMMAND git rev-parse HEAD
  WORKING_DIRECTORY ${DIR_ROOT}
  OUTPUT_VARIABLE GIT_SHA1
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)
string(SUBSTRING "${GIT_SHA1}" 0 12 GIT_REV)
if(NOT GIT_REV)
  set(GIT_REV "0")
endif()

# Meta information about the project
set(META_PROJECT_NAME        "ts-webgpu-game")
set(META_PROJECT_DESCRIPTION "ts-webgpu-game")
set(META_AUTHOR_ORGANIZATION "hub33k")
set(META_AUTHOR_DOMAIN       "https://github.com/hub33k/ts-webgpu-game")
set(META_AUTHOR_MAINTAINER   "hub33k@hub33k.pl")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "0")
set(META_VERSION_REVISION    "${GIT_REV}")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
# set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")
set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION}")
set(META_CMAKE_INIT_SHA      "${GIT_REV}")

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)
# string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_NORMALIZED)
# TODO (hub33k): add META_PROJECT_NORMALIZED - eg. if META_PROJECT_NAME is "Game Sandbox Two" then META_PROJECT_NORMALIZED is "GameSandboxTwo"

# PreConfigure
# ================================================================

include(Utils)
include(FetchContent)
include(PreConfig)

# Project
# ================================================================

# Generate folders for IDE targets (e.g., VisualStudio solutions)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(IDE_FOLDER "")

# Declare project
project(${META_PROJECT_NAME}
  VERSION ${META_VERSION}
  DESCRIPTION ${META_PROJECT_DESCRIPTION}
  LANGUAGES C CXX
)

# Configuration
# ================================================================

include(Config)
include(CompileOptions)

# Project modules and targets
# ================================================================

add_subdirectory(source)
