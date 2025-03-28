set(entt_target entt)
set(entt_dir ${DIR_VENDOR}/entt)
set(entt_vendor_dir ${DIR_ROOT}/build-vendor/entt)
set(entt_install_dir ${entt_vendor_dir}/install)
set(entt_code_dir ${entt_vendor_dir}/code)

message(STATUS "[vendor] Using entt: unknown")

add_library(${entt_target} INTERFACE)
add_library(${entt_target}::${entt_target} ALIAS ${entt_target})

target_include_directories(${entt_target}
  SYSTEM
  INTERFACE
  ${entt_code_dir}/src
)

if(WIN32)
  target_link_libraries(${entt_target}
    INTERFACE
    # ${entt_install_dir}/lib/glm.lib
  )
endif()

if(APPLE)
  target_link_libraries(${entt_target}
    INTERFACE
    # ${entt_install_dir}/lib/libglm.a
  )
endif()
