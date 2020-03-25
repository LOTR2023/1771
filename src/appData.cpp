#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#define VK_IMPORT_SHADERS
#include "render/shaders/importshaders.hpp"

public enum  VkApplicationData::xOrY { x, y };

// Finds window Resolution on x or y axis of display
int VkApplicationData::determineWindowResolution(bool xOrY) {
      glfwInit();

      int xResolution, yResolution;
      glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), nullptr, nullptr, &xResolution, &yResolution);
      return (xOrY)? (uint32_t)xResolution * 0.6f: (int)yResolution * 0.6f;
}
