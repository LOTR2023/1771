#include "importshaders.hpp"


#define VK_APPDATA_VK
#include "appData.hpp"


VkShader::VkShader(const std::string& vertShader, const std::string& fragShader, VkDevice device) {
      vert = readFile(vertShader);
      frag = readFile(fragShader);

      this->vertShader = vertShader;
      this->fragShader = fragShader;
      this->device = device;
}

void VkShader::createShaderModules() {
      vertShaderModule = createShaderModule(vert, device);
      fragShaderModule = createShaderModule(frag, device);

      shaderStagesVert.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      shaderStagesVert.stage = VK_SHADER_STAGE_VERTEX_BIT;
      shaderStagesVert.module = vertShaderModule;
      shaderStagesVert.pName = "main";
      shaderStagesVert.flags = 0x0;
      shaderStagesVert.pNext = nullptr;
      shaderStagesVert.pSpecializationInfo = nullptr;


      shaderStagesFrag.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      shaderStagesFrag.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      shaderStagesFrag.module = fragShaderModule;
      shaderStagesFrag.pName = "main";
      shaderStagesFrag.flags = 0x0;
      shaderStagesFrag.pNext = nullptr;
      shaderStagesFrag.pSpecializationInfo = nullptr;

      shaderStages[0] = shaderStagesVert;
      shaderStages[1] = shaderStagesFrag;
}

VkShaderModule VkShader::createShaderModule(const std::vector<char>& code, VkDevice device) {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;

        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
}

std::vector<char> VkShader::readFile(const std::string& filename) {
      std::ifstream file(filename, std::ios::ate | std::ios::binary);

      if(!file.is_open()) {
            std::cout << "failed to open file: " << filename << std::endl;
            throw std::exception();
      }

      size_t fileSize = (size_t) file.tellg();
      std::vector<char> buffer(fileSize);

      file.seekg(0);
      file.read(buffer.data(), fileSize);

      return buffer;
}
