#define STB_IMAGE_IMPLEMENTATION
#include "core.hpp"

using namespace BladeGDK;

Core::Color Core::Color::red() {
  Core::Color color{};
  color._red = 1;
  color._green = 0;
  color._blue = 0;
  color._alpha = 1;
  return color;
}

Core::Color Core::Color::blue() {
  Core::Color color{};
  color._red = 0;
  color._green = 0;
  color._blue = 1;
  color._alpha = 1;
  return color;
}

Core::Color Core::Color::green() {
  Core::Color color{};
  color._red = 0;
  color._green = 1;
  color._blue = 0;
  color._alpha = 1;
  return color;
}

Core::Color Core::Color::color(float red, float green, float blue,
                               float alpha) {
  Core::Color color{};
  color._red = red;
  color._green = green;
  color._blue = blue;
  color._alpha = alpha;
  return color;
}

std::array<VkVertexInputAttributeDescription, 2>
Core::VertexColor::getAttributeDescriptions(uint32_t binding,
                                            std::array<uint32_t, 2> locations) {
  std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
  attributeDescriptions[0].binding = binding;
  attributeDescriptions[0].location = locations.at(0);
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(VertexColor, position);

  attributeDescriptions[1].binding = binding;
  attributeDescriptions[1].location = locations.at(1);
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(VertexColor, color);
  return attributeDescriptions;
}

std::array<VkVertexInputAttributeDescription, 3>
Core::VertexColorTexture::getAttributeDescriptions(
    uint32_t binding, std::array<uint32_t, 3> locations) {
  std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
  attributeDescriptions[0].binding = binding;
  attributeDescriptions[0].location = locations.at(0);
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(VertexColorTexture, position);

  attributeDescriptions[1].binding = binding;
  attributeDescriptions[1].location = locations.at(1);
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(VertexColorTexture, color);

  attributeDescriptions[2].binding = binding;
  attributeDescriptions[2].location = locations.at(2);
  attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[2].offset =
      offsetof(VertexColorTexture, textureCoordinate);
  return attributeDescriptions;
}

std::array<VkVertexInputAttributeDescription, 3>
Core::VertexNormalTexture::getAttributeDescriptions(
    uint32_t binding, std::array<uint32_t, 3> locations) {
  std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
  attributeDescriptions[0].binding = binding;
  attributeDescriptions[0].location = locations.at(0);
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(VertexNormalTexture, position);

  attributeDescriptions[1].binding = binding;
  attributeDescriptions[1].location = locations.at(1);
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(VertexNormalTexture, normalAxis);

  attributeDescriptions[2].binding = binding;
  attributeDescriptions[2].location = locations.at(2);
  attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[2].offset =
      offsetof(VertexNormalTexture, textureCoordinate);
  return attributeDescriptions;
}

template <typename Vertex>
VkVertexInputBindingDescription
Core::getVertexBindingDescription(uint32_t binding) {
  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = binding;
  bindingDescription.stride = sizeof(Vertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescription;
}

template VkVertexInputBindingDescription
Core::getVertexBindingDescription<Core::VertexColor>(uint32_t binding);

template VkVertexInputBindingDescription
Core::getVertexBindingDescription<Core::VertexColorTexture>(uint32_t binding);

template VkVertexInputBindingDescription
Core::getVertexBindingDescription<Core::VertexNormalTexture>(uint32_t binding);

Core::SwapChainSupportDetails
Core::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
  SwapChainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);
  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.formats.data());
  }
  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            nullptr);
  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data());
  }
  return details;
}

VkBool32 Core::ComputeQueueFamilyIndices::isComplete() {
  return computeFamilyIndice.has_value();
}

VkBool32 Core::GraphicsPresentQueueFramilyIndices::isComplete() {
  return graphicsFamilyIndice.has_value() && presentFamilyIndice.has_value();
}

VkBool32 Core::ProtectedQueueFamilyIndices::isComplete() {
  return protectedFamilyIndice.has_value();
}

VkBool32 Core::TransferQueueFamilyIndices::isComplete() {
  return transferFamilyIndice.has_value();
}

