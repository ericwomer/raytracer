#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <vulkan/vulkan.h>

class ImageView {
public:
  void create(const VkDevice&    logicalDevice,
              VkImage            image,
              VkFormat           format,
              VkImageAspectFlags aspectFlags,
              uint32_t           mipLevels);

  void              cleanup(const VkDevice& logicalDevice);
  const VkImageView get() { return imageView; }

private:
  VkImageView imageView;
};

#endif  // IMAGEVIEW_H
