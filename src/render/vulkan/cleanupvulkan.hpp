#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_APPDATA_VK
#include "../../appData.hpp"

void cleanupVk(VkApplicationData &appData);

void cleanupSwapChain(VkApplicationData &appData);
