#include "Instance.h"
#include "vk_core.h"

#include <SDL.h>
#include <SDL2/SDL_vulkan.h>

void Instance::create()
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

void Instance::cleanup()
{
  vkDestroyInstance(instance, nullptr);
}
