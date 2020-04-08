#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_APPDATA_VK
#include "../../appData.hpp"

#define VK_INIT_VK
#include "initvulkan.hpp"

#define VK_CLEANUP_VK
#include "cleanupvulkan.hpp"

void mainLoop(VkApplicationData &appData);
