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
        void initialize(VkBuffer indexBuffer, graphics::EngineGraphics* userGraphics, create::EngineInit* userInit, 
        VkBuffer* vertexBuffer, mem::MaMemory* gpuMemory);
        ~EngineDraw();
        void render(void* data, VkDeviceSize dataSize, mem::MaMemory* memory);

    private:
        bool checkPipelineSuitability(uint32_t primitiveCount);
};
}