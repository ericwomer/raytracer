#include "Queue.h"

#include <vector>

/**
* @brief
*
* @param device
* @return Core::QueueFamilyIndices
*/
QueueFamilyIndices Queue::find_queue_families(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) const
{
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies) {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    if (queueFamily.queueCount > 0 && presentSupport) {
      indices.presentFamily = i;
    }

    if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }

    if (indices.is_complete()) {
      break;
    }
    i++;
  }
  return indices;
}

/**
 * @brief
 *
 */
void Queue::init(const VkDevice& device)
{
  vkGetDeviceQueue(device, familyIndicies.presentFamily.value(), 0, &presentQueue);
  vkGetDeviceQueue(device, familyIndicies.graphicsFamily.value(), 0, &graphicsQueue);
}
