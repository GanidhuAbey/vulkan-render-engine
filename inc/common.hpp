#pragma once

#include "data_formats.hpp"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <vector>

extern std::vector<data::Vertex2D> vertices;
extern std::vector<uint16_t> indices;

extern VkBuffer vertexBuffer;
extern VkBuffer indexBuffer;
