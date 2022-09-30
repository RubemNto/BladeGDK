#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <algorithm> // Necessary for std::clamp
#include <array>
#include <cstdint> // Necessary for uint32_t
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include <string>
#include <vector>
namespace BladeGDK {
class Core {
public:
  struct VertexColor {
    glm::vec3 position;
    glm::vec3 color;

    static std::array<VkVertexInputAttributeDescription, 2>
    getAttributeDescriptions(uint32_t binding,
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
  };
  struct VertexColorTexture {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 textureCoordinate;
    static std::array<VkVertexInputAttributeDescription, 3>
    getAttributeDescriptions(uint32_t binding,
                             std::array<uint32_t, 3> locations) {
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
  };

  struct VertexNormalTexture {
    glm::vec3 position;
    glm::vec3 normalAxis;
    glm::vec2 textureCoordinate;
    static std::array<VkVertexInputAttributeDescription, 3>
    getAttributeDescriptions(uint32_t binding,
                             std::array<uint32_t, 3> locations) {
      std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
      attributeDescriptions[0].binding = binding;
      attributeDescriptions[0].location = locations.at(0);
      attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[0].offset = offsetof(VertexNormalTexture, position);

      attributeDescriptions[1].binding = binding;
      attributeDescriptions[1].location = locations.at(1);
      attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[1].offset =
          offsetof(VertexNormalTexture, normalAxis);

      attributeDescriptions[2].binding = binding;
      attributeDescriptions[2].location = locations.at(2);
      attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
      attributeDescriptions[2].offset =
          offsetof(VertexNormalTexture, textureCoordinate);
      return attributeDescriptions;
    }
  };

  template <typename Vertex>
  VkVertexInputBindingDescription
  getVertexBindingDescription(uint32_t binding) {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = binding;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
  }

  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
                                                VkSurfaceKHR surface) {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                              &details.capabilities);
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         nullptr);
    if (formatCount != 0) {
      details.formats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                           details.formats.data());
    }
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
                                              &presentModeCount, nullptr);
    if (presentModeCount != 0) {
      details.presentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(
          device, surface, &presentModeCount, details.presentModes.data());
    }
    return details;
  }

  struct GraphicsPresentQueueFramilyIndices {
    std::optional<uint32_t> graphicsFamilyIndice;
    uint32_t graphicsFamilyQueueCount;
    std::optional<uint32_t> presentFamilyIndice;
    uint32_t presentFamilyQueueCount;

    VkBool32 isComplete() {
      return graphicsFamilyIndice.has_value() &&
             presentFamilyIndice.has_value();
    }
  };
  struct ComputeQueueFamilyIndices {
    std::optional<uint32_t> computeFamilyIndice;
    uint32_t computeFamilyQueueCount;

    VkBool32 isComplete() { return computeFamilyIndice.has_value(); }
  };
  struct TransferQueueFamilyIndices {
    std::optional<uint32_t> transferFamilyIndice;
    uint32_t transferFamilyQueueCount;

    VkBool32 isComplete() { return transferFamilyIndice.has_value(); }
  };
  struct ProtectedQueueFamilyIndices {
    std::optional<uint32_t> protectedFamilyIndice;
    uint32_t protectedFamilyQueueCount;

    VkBool32 isComplete() { return protectedFamilyIndice.has_value(); }
  };

  GraphicsPresentQueueFramilyIndices
  findGraphicsPresentQueueFamilyIndices(VkPhysicalDevice device,
                                        VkSurfaceKHR *surface) {
    uint32_t queueFamilyPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyPropertiesCount, nullptr);
    if (queueFamilyPropertiesCount <= 0) {
      std::runtime_error("Failed to get Queue Families from Physical Device");
    }

    GraphicsPresentQueueFramilyIndices queueFamilyIndices;

    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    queueFamilyProperties.resize(queueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyPropertiesCount, queueFamilyProperties.data());
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

  ComputeQueueFamilyIndices
  findComputeQueueFamilyIndices(VkPhysicalDevice device) {
    uint32_t queueFamilyPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyPropertiesCount, nullptr);
    if (queueFamilyPropertiesCount <= 0) {
      std::runtime_error("Failed to get Queue Families from Physcial Device");
    }

    ComputeQueueFamilyIndices queueFamilyIndices;

    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    queueFamilyProperties.resize(queueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyPropertiesCount, queueFamilyProperties.data());
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

  TransferQueueFamilyIndices
  findTransferQueueFamilyIndices(VkPhysicalDevice device) {
    uint32_t queueFamilyPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyPropertiesCount, nullptr);
    if (queueFamilyPropertiesCount <= 0) {
      std::runtime_error("Failed to get Queue Families from Physcial Device");
    }

    TransferQueueFamilyIndices queueFamilyIndices;

    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    queueFamilyProperties.resize(queueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyPropertiesCount, queueFamilyProperties.data());
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

  ProtectedQueueFamilyIndices
  findProtectedQueueFamilyIndices(VkPhysicalDevice device) {
    uint32_t queueFamilyPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyPropertiesCount, nullptr);
    if (queueFamilyPropertiesCount <= 0) {
      std::runtime_error("Failed to get Queue Families from Physcial Device");
    }

    ProtectedQueueFamilyIndices queueFamilyIndices;

    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    queueFamilyProperties.resize(queueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyPropertiesCount, queueFamilyProperties.data());
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

  void initBlade() {
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
  }

  void setWindowSize(uint32_t width, uint32_t height) {
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
  }

  void mainLoop() {
    while (!glfwWindowShouldClose(_window)) {
      glfwPollEvents();
    }
  }

