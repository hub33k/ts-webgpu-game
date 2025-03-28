set(glm_target glm)
set(glm_dir ${DIR_VENDOR}/glm)
set(glm_vendor_dir ${DIR_ROOT}/build-vendor/glm)
set(glm_install_dir ${glm_vendor_dir}/install)

# glm/glm/detail/setup.hpp
message(STATUS "[vendor] Using GLM: 1.0.2")

add_library(${glm_target} INTERFACE)
add_library(${glm_target}::${glm_target} ALIAS ${glm_target})

target_include_directories(${glm_target}
  SYSTEM
  INTERFACE
  ${glm_install_dir}/include
)

if(WIN32)
  target_link_libraries(${glm_target}
    INTERFACE
    ${glm_install_dir}/lib/glm.lib
  )
endif()

if(APPLE)
  target_link_libraries(${glm_target}
    INTERFACE
    ${glm_install_dir}/lib/libglm.a
  )
endif()
