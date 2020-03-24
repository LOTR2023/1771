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
      public:
            VkShader(const std::string& vertShader, const std::string& fragShader, VkApplicationData& appData);

            ~VkShader();

            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;

            VkPipelineShaderStageCreateInfo shaderStages[2];
                                                                      
            std::vector<char> vert;
            std::vector<char> frag;

            std::string vertShader;
            std::string fragShader;
      private:
            void createShaderModule(bool fragOrVert, VkApplicationData& appData);
            static std::vector<char> readFile(const std::string& filename);
};
