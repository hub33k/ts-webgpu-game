#include "Platform/WebGPU/WebGPU.hpp"
#include <webgpu/webgpu_cpp.h>

wgpu::Instance instance;
wgpu::Adapter adapter;
wgpu::Device device;
wgpu::RenderPipeline pipeline;
wgpu::Surface surface;
wgpu::TextureFormat format;

const uint32_t kWidth = 512;
const uint32_t kHeight = 512;

void ConfigureSurface() {
  wgpu::SurfaceCapabilities capabilities;
  surface.GetCapabilities(adapter, &capabilities);
  format = capabilities.formats[0];

  wgpu::SurfaceConfiguration config{
    .device = device,
    .format = format,
    .viewFormats = &format,
    .width = kWidth,
    .height = kHeight,
  };
  surface.Configure(&config);
}

#if HK_PLATFORM_IS(EMSCRIPTEN)
void GetAdapter(void (*callback)(wgpu::Adapter)) {
  instance.RequestAdapter(
    nullptr,
    // TODO(https://bugs.chromium.org/p/dawn/issues/detail?id=1892): Use
    // wgpu::RequestAdapterStatus and wgpu::Adapter.
    [](WGPURequestAdapterStatus status, WGPUAdapter cAdapter, const char *message, void *userdata) {
      if (message) {
        printf("RequestAdapter: %s\n", message);
      }
      if (status != WGPURequestAdapterStatus_Success) {
        exit(0);
      }
      wgpu::Adapter adapter = wgpu::Adapter::Acquire(cAdapter);
      reinterpret_cast<void (*)(wgpu::Adapter)>(userdata)(adapter);
    },
    reinterpret_cast<void *>(callback)
  );
}
#endif

void mainloop(void *arg) {
  SDL_Renderer *renderer = (SDL_Renderer *)arg;
  SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255); // Set color to blue
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

int main() {
  instance = WebGPU::CreateInstance();
  std::cout << "Instance: " << instance.Get() << std::endl;

  std::cout << "Hello, World!\n";

  HK_LOG_CORE_INFO("INFO CORE");
  HK_LOG_INFO("INFO");

#if HK_PLATFORM_IS(EMSCRIPTEN)
  std::cout << "EMSCRIPTEN" << std::endl;
#endif

  std::println("YO println");

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow("SDL3 + Emscripten Example", kWidth, kHeight, SDL_WINDOW_HIDDEN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

#if HK_PLATFORM_IS(EMSCRIPTEN)
  emscripten_set_main_loop_arg(mainloop, renderer, 0, 1);
#else
  bool isRunning = true;
  SDL_ShowWindow(window);
  while (isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        isRunning = false;
      }
      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
          isRunning = false;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255); // Set color to blue
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }
#endif

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  std::cout << "Bye, World!\n";

  return 0;
}
