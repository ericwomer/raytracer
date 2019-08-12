#include "PhysicalDevice.h"
#include "SwapChain.h"

#include <vector>
#include <set>
#include <string>

/**
* @brief
*
*/
void PhysicalDevice::pick(const VkInstance*              _instance,
                          Queue*                         _queue,
                          const SwapChain&               swapchain,
                          const VkSurfaceKHR&            _surface,
                          const std::vector<const char*> _deviceExtensions)
{
  instance             = *_instance;
  queue                = _queue;
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

  if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support.");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  for (const auto& device : devices) {
    if (is_device_suitable(device, _surface, _deviceExtensions, swapchain)) {
      physicalDevice = device;
      msaaSamples    = get_max_usable_sample_count(physicalDevice);
    }
  }

  if (physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a sutible GPU!");
  }
}

/**
* @brief
*
* @param device
* @return true
* @return false
*/
bool PhysicalDevice::is_device_suitable(VkPhysicalDevice               device,
                                        const VkSurfaceKHR&            _surface,
                                        const std::vector<const char*> _deviceExtensions,
                                        const SwapChain&               swapchain)
{
  QueueFamilyIndices indicies           = queue->find_queue_families(device, _surface);
  bool               extensionSupported = check_device_extension_support(device, _deviceExtensions);
  bool               swapChainAdequate  = false;

  if (extensionSupported) {
    SwapChainSupportDetails swapChainSupport = swapchain.query_swap_chain_support(device, _surface);
    swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
  }

  VkPhysicalDeviceFeatures supportedFeatures;
  vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

  return indicies.is_complete() && extensionSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

/**
* @brief
*
* @param device
* @return true
* @return false
*/
bool PhysicalDevice::check_device_extension_support(VkPhysicalDevice               device,
                                                    const std::vector<const char*> deviceExtensions)
{
  uint32_t extensionCount = 0;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

  std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

  for (const auto& extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

/**
 * @brief
 * @return
 */
VkSampleCountFlagBits PhysicalDevice::get_max_usable_sample_count(VkPhysicalDevice& physicalDevice)
{
  VkPhysicalDeviceProperties physicalDeviceProperties;
  vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

  VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts,
                                       physicalDeviceProperties.limits.framebufferDepthSampleCounts);

  if (counts & VK_SAMPLE_COUNT_64_BIT) {
    return VK_SAMPLE_COUNT_64_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_32_BIT) {
    return VK_SAMPLE_COUNT_32_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_16_BIT) {
    return VK_SAMPLE_COUNT_16_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_8_BIT) {
    return VK_SAMPLE_COUNT_8_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_4_BIT) {
    return VK_SAMPLE_COUNT_4_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_2_BIT) {
    return VK_SAMPLE_COUNT_2_BIT;
  }
  return VK_SAMPLE_COUNT_1_BIT;
}
