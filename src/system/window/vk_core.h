#if !defined(VULKAN_H)
#define VULKAN_H

#include "Instance.h"
#include "PhysicalDevice.h"
#include "Surface.h"
#include "Queue.h"
#include "SwapChain.h"
#include "LogicalDevice.h"

#include <iostream>
#include <vector>
#include <optional>
#include <exception>
#include <array>

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

#include <SDL.h>

#if defined(NDEBUG)
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// Validation Layers/Extensions
// Add aditional extensions here if needed, defaults handled by SDL.
const std::vector<const char*> instanceExtensions = {/*"VK_KHR_xcb_surface", "VK_KHR_surface"*/};
const std::vector<const char*> validationLayers   = {"VK_LAYER_LUNARG_standard_validation"};
const std::vector<const char*> deviceExtensions   = {"VK_KHR_swapchain"};

VkResult create_debug_util_messenger_ext(const VkInstance&                         instance,
                                         const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                         const VkAllocationCallbacks*              pAllocator,
                                         VkDebugUtilsMessengerEXT*                 pCallback);

void destroy_debug_utils_messenger_ext(const VkInstance&            instance,
                                       VkDebugUtilsMessengerEXT     callback,
                                       const VkAllocationCallbacks* pAllocator);

std::vector<const char*> get_required_extensions(const std::vector<const char*>& instanceExtensions);
bool                     check_validation_layer_support(const std::vector<const char*>& validationLayers);

/**
 * @class UniformBufferObject
 * @author ewomer
 * @date 02/12/18
 * @file
 * @brief
 */
struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};

/**
 * @class Vertex
 * @author ewomer
 * @date 02/12/18
 * @file
 * @brief
 */
struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 texCoord;

  static VkVertexInputBindingDescription getBindingDescription()
  {
    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding                         = 0;
    bindingDescription.stride                          = sizeof(Vertex);
    bindingDescription.inputRate                       = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 3> getAttributesDescription()
  {
    std::array<VkVertexInputAttributeDescription, 3> attributesDescription = {};

    attributesDescription[0].binding  = 0;
    attributesDescription[0].location = 0;
    attributesDescription[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributesDescription[0].offset   = offsetof(Vertex, pos);

    attributesDescription[1].binding  = 0;
    attributesDescription[1].location = 1;
    attributesDescription[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributesDescription[1].offset   = offsetof(Vertex, color);

    attributesDescription[2].binding  = 0;
    attributesDescription[2].location = 2;
    attributesDescription[2].format   = VK_FORMAT_R32G32_SFLOAT;
    attributesDescription[2].offset   = offsetof(Vertex, texCoord);

    return attributesDescription;
  }

  bool operator==(const Vertex& other) const
  {
    return pos == other.pos && color == other.color && texCoord == other.texCoord;
  }
};

/**
 * @class Model
 * @author ewomer
 * @date 02/12/18
 * @file
 * @brief
 */
struct Model {
  int width;   // = 800;
  int height;  // = 600;

  std::string modelPath;    // = "data/models/chalet.obj"
  std::string texturePath;  // = "data/textures/chalet.jpg"

  std::vector<Vertex>   verticies;
  std::vector<uint32_t> indices;
  VkBuffer              vertexBuffer;
  VkDeviceMemory        vertexBufferMemory;
};

// Vulkan Public Interface Methods.
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                                                     VkDebugUtilsMessageTypeFlagsEXT             messageType,
                                                     const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                     void*                                       pUserData)
{
  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}

class Core {
public:
  // Initialization public methods
  void init(SDL_Window* window);
  void cleanup();

  // Rendering
  bool draw();  // Vulkan-tutorial.com DrawFrame
  bool ready_to_draw() { return canRender; }
  bool on_window_size_changed();

private:
  const int MAX_FRAMES_IN_FLIGHT = 2;
  size_t    currentFrame         = 0;

  // SDL2
  SDL_Window* window;

  Instance       instance;
  Surface        primarySurface;
  PhysicalDevice physicalDevice;
  LogicalDevice  logicalDevice;
  Queue          queue;
  SwapChain      swapchain;

  // Debug
  VkDebugUtilsMessengerEXT callback;

  // Graphics Pipeline
  VkRenderPass          renderPass;
  VkDescriptorSetLayout descriptorSetLayout;
  VkPipelineLayout      pipelineLayout;
  VkPipeline            graphicsPipeline;

  // Command Pools
  VkCommandPool                commandPool;
  std::vector<VkCommandBuffer> commandBuffers;

  // Vertex Buffers
  VkBuffer       vertexBuffer = VK_NULL_HANDLE;
  VkDeviceMemory vertexBufferMemory;

  // Index buffers
  VkBuffer       indexBuffer;
  VkDeviceMemory indexBufferMemory;

  // Uniform Buffers
  std::vector<VkBuffer>       uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;

  // Descriptor Pools/Sets
  VkDescriptorPool             descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;

  // Images, base/template class texture, depth, color could derive from?
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

  // Concurrency
  std::vector<VkSemaphore> imageAvailableSemaphore;
  std::vector<VkSemaphore> renderFinishedSemaphore;
  std::vector<VkFence>     inFlightFences;

  // Graphics/Rendering
  bool framebufferResized = false;
  bool canRender          = false;

  // Debug
  void setup_debug_callback();

  // Image Views

  // Render Pass
  void create_render_pass();

  // Descriptor Sets
  void create_descriptor_set_layout();

  // Graphics Pipeline
  void create_graphics_pipeline();

  // Swapchain
  void cleanup_swapchain();
  bool recreate_swap_chain();

  // Graphics/Rendering

  VkFormat find_depth_format(const VkPhysicalDevice& physicalDevice);
  VkFormat find_supported_format(const VkPhysicalDevice&      device,
                                 const std::vector<VkFormat>& candidates,
                                 const VkImageTiling&         tiling,
                                 const VkFormatFeatureFlags&  features) const;

  // Image Layout
  bool has_stencil_component(VkFormat& format);

  // Factory, Object related
  VkShaderModule create_shader_module(VkDevice device, const std::vector<char> code);

  // Utility
  std::vector<char> read_file(const std::string& filename);
};

#endif  // VULKAN_H
