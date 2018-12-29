#ifndef IMAGE_H
#define IMAGE_H

class Image {
public:
private:
  uint32_t       mipLevels;
  VkImage        image;
  VkDeviceMemory imageMemory;
  VkImageView    imageView;
  VkSampler      sampler;
};

#endif  // IMAGE_H
