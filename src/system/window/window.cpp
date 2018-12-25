#include <functional>
#include <utility>

#include "window.h"

Window::Window()
    : width(640)
    , height(480)
{
  SDL_Init(SDL_INIT_VIDEO);
  window =
      SDL_CreateWindow("RayTracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_VULKAN);

  if (window == nullptr) {
    std::runtime_error("Failed to create SDL window");
    exit;
  }
  Core core(window);
  // core.init(window);
}

Window::~Window()
{
  SDL_DestroyWindow(window);
  SDL_Quit();
}
