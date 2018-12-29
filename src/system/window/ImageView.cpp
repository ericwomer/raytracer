#include "ImageView.h"

#include <stdexcept>

/**
 * @brief
 * @param image
 * @param format
 * @return
 */
void ImageView::create(const VkDevice&    logicalDevice,
                       VkImage            image,
                       VkFormat           format,
                       VkImageAspectFlags aspectFlags,
                       uint32_t           mipLevels)
{
  VkImageViewCreateInfo viewInfo           = {};
  viewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image                           = image;
  viewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format                          = format;
  viewInfo.subresourceRange.aspectMask     = aspectFlags;
  viewInfo.subresourceRange.baseMipLevel   = 0;
  viewInfo.subresourceRange.levelCount     = mipLevels;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount     = 1;

  if (vkCreateImageView(logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create texture image view!");
  }
}

void ImageView::cleanup(const VkDevice& logicalDevice)
{
  vkDestroyImageView(logicalDevice, imageView, nullptr);
}
