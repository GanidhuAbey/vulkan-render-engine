#pragma once

#include "data_formats.hpp"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <vector>

<<<<<<< HEAD
extern uint16_t indexCount;
extern uint16_t vertexCount;
extern uint16_t objectCount;
=======
extern std::vector<data::Vertex2D> vertices;
extern std::vector<uint16_t> indices;
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9

extern VkBuffer vertexBuffer;
extern VkBuffer indexBuffer;
