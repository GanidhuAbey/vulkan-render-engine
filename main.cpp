//include GLFW and vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

//TODO: can i change this to a #define?
//screen size, maybe move to a header
const uint32_t WIDTH  = 800;
const uint32_t HEIGHT = 600;

//this class can be moved to a header
class HelloTriangleApplication {
public:
    void run() {
        //setup glfw window
        initwindow();
        //this initializes any of the vulkan related objects we will need
        initvulkan();
        //this is where the frames are rendered and updated using the vulkan objects
        mainloop();
        //once the window is closed the vulkan objects are freed from memory here
        cleanup();
    }
private:
    GLFWwindow* window;

private:
    void initwindow() {
        //initializes glfw
        //add check for initialize failure
        glfwInit();

        //turn off opengl context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        //turn off ability to resize window
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        //create a basic window
        window = glfwCreateWindow(WIDTH, HEIGHT, "vulkan renderer", nullptr, nullptr);
    }

    void initvulkan() {

    }

    void mainloop() {
        //loop continues till a close request is made to window
        while (!glfwWindowShouldClose(window)) {
            //this function will process events, it seems that their is no need to catch the event
            glfwPollEvents();
        }
    }

    void cleanup() {
        //end window processes
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
