#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<iostream>
#include<array>

struct Vertex {
      glm::vec2 position;
      glm::vec3 color;

      static VkVertexInputBindingDescription getBindingDescription();

      static std::array<VkVertexInputAttributeDescription, 2> getAttributePerVertexDescriptions();

      void print();
};