private:
  // VULKAN BASE SETUP
  GLFWwindow *_window;
  VkSurfaceKHR _windowSurface;
  VkInstance _instance;
  VkPhysicalDevice _physicalDevice;
  VkQueue _graphicsQueue;
  VkQueue _presentQueue;
  const std::vector<const char *> _deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  };
  VkDevice _device;

  uint32_t WINDOW_WIDTH = 800;
  uint32_t WINDOW_HEIGHT = 600;

  // SWAP CHAIN SETUP
  VkSwapchainKHR _swapChain;
  std::vector<VkImage> _swapChainImages;
  std::vector<VkImageView> _swapChainImageViews;
  VkFormat _swapChainImageFormat;
  VkExtent2D _swapChainExtent;
  std::vector<VkFramebuffer> _swapChainFramebuffers;
  VkRenderPass _renderPass;

  // UNIFORM BUFFER
  VkDescriptorSetLayout _descriptorSetLayout;

  // GRAPHICS PIPELINE
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;

  // Drawing
  VkCommandPool _commandPool;

  // Depth Buffer
  VkImage _depthImage;
  VkDeviceMemory _depthImageMemory;
  VkImageView _depthImageView;

  void initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BLADE GDK",
                               nullptr, nullptr);
  }

  void createInstance() {
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

    if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
      std::runtime_error("Failed to create Vulkan Instance!");
    }
    std::cout << "Created Instance" << std::endl;
  }

  void createWindowSurface() {
    if (glfwCreateWindowSurface(_instance, _window, nullptr, &_windowSurface) !=
        VK_SUCCESS) {
      std::runtime_error("Failed to create GLFW Window Surface!");
    }
    std::cout << "Created GLFW Window Surface" << std::endl;
  }

  VkBool32 checkDeviceExtensionSupport(VkPhysicalDevice device) {
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

  VkBool32 checkDeviceSuitability(VkPhysicalDevice physicalDevice,
                                  VkSurfaceKHR windowSurface) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(physicalDevice, &features);

    GraphicsPresentQueueFramilyIndices GPQueueFamilyIndices =
        findGraphicsPresentQueueFamilyIndices(physicalDevice, &windowSurface);

    VkBool32 swapChainAdequate = false;
    if (checkDeviceExtensionSupport(physicalDevice) == VK_TRUE) {
      SwapChainSupportDetails swapChainSupportDetails =
          querySwapChainSupport(physicalDevice, windowSurface);
      swapChainAdequate = !swapChainSupportDetails.formats.empty() &&
                          !swapChainSupportDetails.presentModes.empty();
    }

    return features.samplerAnisotropy && GPQueueFamilyIndices.isComplete() &&
           swapChainAdequate;
  }

  void pickPhysicalDevice() {
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
      if (checkDeviceSuitability(physicalDevice, _windowSurface) == VK_TRUE) {
        _physicalDevice = physicalDevice;
        std::cout << "Selected Vulkan Supported Physical Device!" << std::endl;
        return;
      }
    }
    std::runtime_error("Failed to select suitable Physcial Device");
  }
  void createLogicalDevice() {
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
    vkGetDeviceQueue(_device, GPQueueFamilyIndices.presentFamilyIndice.value(),
                     0, &_presentQueue);
  }

  VkSurfaceFormatKHR
  pickSurfaceFormat(const std::vector<VkSurfaceFormatKHR> formats) {
    for (const auto &availableFormat : formats) {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
          availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
      }
    }

    return formats[0];
  }

  VkPresentModeKHR
  pickPresentMode(const std::vector<VkPresentModeKHR> presentModes) {
    for (const auto &availablePresentMode : presentModes) {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        return availablePresentMode;
      }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  VkExtent2D pickExtent(GLFWwindow *window,
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
  void createSwapChain() {
    SwapChainSupportDetails swapChainSupport =
        querySwapChainSupport(_physicalDevice, _windowSurface);

    VkSurfaceFormatKHR surfaceFormat =
        pickSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode =
        pickPresentMode(swapChainSupport.presentModes);
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

  VkImageView createImageView(VkImage image, VkFormat format,
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

  void createImageViews() {
    _swapChainImageViews.resize(_swapChainImages.size());
    for (size_t i = 0; i < _swapChainImages.size(); i++) {
      _swapChainImageViews[i] =
          createImageView(_swapChainImages[i], _swapChainImageFormat,
                          VK_IMAGE_ASPECT_COLOR_BIT);
    }
    std::cout << "Created Image Views" << std::endl;
  }

  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
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

  VkFormat findDepthFormat() {
    return findSupportedFormat({VK_FORMAT_D32_SFLOAT,
                                VK_FORMAT_D32_SFLOAT_S8_UINT,
                                VK_FORMAT_D24_UNORM_S8_UINT},
                               VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
  }

  void createRenderPass() {
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
    depthAttachmentRef.layout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

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
  void createDescriptorSetLayout() {

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

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {
        layoutBinding, samplerLayoutBinding};

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

  VkShaderModule createShaderModule(const std::vector<char> &code) {
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

  void createGraphicsPipeline() {
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

    auto bindingDescription =
        getVertexBindingDescription<VertexColorTexture>(0);
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
    rasterizer.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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
    dynamicState.dynamicStateCount =
        static_cast<uint32_t>(dynamicStates.size());
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
  void createCommandPool() {
    GraphicsPresentQueueFramilyIndices GPQueueFamilyIndices =
        findGraphicsPresentQueueFamilyIndices(_physicalDevice, &_windowSurface);
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex =
        GPQueueFamilyIndices.graphicsFamilyIndice.value();
    if (vkCreateCommandPool(_device, &poolInfo, nullptr, &_commandPool) !=
        VK_SUCCESS) {
      throw std::runtime_error("failed to create command pool!");
    }
    std::cout << "Created Command Pool" << std::endl;
  }

  VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice,
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
  VkFormat findDepthFormat(VkPhysicalDevice physicalDevice) {
    return findSupportedFormat(physicalDevice,
                               {VK_FORMAT_D32_SFLOAT,
                                VK_FORMAT_D32_SFLOAT_S8_UINT,
                                VK_FORMAT_D24_UNORM_S8_UINT},
                               VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
  }
  uint32_t findMemoryType(uint32_t typeFilter,
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
  void createImage(VkFormat format, VkImageTiling tiling,
                   VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
                   VkImage &image, VkDeviceMemory &imageMemory) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = _swapChainExtent.width;
    imageInfo.extent.height = _swapChainExtent.height;
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

  VkCommandBuffer beginSingleTimeCommands() {
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
  void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
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

  void transitionImageLayout(VkImage image, VkFormat format,
                             VkImageLayout oldLayout, VkImageLayout newLayout) {
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

  void createDepthResources() {
    VkFormat depthFormat = findDepthFormat(_physicalDevice);

    createImage(depthFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _depthImage,
                _depthImageMemory);
    _depthImageView =
        createImageView(_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    transitionImageLayout(_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    std::cout << "Created Depth Resources" << std::endl;
  }

  void createFrameBuffers();
  void createTextureImage();
  void createTextureImageView();
  void createTextureSampler();
  void createVertexBuffer();
  void createIndexBuffer();
  void createUniformBuffer();
  void createDescriptorPool();
  void createDescriptorSets();
  void createCommandBuffers();
  void createSyncObjects();
  void cleanup();
};
}; // namespace BladeGDK
