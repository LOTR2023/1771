#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#define VK_IMPORT_SHADERS
#include "render/shaders/importshaders.hpp"

#define OBJECT
#include "render/object/mesh.hpp"

class VkShader;
struct Vertex;

class VkApplicationData {
public:
  std::tuple<uint32_t, uint32_t> determineWindowResolution();

  static void framebufferResizeCallback(GLFWwindow *window, int width,
                                        int height);

  VkDebugUtilsMessengerEXT debugMessenger;

  // Holds X and Y resolution
  std::tuple<uint32_t, uint32_t> resolution = determineWindowResolution();
  uint32_t xResolution = std::get<0>(resolution),
           yResolution = std::get<1>(resolution);
  VkSurfaceKHR surface;
  VkQueue graphicsQueue;
  VkQueue presentQueue;
  VkDevice device;
  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  std::vector<VkImageView> swapChainImageViews;
  VkPhysicalDevice physicalDevice;
  VkInstance instance;
  GLFWwindow *window;
  std::string appName = "1771";
  std::vector<VkShader> activeShaders;
  VkPipeline graphicsPipeline;
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkCommandPool commandPool;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
  std::vector<VkCommandBuffer> commandBuffers;
  bool framebufferResized;
};
