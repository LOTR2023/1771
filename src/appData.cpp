#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#define VK_IMPORT_SHADERS
#include "rndr/shaders/importshaders.hpp"

struct VkApplicationData {
public:
      int determineWindowResolution(bool xOrY) {
            glfwInit();

            int xResolution, yResolution;
            glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), nullptr, nullptr, &xResolution, &yResolution);
            return (xOrY)? (uint32_t)xResolution * 0.6f: (int)yResolution * 0.6f;
      }

      VkDebugUtilsMessengerEXT debugMessenger;
      const uint32_t resX = determineWindowResolution(true);
      const uint32_t resY = determineWindowResolution(false);
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
