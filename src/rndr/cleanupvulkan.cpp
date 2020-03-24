#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_INIT_VK
#include "initvulkan.hpp"

#define VK_APPDATA_VK
#include "appData.hpp"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#include <iostream>
#include <vector>

void cleanupVk(VkApplicationData& appData) {
      vkDestroyPipeline(appData.device, appData.graphicsPipeline, nullptr);
      vkDestroyPipelineLayout(appData.device, appData.pipelineLayout, nullptr);
      vkDestroyRenderPass(appData.device, appData.renderPass, nullptr);

      for (auto imageView : appData.swapChainImageViews) {
            vkDestroyImageView(appData.device, imageView, nullptr);
      }

      vkDestroySwapchainKHR(appData.device, appData.swapChain, nullptr);
      vkDestroyDevice(appData.device, nullptr);

      if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(appData.instance, appData.debugMessenger, nullptr);
      }

      vkDestroySurfaceKHR(appData.instance, appData.surface, nullptr);
      vkDestroyInstance(appData.instance, nullptr);
}