Core::GraphicsPresentQueueFramilyIndices
Core::findGraphicsPresentQueueFamilyIndices(VkPhysicalDevice device,
                                            VkSurfaceKHR *surface) {
  uint32_t queueFamilyPropertiesCount;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           nullptr);
  if (queueFamilyPropertiesCount <= 0) {
    std::runtime_error("Failed to get Queue Families from Physical Device");
  }

  GraphicsPresentQueueFramilyIndices queueFamilyIndices;

  std::vector<VkQueueFamilyProperties> queueFamilyProperties;
  queueFamilyProperties.resize(queueFamilyPropertiesCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           queueFamilyProperties.data());
  uint32_t i = 0;
  for (const auto &queueFamilyProperty : queueFamilyProperties) {
    VkBool32 presentable;
    if (queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      queueFamilyIndices.graphicsFamilyIndice = i;
      queueFamilyIndices.graphicsFamilyQueueCount =
          queueFamilyProperty.queueCount;
    }
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *surface, &presentable);
    if (presentable == true) {
      queueFamilyIndices.presentFamilyIndice = i;
      queueFamilyIndices.presentFamilyQueueCount =
          queueFamilyProperty.queueCount;
    }

    if (queueFamilyIndices.isComplete()) {
      break;
    }
    i++;
  }
  return queueFamilyIndices;
}

Core::ComputeQueueFamilyIndices
Core::findComputeQueueFamilyIndices(VkPhysicalDevice device) {
  uint32_t queueFamilyPropertiesCount;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           nullptr);
  if (queueFamilyPropertiesCount <= 0) {
    std::runtime_error("Failed to get Queue Families from Physcial Device");
  }

  ComputeQueueFamilyIndices queueFamilyIndices;

  std::vector<VkQueueFamilyProperties> queueFamilyProperties;
  queueFamilyProperties.resize(queueFamilyPropertiesCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           queueFamilyProperties.data());
  uint32_t i = 0;
  for (const auto &queueFamilyProperty : queueFamilyProperties) {
    if (queueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) {
      queueFamilyIndices.computeFamilyIndice = i;
      queueFamilyIndices.computeFamilyQueueCount =
          queueFamilyProperty.queueCount;
    }
    if (queueFamilyIndices.isComplete()) {
      break;
    }
    i++;
  }
  return queueFamilyIndices;
}

Core::TransferQueueFamilyIndices
Core::findTransferQueueFamilyIndices(VkPhysicalDevice device) {
  uint32_t queueFamilyPropertiesCount;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           nullptr);
  if (queueFamilyPropertiesCount <= 0) {
    std::runtime_error("Failed to get Queue Families from Physcial Device");
  }

  TransferQueueFamilyIndices queueFamilyIndices;

  std::vector<VkQueueFamilyProperties> queueFamilyProperties;
  queueFamilyProperties.resize(queueFamilyPropertiesCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           queueFamilyProperties.data());
  uint32_t i = 0;
  for (const auto &queueFamilyProperty : queueFamilyProperties) {
    if (queueFamilyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT) {
      queueFamilyIndices.transferFamilyIndice = i;
      queueFamilyIndices.transferFamilyQueueCount =
          queueFamilyProperty.queueCount;
    }
    if (queueFamilyIndices.isComplete()) {
      break;
    }
    i++;
  }
  return queueFamilyIndices;
}

Core::ProtectedQueueFamilyIndices
Core::findProtectedQueueFamilyIndices(VkPhysicalDevice device) {
  uint32_t queueFamilyPropertiesCount;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           nullptr);
  if (queueFamilyPropertiesCount <= 0) {
    std::runtime_error("Failed to get Queue Families from Physcial Device");
  }

  ProtectedQueueFamilyIndices queueFamilyIndices;

  std::vector<VkQueueFamilyProperties> queueFamilyProperties;
  queueFamilyProperties.resize(queueFamilyPropertiesCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount,
                                           queueFamilyProperties.data());
  uint32_t i = 0;
  for (const auto &queueFamilyProperty : queueFamilyProperties) {
    if (queueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) {
      queueFamilyIndices.protectedFamilyIndice = i;
      queueFamilyIndices.protectedFamilyQueueCount =
          queueFamilyProperty.queueCount;
    }
    if (queueFamilyIndices.isComplete()) {
      break;
    }
    i++;
  }
  return queueFamilyIndices;
}

