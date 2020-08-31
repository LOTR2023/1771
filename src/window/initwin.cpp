#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_APPDATA_VK
#include "appData.hpp"

void initWindow(VkApplicationData& appData) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    appData.window = glfwCreateWindow(appData.xResolution, appData.yResolution, appData.appName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(appData.window, &appData);
    glfwSetWindowSizeLimits(appData.window, 480, 320, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetFramebufferSizeCallback(appData.window, appData.framebufferResizeCallback);
}
