#if !defined(WINDOW_H)
#define WINDOW_H

#include <SDL.h>
#include <vulkan/vulkan.h>
#include <memory>

#include "vk_core.h"

class Window {
  public:
  Window();
  ~Window();

  private:
  int width;
  int height;

  std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> window;
  SDL_Surface                                                   surface;
};

#endif  // WINDOW_H