void Core::initBlade() {
  initWindow();
  createInstance();
  createWindowSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createSwapChain();
  createImageViews();
  createRenderPass();
  createDescriptorSetLayout();
  createGraphicsPipeline();
  createCommandPool();
  createDepthResources();
  createFrameBuffers();
  createTextureImage("Images/Background_layers/Layer_0002_7.png");
  createTextureImageView();
  createTextureSampler();
  createVertexBuffer<VertexColorTexture>(_vertices, sizeof(_vertices[0]) *
                                                        _vertices.size());
  createIndexBuffer<uint32_t>(_indices, sizeof(_indices[0]) * _indices.size());
  createUniformBuffer(sizeof(UniformBufferObject));
  createDescriptorPool();
  createDescriptorSets();
  createCommandBuffers();
  createSyncObjects();
}

void Core::setWindowSize(uint32_t width, uint32_t height) {
  WINDOW_WIDTH = width;
  WINDOW_HEIGHT = height;
}

void Core::mainLoop(Camera *camera) {
  while (!glfwWindowShouldClose(_window)) {
    drawFrame(camera);
    glfwPollEvents();
  }
  vkDeviceWaitIdle(_device);
}

void Core::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BLADE GDK", nullptr,
                             nullptr);
}

void Core::createInstance() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.applicationVersion = 1;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;

  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;

  if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
    std::runtime_error("Failed to create Vulkan Instance!");
  }
  std::cout << "Created Instance" << std::endl;
}

void Core::createWindowSurface() {
  if (glfwCreateWindowSurface(_instance, _window, nullptr, &_windowSurface) !=
      VK_SUCCESS) {
    std::runtime_error("Failed to create GLFW Window Surface!");
  }
  std::cout << "Created GLFW Window Surface" << std::endl;
}

VkBool32 Core::checkDeviceExtensionSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(_deviceExtensions.begin(),
                                           _deviceExtensions.end());

  for (const auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

VkBool32 Core::checkDeviceSuitability(VkPhysicalDevice physicalDevice) {
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(physicalDevice, &properties);

  VkPhysicalDeviceFeatures features;
  vkGetPhysicalDeviceFeatures(physicalDevice, &features);

  GraphicsPresentQueueFramilyIndices GPQueueFamilyIndices =
      findGraphicsPresentQueueFamilyIndices(physicalDevice, &_windowSurface);

  VkBool32 swapChainAdequate = false;
  if (checkDeviceExtensionSupport(physicalDevice) == VK_TRUE) {
    SwapChainSupportDetails swapChainSupportDetails =
        querySwapChainSupport(physicalDevice, _windowSurface);
    swapChainAdequate = !swapChainSupportDetails.formats.empty() &&
                        !swapChainSupportDetails.presentModes.empty();
  }

  return features.samplerAnisotropy && GPQueueFamilyIndices.isComplete() &&
         swapChainAdequate;
}

void Core::pickPhysicalDevice() {
  uint32_t physicalDeviceCount;
  vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, nullptr);

  if (physicalDeviceCount <= 0) {
    std::runtime_error("No Vulkan Supported Physical Device Found");
  }
  std::vector<VkPhysicalDevice> physicalDevices;
  physicalDevices.resize(physicalDeviceCount);
  vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount,
                             physicalDevices.data());
  for (const auto &physicalDevice : physicalDevices) {
    if (checkDeviceSuitability(physicalDevice) == VK_TRUE) {
      _physicalDevice = physicalDevice;
      std::cout << "Selected Vulkan Supported Physical Device!" << std::endl;
      return;
    }
  }
  std::runtime_error("Failed to select suitable Physcial Device");
}
void Core::createLogicalDevice() {
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

  // Setup Graphics and Present Queues
  GraphicsPresentQueueFramilyIndices GPQueueFamilyIndices =
      findGraphicsPresentQueueFamilyIndices(_physicalDevice, &_windowSurface);
  uint32_t familyCount = 2;
  float queuePriority = 1.0f;
  for (uint32_t i = 0; i < familyCount; i++) {
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    if (i == 0) {
      queueInfo.queueCount = GPQueueFamilyIndices.graphicsFamilyQueueCount;
      queueInfo.queueFamilyIndex =
          GPQueueFamilyIndices.graphicsFamilyIndice.value();
    } else {
      queueInfo.queueCount = GPQueueFamilyIndices.presentFamilyQueueCount;
      queueInfo.queueFamilyIndex =
          GPQueueFamilyIndices.presentFamilyIndice.value();
    }
    queueInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueInfo);
  }

  // Setup Device Features of the Queues
  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount =
      static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(_deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = _deviceExtensions.data();
  createInfo.pEnabledFeatures = &deviceFeatures;
  if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) !=
      VK_SUCCESS) {
    std::runtime_error("Failed to create Logical Device");
  }

  std::cout << "Created Vulkan Logical Device!" << std::endl;
  vkGetDeviceQueue(_device, GPQueueFamilyIndices.graphicsFamilyIndice.value(),
                   0, &_graphicsQueue);
  vkGetDeviceQueue(_device, GPQueueFamilyIndices.presentFamilyIndice.value(), 0,
                   &_presentQueue);
}

