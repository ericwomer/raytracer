#include <functional>
#include <utility>

#include "window.h"

Window::Window()
    : width(640)
    , height(480)
{
  SDL_Init(SDL_INIT_VIDEO);

  window = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>(SDL_CreateWindow("RayTracer",
                                                                                          SDL_WINDOWPOS_UNDEFINED,
                                                                                          SDL_WINDOWPOS_UNDEFINED,
                                                                                          width,
                                                                                          height,
                                                                                          SDL_WINDOW_VULKAN),
                                                                         SDL_DestroyWindow);

  if (window == nullptr) {
    std::runtime_error("Failed to create SDL window");
    exit;
  }
}

Window::~Window()
{
  SDL_Quit();
}
