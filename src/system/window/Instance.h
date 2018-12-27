#if !defined(INSTANCE_H)
#define INSTANCE_H

#include <vulkan/vulkan.h>

#include <vector>

class Instance {
public:
  void              create();
  void              cleanup();
  const VkInstance& get() const { return instance; }

private:
  VkInstance instance;
};

#endif  // INSTANCE_H
