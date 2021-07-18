#include "../inc/game_objects.hpp"
#include "common.hpp"

using namespace gameObject;

//Square GameObject  ------------------------------------------------------------------------------------------
Square::Square(int x, int y, int s, Color c, create::Engine* eng) {
    engine = eng;

    float vulkanX = engine->screenToVulkan(x, engine->screenWidth, -1);
    float vulkanY = engine->screenToVulkan(y, engine->screenHeight, -1);

    float vulkanWidth = engine->screenToVulkan(s, engine->screenWidth, 0);
    float vulkanHeight = engine->screenToVulkan(s, engine->screenHeight, 0);

    //std::cout << vulkanWidth << std::endl;

    //create appropriate vertices
    vertices = {
        {{vulkanX - vulkanWidth, vulkanY - vulkanHeight}, {c.red, c.green, c.blue}},
        {{vulkanX + vulkanWidth, vulkanY - vulkanHeight}, {c.red, c.green, c.blue}},
        {{vulkanX + vulkanWidth, vulkanY + vulkanHeight}, {c.red, c.green, c.blue}},
        {{vulkanX - vulkanWidth, vulkanY + vulkanHeight}, {c.red, c.green, c.blue}}
    };

    indices = {
        0, 1, 2, 2, 3, 0
    };

    /*
    for (int i = 0; i < indices.size(); ++i) {
        std::cout << indices[i] << std::endl;
    }
    */

    /*
    for (size_t i = 0; i < engine->vertices.size(); ++i) {
        for (size_t j = 0; j < 2; ++j) {
            std::cout << engine->vertices[i].position[j] << ", ";
        }
        std::cout << "\n";
    }
    */

    engine->engGraphics.ubo.translation = glm::mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 0.0
    );


    //upload the vertices data over here instead
    engine->writeToBuffer(vertices.data(), sizeof(vertices[0]) * vertices.size(), &engine->gpuMemory);
    engine->writeToBuffer(indices.data(), sizeof(indices[0]) * indices.size(), &engine->indexMemory);

    //not sure if the best place to put this...
    engine->engGraphics.createCommandBuffers(vertexBuffer, indexBuffer, vertices, indices);
    //engine->engGraphics->createCommandBuffers(*vertexBuffer, indexBuffer, vertices, indices);
    
}

Square::~Square() {
    //delete verticesz
}

void Square::translate(float x, float y) {
    //update the uniform buffer here
    float vulkanX = engine->screenToVulkan(x, engine->screenWidth, 0);
    float vulkanY = engine->screenToVulkan(y, engine->screenHeight, 0);

    engine->engGraphics.ubo.translation = glm::mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        vulkanX, vulkanY, 1.0, 0.0,
        0.0, 0.0, 0.0, 0.0
    );


    //engine->engGraphics.updateUniformBuffers()
}

//-------------------------------------------------------------------------------------------------------------