#pragma once

namespace WebGPU {

  wgpu::Instance CreateInstance(const std::optional<wgpu::InstanceDescriptor> &descriptor = std::nullopt);

  wgpu::Adapter CreateAdapter(
    const wgpu::Instance &instance, const wgpu::Surface &surface = nullptr,
    const std::optional<wgpu::RequestAdapterOptions> &requestAdapterOptions = std::nullopt
  );

  wgpu::Device CreateDevice(
    const wgpu::Instance &instance, const wgpu::Adapter &adapter,
    const std::optional<wgpu::DeviceDescriptor> &deviceDescriptor = std::nullopt
  );

  wgpu::Surface CreateSurface(const wgpu::Instance &instance, SDL_Window *window);

  wgpu::SurfaceConfiguration CreateSurfaceConfiguration(
    int width, int height, bool vsync, const wgpu::Surface &surface, const wgpu::Adapter &adapter,
    const wgpu::Device &device, wgpu::TextureFormat &preferredSurfaceTextureFormat
  );

  wgpu::SurfaceConfiguration CreateSurfaceConfiguration(
    const wgpu::Surface &surface, const wgpu::Adapter &adapter, wgpu::TextureFormat &preferredSurfaceTextureFormat,
    const wgpu::SurfaceConfiguration &surfaceConfiguration
  );

  // ================================================================

  void PrintAdapterInfo(const wgpu::Adapter &adapter);

  // ================================================================

  wgpu::ShaderModule CreateShaderModule(const wgpu::Device &device, const std::string &source);

  wgpu::Buffer
  CreateBufferFromData(const wgpu::Device &device, const void *data, uint64_t size, wgpu::BufferUsage usage);

} // namespace WebGPU