VkSurfaceFormatKHR
Core::pickSurfaceFormat(const std::vector<VkSurfaceFormatKHR> formats) {
  for (const auto &availableFormat : formats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return formats[0];
}

VkPresentModeKHR
Core::pickPresentMode(const std::vector<VkPresentModeKHR> presentModes) {
  for (const auto &availablePresentMode : presentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Core::pickExtent(GLFWwindow *window,
                            const VkSurfaceCapabilitiesKHR &capabilities) {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                               static_cast<uint32_t>(height)};

    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);

    return actualExtent;
  }
}
void Core::createSwapChain() {
  SwapChainSupportDetails swapChainSupport =
      querySwapChainSupport(_physicalDevice, _windowSurface);

  VkSurfaceFormatKHR surfaceFormat =
      pickSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode = pickPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = pickExtent(_window, swapChainSupport.capabilities);

  _swapChainImageFormat = surfaceFormat.format;
  _swapChainExtent = extent;

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }
  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = _windowSurface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  GraphicsPresentQueueFramilyIndices GPQueueFamilyIndices =
      findGraphicsPresentQueueFamilyIndices(_physicalDevice, &_windowSurface);

  if (GPQueueFamilyIndices.graphicsFamilyIndice.value() !=
      GPQueueFamilyIndices.presentFamilyIndice.value()) {
    std::array<uint32_t, 2> indices = {
        GPQueueFamilyIndices.graphicsFamilyIndice.value(),
        GPQueueFamilyIndices.presentFamilyIndice.value(),
    };
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = indices.data();
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }
  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapChain) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create swap chain!");
  } else {
    std::cout << "Created Vulkan Swap Chain" << std::endl;
  }

  vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, nullptr);
  _swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount,
                          _swapChainImages.data());
}

VkImageView Core::createImageView(VkImage image, VkFormat format,
                                  VkImageAspectFlags aspectFlags) {
  VkImageViewCreateInfo viewInfo{};
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = image;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.subresourceRange.aspectMask = aspectFlags;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  VkImageView imageView;
  if (vkCreateImageView(_device, &viewInfo, nullptr, &imageView) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create texture image view!");
  }

  return imageView;
}

