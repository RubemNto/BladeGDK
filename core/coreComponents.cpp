#include "coreComponents.hpp"

// CORE BALDE GDK COMPONENTS
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
std::vector<VkBuffer> _uniformBuffers;
std::vector<VkDeviceMemory> _uniformBuffersMemory;
VkPipelineLayout _pipelineLayout;
VkDescriptorPool _descriptorPool;
std::vector<VkDescriptorSet> _descriptorSets;

// GRAPHICS PIPELINE
VkPipelineLayout pipelineLayout;
VkPipeline graphicsPipeline;

// DRAWING
const int FRAMES_IN_FLIGHT = 3;
VkCommandPool _commandPool;
uint32_t _currentFrame = 0;
std::vector<VkCommandBuffer> _commandBuffer;

std::vector<VkSemaphore> imageAvailableSemaphores;
std::vector<VkSemaphore> renderFinishedSemaphores;
std::vector<VkFence> inFlightFences;

// DEPTH BUFFER
VkImage _depthImage;
VkDeviceMemory _depthImageMemory;
VkImageView _depthImageView;

// TEXTURE IMAGE
VkImageView _textureImageView;
VkImage _textureImage;
VkSampler _textureSampler;
VkDeviceMemory _textureImageMemory;

// VERTEX BUFFER
VkBuffer _vertexBuffer;
VkDeviceMemory _vertexBufferMemory;

// INDEX BUFFER
VkBuffer _indexBuffer;
VkDeviceMemory _indexBufferMemory;