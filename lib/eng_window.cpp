#include "../inc/eng_window.hpp"


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace create {
    EngWindow::~EngWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    GLFWwindow* EngWindow::initWindow(int w, int h, const char* name) {
        width = w;
        height = h;
        title = name;

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        
        if (enableValidationLayers) {
            glfwSetErrorCallback(debug::glfwErrorCallback);
        }

        //create window
        window = glfwCreateWindow(width, height, title, nullptr,  nullptr);

        return window;
    }

    //=======================================================================
    //user interact functions
    //closeRequest() - returns true when user makes request to close window
    //                 abstraction of the glfw functions for user.
    //parameters:
    //  NONE
    //returns : bool - user window close request
    bool EngWindow::closeRequest() {
        glfwPollEvents();

        return glfwWindowShouldClose(window);
    }
}