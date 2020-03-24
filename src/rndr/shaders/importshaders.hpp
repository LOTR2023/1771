#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_APPDATA_VK
#include "appData.hpp"

struct VkShader {
            VkShader(const std::string& vertShader, const std::string& fragShader, VkApplicationData& appData);

            ~VkShader();

            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;

            VkPipelineShaderStageCreateInfo shaderStages[2];
};
