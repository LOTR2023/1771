#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

void cleanupWin(GLFWwindow* window) {
  glfwDestroyWindow(window);

  glfwTerminate();
}
