#ifndef QUEUE_H
#define QUEUE_H

#include <optional>

#include <vulkan/vulkan.h>

/**
 * @class QueueFamilyIndices
 * @author Salamanderrake
 * @date 26/12/18
 * @file Queue.h
 * @brief
 */
struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool is_complete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

/**
 * @class Queue
 * @author Salamanderrake
 * @date 26/12/18
 * @file Queue.h
 * @brief
 */
class Queue {
public:
  QueueFamilyIndices find_queue_families(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) const;

  void init(const VkDevice& device);

  const VkQueue&            graphics() { return graphicsQueue; }
  const VkQueue&            present() { return presentQueue; }
  const QueueFamilyIndices& family_indicies() { return familyIndicies; }
  void                      init_family_indicies(QueueFamilyIndices& _indicies) { familyIndicies = _indicies; }

private:
  // Logical Queues
  VkQueue            graphicsQueue;
  VkQueue            presentQueue;
  QueueFamilyIndices familyIndicies;
};

#endif  // QUEUE_H
