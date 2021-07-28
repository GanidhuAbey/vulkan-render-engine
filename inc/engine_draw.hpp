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
        mem::MaMemory* memory;

    public:
<<<<<<< HEAD
<<<<<<< HEAD
        void initialize(VkBuffer indexBuffer, graphics::EngineGraphics* userGraphics, create::EngineInit* userInit, 
        VkBuffer* vertexBuffer, mem::MaMemory* gpuMemory);
=======
        void initialize(std::vector<data::Vertex2D> vertices, std::vector<uint16_t> indices, VkBuffer indexBuffer, 
        graphics::EngineGraphics* userGraphics, create::EngineInit* userInit, VkBuffer* vertexBuffer, mem::MaMemory* gpuMemory);
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
        void initialize(std::vector<data::Vertex2D> vertices, std::vector<uint16_t> indices, VkBuffer indexBuffer, 
        graphics::EngineGraphics* userGraphics, create::EngineInit* userInit, VkBuffer* vertexBuffer, mem::MaMemory* gpuMemory);
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
        ~EngineDraw();
        void render(void* data, VkDeviceSize dataSize, mem::MaMemory* memory);

    private:
        bool checkPipelineSuitability(uint32_t primitiveCount);
};
}