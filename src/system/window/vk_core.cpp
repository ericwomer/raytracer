
#include "vk_core.h"

#include <set>
#include <SDL2/SDL_vulkan.h>

VkResult create_debug_util_messenger_ext(VkInstance                                instance,
                                         const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                         const VkAllocationCallbacks*              pAllocator,
                                         VkDebugUtilsMessengerEXT*                 pCallback)
{
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pCallback);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void destroy_debug_utils_messenger_ext(VkInstance                   instance,
                                       VkDebugUtilsMessengerEXT     callback,
                                       const VkAllocationCallbacks* pAllocator)
{
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, callback, pAllocator);
  }
}

Core::Core(SDL_Window* window)
{
  create_instance();
  setup_debug_callback();
  create_surface(window);
  pick_physical_device();
}

Core::~Core()
{
  cleanup();
}

void Core::init(SDL_Window* window)
{
}

/**
 * @brief
 *
 */
void Core::create_surface(SDL_Window* window)
{
  //  VkXcbSurfaceCreateInfoKHR surface_create_info = {VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
  //                                                   nullptr,
  //                                                   0,
  //                                                   connection,
  //                                                   handle};
  //
  //  if (auto result = vkCreateXcbSurfaceKHR(instance, &surface_create_info, nullptr, &surface); result != VK_SUCCESS) {
  //    throw std::runtime_error("Failed to create surface!");
  //  }

  SDL_Vulkan_CreateSurface(window, instance, &primarySurface);
}

/**
* @brief
*
*/
void Core::pick_physical_device()
{
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

  if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support.");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  for (const auto& device : devices) {
    if (is_device_suitable(device)) {
      physicalDevice = device;
      msaaSamples    = get_max_usable_sample_count(physicalDevice);
    }
  }

  if (physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a sutible GPU!");
  }
}

void Core::create_instance()
{
  if (enableValidationLayers) {
    if (!check_validation_layer_support(validationLayers)) {
      throw std::runtime_error("\tValidation layers requested, but not available!");
    }
  }

  VkApplicationInfo appInfo  = {};
  appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName   = "Ray Tracer";
  appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
  appInfo.pEngineName        = "RayTracer";
  appInfo.engineVersion      = VK_MAKE_VERSION(0, 0, 1);
  appInfo.apiVersion         = VK_API_VERSION_1_1;
  appInfo.pNext              = nullptr;

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo     = &appInfo;

  auto extensions                    = get_required_extensions(instanceExtensions);
  createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  if (enableValidationLayers) {
    createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
  if (result != VK_SUCCESS) {
    std::runtime_error("vkCreateInstance failed\n");
  }
}

/**
 * @brief
 * @return
 */
VkSampleCountFlagBits Core::get_max_usable_sample_count(VkPhysicalDevice& physicalDevice)
{
  VkPhysicalDeviceProperties physicalDeviceProperties;
  vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

  VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts,
                                       physicalDeviceProperties.limits.framebufferDepthSampleCounts);

  if (counts & VK_SAMPLE_COUNT_64_BIT) {
    return VK_SAMPLE_COUNT_64_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_32_BIT) {
    return VK_SAMPLE_COUNT_32_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_16_BIT) {
    return VK_SAMPLE_COUNT_16_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_8_BIT) {
    return VK_SAMPLE_COUNT_8_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_4_BIT) {
    return VK_SAMPLE_COUNT_4_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_2_BIT) {
    return VK_SAMPLE_COUNT_2_BIT;
  }
  return VK_SAMPLE_COUNT_1_BIT;
}

/**
* @brief
*
* @param device
* @return true
* @return false
*/
bool Core::is_device_suitable(VkPhysicalDevice device)
{
  Core::QueueFamilyIndices indicies           = find_queue_families(device);
  bool                     extensionSupported = check_device_extension_support(device);
  bool                     swapChainAdequate  = false;

  if (extensionSupported) {
    Core::SwapChainSupportDetails swapChainSupport = query_swap_chain_support(device);
    swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
  }

  VkPhysicalDeviceFeatures supportedFeatures;
  vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

  return indicies.is_complete() && extensionSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

/**
* @brief
*
* @param device
* @return Core::QueueFamilyIndices
*/
Core::QueueFamilyIndices Core::find_queue_families(VkPhysicalDevice& device)
{
  Core::QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies) {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, primarySurface, &presentSupport);
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
* @param device
* @return true
* @return false
*/
bool Core::check_device_extension_support(VkPhysicalDevice device)
{
  uint32_t extensionCount = 0;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

  std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

  for (const auto& extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

/**
 * @brief
 * @param device
 * @return
 */
Core::SwapChainSupportDetails Core::query_swap_chain_support(VkPhysicalDevice& device)
{
  Core::SwapChainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, primarySurface, &details.capabilities);

  uint32_t formatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, primarySurface, &formatCount, nullptr);

  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, primarySurface, &formatCount, details.formats.data());
  }

  uint32_t presentModeCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, primarySurface, &presentModeCount, nullptr);

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, primarySurface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

void Core::setup_debug_callback()
{
  if (!enableValidationLayers) {
    return;
  }

  VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
  createInfo.sType                              = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity                    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debug_callback;
  createInfo.pUserData       = nullptr;

  if (create_debug_util_messenger_ext(instance, &createInfo, nullptr, &callback) != VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug callback");
  }
}

