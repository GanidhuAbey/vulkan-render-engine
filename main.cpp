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