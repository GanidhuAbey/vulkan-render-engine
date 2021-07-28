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
<<<<<<< HEAD
<<<<<<< HEAD
    gameObject::Cube cube(WIDTH/3, HEIGHT/4, 1, 100, Color(0.4, 0.5, 0.1), &engine);
    gameObject::Light light(0, 0, 1, Color(1.0, 1.0, 1.0), &engine);


    //add texture to cube
    //cube.material("./build/textures/create.jpg");

    //gameObject::Square square(WIDTH/2, HEIGHT/2, 50, Color(0.4, 0.5, 0.1), &engine);
    //square.material('x');
    //engine.Rect(WIDTH/2, HEIGHT/2, 50, 50, Color(0.4, 0.5, 0.1));
    float xTranslate = 0;   
    int frameCount = 0;
    //in radians
    float angle = 0;
    while (!engine.userWindow.closeRequest()) {
        auto t1 = std::chrono::high_resolution_clock::now();
        engine.clearScreen();
        auto t2 = std::chrono::high_resolution_clock::now();
        //cube.rotate(angle);
        //square.rotate(angle);
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
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
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9

        engine.draw();

        std::chrono::duration<double, std::milli> ms_double = t2 - t1;

        xTranslate += 0.1;

        frameCount++;
<<<<<<< HEAD
<<<<<<< HEAD
        angle += 0.001;
        //std::cout << frameCount << std::endl;
        //std::cout << ms_double.count() << std::endl;
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
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