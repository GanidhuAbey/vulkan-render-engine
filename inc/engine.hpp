#include "engine_init.hpp"
#include "eng_window.hpp"
#include "engine_graphics.hpp"

namespace create {
class Engine {
    private:
        GLFWwindow* glfWindow;
    
    public:
        EngWindow userWindow;
        EngineInit engInit;
        EngineGraphics engGraphics;

    public:
        Engine(int w, int h, const char* title);
        ~Engine();

    public:
        void drawSquare();

};
}