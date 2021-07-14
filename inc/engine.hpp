#pragma once

#include "engine_init.hpp"
#include "eng_window.hpp"
#include "engine_graphics.hpp"
#include "engine_draw.hpp"
#include "memory_allocator.hpp"

#include "data_formats.hpp"

#include "glm/glm.hpp"


const int vulkanMaxWidth = 1;
const int vulkanMinWidth = -1;

class Color {
    public:
        float red;
        float green;
        float blue;

    public:
        Color(float r, float g, float b) : red{r}, green{g}, blue{b} {};
};

namespace create {
class Engine {
    private:
        GLFWwindow* glfWindow;
        int screenWidth;
        int screenHeight;

        uint32_t primitiveCount = 0;
    public:
        EngWindow userWindow;
        EngineInit engInit;
        graphics::EngineGraphics engGraphics;
        mem::MemoryPool memoryPool;
        //draw::EngineDraw engineDraw;

    public:
        Engine(int w, int h, const char* title);
        ~Engine();

    public:
        void drawRect(int x, int y, int w, int h, Color c);
        void clearScreen();

};
}