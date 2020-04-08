#include"appData.hpp"
// Finds window Resolution on x or y axis of display
std::tuple<uint32_t, uint32_t> VkApplicationData::determineWindowResolution() {
      glfwInit();

      int xResolution, yResolution;
      glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), nullptr, nullptr, &xResolution, &yResolution);
      return std::make_tuple(xResolution, yResolution);
}

void VkApplicationData::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
      auto app = reinterpret_cast<VkApplicationData*>(glfwGetWindowUserPointer(window));

      if(width < app->swapChainExtent.width) {
            app->swapChainExtent.width = 200;
      }
      app->framebufferResized = true;
}
