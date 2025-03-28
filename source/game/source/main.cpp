void mainloop(void *arg) {
  SDL_Renderer *renderer = (SDL_Renderer *)arg;
  SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255); // Set color to blue
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

int main() {
  std::cout << "Hello, World!\n";

  HK_LOG_CORE_INFO("INFO CORE");
  HK_LOG_INFO("INFO");

#if HK_PLATFORM_IS(EMSCRIPTEN)
  std::cout << "EMSCRIPTEN" << std::endl;
#endif

  std::println("YO println");

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow("SDL3 + Emscripten Example", 800, 600, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

#if HK_PLATFORM_IS(EMSCRIPTEN)
  emscripten_set_main_loop_arg(mainloop, renderer, 0, 1);
#else
  bool isRunning = true;
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
