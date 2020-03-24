#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_INIT_WIN
#include "src/wndw/initwin.hpp"

#define VK_CLEANUP_WIN
#include "src/wndw/cleanupwin.hpp"

#define VK_INIT_VK
#include "src/rndr/initvulkan.hpp"

#define VK_VK
#include "src/rndr/vulkan.hpp"

#define VK_CLEANUP_VK
#include "src/rndr/cleanupvulkan.hpp"

#define VK_APPDATA_VK
#include "appData.hpp"

#include <iostream>
#include <vector>

//class that contains functions to run game and all scenes.
class VkApplication {
public:
 void run() {
   initWindow(appData);
   initVulkan(appData);
   mainLoop(appData);
   cleanupWin(appData.window);
   cleanupVk(appData);
 }

private:
  // All variables in VkApplication
  VkApplicationData appData;
};

int main() {
  VkApplication app;

  try {
    app.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
