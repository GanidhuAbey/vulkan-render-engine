#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "data_formats.hpp"

#include "engine_graphics.hpp"

#include <vector>

namespace draw {
class EngineDraw {
    private:
        create::EngineInit* engineInit;
        graphics::EngineGraphics* engGraphics;

    public:
        void initialize(std::vector<data::Vertex2D> vertices, graphics::EngineGraphics* engGraphics, uint32_t primitiveCount);
        ~EngineDraw();

    private:
        bool checkPipelineSuitability(uint32_t primitiveCount);
};
}