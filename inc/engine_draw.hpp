#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "data_formats.hpp"

#include "engine_graphics.hpp"
#include "queue.hpp"
#include "memory_allocator.hpp"

#include <vector>

namespace draw {
class EngineDraw {
    private:
        create::EngineInit* engineInit;
        graphics::EngineGraphics* engGraphics;
        mem::MemoryPool* memoryPool;

        VkBuffer vertexBuffer;
        //instead of a vertex memory we'll hold this to manage the memory of the vertex buffer
        mem::MemoryPool::MemoryData gpuMemory;

    public:
        void initialize(std::vector<data::Vertex2D> vertices, graphics::EngineGraphics* userGraphics, mem::MemoryPool* userPool, 
        create::EngineInit* userInit, uint32_t primitiveCount);
        ~EngineDraw();

    private:
        bool checkPipelineSuitability(uint32_t primitiveCount);
        mem::MemoryPool::MemoryData createBuffer(VkBuffer* buffer, VkDeviceSize bufferSize, VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties);
        void createVertexBuffer(std::vector<data::Vertex2D> vertices, VkDeviceSize bufferSize, VkBuffer* vertexBuffer, mem::MemoryPool::MemoryData* gpuMemory);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};
}