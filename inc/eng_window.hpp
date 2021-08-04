#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//#include "engine_init.hpp"
#include "error_setup.hpp"

namespace create {

class EngWindow {
    //construct/deconstruct functions
    public:
        GLFWwindow* initWindow(int w, int h, const char* name);
        ~EngWindow();

    //private functions for library
    private:
        GLFWwindow *window;
        int width;
        int height;
        const char* title;
    //functions user can interact with
    public:
        bool closeRequest();
};

}
