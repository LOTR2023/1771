#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <map>
#include <set>

#define VK_APPDATA_VK
#include "appData.hpp"

#define VK_IMPORT_SHADERS
#include "rndr/shaders/importshaders.hpp"


const std::vector<const char*> validationLayers = {
      "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::string defaultVertShader = "src/rndr/shaders/default/vert.spv";;
const std::string defaultFragShader = "src/rndr/shaders/default/frag.spv";

static VkInstance instance;
static VkSurfaceKHR surface;

static VkQueue presentQueue;

VkDebugUtilsMessengerEXT vkDebugMessenger;

uint32_t glfwExtensionCount = 0;
const char** glfwExtensions;

uint32_t extensionCount = 0;

std::vector<VkExtensionProperties> extensions(extensionCount);

VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

struct QueueFamilyIndices {
      std::optional<uint32_t> graphicsFamily;
      std::optional<uint32_t> presentFamily;

      bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
      }
};

struct SwapChainSupportDetails {
      VkSurfaceCapabilitiesKHR capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;
};


bool checkDeviceExtensionSupport(VkPhysicalDevice device);
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
      QueueFamilyIndices indices;
      VkBool32 presentSupport = false;

      uint32_t queueFamilyCount = 0;
      //Enumerates wether certain Queues exist on the graphics card
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

      int i = 0;

      // Searches Queues for a queue that supports GRAPHICS_BIT
      for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                  indices.graphicsFamily = i;
            }

            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            indices.presentFamily = (presentSupport)? i:indices.presentFamily;
            if (indices.isComplete()) { break; }
            i++;
      }

      // Logic to find queue family indices to populate struct with
      return indices;
}

// validation Layers make sure no hardware problems are detected
// that would otherwise give an unknown error.
bool checkValidationLayerSupport() {
      uint32_t layerCount;
      vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

      std::vector<VkLayerProperties> availableLayers(layerCount);
      vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

      for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                  if (strcmp(layerName, layerProperties.layerName) == 0) {
                        layerFound = true;
                        break;
                  }
            }

            if (!layerFound) {
                  return false;
            }
      }

      return true;
}

std::vector<const char*> getRequiredExtensions() {
      uint32_t glfwExtensionCount = 0;
      const char** glfwExtensions;
      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

      std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

      if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      }

      return extensions;
}


