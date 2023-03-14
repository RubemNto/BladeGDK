#pragma once
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include "camera.hpp"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <algorithm> // Necessary for std::clamp
#include <array>
#include <chrono>
#include <cstdint> // Necessary for uint32_t
#include <cstring>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #include <glslang/SPIRV/GlslangToSpv.h>
#include <filesystem>
#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include <string>
#include <vector>

// CORE BALDE GDK COMPONENTS
// VULKAN BASE SETUP
extern GLFWwindow *_window;
extern VkSurfaceKHR _windowSurface;
extern VkInstance _instance;
extern VkPhysicalDevice _physicalDevice;
extern VkQueue _graphicsQueue;
extern VkQueue _presentQueue;

extern const std::vector<const char *> _deviceExtensions;

extern VkDevice _device;

extern uint32_t WINDOW_WIDTH;
extern uint32_t WINDOW_HEIGHT;

// SWAP CHAIN SETUP
extern VkSwapchainKHR _swapChain;
extern std::vector<VkImage> _swapChainImages;
extern std::vector<VkImageView> _swapChainImageViews;
extern VkFormat _swapChainImageFormat;
extern VkExtent2D _swapChainExtent;
extern std::vector<VkFramebuffer> _swapChainFramebuffers;
extern VkRenderPass _renderPass;

// UNIFORM BUFFER
extern VkDescriptorSetLayout _descriptorSetLayout;
extern std::vector<VkBuffer> _uniformBuffers;
extern std::vector<VkDeviceMemory> _uniformBuffersMemory;
extern VkPipelineLayout _pipelineLayout;
extern VkDescriptorPool _descriptorPool;
extern std::vector<VkDescriptorSet> _descriptorSets;

// GRAPHICS PIPELINE
extern VkPipelineLayout pipelineLayout;
extern VkPipeline graphicsPipeline;

// DRAWING
extern const int FRAMES_IN_FLIGHT;

extern VkCommandPool _commandPool;
extern uint32_t _currentFrame;
extern std::vector<VkCommandBuffer> _commandBuffer;

extern std::vector<VkSemaphore> imageAvailableSemaphores;
extern std::vector<VkSemaphore> renderFinishedSemaphores;
extern std::vector<VkFence> inFlightFences;

// DEPTH BUFFER
extern VkImage _depthImage;
extern VkDeviceMemory _depthImageMemory;
extern VkImageView _depthImageView;

// TEXTURE IMAGE
extern VkImageView _textureImageView;
extern VkImage _textureImage;
extern VkSampler _textureSampler;
extern VkDeviceMemory _textureImageMemory;

// VERTEX BUFFER
extern VkBuffer _vertexBuffer;
extern VkDeviceMemory _vertexBufferMemory;

// INDEX BUFFER
extern VkBuffer _indexBuffer;
extern VkDeviceMemory _indexBufferMemory;