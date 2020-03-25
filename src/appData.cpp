#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#define VK_IMPORT_SHADERS
#include "render/shaders/importshaders.hpp"

public enum  VkApplicationData::xOrY { x, y };

// Finds window Resolution on x or y axis of display
std::tuple<uint32_t, uint32_t> VkApplicationData::determineWindowResolution(bool xOrY) {
      glfwInit();

      int xResolution, yResolution;
      glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), nullptr, nullptr, &xResolution, &yResolution);
      return std::make_tuple(xResolution, yResolution);
}
