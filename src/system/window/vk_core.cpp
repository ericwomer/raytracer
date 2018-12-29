
#include "vk_core.h"
// #include "Instance.h"

#include <set>
#include <array>
#include <fstream>

#include <SDL2/SDL_vulkan.h>

/**
 * @brief
 * @param instance
 * @param pCreateInfo
 * @param pAllocator
 * @param pCallback
 * @return
 */
VkResult create_debug_util_messenger_ext(const VkInstance&                         instance,
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

/**
 * @brief
 * @param instance
 * @param callback
 * @param pAllocator
 */
void destroy_debug_utils_messenger_ext(const VkInstance&            instance,
                                       VkDebugUtilsMessengerEXT     callback,
                                       const VkAllocationCallbacks* pAllocator)
{
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, callback, pAllocator);
  }
}

bool check_validation_layer_support(const std::vector<const char*>& validationLayers)
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

/**
 * @brief
 * @param instanceExtensions
 * @return
 */
std::vector<const char*> get_required_extensions(const std::vector<const char*>& instanceExtensions)
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

/**
 * @brief
 */
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

  if (create_debug_util_messenger_ext(instance.get(), &createInfo, nullptr, &callback) != VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug callback");
  }
}

/**
 * @brief
 * @param window
 */
void Core::init(SDL_Window* window)
{
  instance.create();
  setup_debug_callback();
  primarySurface.create(instance.get(), window);
  physicalDevice.pick(&instance.get(), &queue, swapchain, primarySurface.get(), deviceExtensions);
  logicalDevice
      .create(physicalDevice, primarySurface, queue, enableValidationLayers, deviceExtensions, validationLayers);

  queue.init(logicalDevice.get());
  canRender = swapchain.create(physicalDevice.get(), logicalDevice.get(), primarySurface.get(), queue, window);
  swapchain.create_image_views(logicalDevice.get());
  create_render_pass();
  create_descriptor_set_layout();
  create_graphics_pipeline();
}

/**
 * @brief
 * @return
 */
bool Core::on_window_size_changed()
{
  return recreate_swap_chain();  // ChildOnwindowSizeChanged();
}

/**
 * @brief
 * @return
 */
bool Core::recreate_swap_chain()
{
  if (logicalDevice.get() != VK_NULL_HANDLE) {
    vkDeviceWaitIdle(logicalDevice.get());
  }

  cleanup_swapchain();
  swapchain.create(physicalDevice.get(), logicalDevice.get(), primarySurface.get(), queue, window);

  if (canRender) {
    swapchain.create_image_views(logicalDevice.get());
    create_render_pass();
    create_graphics_pipeline();
    //    create_color_resources();
    //    create_depth_resources();
    //    create_frame_buffer();
    //    create_command_buffers();
    return true;
  }
  return false;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool Core::draw()  // Eric: Draw is draw frame.
{
  /*
  vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

  uint32_t imageIndex;
  VkResult result = vkAcquireNextImageKHR(device,
                                          swapchain,
                                          std::numeric_limits<uint64_t>::max(),
                                          imageAvailableSemaphore[currentFrame],
                                          VK_NULL_HANDLE,
                                          &imageIndex);

  switch (result) {
    case VK_SUCCESS:
    case VK_SUBOPTIMAL_KHR:
      break;
    case VK_ERROR_OUT_OF_DATE_KHR:
      return on_window_size_changed();
      break;
    default:
      std::cout << "Problem occurred during swap chain image acquisition!" << std::endl;
      return EXIT_FAILURE;
  }

  update_uniform_buffer(imageIndex);

  VkSubmitInfo submitInfo = {};
  submitInfo.sType        = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore          waitSemaphores[] = {imageAvailableSemaphore[currentFrame]};
  VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores    = waitSemaphores;
  submitInfo.pWaitDstStageMask  = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers    = &commandBuffers[imageIndex];

  VkSemaphore signalSemaphores[]  = {renderFinishedSemaphore[currentFrame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores    = signalSemaphores;

  vkResetFences(device, 1, &inFlightFences[currentFrame]);
  if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
    throw std::runtime_error("Failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType            = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores    = signalSemaphores;

  VkSwapchainKHR swapchains[] = {swapchain};
  presentInfo.swapchainCount  = 1;
  presentInfo.pSwapchains     = swapchains;
  presentInfo.pImageIndices   = &imageIndex;

  presentInfo.pResults = nullptr;  // Optional

  result = vkQueuePresentKHR(presentQueue, &presentInfo);

  switch (result) {
    case VK_SUCCESS:
      break;
    case VK_ERROR_OUT_OF_DATE_KHR:
    case VK_SUBOPTIMAL_KHR:
      return on_window_size_changed();
      break;
    default:
      throw std::runtime_error("Problem occurred during image presentation!\n");
      return false;
  }

  currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
*/
  return true;
}

/**
 * @brief
 */
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
  vkDestroyInstance(instance, nullptr);
*/
  logicalDevice.cleanup();

  cleanup_swapchain();

  if (enableValidationLayers) {
    destroy_debug_utils_messenger_ext(instance.get(), callback, nullptr);
  }

  primarySurface.cleanup(instance.get());
  instance.cleanup();
}

