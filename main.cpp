#include "inc/engine.hpp"
#include "inc/game_objects.hpp"

#include <chrono>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

int main() {
    //initialize engine
    create::Engine engine(WIDTH, HEIGHT, "hello world");
    
    //create square object
    gameObject::Square square(WIDTH/2, HEIGHT/2, 50, Color(0.4, 0.5, 0.1), &engine);

    //engine.Rect(WIDTH/2, HEIGHT/2, 50, 50, Color(0.4, 0.5, 0.1));
    float xTranslate = 0;
    int frameCount = 0;
    while (!engine.userWindow.closeRequest()) {
        auto t1 = std::chrono::high_resolution_clock::now();
        //std::cout << window.closeRequest() << std::endl;
        engine.clearScreen();
        auto t2 = std::chrono::high_resolution_clock::now();

        square.translate(xTranslate, 0);
        //std::cout << "n" << std::endl;

        engine.draw();

        std::chrono::duration<double, std::milli> ms_double = t2 - t1;

        xTranslate += 0.1;

        frameCount++;
    }
    //vkDeviceWaitIdle(device);

    //return EXIT_SUCCESS;

    return 1;
}

/*
    void MemoryPool::deallocate(VkDevice device, VkBuffer buffer, MemoryData memoryData) {
        //instead of actually deallocating the memory and wasting time, allocating it later
        //we'll just tell the user we deallocated the data while keeping it to write new data later.
        pools[memoryData.poolIndex].offsets[memoryData.poolOffsetIndex] = memoryData.offset;
        
        //TODO: this is inefficient as fuck and i dont see it scaling very well
        pools[memoryData.poolIndex].sizes[memoryData.poolOffsetIndex] += memoryData.resourceSize;

        //delete buffer now
        vkDestroyBuffer(device, buffer, nullptr);
    }
*/