void Core::createImageViews() {
  _swapChainImageViews.resize(_swapChainImages.size());
  for (size_t i = 0; i < _swapChainImages.size(); i++) {
    _swapChainImageViews[i] = createImageView(
        _swapChainImages[i], _swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
  }
  std::cout << "Created Image Views" << std::endl;
}

VkFormat Core::findSupportedFormat(const std::vector<VkFormat> &candidates,
                                   VkImageTiling tiling,
                                   VkFormatFeatureFlags features) {
  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &props);
    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
               (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  throw std::runtime_error("failed to find supported format!");
}

VkFormat Core::findDepthFormat() {
  return findSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

void Core::createRenderPass() {
  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = findDepthFormat();
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout =
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = 1;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = _swapChainImageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcAccessMask = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                             VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  std::array<VkAttachmentDescription, 2> attachments = {colorAttachment,
                                                        depthAttachment};
  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;
  if (vkCreateRenderPass(_device, &renderPassInfo, nullptr, &_renderPass) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create render pass!");
  } else {
    std::cout << "Created Render pass" << std::endl;
  }
}
void Core::createDescriptorSetLayout() {

  // Setup Vertex Data In Vertex Shader
  VkDescriptorSetLayoutBinding layoutBinding{};
  layoutBinding.binding = 0;
  layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  layoutBinding.descriptorCount = 1;
  layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

  // Setup Image Sampler Input Data In Fragment Shader
  VkDescriptorSetLayoutBinding samplerLayoutBinding{};
  samplerLayoutBinding.binding = 1;
  samplerLayoutBinding.descriptorCount = 1;
  samplerLayoutBinding.descriptorType =
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerLayoutBinding.pImmutableSamplers = nullptr;
  samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

  std::array<VkDescriptorSetLayoutBinding, 2> bindings = {layoutBinding,
                                                          samplerLayoutBinding};

  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
  layoutInfo.pBindings = bindings.data();

  if (vkCreateDescriptorSetLayout(_device, &layoutInfo, nullptr,
                                  &_descriptorSetLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor set layout!");
  }

  std::cout << "Created Descriptor Set Layout" << std::endl;
}

std::vector<char> readFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file!");
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return buffer;
}

VkShaderModule Core::createShaderModule(const std::vector<char> &code) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
  VkShaderModule shaderModule;
  if (vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create shader module!");
  }
  return shaderModule;
}

void Core::createGraphicsPipeline() {
  auto vertShaderCode = readFile("vert.spv");
  auto fragShaderCode = readFile("frag.spv");

  VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
  VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShaderModule;
  vertShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragShaderModule;
  fragShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                    fragShaderStageInfo};

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

  auto bindingDescription = getVertexBindingDescription<VertexColorTexture>(0);
  std::array<uint32_t, 3> locations = {0, 1, 2};
  auto attributeDescriptions =
      VertexColorTexture::getAttributeDescriptions(0, locations);

  vertexInputInfo.vertexBindingDescriptionCount = 1;
  vertexInputInfo.vertexAttributeDescriptionCount =
      static_cast<uint32_t>(attributeDescriptions.size());
  vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.scissorCount = 1;

  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;

  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

  rasterizer.depthBiasEnable = VK_FALSE;

  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;

  VkPipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
                                               VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamicState{};
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
  dynamicState.pDynamicStates = dynamicStates.data();

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &_descriptorSetLayout;

  if (vkCreatePipelineLayout(_device, &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
  VkPipelineDepthStencilStateCreateInfo depthStencil{};
  depthStencil.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencil.depthTestEnable = VK_TRUE;
  depthStencil.depthWriteEnable = VK_TRUE;
  depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
  depthStencil.depthBoundsTestEnable = VK_FALSE;
  depthStencil.stencilTestEnable = VK_FALSE;

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = &dynamicState;
  pipelineInfo.layout = pipelineLayout;
  pipelineInfo.renderPass = _renderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.pDepthStencilState = &depthStencil;

  if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo,
                                nullptr, &graphicsPipeline) != VK_SUCCESS) {
    throw std::runtime_error("failed to create graphics pipeline!");
  }

  std::cout << "Created Graphics Pipeline" << std::endl;
  vkDestroyShaderModule(_device, fragShaderModule, nullptr);
  vkDestroyShaderModule(_device, vertShaderModule, nullptr);
}
void Core::createCommandPool() {
  GraphicsPresentQueueFramilyIndices GPQueueFamilyIndices =
      findGraphicsPresentQueueFamilyIndices(_physicalDevice, &_windowSurface);
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = GPQueueFamilyIndices.graphicsFamilyIndice.value();
  if (vkCreateCommandPool(_device, &poolInfo, nullptr, &_commandPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
  }
  std::cout << "Created Command Pool" << std::endl;
}

VkFormat Core::findSupportedFormat(VkPhysicalDevice physicalDevice,
                                   const std::vector<VkFormat> &candidates,
                                   VkImageTiling tiling,
                                   VkFormatFeatureFlags features) {
  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
               (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  throw std::runtime_error("failed to find supported format!");
}
VkFormat Core::findDepthFormat(VkPhysicalDevice physicalDevice) {
  return findSupportedFormat(
      physicalDevice,
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}
uint32_t Core::findMemoryType(uint32_t typeFilter,
                              VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memoryProperties;
  vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memoryProperties);
  for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) &&
        (memoryProperties.memoryTypes[i].propertyFlags & properties) ==
            properties) {
      return i;
    }
  }

  throw std::runtime_error("failed to find suitable memory type!");
};
void Core::createImage(uint32_t width, uint32_t height, VkFormat format,
                       VkImageTiling tiling, VkImageUsageFlags usage,
                       VkMemoryPropertyFlags properties, VkImage &image,
                       VkDeviceMemory &imageMemory) {
  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateImage(_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
    throw std::runtime_error("failed to create image!");
  }

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(_device, image, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      findMemoryType(memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate image memory!");
  }

  vkBindImageMemory(_device, image, imageMemory, 0);
}

VkCommandBuffer Core::beginSingleTimeCommands() {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = _commandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  return commandBuffer;
}
void Core::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(_graphicsQueue);

  vkFreeCommandBuffers(_device, _commandPool, 1, &commandBuffer);
}
bool hasStencilComponent(VkFormat format) {
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
         format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void Core::transitionImageLayout(VkImage image, VkFormat format,
                                 VkImageLayout oldLayout,
                                 VkImageLayout newLayout) {
  VkCommandBuffer commandBuffer = beginSingleTimeCommands();
  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;

  if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    if (hasStencilComponent(format)) {
      barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
  } else {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  }

  // barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  VkPipelineStageFlags sourceStage;
  VkPipelineStageFlags destinationStage;
  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
      newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
             newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  } else {
    throw std::invalid_argument("unsupported layout transition!");
  }
  vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0,
                       nullptr, 0, nullptr, 1, &barrier);
  endSingleTimeCommands(commandBuffer);
}