std::vector<const char*> Core::get_required_extensions(const std::vector<const char*>& instanceExtensions)
{
  uint32_t                 sdlCount = 0;
  std::vector<const char*> sdlInstanceExtensions;
  SDL_Vulkan_GetInstanceExtensions(nullptr, &sdlCount, nullptr);
  sdlInstanceExtensions.resize(sdlCount);
  SDL_Vulkan_GetInstanceExtensions(nullptr, &sdlCount, sdlInstanceExtensions.data());

  if (enableValidationLayers) {
    std::cout << "\nNumber of availiable instance extensions\t" << sdlCount << '\n';
    std::cout << "Available Extension List: \n";
    for (auto ext : sdlInstanceExtensions) {
      std::cout << '\t' << ext << '\n';
    }
  }

  //  uint32_t                           instanceExtensionCount = 0;
  //  std::vector<VkExtensionProperties> vkInstanceExtensions;
  //  vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr);
  //  vkInstanceExtensions.resize(instanceExtensionCount);
  //  vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, vkInstanceExtensions.data());

  //  if (enableValidationLayers) {
  //    std::cout << "\nNumber of availiable instance extensions\t" << vkInstanceExtensions.size() << "\n";
  //    std::cout << "Available Extension List: \n";
  //    for (auto& ext : vkInstanceExtensions) {
  //      std::cout << "\t" << ext.extensionName << "\n";
  //    }
  //  }

  //  std::vector<const char*> extensions;
  //  for (auto& ext : instanceExtensions) {
  //    extensions.push_back(ext);
  //  }

  if (enableValidationLayers) {
    sdlInstanceExtensions.push_back("VK_EXT_debug_report");
    sdlInstanceExtensions.push_back("VK_EXT_debug_utils");

    std::cout << "Number of active instance extensions\t" << sdlInstanceExtensions.size() << "\n";
    std::cout << "Active Extension List: \n";
    for (auto const& ext : sdlInstanceExtensions) {
      std::cout << "\t" << ext << "\n";
    }
  }

  return sdlInstanceExtensions;
}

bool Core::check_validation_layer_support(const std::vector<const char*>& validationLayers)
{
  uint32_t                 layerCount = 0;
  std::vector<std::string> activeLayers;

  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  if (enableValidationLayers) {
    std::cout << "Number of available validation layers " << layerCount << "\n";
    std::cout << "Available Layers: \n";
  }

  for (const char* layerName : validationLayers) {
    bool layerFound = false;
    activeLayers.push_back(layerName);
    for (auto& layerProperties : availableLayers) {
      if (enableValidationLayers) {
        std::cout << '\t' << layerProperties.layerName << '\n';
      }

      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }
    if (!layerFound) {
      return false;
    }
  }

  if (enableValidationLayers) {
    std::cout << "Number of active layers " << activeLayers.size() << '\n';
    std::cout << "Active Layers: \n";
    for (const auto& activeLayer : activeLayers) {
      std::cout << '\t' << activeLayer << '\n';
    }
  }

  return true;
}

void Core::cleanup()
{
  /*
  if (device != VK_NULL_HANDLE) {
    vkDeviceWaitIdle(device);
  }

  cleanup_swapchain();

  vkDestroySampler(device, textureSampler, nullptr);
  vkDestroyImageView(device, textureImageView, nullptr);
  vkDestroyImage(device, textureImage, nullptr);
  vkFreeMemory(device, textureImageMemory, nullptr);

  vkDestroyDescriptorPool(device, descriptorPool, nullptr);
  vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

  for (size_t i = 0; i < swapchainImages.size(); i++) {
    vkDestroyBuffer(device, uniformBuffers[i], nullptr);
    vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
  }

  vkDestroyBuffer(device, indexBuffer, nullptr);
  vkFreeMemory(device, indexBufferMemory, nullptr);
  vkDestroyBuffer(device, vertexBuffer, nullptr);
  vkFreeMemory(device, vertexBufferMemory, nullptr);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroySemaphore(device, renderFinishedSemaphore[i], nullptr);
    vkDestroySemaphore(device, imageAvailableSemaphore[i], nullptr);
    vkDestroyFence(device, inFlightFences[i], nullptr);
  }

  vkDestroyCommandPool(device, commandPool, nullptr);
  vkDestroyDevice(device, nullptr);

  if (enableValidationLayers) {
    destroy_debug_utils_messenger_ext(instance, callback, nullptr);
  }

  vkDestroySurfaceKHR(instance, surface, nullptr);
*/

  if (enableValidationLayers) {
    destroy_debug_utils_messenger_ext(instance, callback, nullptr);
  }

  vkDestroyInstance(instance, nullptr);
}

void Core::cleanup_swapchain()
{
  switch (auto result = vkGetFenceStatus(logicalDevice, inFlightFences[currentFrame]); result) {
    case VK_SUCCESS:
      break;
    case VK_NOT_READY:
      vkDeviceWaitIdle(logicalDevice);
      break;
    case VK_ERROR_OUT_OF_HOST_MEMORY:
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
    case VK_ERROR_DEVICE_LOST:
      throw std::runtime_error("Fence Status Error!");
      return;
  }

  vkDestroyImageView(logicalDevice, colorImageView, nullptr);
  vkDestroyImage(logicalDevice, colorImage, nullptr);
  vkFreeMemory(logicalDevice, colorImageMemory, nullptr);

  for (auto framebuffer : swapchainFramebuffers) {
    vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
  }

  vkDestroyImageView(logicalDevice, depthImageView, nullptr);
  vkDestroyImage(logicalDevice, depthImage, nullptr);
  vkFreeMemory(logicalDevice, depthImageMemory, nullptr);

  vkFreeCommandBuffers(logicalDevice, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
  commandBuffers.clear();

  vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
  vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
  vkDestroyRenderPass(logicalDevice, renderPass, nullptr);

  for (auto imageView : swapchainImageViews) {
    vkDestroyImageView(logicalDevice, imageView, nullptr);
  }

  if (swapchain != VK_NULL_HANDLE) {
    vkDestroySwapchainKHR(logicalDevice, swapchain, nullptr);
  }
}
