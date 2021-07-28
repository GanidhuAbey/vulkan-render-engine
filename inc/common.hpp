#pragma once

#include "data_formats.hpp"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <vector>

extern uint16_t indexCount;
extern uint16_t vertexCount;
extern uint16_t objectCount;

extern VkBuffer vertexBuffer;
extern VkBuffer indexBuffer;