void Core::createDepthResources() {
  VkFormat depthFormat = findDepthFormat(_physicalDevice);

  createImage(
      _swapChainExtent.width, _swapChainExtent.height, depthFormat,
      VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _depthImage, _depthImageMemory);
  _depthImageView =
      createImageView(_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
  transitionImageLayout(_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  std::cout << "Created Depth Resources" << std::endl;
}

void Core::createFrameBuffers() {
  _swapChainFramebuffers.resize(_swapChainImageViews.size());
  for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
    std::array<VkImageView, 2> attachments = {_swapChainImageViews[i],
                                              _depthImageView};
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = _renderPass;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = _swapChainExtent.width;
    framebufferInfo.height = _swapChainExtent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(_device, &framebufferInfo, nullptr,
                            &_swapChainFramebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }

  std::cout << "Created Frame Buffers" << std::endl;
}

void Core::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                        VkMemoryPropertyFlags properties, VkBuffer &buffer,
                        VkDeviceMemory &bufferMemory) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to create buffer!");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(_device, buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      findMemoryType(memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(_device, &allocInfo, nullptr, &bufferMemory) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate buffer memory!");
  }

  vkBindBufferMemory(_device, buffer, bufferMemory, 0);
}

void Core::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                             uint32_t height) {
  VkCommandBuffer commandBuffer = beginSingleTimeCommands();
  VkBufferImageCopy region{};
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;

  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;

  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};

  vkCmdCopyBufferToImage(commandBuffer, buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
  endSingleTimeCommands(commandBuffer);
}

void Core::createTextureImage(const char *imagePath) {
  int texWidth, texHeight, texChannels;
  stbi_uc *pixels =
      stbi_load(imagePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  VkDeviceSize imageSize = texWidth * texHeight * 4;

  if (!pixels) {
    throw std::runtime_error("failed to load texture image!");
  }
  std::cout << "Loaded Texture Sucessfull" << std::endl;

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;

  createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               stagingBuffer, stagingBufferMemory);
  void *data;
  vkMapMemory(_device, stagingBufferMemory, 0, imageSize, 0, &data);
  memcpy(data, pixels, static_cast<size_t>(imageSize));
  vkUnmapMemory(_device, stagingBufferMemory);
  stbi_image_free(pixels);

  createImage(
      texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _textureImage, _textureImageMemory);
  transitionImageLayout(_textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  copyBufferToImage(stagingBuffer, _textureImage,
                    static_cast<uint32_t>(texWidth),
                    static_cast<uint32_t>(texHeight));
  transitionImageLayout(_textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  vkDestroyBuffer(_device, stagingBuffer, nullptr);
  vkFreeMemory(_device, stagingBufferMemory, nullptr);
  std::cout << "Created Texture Image" << std::endl;
}
void Core::createTextureImageView() {
  _textureImageView = createImageView(_textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                                      VK_IMAGE_ASPECT_COLOR_BIT);
  std::cout << "Created Texture Image View" << std::endl;
}
void Core::createTextureSampler() {
  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.anisotropyEnable = VK_TRUE;

  VkPhysicalDeviceProperties properties{};
  vkGetPhysicalDeviceProperties(_physicalDevice, &properties);

  samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = 0.0f;
  if (vkCreateSampler(_device, &samplerInfo, nullptr, &_textureSampler) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create texture sampler!");
  }
  std::cout << "Created Texture Sampler" << std::endl;
}
void Core::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer,
                      VkDeviceSize size) {
  VkCommandBuffer commandBuffer = beginSingleTimeCommands();

  VkBufferCopy copyRegion{};
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

  endSingleTimeCommands(commandBuffer);
}

template <typename Vertex>
void Core::createVertexBuffer(std::vector<Vertex> vertices,
                              VkDeviceSize bufferSize) {
  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               stagingBuffer, stagingBufferMemory);

  void *data;
  vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(_device, stagingBufferMemory);

  createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexBufferMemory);
  copyBuffer(stagingBuffer, _vertexBuffer, bufferSize);
  vkDestroyBuffer(_device, stagingBuffer, nullptr);
  vkFreeMemory(_device, stagingBufferMemory, nullptr);
  std::cout << "Created Vertex Buffer" << std::endl;
}

