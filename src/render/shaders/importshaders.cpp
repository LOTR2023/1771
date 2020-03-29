#include "importshaders.hpp"


#define VK_APPDATA_VK
#include "appData.hpp"


            VkShader::VkShader(const std::string& vertShader, const std::string& fragShader, VkDevice& device) {
                  vert = readFile(vertShader);
                  frag = readFile(fragShader);

                  this->vertShader = vertShader;
                  this->fragShader = fragShader;
                  this->device = device;

                  createShaderModule(true, device);
                  createShaderModule(false, device);

                  shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                  shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
                  shaderStages[0].module = vertShaderModule;
                  shaderStages[0].pName = "main";

                  shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                  shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                  shaderStages[1].module = fragShaderModule;
                  shaderStages[1].pName = "main";
            }

            VkShader::~VkShader() {
                  vkDestroyShaderModule(device, vertShaderModule, nullptr);
                  vkDestroyShaderModule(device, fragShaderModule, nullptr);
            }


            void VkShader::createShaderModule(bool fragOrVert, VkDevice& device) {
                  VkShaderModuleCreateInfo createInfo = {};
                  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                  createInfo.codeSize = (fragOrVert)? frag.size():vert.size();
                  if(fragOrVert) {
                  createInfo.pCode = reinterpret_cast<const uint32_t*>(frag.data());
                  } else {
                  createInfo.pCode = reinterpret_cast<const uint32_t*>(vert.data());
                  }
                  if(vkCreateShaderModule(device, &createInfo, nullptr, (fragOrVert)? &fragShaderModule: &vertShaderModule) != VK_SUCCESS) {
                        throw std::runtime_error("failed to create shader module!");
                  }
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
