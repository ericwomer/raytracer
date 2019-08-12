#if !defined(WINDOW_H)
#define WINDOW_H

#include <SDL.h>
#include <vulkan/vulkan.h>
#include <memory>

#include "vk_core.h"

class Window {
  public:
  void init();
  void cleanup();

  bool rendering_loop();

  private:
  int width  = 640;
  int height = 480;

  SDL_Window* window = nullptr;
  Core        core;
};

#endif  // WINDOW_H
