#include "LogicalDevice.h"

#include <vector>
#include <set>

void LogicalDevice::create(const PhysicalDevice&           physicalDevice,
                           const Surface&                  surface,
                           Queue&                          queue,
                           const bool&                     enableValidationLayers,
                           const std::vector<const char*>& deviceExtensions,
                           const std::vector<const char*>& validationLayers)
{
  // Refactor this out?
  QueueFamilyIndices familyIndicies = queue.find_queue_families(physicalDevice.get(), surface.get());
  queue.init_family_indicies(familyIndicies);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t>                   uniqueQueueFamilies = {familyIndicies.graphicsFamily.value(),
                                            familyIndicies.presentFamily.value()};

  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex        = queueFamily;
    queueCreateInfo.queueCount              = 1;
    queueCreateInfo.pQueuePriorities        = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures = {};
  deviceFeatures.samplerAnisotropy        = VK_TRUE;
  deviceFeatures.sampleRateShading        = VK_TRUE;

  VkDeviceCreateInfo createInfo      = {};
  createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount    = queueCreateInfos.size();
  createInfo.pQueueCreateInfos       = queueCreateInfos.data();
  createInfo.pEnabledFeatures        = &deviceFeatures;
  createInfo.enabledExtensionCount   = static_cast<uint32_t>(deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions.data();

  if (enableValidationLayers) {
    createInfo.enabledLayerCount   = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(physicalDevice.get(), &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device!");
  }
}

void LogicalDevice::cleanup()
{
  if (logicalDevice != VK_NULL_HANDLE) {
    vkDeviceWaitIdle(logicalDevice);
  }  // Enter destruction code after here.
}
