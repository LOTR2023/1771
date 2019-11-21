#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_INIT_WIN
#include "wndw/initwin.hpp"

#define VK_INIT_VK
#include "rndr/initvulkan.hpp"

#define VK_VK
#include "rndr/vulkan.hpp"

#include <iostream>

class VkApplication {
public:
 void run() {
   initWindow("VkEngine");
   initVulkan();
   mainLoop();
   cleanup();
 }
private:

  void cleanup() {

  }
};

int main() {
  VkApplication app;

  try {
    app.run();
  } catch (const std::exception e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