/**
 * @brief
 */
void Core::cleanup_swapchain()
{
  /*
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
*/

  vkDestroyPipeline(logicalDevice.get(), graphicsPipeline, nullptr);
  vkDestroyPipelineLayout(logicalDevice.get(), pipelineLayout, nullptr);
  vkDestroyRenderPass(logicalDevice.get(), renderPass, nullptr);

  swapchain.destroy_image_views(logicalDevice.get());

  if (swapchain.get() != VK_NULL_HANDLE) {
    swapchain.cleanup(logicalDevice.get());
  }
}

/**
* @brief
*
*/
void Core::create_graphics_pipeline()
{
  auto vertShaderCode = read_file("triangle.vert.spv");
  auto fragShaderCode = read_file("triangle.frag.spv");

  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;

  vertShaderModule = create_shader_module(logicalDevice.get(), vertShaderCode);
  fragShaderModule = create_shader_module(logicalDevice.get(), fragShaderCode);

  VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
  vertShaderStageInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage                           = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module                          = vertShaderModule;
  vertShaderStageInfo.pName                           = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
  fragShaderStageInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage                           = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module                          = fragShaderModule;
  fragShaderStageInfo.pName                           = "main";

  VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

  VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
  vertexInputInfo.sType                                = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

  auto bindingDescription   = Vertex::getBindingDescription();
  auto attributeDescription = Vertex::getAttributesDescription();

  vertexInputInfo.vertexBindingDescriptionCount   = 1;
  vertexInputInfo.pVertexBindingDescriptions      = &bindingDescription;  // optional
  vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());
  vertexInputInfo.pVertexAttributeDescriptions    = attributeDescription.data();  // optional

  VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
  inputAssembly.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology                               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable                 = VK_FALSE;

  VkViewport viewport = {};
  viewport.x          = 0.0f;
  viewport.y          = 0.0f;
  viewport.width      = (float)swapchain.extent().width;
  viewport.height     = (float)swapchain.extent().height;
  viewport.minDepth   = 0.0f;
  viewport.maxDepth   = 1.0f;

  VkRect2D scissor = {};
  scissor.offset   = {0, 0};
  scissor.extent   = swapchain.extent();

  VkPipelineViewportStateCreateInfo viewportState = {};
  viewportState.sType                             = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount                     = 1;
  viewportState.pViewports                        = &viewport;
  viewportState.scissorCount                      = 1;
  viewportState.pScissors                         = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer = {};
  rasterizer.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable                       = VK_FALSE;
  rasterizer.rasterizerDiscardEnable                = VK_FALSE;
  rasterizer.polygonMode                            = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth                              = 1.0f;
  rasterizer.cullMode                               = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace                              = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable                        = VK_FALSE;
  rasterizer.depthBiasConstantFactor                = 0.0f;  // optional
  rasterizer.depthBiasClamp                         = 0.0f;  // optional
  rasterizer.depthBiasSlopeFactor                   = 0.0f;  // optional

  VkPipelineMultisampleStateCreateInfo multisampling = {};
  multisampling.sType                                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable                  = VK_TRUE;
  multisampling.rasterizationSamples                 = physicalDevice.get_multisampling();
  multisampling.minSampleShading                     = 0.2f;      // optional
  multisampling.pSampleMask                          = nullptr;   // optional
  multisampling.alphaToCoverageEnable                = VK_FALSE;  // optional
  multisampling.alphaToOneEnable                     = VK_FALSE;  // optional

  VkPipelineDepthStencilStateCreateInfo depthStencil = {};
  depthStencil.sType                                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencil.depthTestEnable                       = VK_TRUE;
  depthStencil.depthWriteEnable                      = VK_TRUE;
  depthStencil.depthCompareOp                        = VK_COMPARE_OP_LESS;
  depthStencil.depthBoundsTestEnable                 = VK_FALSE;
  depthStencil.minDepthBounds                        = 0.0f;  // Optional
  depthStencil.maxDepthBounds                        = 1.0f;  // Optional
  depthStencil.stencilTestEnable                     = VK_FALSE;
  depthStencil.front                                 = {};  // Optional
  depthStencil.back                                  = {};  // Optional

  VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
  colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable         = VK_FALSE;
  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // optional
  colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // optional
  colorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD;       // optional
  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // optional
  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // optional
  colorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;       // optional

  VkPipelineColorBlendStateCreateInfo colorBlending = {};
  colorBlending.sType                               = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable                       = VK_FALSE;
  colorBlending.logicOp                             = VK_LOGIC_OP_COPY;  // optional
  colorBlending.attachmentCount                     = 1;
  colorBlending.pAttachments                        = &colorBlendAttachment;
  colorBlending.blendConstants[0]                   = 0.0f;  // optional
  colorBlending.blendConstants[1]                   = 0.0f;  // optional
  colorBlending.blendConstants[2]                   = 0.0f;  // optional
  colorBlending.blendConstants[3]                   = 0.0f;  // optional

  VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
  pipelineLayoutInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount             = 1;                     // optional
  pipelineLayoutInfo.pSetLayouts                = &descriptorSetLayout;  // optional
  pipelineLayoutInfo.pushConstantRangeCount     = 0;                     // optional

  if (vkCreatePipelineLayout(logicalDevice.get(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipeline layout!");
  }

  VkGraphicsPipelineCreateInfo pipelineInfo = {};
  pipelineInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount                   = 2;
  pipelineInfo.pStages                      = shaderStages;
  pipelineInfo.pVertexInputState            = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState          = &inputAssembly;
  pipelineInfo.pViewportState               = &viewportState;
  pipelineInfo.pRasterizationState          = &rasterizer;
  pipelineInfo.pMultisampleState            = &multisampling;
  pipelineInfo.pDepthStencilState           = &depthStencil;  // optional
  pipelineInfo.pColorBlendState             = &colorBlending;
  pipelineInfo.pDynamicState                = nullptr;  // optional

  pipelineInfo.layout     = pipelineLayout;
  pipelineInfo.renderPass = renderPass;
  pipelineInfo.subpass    = 0;

  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;  // optional

  if (vkCreateGraphicsPipelines(logicalDevice.get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create graphics pipeline!");
  }

  vkDestroyShaderModule(logicalDevice.get(), fragShaderModule, nullptr);
  vkDestroyShaderModule(logicalDevice.get(), vertShaderModule, nullptr);
}

/**
 * @brief
 */
void Core::create_descriptor_set_layout()
{
  VkDescriptorSetLayoutBinding uboLayoutBinding = {};
  uboLayoutBinding.binding                      = 0;
  uboLayoutBinding.descriptorType               = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.descriptorCount              = 1;
  uboLayoutBinding.stageFlags                   = VK_SHADER_STAGE_VERTEX_BIT;
  uboLayoutBinding.pImmutableSamplers           = nullptr;  // Optional

  VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
  samplerLayoutBinding.binding                      = 1;
  samplerLayoutBinding.descriptorCount              = 1;
  samplerLayoutBinding.descriptorType               = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerLayoutBinding.pImmutableSamplers           = nullptr;
  samplerLayoutBinding.stageFlags                   = VK_SHADER_STAGE_FRAGMENT_BIT;

  std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};

  VkDescriptorSetLayoutCreateInfo layoutInfo = {};
  layoutInfo.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount                    = static_cast<uint32_t>(bindings.size());
  layoutInfo.pBindings                       = bindings.data();

  if (vkCreateDescriptorSetLayout(logicalDevice.get(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor set layout!");
  }
}

/**
 * @brief
 */
void Core::create_render_pass()
{
  VkAttachmentDescription colorAttachment = {};
  colorAttachment.format                  = swapchain.image_format();
  colorAttachment.samples                 = physicalDevice.get_multisampling();
  colorAttachment.loadOp                  = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout             = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription depthAttachment = {};
  depthAttachment.format                  = find_depth_format(physicalDevice.get());
  depthAttachment.samples                 = physicalDevice.get_multisampling();
  depthAttachment.loadOp                  = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp                 = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout             = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription colorAttachmentResolve = {};
  colorAttachmentResolve.format                  = swapchain.image_format();
  colorAttachmentResolve.samples                 = VK_SAMPLE_COUNT_1_BIT;
  colorAttachmentResolve.loadOp                  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachmentResolve.storeOp                 = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachmentResolve.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachmentResolve.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachmentResolve.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachmentResolve.finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef = {};
  colorAttachmentRef.attachment            = 0;
  colorAttachmentRef.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthAttachmentRef = {};
  depthAttachmentRef.attachment            = 1;
  depthAttachmentRef.layout                = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference colorAttachmentResolveRef = {};
  colorAttachmentResolveRef.attachment            = 2;
  colorAttachmentResolveRef.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass    = {};
  subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount    = 1;
  subpass.pColorAttachments       = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;
  subpass.pResolveAttachments     = &colorAttachmentResolveRef;

  VkSubpassDependency dependency = {};
  dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass          = 0;
  dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask       = 0;
  dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  std::array<VkAttachmentDescription, 3> attachments = {colorAttachment, depthAttachment, colorAttachmentResolve};

  VkRenderPassCreateInfo renderPassInfo = {};
  renderPassInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount        = static_cast<uint32_t>(attachments.size());
  renderPassInfo.pAttachments           = attachments.data();
  renderPassInfo.subpassCount           = 1;
  renderPassInfo.pSubpasses             = &subpass;
  renderPassInfo.dependencyCount        = 1;
  renderPassInfo.pDependencies          = &dependency;

  if (vkCreateRenderPass(logicalDevice.get(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create render pass!");
  }
}

/**
 * @brief
 * @return
 */
VkFormat Core::find_depth_format(const VkPhysicalDevice& physicalDevice)
{
  std::vector<VkFormat> candidates = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};
  return find_supported_format(physicalDevice,
                               candidates,
                               VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

/**
 * @brief
 * @param candidates
 * @param tiling
 * @param features
 * @return
 */
VkFormat Core::find_supported_format(const VkPhysicalDevice&      physicalDevice,
                                     const std::vector<VkFormat>& candidates,
                                     const VkImageTiling&         tiling,
                                     const VkFormatFeatureFlags&  features) const
{
  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }
  throw std::runtime_error("Failed to find supported format!");
}

/**
 * @brief
 *
 * @param filename
 * @return std::vector<char>
 */
std::vector<char> Core::read_file(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file!");
  }

  size_t            fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

/**
 * @brief
 *
 * @param code
 * @return VkShaderModule
 */
VkShaderModule Core::create_shader_module(VkDevice device, const std::vector<char> code)
{
  VkShaderModuleCreateInfo createInfo = {};
  createInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize                 = code.size();
  createInfo.pCode                    = reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule shaderModule;

  if (auto result = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule); result != VK_SUCCESS) {
    throw std::runtime_error("Failed to create shader module!");
  }
  return shaderModule;
}
