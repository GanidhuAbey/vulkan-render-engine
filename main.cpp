#include "inc/engine.hpp"

#include <chrono>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

int main() {
    //initialize engine
    create::Engine engine(WIDTH, HEIGHT, "hello world");
    
    while (!engine.userWindow.closeRequest()) {
        auto t1 = std::chrono::high_resolution_clock::now();
        engine.drawRect(0, 0, 50, 50, Color(0.4, 0.5, 0.1));
        //std::cout << window.closeRequest() << std::endl;
        engine.clearScreen();
        auto t2 = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> ms_double = t2 - t1;
        std::cout << "frametime: " << ms_double.count() << "ms" << std::endl;
    }
    //vkDeviceWaitIdle(device);

    //return EXIT_SUCCESS;

    return 1;
}