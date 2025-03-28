set(dawn_target dawn)
set(dawn_vendor_dir ${DIR_ROOT}/build-vendor/dawn)
set(dawn_install_dir ${dawn_vendor_dir}/install)

# TODO (hub33k): build as static library

# vendor\dawn\CMakeLists.txt
message(STATUS "[vendor] Using Dawn (WebGPU): 0.0.0")

add_library(${dawn_target} INTERFACE)
add_library(${dawn_target}::${dawn_target} ALIAS ${dawn_target})

target_include_directories(${dawn_target}
  SYSTEM
  INTERFACE
  ${dawn_install_dir}/include

  # "${dawn_vendor_dir}/code"
  # "${dawn_vendor_dir}/code/src"
  # "${dawn_vendor_dir}/code/src/dawn/common"
  # "${dawn_vendor_dir}/code/src/dawn/utils"
)

target_compile_definitions(${dawn_target}
  INTERFACE
  WGPU_GLFW_IMPLEMENTATION
  WGPU_GLFW_SHARED_LIBRARY
)

if(WIN32)
  target_link_libraries(${dawn_target}
    INTERFACE
    ${dawn_install_dir}/lib/webgpu_dawn.lib

    # glfw
    ${dawn_vendor_dir}/build/src/dawn/common/dawn_common.lib
    ${dawn_vendor_dir}/build/src/dawn/glfw/dawn_glfw.lib

    # extra
    # ${dawn_vendor_dir}/build/src/dawn/utils/dawn_wgpu_utils.lib
    # ${dawn_vendor_dir}/build/src/dawn/utils/dawn_system_utils.lib
  )
endif()

if(APPLE)
  target_link_libraries(${dawn_target}
    INTERFACE
    ${dawn_install_dir}/lib/libwebgpu_dawn.dylib

    # glfw
    ${dawn_vendor_dir}/build/src/dawn/common/libdawn_common.a
    ${dawn_vendor_dir}/build/src/dawn/glfw/libdawn_glfw.a

    # extra
    # ${dawn_vendor_dir}/build/src/dawn/utils/dawn_wgpu_utils.lib
    # ${dawn_vendor_dir}/build/src/dawn/utils/dawn_system_utils.lib
  )
endif()

configure_file(
  "${dawn_vendor_dir}/code/include/webgpu/webgpu_glfw.h"
  "${dawn_install_dir}/include/webgpu"
  COPYONLY
)

function(setup_dawn target)
  set(dawn_target dawn)
  set(dawn_vendor_dir ${DIR_ROOT}/build-vendor/dawn)
  set(dawn_install_dir ${dawn_vendor_dir}/install)

  if(WIN32)
    add_custom_command(
      TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${dawn_install_dir}/bin/webgpu_dawn.dll"
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      # "$<TARGET_FILE_DIR:${target}>"
    )
  endif()

  if(APPLE)
    # copy dll file to bin dir
    add_custom_command(
      TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${dawn_install_dir}/lib/libwebgpu_dawn.dylib"
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      # "$<TARGET_FILE_DIR:${target}>"
    )
  endif()
endfunction(setup_dawn)