static VkInstance createInstance(std::string appName) {
      if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
      }

      VkApplicationInfo appInfo = {};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = appName.c_str();
      appInfo.applicationVersion = VK_MAKE_VERSION(1, 3, 0);
      appInfo.pEngineName = "VkEngine";
      appInfo.engineVersion = VK_MAKE_VERSION(1, 3, 0);
      appInfo.apiVersion = VK_API_VERSION_1_1;

      VkInstanceCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      createInfo.pApplicationInfo = &appInfo;

      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

      if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
      } else {
            createInfo.enabledLayerCount = 0;
      }
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

      std::cout << "available extensions:" << std::endl;

      for (const auto& extension : extensions) {
            std::cout << "\t" << extension.extensionName << std::endl;
      }

      auto extensions = getRequiredExtensions();
      createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
      createInfo.ppEnabledExtensionNames = extensions.data();

      if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
      }


      std::cout << "created instance!" << '\n';
      return instance;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
      void* pUserData) {

            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

            return VK_FALSE;
      }

      static VkResult CreateDebugUtilsMessengerEXT(VkInstance& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
            auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
            if (func != nullptr) {
                  return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
            } else {
                  return VK_ERROR_EXTENSION_NOT_PRESENT;
            }
      }

      void DestroyDebugUtilsMessengerEXT(VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger, const VkAllocationCallbacks* pAllocator) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                  func(instance, debugMessenger, pAllocator);
            }
      }

      void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
            createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debugCallback;
      }

      VkDebugUtilsMessengerEXT setupDebugMessenger(VkInstance& vkInstance) {

            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            populateDebugMessengerCreateInfo(createInfo);

            if (CreateDebugUtilsMessengerEXT(vkInstance, &createInfo, nullptr, &vkDebugMessenger) != VK_SUCCESS) {
                  throw std::runtime_error("failed to set up debug messenger!");
            }

            std::cout << "set up debug messenger!" << '\n';
            return vkDebugMessenger;
      }

      VkSurfaceKHR createSurface(VkApplicationData appData) {
            if(glfwCreateWindowSurface(appData.instance, appData.window, nullptr, &surface) != VK_SUCCESS) {
                  throw std::runtime_error("Failed to create surface!");
            }

            return surface;
      }

      int rateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR& surface) {
            QueueFamilyIndices indices = findQueueFamilies(device);

            VkPhysicalDeviceProperties deviceProperties;
            VkPhysicalDeviceFeatures deviceFeatures;
            vkGetPhysicalDeviceProperties(device, &deviceProperties);
            vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

            int score = 0;

            bool extensionsSupported = checkDeviceExtensionSupport(device);

            // Discrete GPUs have a significant performance advantage
            if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                  score += 1000;
            }

            // Maximum possible size of textures affects graphics quality
            score += deviceProperties.limits.maxImageDimension2D;
            score += (extensionsSupported)? 1000:-score;

            bool swapChainAdequate = false;
            if(extensionsSupported) {
                  SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
                  swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
            }

            score = (swapChainAdequate)? score: 0;

            // Application can't function without geometry shaders
            if (!deviceFeatures.geometryShader) {
                  return 0;
            }

            std::string types[] = {"unknown", "integrated graphics", "discrete graphics", "virtual graphics", "software"};

            std::cout << "Device score: " << score << ", Name: " << deviceProperties.deviceName << ", Type: "<< types[deviceProperties.deviceType]<< std::endl;
            return score;
      }

      bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

            std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

            for(const auto& extension : availableExtensions) {
                  requiredExtensions.erase(extension.extensionName);
            }

            return requiredExtensions.empty();

      }

      VkPhysicalDevice pickPhysicalDevice(VkInstance& VkInstance, VkSurfaceKHR& surface) {
            uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices(VkInstance, &deviceCount, nullptr);

            if(deviceCount == 0) {
                  throw std::runtime_error("Failed to find GPUs with Vulkan support!");
            }

            std::vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(VkInstance, &deviceCount, devices.data());
            // first is int value, second is VkPhysicalDevice
            std::multimap<int, VkPhysicalDevice> candidates;

            for (const auto& device : devices) {
                  int score = rateDeviceSuitability(device, surface);
                  candidates.insert(std::make_pair(score, device));
            }

            QueueFamilyIndices indices = findQueueFamilies(candidates.rbegin()->second);

            // Check if the best candidate is suitable at all
            if (candidates.rbegin()->first > 0 && indices.isComplete()) {
                  physicalDevice = candidates.rbegin()->second;
            } else {
                  throw std::runtime_error("failed to find a suitable GPU!");
            }

            return physicalDevice;
      }

      VkDevice createLogicalDevice(VkQueue& graphicsQueue) {
            VkDevice device;

            QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

            // Fills a struct with info about a logical device
            std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = {};
            std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

            float queuePriority = 1.0f;
            for (uint32_t queueFamily: uniqueQueueFamilies) {
                  VkDeviceQueueCreateInfo queueCreateInfo = {};
                  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                  queueCreateInfo.queueFamilyIndex = queueFamily;
                  queueCreateInfo.queueCount = 1;
                  queueCreateInfo.pQueuePriorities = &queuePriority;
                  queueCreateInfos.push_back(queueCreateInfo);
            }

            VkPhysicalDeviceFeatures deviceFeatures = {};

            VkDeviceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

            createInfo.pQueueCreateInfos     = queueCreateInfos.data();
            createInfo.queueCreateInfoCount  = static_cast<uint32_t>(queueCreateInfos.size());
            createInfo.pEnabledFeatures      = &deviceFeatures;
            createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
            createInfo.ppEnabledExtensionNames = deviceExtensions.data();

            if (enableValidationLayers) {
                  createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
                  createInfo.ppEnabledLayerNames = validationLayers.data();
            }  else {
                  createInfo.enabledLayerCount = 0;
            }

            if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
                  throw std::runtime_error("failed to create logical device!");
            }

            vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
            vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);

            return device;
      }

      VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
            for (const auto availableFormat : availableFormats) {
                  if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                        return availableFormat;
                  }
            }

            return availableFormats[0];
      }

      VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
            for (const auto& availablePresentMode : availablePresentModes) {
                  if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                        return availablePresentMode;
                  }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
      }

      VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t resX, uint32_t resY) {
            if (capabilities.currentExtent.width != UINT32_MAX) {
                  return capabilities.currentExtent;
            } else {
                  VkExtent2D actualExtent = {resX, resY};

                  actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
                  actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

                  return actualExtent;
            }
      }

      SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& device, VkSurfaceKHR& surface) {
            SwapChainSupportDetails details;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

            if(formatCount != 0) {
                  details.formats.resize(formatCount);
                  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

            if (presentModeCount != 0) {
                  details.presentModes.resize(presentModeCount);
                  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
            }
            return details;
      }

      VkSwapchainKHR createSwapChain(VkApplicationData& appData) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(appData.physicalDevice, appData.surface);

            VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
            VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
            VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, appData.resX, appData.resY);

            uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

            if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
                  imageCount = swapChainSupport.capabilities.maxImageCount;
            }

            VkSwapchainCreateInfoKHR createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = surface;
            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
            uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

            if (indices.graphicsFamily != indices.presentFamily) {
                  createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                  createInfo.queueFamilyIndexCount = 2;
                  createInfo.pQueueFamilyIndices = queueFamilyIndices;
            } else {
                  createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                  createInfo.queueFamilyIndexCount = 0;
                  createInfo.pQueueFamilyIndices = nullptr;
            }

            createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = presentMode;
            createInfo.clipped = VK_TRUE;
            createInfo.oldSwapchain = VK_NULL_HANDLE;

            VkSwapchainKHR swapChain;

            if (vkCreateSwapchainKHR(appData.device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
                  throw std::runtime_error("Failed to create swap chain!");
            }

            vkGetSwapchainImagesKHR(appData.device, swapChain, &imageCount, nullptr);
            appData.swapChainImages.resize(imageCount);
            vkGetSwapchainImagesKHR(appData.device, swapChain, &imageCount, appData.swapChainImages.data());

            appData.swapChainImageFormat = surfaceFormat.format;
            appData.swapChainExtent = extent;

            return swapChain;
      }

      std::vector<VkImageView> createImageViews(const std::vector<VkImage>& swapChainImages, VkFormat& swapChainImageFormat, VkDevice& device) {
            std::vector<VkImageView> swapChainImageViews(swapChainImages.size());

            for (size_t i = 0; i < swapChainImages.size(); i++) {
                  VkImageViewCreateInfo createInfo = {};
                  createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                  createInfo.image = swapChainImages[i];

                  createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                  createInfo.format = swapChainImageFormat;

                  createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                  createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                  createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                  createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

                  createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                  createInfo.subresourceRange.baseMipLevel = 0;
                  createInfo.subresourceRange.levelCount = 1;
                  createInfo.subresourceRange.baseArrayLayer = 0;
                  createInfo.subresourceRange.layerCount = 1;

                  if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
                        throw std::runtime_error("failed to create image views!");
                  }
            }

            return swapChainImageViews;
      }

      VkRenderPass createRenderPass(VkApplicationData& appData) {
            VkAttachmentDescription colorAttachment = {};
            colorAttachment.format = appData.swapChainImageFormat;
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference colorAttachmentRef = {};
            colorAttachmentRef.attachment = 0;
            colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass = {};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &colorAttachmentRef;

            VkRenderPass renderPass;

            VkRenderPassCreateInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = 1;
            renderPassInfo.pAttachments = &colorAttachment;
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpass;

            if (vkCreateRenderPass(appData.device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
                  throw std::runtime_error("failed to create render pass!");
            }

            return renderPass;
      }

      VkPipeline createGraphicsPipeline(VkApplicationData& appData) {
            appData.activeShaders.push_back(VkShader(defaultVertShader, defaultFragShader, appData));

            VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertexInputInfo.vertexBindingDescriptionCount = 0;
            vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
            vertexInputInfo.vertexAttributeDescriptionCount = 0;
            vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

            VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
            inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            inputAssembly.primitiveRestartEnable = VK_FALSE;

            VkViewport viewport = {};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float) appData.swapChainExtent.width;
            viewport.height = (float) appData.swapChainExtent.height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            VkRect2D scissor = {};
            scissor.offset = {0, 0};
            scissor.extent = appData.swapChainExtent;

            VkPipelineViewportStateCreateInfo viewportState = {};
            viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewportState.viewportCount = 1;
            viewportState.pViewports = &viewport;
            viewportState.scissorCount = 1;
            viewportState.pScissors = &scissor;

            VkPipelineRasterizationStateCreateInfo rasterizer = {};
            rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterizer.depthClampEnable = VK_FALSE;
            rasterizer.rasterizerDiscardEnable = VK_FALSE;
            rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
            rasterizer.lineWidth = 1.0f;
            rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
            rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
            rasterizer.depthBiasEnable = VK_FALSE;
            rasterizer.depthBiasConstantFactor = 0.0f; // Optional
            rasterizer.depthBiasClamp = 0.0f; // Optional
            rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

            VkPipelineMultisampleStateCreateInfo multisampling = {};
            multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multisampling.sampleShadingEnable = VK_FALSE;
            multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            multisampling.minSampleShading = 1.0f; // Optional
            multisampling.pSampleMask = nullptr; // Optional
            multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
            multisampling.alphaToOneEnable = VK_FALSE; // Optional

            VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
            colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            colorBlendAttachment.blendEnable = VK_FALSE;
            colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
            colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
            colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
            colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
            colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
            colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

            VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = 0; // Optional
            pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
            pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
            pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

            if (vkCreatePipelineLayout(appData.device, &pipelineLayoutInfo, nullptr, &appData.pipelineLayout) != VK_SUCCESS) {
                  throw std::runtime_error("failed to create pipeline layout!");
            }

            VkGraphicsPipelineCreateInfo pipelineInfo = {};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.stageCount = 2;
            pipelineInfo.pStages = appData.activeShaders[0].shaderStages;
            pipelineInfo.pVertexInputState = &vertexInputInfo;
            pipelineInfo.pInputAssemblyState = &inputAssembly;
            pipelineInfo.pViewportState = &viewportState;
            pipelineInfo.pRasterizationState = &rasterizer;
            pipelineInfo.pMultisampleState = &multisampling;
            pipelineInfo.pDepthStencilState = nullptr; // Optional
            pipelineInfo.pColorBlendState = &colorBlending;
            pipelineInfo.pDynamicState = nullptr; // Optional
            pipelineInfo.layout = pipelineLayout;
            pipelineInfo.renderPass = appData.renderPass;
            pipelineInfo.subpass = 0;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
            pipelineInfo.basePipelineIndex = -1; // Optional

            VkPipeline graphicsPipeline;

            if (vkCreateGraphicsPipelines(appData.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
                  throw std::runtime_error("failed to create graphics pipeline!");
            }

            return graphicsPipeline;
      }

      void initVulkan(VkApplicationData& appData) {
            appData.instance = createInstance(appData.appName);
            if(enableValidationLayers)
            appData.debugMessenger = setupDebugMessenger(appData.instance);
            appData.surface = createSurface(appData);
            appData.physicalDevice = pickPhysicalDevice(appData.instance, appData.surface);
            appData.device = createLogicalDevice(appData.graphicsQueue);
            appData.swapChain = createSwapChain(appData);
            appData.swapChainImageViews = createImageViews(appData.swapChainImages, appData.swapChainImageFormat, appData.device);
            appData.renderPass = createRenderPass(appData);
            appData.graphicsPipeline = createGraphicsPipeline(appData);
      }
