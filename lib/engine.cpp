#include "../inc/engine.hpp"

namespace create {
    Engine::Engine(int w, int h, const char* title) {
        screenWidth = w;
        screenHeight = h;
        GLFWwindow* window = userWindow.initWindow(w, h, title);
        engInit.initialize(window);
        engGraphics.initialize(&engInit);

        glfWindow = window;

    }

    Engine::~Engine() {
        //nothing to do now, maybe in the future we can add a check to see if memory is leaking
        std::cout << "program exectuted and closed" << std::endl;
    }

    void Engine::drawRect(int x, int y, int w, int h, Color c) {
        //got to convert the screen space coordinates into vulkan space coordinates
        int newRange = vulkanMaxWidth - vulkanMinWidth; //vulkan will convert both width and height to the same range (-1 , 1)
        int oldRangeX = screenWidth - 0;
        float vulkanX = ((x * (newRange))/oldRangeX) - 1;
        int oldRangeY = screenHeight - 0;
        float vulkanY = ((y * (newRange))/oldRangeY) - 1;

        //map the width and height of the rectangle to vulkan space coordinates
        float vulkanWidth = (((float) w * (newRange)) /oldRangeX);
        float vulkanHeight = (((float) h * (newRange)) /oldRangeY);

        //std::cout << "x location in vulkan: " << vulkanX << std::endl;
        //std::cout << "y location in vulkan: " << vulkanY << std::endl;

        //std::cout << "width in vulkan: " << vulkanWidth << std::endl;
        //std::cout << "height in vulkan: " << vulkanHeight << std::endl;

        //create appropriate vertices
        std::vector<data::Vertex2D> vertices = {
            {{vulkanX - vulkanWidth, vulkanY - vulkanHeight}, {c.red, c.green, c.blue}},
            {{vulkanX + vulkanWidth, vulkanY - vulkanHeight}, {c.red, c.green, c.blue}},
            {{vulkanX - vulkanWidth, vulkanY + vulkanHeight}, {c.red, c.green, c.blue}},
            {{vulkanX + vulkanWidth, vulkanY - vulkanHeight}, {c.red, c.green, c.blue}},
            {{vulkanX + vulkanWidth, vulkanY + vulkanHeight}, {c.red, c.green, c.blue}},
            {{vulkanX - vulkanWidth, vulkanY + vulkanHeight}, {c.red, c.green, c.blue}}
        };

        primitiveCount++;

        draw::EngineDraw engineDraw;
        engineDraw.initialize(vertices, &engGraphics, &engInit, primitiveCount);
    }

    void Engine::clearScreen() {
        primitiveCount = 0;
    }
}