#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "Queue.h"
#include "ImageView.h"

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

struct SDL_Window;

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR        capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR>   presentModes;
};

class SwapChain {
public:
  bool create(const VkPhysicalDevice& physicalDevice,
              const VkDevice&         logicalDevice,
              const VkSurfaceKHR&     surface,
              const Queue&            queue,
              SDL_Window*             window);
  void cleanup(const VkDevice& logicalDevice);

  const VkSwapchainKHR&   get() { return swapchain; }
  SwapChainSupportDetails query_swap_chain_support(const VkPhysicalDevice& device, const VkSurfaceKHR surface) const;

  const std::vector<VkImage>& images() const { return swapchainImages; }
  const VkFormat&             image_format() const { return swapchainImageFormat; }

  // const std::vector<ImageView>& image_views() const { return swapchainImageViews; }
  void resize_image_views(size_t size) { swapchainImageViews.resize(size); }
  // void add_image_view(VkImageView imageView, size_t pos) { swapchainImageViews[pos] = imageView; }

  const VkExtent2D&                 extent() const { return swapchainExtent; }
  const std::vector<VkFramebuffer>& framebuffers() const { return swapchainFramebuffers; }
  bool                              create_image_views(const VkDevice& logicalDevice);
  void                              destroy_image_views(const VkDevice& logicalDevice);

private:
  VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR   choose_swap_present_mode(const std::vector<VkPresentModeKHR> availablePresentModes);
  VkExtent2D         choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities, SDL_Window* window);

  VkSwapchainKHR swapchain = VK_NULL_HANDLE;

  std::vector<VkImage>   swapchainImages;
  VkFormat               swapchainImageFormat;
  std::vector<ImageView> swapchainImageViews;

  VkExtent2D                 swapchainExtent;
  std::vector<VkFramebuffer> swapchainFramebuffers;
};

#endif  // SWAPCHAIN_H