template void
Core::createVertexBuffer<Core::VertexColor>(std::vector<Core::VertexColor>,
                                            VkDeviceSize bufferSize);
template void Core::createVertexBuffer<Core::VertexColorTexture>(
    std::vector<Core::VertexColorTexture>, VkDeviceSize bufferSize);
template void Core::createVertexBuffer<Core::VertexNormalTexture>(
    std::vector<Core::VertexNormalTexture>, VkDeviceSize bufferSize);

template <typename IndicesType>
void Core::createIndexBuffer(std::vector<IndicesType> indices,
                             VkDeviceSize bufferSize) {
  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               stagingBuffer, stagingBufferMemory);

  void *data;
  vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, indices.data(), (size_t)bufferSize);
  vkUnmapMemory(_device, stagingBufferMemory);

  createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer, _indexBufferMemory);
  copyBuffer(stagingBuffer, _indexBuffer, bufferSize);
  vkDestroyBuffer(_device, stagingBuffer, nullptr);
  vkFreeMemory(_device, stagingBufferMemory, nullptr);
  std::cout << "Created Index Buffer" << std::endl;
}

template void Core::createIndexBuffer<uint8_t>(std::vector<uint8_t> indices,
                                               VkDeviceSize bufferSize);
template void Core::createIndexBuffer<uint16_t>(std::vector<uint16_t> indices,
                                                VkDeviceSize bufferSize);
template void Core::createIndexBuffer<uint32_t>(std::vector<uint32_t> indices,
                                                VkDeviceSize bufferSize);

void Core::createUniformBuffer(VkDeviceSize bufferSize) {
  _uniformBuffers.resize(FRAMES_IN_FLIGHT);
  _uniformBuffersMemory.resize(FRAMES_IN_FLIGHT);

  for (size_t i = 0; i < FRAMES_IN_FLIGHT; i++) {
    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 _uniformBuffers[i], _uniformBuffersMemory[i]);
  }
  std::cout << "Created Uniform Buffer" << std::endl;
}

