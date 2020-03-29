#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <tuple>

#define VK_IMPORT_SHADERS
#include "render/shaders/importshaders.hpp"

struct VkApplicationData {
public:
      std::tuple<uint32_t, uint32_t> determineWindowResolution();

      VkDebugUtilsMessengerEXT debugMessenger;

      // Holds X and Y resolution
      std::tuple<uint32_t, uint32_t> resolution = determineWindowResolution();
      uint32_t xResolution = std::get<0>(resolution), yResolution = std::get<1>(resolution);
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
      GLFWwindow* window;
      std::string appName = "1771";
      std::vector<VkShader> activeShaders;
      VkPipeline graphicsPipeline;
      VkRenderPass renderPass;
      VkPipelineLayout pipelineLayout;
};
