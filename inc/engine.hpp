#pragma once
#include "engine_init.hpp"
#include "eng_window.hpp"
#include "engine_graphics.hpp"
#include "engine_draw.hpp"
#include "memory_allocator.hpp"



#include "data_formats.hpp"

#include "glm/glm.hpp"

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

        uint32_t primitiveCount = 0;
    public:
        EngWindow userWindow;
        EngineInit engInit;
        graphics::EngineGraphics engGraphics;

        int screenWidth;
        int screenHeight;

        mem::MaMemory gpuMemory;
        mem::MaMemory indexMemory;

        mem::MaMemoryData memoryData;
        //draw::EngineDraw engineDraw;
        //in the future we can make this an array of vertices to hold multiple gameobject data.

    public:
        Engine(int w, int h, const char* title);
        ~Engine();

    public:
        //refer to brendan galea vid on how to properly seperate gameobjects
        void Rect(int x, int y, int w, int h, Color c);
        void clearScreen();
        void draw();
        void writeToBuffer(void* data, VkDeviceSize dataSize, mem::MaMemory* memory);
        float screenToVulkan(int screenCoord, int screenSize, int vulkanMin);

    private:
        void createVertexBuffer(VkBuffer* vertexBuffer, mem::MaMemory* gpuMemory);
        mem::MaMemory createBuffer(VkBuffer* buffer, VkDeviceSize memorySize, VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties);

};
}
