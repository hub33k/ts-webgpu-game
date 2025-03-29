#include "WebGPU.hpp"

// https://github.com/emscripten-core/emscripten/blob/main/system/include/webgpu/webgpu_cpp.h
// https://github.com/emscripten-core/emscripten/blob/main/system/lib/webgpu/webgpu.cpp
// https://github.com/emscripten-core/emscripten/blob/main/system/lib/webgpu/webgpu_cpp.cpp

namespace WebGPU {

  wgpu::Instance CreateInstance(const std::optional<wgpu::InstanceDescriptor> &instanceDescriptor) {
    HK_PROFILE_FUNCTION();

#if HK_PLATFORM_IS(EMSCRIPTEN)
    (void)instanceDescriptor;

    // NOTE (hub33k): instanceDescriptor is not supported yet
    wgpu::Instance instance = wgpu::CreateInstance(nullptr);
#else
    wgpu::InstanceDescriptor descriptor = {};

    if (!instanceDescriptor) {
      descriptor = {
        .nextInChain = nullptr,
        .capabilities =
          {
            .timedWaitAnyEnable = true,
          },
      };
    } else {
      descriptor = *instanceDescriptor;

      // NOTE (hub33k): this must be set, so Adapter and Device can be fetched
      // if not set - Error: Timeout waits are either not enabled or not supported.
      descriptor.capabilities.timedWaitAnyEnable = true;
    }

    wgpu::Instance instance = wgpu::CreateInstance(&descriptor);
#endif

    HK_ASSERT(instance, "Failed to create wgpu::Instance")
    return instance;
  }

} // namespace WebGPU
