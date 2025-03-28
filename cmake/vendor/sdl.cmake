set(sdl_target SDL3)
set(sdl_vendor_dir ${DIR_ROOT}/build-vendor/sdl)
set(sdl_install_dir ${sdl_vendor_dir}/install)

# vendor\SDL\include\SDL3\SDL_version.h
message(STATUS "[vendor] Using SDL: SDL3-3.1.7-preview-3.1.6-162-gec2ef5f9b")

add_library(${sdl_target} INTERFACE)
add_library(${sdl_target}::${sdl_target} ALIAS ${sdl_target})

target_include_directories(${sdl_target}
  SYSTEM
  INTERFACE
  ${sdl_install_dir}/include
)

if(WIN32)
  target_link_libraries(${sdl_target}
    INTERFACE
    ${sdl_install_dir}/lib/SDL3-static.lib
    winmm      # For timeBeginPeriod, timeEndPeriod
    setupapi   # For SetupDi* functions
    imm32      # For Imm* functions
    cfgmgr32   # For CM_* functions
    version    # Sometimes needed for Windows version checking
  )
endif()

if(APPLE)
  target_link_libraries(${sdl_target}
    INTERFACE
      ${sdl_install_dir}/lib/libSDL3.0.dylib
      # ${sdl_install_dir}/lib/libSDL3.a
      "-framework IOKit"
      "-framework Cocoa"
      "-framework OpenGL"
  )
endif()

function(setup_sdl target)
  set(sdl_target SDL3)
  set(sdl_vendor_dir ${DIR_ROOT}/build-vendor/SDL)
  set(sdl_install_dir ${sdl_vendor_dir}/install)

  # if(WIN32)
  #   add_custom_command(
  #     TARGET ${target} POST_BUILD
  #     COMMAND ${CMAKE_COMMAND} -E copy_if_different
  #     "${sdl_install_dir}/lib/sdl3.dll"
  #     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  #     # "$<TARGET_FILE_DIR:${target}>"
  #   )
  # endif()

  if(APPLE)
    # copy dll file to bin dir
    add_custom_command(
      TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${sdl_install_dir}/lib/libSDL3.0.dylib"
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      # "$<TARGET_FILE_DIR:${target}>"
    )
  endif()
endfunction(setup_sdl)
