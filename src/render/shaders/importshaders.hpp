#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_APPDATA_VK
#include "appData.hpp"

class VkShader {
      public:
            VkShader(const std::string& vertShader, const std::string& fragShader, VkDevice device);
            void createShaderModules();

            // ~VkShader();

            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;

            VkDevice device;

            VkPipelineShaderStageCreateInfo shaderStagesVert;
            VkPipelineShaderStageCreateInfo shaderStagesFrag;

            VkPipelineShaderStageCreateInfo shaderStages[2];

            std::vector<char> vert;
            std::vector<char> frag;

            std::string vertShader;
            std::string fragShader;
      private:
            VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
            static std::vector<char> readFile(const std::string& filename);
};
