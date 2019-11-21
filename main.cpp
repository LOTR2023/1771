#define VK_INIT_WIN
#include "src/wndw/initwin.hpp"

#define VK_INIT_VK
#include "src/rndr/initvulkan.hpp"

#define VK_VK
#include "src/rndr/vulkan.hpp"

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
