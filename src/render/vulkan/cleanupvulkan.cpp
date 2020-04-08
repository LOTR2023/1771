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

void cleanupSwapChain(VkApplicationData& appData);

void cleanupVk(VkApplicationData& appData) {

      for (size_t i = 0; i < appData.activeShaders.size(); i++) {
            vkDestroyShaderModule(appData.device, appData.activeShaders[i].vertShaderModule, nullptr);
            vkDestroyShaderModule(appData.device, appData.activeShaders[i].fragShaderModule, nullptr);
      }

      vkDestroyPipeline(appData.device, appData.graphicsPipeline, nullptr);
      vkDestroyPipelineLayout(appData.device, appData.pipelineLayout, nullptr);

      cleanupSwapChain(appData);

      vkDestroyBuffer(appData.device, appData.indexBuffer, nullptr);
      vkFreeMemory(appData.device, appData.indexBufferMemory, nullptr);

      vkDestroyBuffer(appData.device, appData.vertexBuffer, nullptr);
      vkFreeMemory(appData.device, appData.vertexBufferMemory, nullptr);

      vkDestroyCommandPool(appData.device, appData.commandPool, nullptr);

      vkDestroyDevice(appData.device, nullptr);

      if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(appData.instance, appData.debugMessenger, nullptr);
      }

      vkDestroySurfaceKHR(appData.instance, appData.surface, nullptr);
      vkDestroyInstance(appData.instance, nullptr);
}


void cleanupSwapChain(VkApplicationData& appData) {


      for (size_t i = 0; i < appData.swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(appData.device, appData.swapChainFramebuffers[i], nullptr);
    }

    vkFreeCommandBuffers(appData.device, appData.commandPool, static_cast<uint32_t>(appData.commandBuffers.size()), appData.commandBuffers.data());

    vkDestroyRenderPass(appData.device, appData.renderPass, nullptr);

    for (size_t i = 0; i < appData.swapChainImageViews.size(); i++) {
        vkDestroyImageView(appData.device, appData.swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(appData.device, appData.swapChain, nullptr);
}
