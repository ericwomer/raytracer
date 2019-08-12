#ifndef PHYSICAL_DEVICE_H
#define PHYSICAL_DEVICE_H

#include "Queue.h"

#include <vulkan/vulkan.h>

class SwapChain;
typedef struct SDL_Window;

class PhysicalDevice {
public:
  void pick(const VkInstance*              _instance,
            Queue*                         _queue,
            const SwapChain&               swapchain,
            const VkSurfaceKHR&            _surface,
            const std::vector<const char*> _deviceExtensions);

  void                         cleanup();
  const VkPhysicalDevice&      get() const { return physicalDevice; }
  const VkSampleCountFlagBits& get_multisampling() const { return msaaSamples; }
  void                         set_queue(Queue* _queue) { queue = _queue; }

private:
  void     pick_physical_device();
  bool     check_device_extension_support(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions);
  bool     is_device_suitable(VkPhysicalDevice               device,
                              const VkSurfaceKHR&            _surface,
                              const std::vector<const char*> _deviceExtensions,
                              const SwapChain&               swapchain);
  uint32_t find_memory_type(VkPhysicalDevice& physicalDevice, uint32_t& typeFilter, VkMemoryPropertyFlags& properties);
  VkSampleCountFlagBits get_max_usable_sample_count(VkPhysicalDevice& physicalDevice);

  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkInstance       instance;  // = VK_NULL_HANDLE;

  VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

  Queue* queue;
};

#endif  // PHYSICAL_DEVICE_H
