#ifndef SURFACE_H
#define SURFACE_H

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>

class Surface {
public:
  void create(const VkInstance& instance, SDL_Window* window);
  void cleanup(const VkInstance& instance);

  const VkSurfaceKHR& get() const { return surface; }

private:
  // Surface
  VkSurfaceKHR surface;
};

#endif  // SURFACE_H
