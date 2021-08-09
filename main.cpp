#include "inc/engine.hpp"
#include "inc/game_objects.hpp"

#include <chrono>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

int main() {
    //initialize engine
    create::Engine engine(WIDTH, HEIGHT, "Test");

    //gameObject::Light light(0, 0, 1, Color(1.0, 1.0, 1.0), &engine);
    //create square object
    //gameObject::Cube cube(WIDTH/3, HEIGHT/4, 1, 100, Color(0.4, 0.5, 0.1), &engine);

    //create a empty object and plug in the mesh data
    gameObject::EmptyObject test(&engine);
    //auto t1 = std::chrono::high_resolution_clock::now();
    test.addMeshData("../objects/test_object/test.obj");
    auto t2 = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double, std::milli> time_took = t2 - t1;
    //std::cout << "the time the function took: " << time_took.count() << std::endl;

    float xTranslate = 0;
    int frameCount = 0;
    //in radians
    float angle = 0;
    while (!engine.userWindow.closeRequest()) {
        auto t1 = std::chrono::high_resolution_clock::now();
        engine.clearScreen();
        auto t2 = std::chrono::high_resolution_clock::now();

        test.addTransform(0, 0, 0, angle);

        engine.draw();

        std::chrono::duration<double, std::milli> ms_double = t2 - t1;

        xTranslate += 0.1;

        frameCount++;
        angle += 0.01;
    }

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
