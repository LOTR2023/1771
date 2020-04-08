#pragma once

#define VK_APPDATA_VK
#include "../../appData.hpp"

#include <cstring>
#include <map>
#include <set>
#include <string>

#define VK_CLEANUP_WIN
#include "cleanupvulkan.hpp"

#define VK_IMPORT_SHADERS
#include "render/shaders/importshaders.hpp"

void initVulkan(VkApplicationData &appData);

void recreateSwapChain(VkApplicationData &appData);

void DestroyDebugUtilsMessengerEXT(VkInstance &instance,
                                   VkDebugUtilsMessengerEXT &debugMessenger,
                                   const VkAllocationCallbacks *pAllocator);
