#include "vulkan.hpp"

#define VK_INIT_VK
#include "initvulkan.hpp"

#define VK_CLEANUP_VK
#include "cleanupvulkan.hpp"

const int MAX_FRAMES_IN_FLIGHT = 2;
size_t currentFrame = 0;

bool framebufferResized = false;

std::vector<VkSemaphore> imageAvailableSemaphores(MAX_FRAMES_IN_FLIGHT);
std::vector<VkSemaphore> renderFinishedSemaphores(MAX_FRAMES_IN_FLIGHT);
std::vector<VkFence> inFlightFences(MAX_FRAMES_IN_FLIGHT);
std::vector<VkFence> imagesInFlight;

static void createSyncObjects(VkDevice device, uint32_t imagesInFlightSize);
static void drawFrame(VkApplicationData &appData);

void mainLoop(VkApplicationData &appData) {
  createSyncObjects(appData.device, appData.swapChainImages.size());

  while (!glfwWindowShouldClose(appData.window)) {
    glfwPollEvents();
    drawFrame(appData);
  }

  vkDeviceWaitIdle(appData.device);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroySemaphore(appData.device, renderFinishedSemaphores[i], nullptr);
    vkDestroySemaphore(appData.device, imageAvailableSemaphores[i], nullptr);
    vkDestroyFence(appData.device, inFlightFences[i], nullptr);
  }
}

// Creates objects used for synchronization of the
// CPU and GPU.
static void createSyncObjects(VkDevice device, uint32_t imagesInFlightSize) {
  imagesInFlight.resize(imagesInFlightSize, VK_NULL_HANDLE);

  VkSemaphoreCreateInfo semaphoreInfo = {};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo = {};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    if (vkCreateSemaphore(device, &semaphoreInfo, nullptr,
                          &imageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateSemaphore(device, &semaphoreInfo, nullptr,
                          &renderFinishedSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) !=
            VK_SUCCESS) {
      throw std::runtime_error(
          "failed to create synchronization objects for a frame!");
    }
  }
}

// Function that handles submitting command buffers and
// presenting image from swapchain to the surface.
static void drawFrame(VkApplicationData &appData) {
  vkWaitForFences(appData.device, 1, &inFlightFences[currentFrame], VK_TRUE,
                  UINT64_MAX);

  uint32_t imageIndex;
  VkResult result = vkAcquireNextImageKHR(
      appData.device, appData.swapChain, UINT64_MAX,
      imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      framebufferResized) {
    framebufferResized = false;
    recreateSwapChain(appData);
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  // Check if a previous frame is using this image (i.e. there is its fence to
  // wait on)
  if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
    vkWaitForFences(appData.device, 1, &imagesInFlight[imageIndex], VK_TRUE,
                    UINT64_MAX);
  }
  // Mark the image as now being in use by this frame
  imagesInFlight[imageIndex] = inFlightFences[currentFrame];

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &appData.commandBuffers[imageIndex];

  VkSemaphore signalSemaphores[] = {imageAvailableSemaphores[currentFrame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  vkResetFences(appData.device, 1, &inFlightFences[currentFrame]);

  if (vkQueueSubmit(appData.graphicsQueue, 1, &submitInfo,
                    inFlightFences[currentFrame]) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = {appData.swapChain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr; // Optional

  result = vkQueuePresentKHR(appData.presentQueue, &presentInfo);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    recreateSwapChain(appData);
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }

  currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
