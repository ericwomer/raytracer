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

  SDL_Window* window;
};

#endif  // WINDOW_H
