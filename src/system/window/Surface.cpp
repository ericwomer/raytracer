#include "Surface.h"

#include <SDL2/SDL_vulkan.h>

#include <stdexcept>

/**
 * @brief
 *
 */
void Surface::create(const VkInstance& instance, SDL_Window* window)
{
  // Handle errors
  if (!SDL_Vulkan_CreateSurface(window, instance, &surface)) {
    throw std::runtime_error("Failed to create Vulkan Surface!");
  }
}

void Surface::cleanup(const VkInstance& instance)
{
  vkDestroySurfaceKHR(instance, surface, nullptr);
}
