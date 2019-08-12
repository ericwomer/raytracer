#ifndef LOGICALDEVICE_H
#define LOGICALDEVICE_H

#include "Queue.h"
#include "Surface.h"
#include "PhysicalDevice.h"

#include <vulkan/vulkan.h>

class LogicalDevice {
public:
  void create(const PhysicalDevice&           physicalDevice,
              const Surface&                  surface,
              Queue&                          queue,
              const bool&                     enableValidationLayers,
              const std::vector<const char*>& deviceExtensions,
              const std::vector<const char*>& validationLayers);

  void            cleanup();
  const VkDevice& get() const { return logicalDevice; }

private:
  VkDevice logicalDevice;
};

#endif  // LOGICALDEVICE_H
