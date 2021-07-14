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
        mem::MaMemoryData memoryData;

    public:
        void initialize(std::vector<data::Vertex2D> vertices, graphics::EngineGraphics* userGraphics, create::EngineInit* userInit, 
        VkBuffer* vertexBuffer, mem::MaMemory* gpuMemory);
        ~EngineDraw();
        void render(std::vector<data::Vertex2D> vertices, mem::MaMemory* gpuMemory);

    private:
        bool checkPipelineSuitability(uint32_t primitiveCount);
};
}