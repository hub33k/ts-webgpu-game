set(stb_target stb)
set(stb_vendor_dir ${DIR_ROOT}/build-vendor/stb)
set(stb_code_dir ${stb_vendor_dir}/code)

message(STATUS "[vendor] Using STB")

add_library(${stb_target} INTERFACE)
add_library(${stb_target}::${stb_target} ALIAS ${stb_target})

target_include_directories(${stb_target}
  SYSTEM
  INTERFACE
  ${stb_code_dir}
)