void Core::createDescriptorPool() {
  std::array<VkDescriptorPoolSize, 2> poolSizes{};
  poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSizes[0].descriptorCount = static_cast<uint32_t>(FRAMES_IN_FLIGHT);
  poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[1].descriptorCount = static_cast<uint32_t>(FRAMES_IN_FLIGHT);

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  poolInfo.pPoolSizes = poolSizes.data();
  poolInfo.maxSets = static_cast<uint32_t>(FRAMES_IN_FLIGHT);

  if (vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor pool!");
  }
  std::cout << "Created Descriptor pool!" << std::endl;
}
void Core::createDescriptorSets() {
  std::vector<VkDescriptorSetLayout> layouts(FRAMES_IN_FLIGHT,
                                             _descriptorSetLayout);
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = _descriptorPool;
  allocInfo.descriptorSetCount = FRAMES_IN_FLIGHT;
  allocInfo.pSetLayouts = layouts.data();
  _descriptorSets.resize(FRAMES_IN_FLIGHT);
  if (vkAllocateDescriptorSets(_device, &allocInfo, _descriptorSets.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate descriptor sets!");
  }

  for (size_t i = 0; i < FRAMES_IN_FLIGHT; i++) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = _uniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = _textureImageView;
    imageInfo.sampler = _textureSampler;

    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = _descriptorSets[i];
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = _descriptorSets[i];
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType =
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(_device,
                           static_cast<uint32_t>(descriptorWrites.size()),
                           descriptorWrites.data(), 0, nullptr);
  }
}
void Core::createCommandBuffers() {
  _commandBuffer.resize(FRAMES_IN_FLIGHT);
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = _commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (uint32_t)_commandBuffer.size();

  if (vkAllocateCommandBuffers(_device, &allocInfo, _commandBuffer.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
  std::cout << "Created Command Buffers" << std::endl;
}
void Core::createSyncObjects() {
  imageAvailableSemaphores.resize(FRAMES_IN_FLIGHT);
  renderFinishedSemaphores.resize(FRAMES_IN_FLIGHT);
  inFlightFences.resize(FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (uint32_t i = 0; i < FRAMES_IN_FLIGHT; i++) {
    if (vkCreateSemaphore(_device, &semaphoreInfo, nullptr,
                          &imageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateSemaphore(_device, &semaphoreInfo, nullptr,
                          &renderFinishedSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence(_device, &fenceInfo, nullptr, &inFlightFences[i]) !=
            VK_SUCCESS) {
      throw std::runtime_error("failed to create semaphores!");
    }
  }
  std::cout << "Created Sync Obejcts" << std::endl;
}

void Core::updateUniformBuffer(uint32_t currentImage, float aspectRatio,
                               Camera *camera) {

  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                   currentTime - startTime)
                   .count();

  UniformBufferObject ubo{};
  ubo.cameraPosition = camera->getCameraPosition();
  ubo.model = glm::mat4(1);
  ubo.view = camera->View();
  ubo.proj = camera->Projection();

  void *data;
  vkMapMemory(_device, _uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0,
              &data);
  memcpy(data, &ubo, sizeof(ubo));
  vkUnmapMemory(_device, _uniformBuffersMemory[currentImage]);
}

void Core::recordElementsCommandBuffer(uint32_t imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(_commandBuffer[_currentFrame], &beginInfo) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }
  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = _renderPass;
  renderPassInfo.framebuffer = _swapChainFramebuffers[imageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = _swapChainExtent;

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {{0.25f, 0.25f, 0.25f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};

  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();
  vkCmdBeginRenderPass(_commandBuffer[_currentFrame], &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(_commandBuffer[_currentFrame],
                    VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
  VkBuffer vertexBuffers[] = {_vertexBuffer};
  VkDeviceSize offsets[] = {0};

  vkCmdBindVertexBuffers(_commandBuffer[_currentFrame], 0, 1, vertexBuffers,
                         offsets);
  vkCmdBindIndexBuffer(_commandBuffer[_currentFrame], _indexBuffer, 0,
                       VK_INDEX_TYPE_UINT32);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(_swapChainExtent.width);
  viewport.height = static_cast<float>(_swapChainExtent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(_commandBuffer[_currentFrame], 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = _swapChainExtent;
  vkCmdSetScissor(_commandBuffer[_currentFrame], 0, 1, &scissor);

  vkCmdBindDescriptorSets(_commandBuffer[_currentFrame],
                          VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                          &_descriptorSets[_currentFrame], 0, nullptr);
  vkCmdDrawIndexed(_commandBuffer[_currentFrame],
                   static_cast<uint32_t>(_indices.size()), 1, 0, 0, 0);

  vkCmdEndRenderPass(_commandBuffer[_currentFrame]);
  if (vkEndCommandBuffer(_commandBuffer[_currentFrame]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }

  // std::cout << "Recorded Command Buffer" << std::endl;
}

void Core::drawFrame(Camera *camera) {
  vkWaitForFences(_device, 1, &inFlightFences[_currentFrame], VK_TRUE,
                  UINT64_MAX);
  vkResetFences(_device, 1, &inFlightFences[_currentFrame]);
  uint32_t imageIndex;
  vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX,
                        imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE,
                        &imageIndex);
  vkResetCommandBuffer(_commandBuffer[_currentFrame], 0);

  updateUniformBuffer(_currentFrame, 800 / 600.0, camera);

  recordElementsCommandBuffer(imageIndex);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[_currentFrame]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &_commandBuffer[_currentFrame];
  VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[_currentFrame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(_graphicsQueue, 1, &submitInfo,
                    inFlightFences[_currentFrame]) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = {_swapChain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  vkQueuePresentKHR(_presentQueue, &presentInfo);
  _currentFrame = (_currentFrame + 1) % FRAMES_IN_FLIGHT;
}

void Core::cleanup() {
  glfwDestroyWindow(_window);
  glfwTerminate();
};