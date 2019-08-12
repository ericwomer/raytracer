#if !defined(VULKAN_H)
#define VULKAN_H

#include <iostream>
#include <vector>
#include <optional>
#include <exception>

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#if defined(VK_USE_PLATFORM_XCB_KHR)
#include <xcb/xcb.h>
#include <dlfcn.h>
#endif

#if defined(NDEBUG)
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers   = {"VK_LAYER_LUNARG_standard_validation"};
const std::vector<const char*> instanceExtensions = {"VK_KHR_xcb_surface", "VK_KHR_surface"};
const std::vector<const char*> deviceExtensions   = {"VK_KHR_swapchain"};

VkResult create_debug_util_messenger_ext(VkInstance                                instance,
                                         const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                         const VkAllocationCallbacks*              pAllocator,
                                         VkDebugUtilsMessengerEXT*                 pCallback);

void destroy_debug_utils_messenger_ext(VkInstance                   instance,
                                       VkDebugUtilsMessengerEXT     callback,
                                       const VkAllocationCallbacks* pAllocator);

class Core {
  public:
  Core();
  ~Core();
  // Vulkan Public Interface Methods.
  static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                                                       VkDebugUtilsMessageTypeFlagsEXT             messageType,
                                                       const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                       void*                                       pUserData)
  {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
  }

  void init();
  bool draw();  // Vulkan-tutorial.com DrawFrame
  void cleanup();
  bool ready_to_draw() { return canRender; }
  bool on_window_size_changed();

  struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool is_complete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
  };

  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;
  };

  private:
  const int MAX_FRAMES_IN_FLIGHT = 2;
  size_t    currentFrame         = 0;

  // Core
  VkInstance               instance;
  VkDebugUtilsMessengerEXT callback;
  VkPhysicalDevice         physicalDevice = VK_NULL_HANDLE;
  VkDevice                 logicalDevice;

  // Queues
  VkQueue graphicsQueue;
  VkQueue presentQueue;

  // Surface
  VkSurfaceKHR primarySurface;

  // Swapchain
  VkSwapchainKHR             swapchain = VK_NULL_HANDLE;
  std::vector<VkImage>       swapchainImages;
  VkFormat                   swapchainImageFormat;
  VkExtent2D                 swapchainExtent;
  std::vector<VkImageView>   swapchainImageViews;
  std::vector<VkFramebuffer> swapchainFramebuffers;

  // Graphics Pipeline
  VkRenderPass          renderPass;
  VkDescriptorSetLayout descriptorSetLayout;
  VkPipelineLayout      pipelineLayout;
  VkPipeline            graphicsPipeline;

  VkCommandPool                commandPool;
  std::vector<VkCommandBuffer> commandBuffers;
  VkBuffer                     vertexBuffer = VK_NULL_HANDLE;
  VkDeviceMemory               vertexBufferMemory;
  VkBuffer                     indexBuffer;
  VkDeviceMemory               indexBufferMemory;
  std::vector<VkBuffer>        uniformBuffers;
  std::vector<VkDeviceMemory>  uniformBuffersMemory;
  VkDescriptorPool             descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;
  VkSampleCountFlagBits        msaaSamples = VK_SAMPLE_COUNT_1_BIT;

  uint32_t       mipLevels;
  VkImage        textureImage;
  VkDeviceMemory textureImageMemory;
  VkImageView    textureImageView;
  VkSampler      textureSampler;

  VkImage        depthImage;
  VkDeviceMemory depthImageMemory;
  VkImageView    depthImageView;

  VkImage        colorImage;
  VkDeviceMemory colorImageMemory;
  VkImageView    colorImageView;

  std::vector<VkSemaphore> imageAvailableSemaphore;
  std::vector<VkSemaphore> renderFinishedSemaphore;
  std::vector<VkFence>     inFlightFences;
  bool                     framebufferResized = false;

  void* VulkanLibrary = nullptr;
  bool  canRender     = false;

  char*    extensionStringNames[64];  // Eric: why am I char const *?
  uint32_t extensionCount = 0;

  QueueFamilyIndices familyIndicies;

  // Initializers
  void create_instance();
  void setup_debug_callback();

  // Cleanup
  void cleanup_swapchain();

  // Eric: Clean this area up
  // Misc
  std::vector<const char*>      get_required_extensions(const std::vector<const char*>& instanceExtensions);
  Core::SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice& device, VkSurfaceKHR& surface);
  Core::QueueFamilyIndices      find_queue_families(VkPhysicalDevice& device, VkSurfaceKHR& surface);
  bool                          check_validation_layer_support(const std::vector<const char*>& validationLayers);
  bool                          check_device_extension_support(VkPhysicalDevice device);
  bool                          is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface);
  void                          get_device_queues(VkDevice&                 device,
                                                  Core::QueueFamilyIndices& familyIndicies,
                                                  VkQueue&                  presentQueue,
                                                  VkQueue&                  graphicsQueue);
  uint32_t find_memory_type(VkPhysicalDevice& physicalDevice, uint32_t& typeFilter, VkMemoryPropertyFlags& properties);
  VkFormat find_supported_format(VkPhysicalDevice&            device,
                                 const std::vector<VkFormat>& candidates,
                                 const VkImageTiling&         tiling,
                                 const VkFormatFeatureFlags&  features);
  VkFormat find_depth_format(VkPhysicalDevice& physicalDevice);
  bool     has_stencil_component(VkFormat& format);
  VkSampleCountFlagBits get_max_usable_sample_count(VkPhysicalDevice& physicalDevice);
};

#endif  // VULKAN_H
