#include "SwapChain.h"

#include <SDL2/SDL_vulkan.h>

bool SwapChain::create(const VkPhysicalDevice& physicalDevice,
                       const VkDevice&         logicalDevice,
                       const VkSurfaceKHR&     surface,
                       const Queue&            queue,
                       SDL_Window*             window)
{
  bool canRender = false;

  SwapChainSupportDetails swapChainSupport = query_swap_chain_support(physicalDevice, surface);
  VkSurfaceFormatKHR      surfaceFomat     = choose_swap_surface_format(swapChainSupport.formats);
  VkPresentModeKHR        presentMode      = choose_swap_present_mode(swapChainSupport.presentModes);
  VkExtent2D              extent           = choose_swap_extent(swapChainSupport.capabilities, window);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  if ((extent.width == 0) || (extent.height == 0)) {
    return false;
  }

  VkSwapchainCreateInfoKHR createInfo = {};
  createInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface                  = surface;
  createInfo.minImageCount            = imageCount;
  createInfo.imageFormat              = surfaceFomat.format;
  createInfo.imageColorSpace          = surfaceFomat.colorSpace;
  createInfo.imageExtent              = extent;
  createInfo.imageArrayLayers         = 1;
  createInfo.imageUsage               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indicies              = queue.find_queue_families(physicalDevice, surface);
  uint32_t           queueFamilyIndicies[] = {indicies.graphicsFamily.value(), indicies.presentFamily.value()};

  if (indicies.graphicsFamily != indicies.presentFamily) {
    createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices   = queueFamilyIndicies;
  } else {
    createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;        // optional?
    createInfo.pQueueFamilyIndices   = nullptr;  // optional?
  }

  createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode    = presentMode;
  createInfo.clipped        = VK_TRUE;

  if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create swap chain!");
  }

  canRender = true;

  vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, nullptr);
  swapchainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, swapchainImages.data());

  swapchainImageFormat = surfaceFomat.format;
  swapchainExtent      = extent;
  return canRender;
}

void SwapChain::cleanup(const VkDevice& logicalDevice)
{
  vkDestroySwapchainKHR(logicalDevice, swapchain, nullptr);
}

/**
 * @brief
 * @param device
 * @return
 */
SwapChainSupportDetails SwapChain::query_swap_chain_support(const VkPhysicalDevice& device,
                                                            const VkSurfaceKHR      surface) const
{
  SwapChainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

  uint32_t formatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
  }

  uint32_t presentModeCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

/**
* @brief
*
* @param availableFormats
* @return VkSurfaceFormatKHR
*/
VkSurfaceFormatKHR SwapChain::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
  if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
    return {VK_FORMAT_B8G8R8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }

  for (const auto& availableFomat : availableFormats) {
    if (availableFomat.format == VK_FORMAT_B8G8R8_UNORM &&
        availableFomat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFomat;
    }
  }

  return availableFormats[0];
}

/**
* @brief
*
* @param availablePresentMode
* @return VkPresentModeKHR
*/
VkPresentModeKHR SwapChain::choose_swap_present_mode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
  VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
      bestMode = availablePresentMode;
    }
  }

  return bestMode;
}

/**
 * @brief
 * @param capabilities
 * @return
 */
VkExtent2D SwapChain::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities, SDL_Window* window)
{
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    int width;
    int height;
    SDL_Vulkan_GetDrawableSize(window, &width, &height);
    VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    actualExtent.width =
        std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height =
        std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
    return actualExtent;
  }
}

/**
 * @brief
 *
 */
bool SwapChain::create_image_views(const VkDevice& logicalDevice)
{
  bool canRender = false;
  resize_image_views(images().size());

  for (size_t i = 0; i < images().size(); i++) {
    swapchainImageViews[i].create(logicalDevice, images()[i], image_format(), VK_IMAGE_ASPECT_COLOR_BIT, 1);
    canRender = true;
  }
  return canRender;
}

void SwapChain::destroy_image_views(const VkDevice& logicalDevice)
{
  for (auto imageView : swapchainImageViews) {
    imageView.cleanup(logicalDevice);
  }
}
