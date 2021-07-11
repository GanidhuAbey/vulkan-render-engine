#include "../inc/engine.hpp"

namespace create {
    Engine::Engine(int w, int h, const char* title) {
        GLFWwindow* window = userWindow.initWindow(w, h, title);
        engInit.initialize(window);
        engGraphics.initialize(&engInit);

        glfWindow = window;

    }

    Engine::~Engine() {
        //nothing to do now, maybe in the future we can add a check to see if memory is leaking
        std::cout << "program exectuted and closed" << std::endl;
    }

    void Engine::drawSquare() {
        //alright now what the fuck?
        engGraphics.drawFrame();
    }
}