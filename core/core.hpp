// Core Functions for the Back-End of the BLADE Game Development Kit

#pragma once
#include "coreComponents.hpp"

namespace BladeGDK {
class Core {
public:
  struct Color {
    float _red;
    float _green;
    float _blue;
    float _alpha;

    static Color red();
    static Color green();
    static Color blue();
    static Color color(float red, float green, float blue, float alpha);
  };

  struct UniformBufferObject {
    glm::vec3 cameraPosition;
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
  };

  struct VertexColor {
    glm::vec3 position;
    glm::vec3 color;

    static std::array<VkVertexInputAttributeDescription, 2>
    getAttributeDescriptions(uint32_t binding,
                             std::array<uint32_t, 2> locations);
  };
  struct VertexColorTexture {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 textureCoordinate;
    static std::array<VkVertexInputAttributeDescription, 3>
    getAttributeDescriptions(uint32_t binding,
                             std::array<uint32_t, 3> locations);
  };

  struct VertexNormalTexture {
    glm::vec3 position;
    glm::vec3 normalAxis;
    glm::vec2 textureCoordinate;
    static std::array<VkVertexInputAttributeDescription, 3>
    getAttributeDescriptions(uint32_t binding,
                             std::array<uint32_t, 3> locations);
  };

  template <typename Vertex>
  VkVertexInputBindingDescription getVertexBindingDescription(uint32_t binding);

  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
                                                VkSurfaceKHR surface);

  struct GraphicsPresentQueueFramilyIndices {
    std::optional<uint32_t> graphicsFamilyIndice;
    uint32_t graphicsFamilyQueueCount;
    std::optional<uint32_t> presentFamilyIndice;
    uint32_t presentFamilyQueueCount;

    VkBool32 isComplete();
  };
  struct ComputeQueueFamilyIndices {
    std::optional<uint32_t> computeFamilyIndice;
    uint32_t computeFamilyQueueCount;

    VkBool32 isComplete();
  };
  struct TransferQueueFamilyIndices {
    std::optional<uint32_t> transferFamilyIndice;
    uint32_t transferFamilyQueueCount;

    VkBool32 isComplete();
  };
  struct ProtectedQueueFamilyIndices {
    std::optional<uint32_t> protectedFamilyIndice;
    uint32_t protectedFamilyQueueCount;

    VkBool32 isComplete();
  };

  GraphicsPresentQueueFramilyIndices
  findGraphicsPresentQueueFamilyIndices(VkPhysicalDevice device,
                                        VkSurfaceKHR *surface);

  ComputeQueueFamilyIndices
  findComputeQueueFamilyIndices(VkPhysicalDevice device);

  TransferQueueFamilyIndices
  findTransferQueueFamilyIndices(VkPhysicalDevice device);

  ProtectedQueueFamilyIndices
  findProtectedQueueFamilyIndices(VkPhysicalDevice device);

  std::vector<VertexColorTexture> _vertices;
  std::vector<uint32_t> _indices;

  void initBlade();

  void setWindowSize(uint32_t width, uint32_t height);

  void mainLoop(Camera *camera);

private:
  void initWindow();

  void createInstance();

  void createWindowSurface();

  VkBool32 checkDeviceExtensionSupport(VkPhysicalDevice device);

  VkBool32 checkDeviceSuitability(VkPhysicalDevice physicalDevice);

  void pickPhysicalDevice();
  void createLogicalDevice();

  VkSurfaceFormatKHR
  pickSurfaceFormat(const std::vector<VkSurfaceFormatKHR> formats);

  VkPresentModeKHR
  pickPresentMode(const std::vector<VkPresentModeKHR> presentModes);

  VkExtent2D pickExtent(GLFWwindow *window,
                        const VkSurfaceCapabilitiesKHR &capabilities);

  void createSwapChain();

  VkImageView createImageView(VkImage image, VkFormat format,
                              VkImageAspectFlags aspectFlags);

  void createImageViews();

  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);

  VkFormat findDepthFormat();

  void createRenderPass();

  void createDescriptorSetLayout();
  std::vector<char> readFile(const std::string &filename);

  VkShaderModule createShaderModule(const std::vector<char> &code);
  // VkShaderModule createShaderModule(const std::vector<unsigned int> &code);
  // VkShaderModule createShaderModule(const std::string &code);

  void createGraphicsPipeline();

  void createCommandPool();

  VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice,
                               const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);

  VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

  uint32_t findMemoryType(uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);

  void createImage(uint32_t width, uint32_t height, VkFormat format,
                   VkImageTiling tiling, VkImageUsageFlags usage,
                   VkMemoryPropertyFlags properties, VkImage &image,
                   VkDeviceMemory &imageMemory);

  VkCommandBuffer beginSingleTimeCommands();

  void endSingleTimeCommands(VkCommandBuffer commandBuffer);

  void transitionImageLayout(VkImage image, VkFormat format,
                             VkImageLayout oldLayout, VkImageLayout newLayout);

  void createDepthResources();

  void createFrameBuffers();

  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags properties, VkBuffer &buffer,
                    VkDeviceMemory &bufferMemory);

  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                         uint32_t height);

  void createTextureImage(const char *imagePath);
  void createTextureImageView();
  void createTextureSampler();

  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

  template <typename Vertex>
  void createVertexBuffer(std::vector<Vertex> vertices,
                          VkDeviceSize bufferSize);

  template <typename IndicesType>
  void createIndexBuffer(std::vector<IndicesType> indices,
                         VkDeviceSize bufferSize);

  void createUniformBuffer(VkDeviceSize bufferSize);

  void createDescriptorPool();

  void createDescriptorSets();

  void createCommandBuffers();

  void createSyncObjects();

  void updateUniformBuffer(uint32_t currentImage, float aspectRatio,
                           Camera *camera);

  void recordElementsCommandBuffer(uint32_t imageIndex);

  void drawFrame(Camera *camera);

  void cleanup();
};
}; // namespace BladeGDK
