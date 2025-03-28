find_package(Threads REQUIRED)

set(spdlog_target spdlog_header_only)
set(spdlog_vendor_dir ${DIR_ROOT}/build-vendor/spdlog)
set(spdlog_install_dir ${spdlog_vendor_dir}/install)

# TODO (hub33k): check if spdlog uses C++ std::format (SPDLOG_USE_STD_FORMAT) instead of fmt lib

# vendor\spdlog\include\spdlog\version.h
message(STATUS "[vendor] Using spdlog: 1.15.1")

# spdlog
# ================================================================

add_library(spdlog INTERFACE)
add_library(spdlog::spdlog ALIAS spdlog)
target_include_directories(spdlog
  SYSTEM
  INTERFACE
  ${spdlog_install_dir}/include
)

if(WIN32)
  target_link_libraries(spdlog
    INTERFACE
    ${spdlog_install_dir}/lib/spdlog.lib
  )
endif()

if(APPLE)
  target_link_libraries(spdlog
    INTERFACE
    ${spdlog_install_dir}/lib/libspdlog.a
  )
endif()

# spdlog_header_only
# ================================================================

add_library(spdlog_header_only INTERFACE)
add_library(spdlog::spdlog_header_only ALIAS spdlog_header_only)

target_include_directories(spdlog_header_only
  SYSTEM
  INTERFACE "$<BUILD_INTERFACE:${spdlog_install_dir}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_link_libraries(spdlog_header_only INTERFACE Threads::Threads)

# target_compile_definitions(spdlog_header_only INTERFACE SPDLOG_FMT_EXTERNAL)
# target_link_libraries(spdlog_header_only INTERFACE fmt::fmt-header-only)
# target_link_libraries(spdlog_header_only INTERFACE fmt::fmt)
