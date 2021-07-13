#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "data_formats.hpp"

#include "engine_graphics.hpp"
#include "queue.hpp"

#include <vector>

namespace draw {
class EngineDraw {
    private:
        create::EngineInit* engineInit;
        graphics::EngineGraphics* engGraphics;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexMemory;

    public:
        void initialize(std::vector<data::Vertex2D> vertices, graphics::EngineGraphics* userGraphics, create::EngineInit* userInit, uint32_t primitiveCount);
        ~EngineDraw();

    private:
        bool checkPipelineSuitability(uint32_t primitiveCount);
        void createBuffer(VkBuffer* buffer, VkDeviceMemory* memory, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
        void createVertexBuffer(std::vector<data::Vertex2D> vertices, VkDeviceSize bufferSize, VkBuffer* vertexBuffer, VkDeviceMemory* vertexMemory);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};
}