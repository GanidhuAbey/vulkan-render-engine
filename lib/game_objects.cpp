#include "../inc/game_objects.hpp"
#include "common.hpp"
<<<<<<< HEAD
#include "math.h"

#include "jpeglib.h"

using namespace gameObject;
#pragma comment(lib, "libjpeg.lib")

const int DEPTH_CONSTANT = 100;
=======

using namespace gameObject;
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9

//Square GameObject  ------------------------------------------------------------------------------------------
Square::Square(int x, int y, int s, Color c, create::Engine* eng) {
    engine = eng;

    float vulkanX = engine->screenToVulkan(x, engine->screenWidth, -1);
    float vulkanY = engine->screenToVulkan(y, engine->screenHeight, -1);

    float vulkanWidth = engine->screenToVulkan(s, engine->screenWidth, 0);
    float vulkanHeight = engine->screenToVulkan(s, engine->screenHeight, 0);

<<<<<<< HEAD
    //create appropriate vertices
    //TODO: sort out a method to combine vertices (resize vertices array from current length to add new vertices),
    //      then specify a offset so command buffer can draw all vertices properly.
    std::vector<data::Vertex2D> vertices = {
        {{vulkanX - vulkanWidth, vulkanY - vulkanHeight, 0.0, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX + vulkanWidth, vulkanY - vulkanHeight, 0.0, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX + vulkanWidth, vulkanY + vulkanHeight, 0.0, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX - vulkanWidth, vulkanY + vulkanHeight, 0.0, 0.0}, {c.red, c.green, c.blue}}
    };

    std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    vertexCount += vertices.size();
    indexCount += indices.size();
    objectCount++;

=======
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

>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
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

<<<<<<< HEAD
    /*
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    engine->engGraphics.ubo.translation = glm::mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 0.0
    );
<<<<<<< HEAD
    */
=======

>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9

    //upload the vertices data over here instead
    engine->writeToBuffer(vertices.data(), sizeof(vertices[0]) * vertices.size(), &engine->gpuMemory);
    engine->writeToBuffer(indices.data(), sizeof(indices[0]) * indices.size(), &engine->indexMemory);

    //not sure if the best place to put this...
<<<<<<< HEAD
    engine->engGraphics.createCommandBuffers(vertexBuffer, indexBuffer, indexCount);
=======
    engine->engGraphics.createCommandBuffers(vertexBuffer, indexBuffer, vertices, indices);
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    //engine->engGraphics->createCommandBuffers(*vertexBuffer, indexBuffer, vertices, indices);
    
}

Square::~Square() {
    //delete verticesz
}

void Square::translate(float x, float y) {
    //update the uniform buffer here
<<<<<<< HEAD
    float translateX = engine->screenToVulkan(x, engine->screenWidth, 0);
    float translateY = engine->screenToVulkan(y, engine->screenHeight, 0);

    /*
    engine->engGraphics.ubo.translation = glm::mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        translateX, translateY, 1.0, 0.0,
        0.0, 0.0, 0.0, 0.0
    );
    */

    //engine->engGraphics.updateUniformBuffers()
}

void Square::rotate(float angle) {
    float x = -sin(angle);
    if (x == -0) {
        x = static_cast<uint32_t>( sin(angle) );
    }

}

//-------------------------------------------------------------------------------------------------------------
//Cube GameObject  ------------------------------------------------------------------------------------------
Cube::Cube(int x, int y, int z, int s, Color c, create::Engine* eng) {
    engine = eng;

    float vulkanX = engine->screenToVulkan(x, engine->screenWidth, -1);
    float vulkanY = engine->screenToVulkan(y, engine->screenWidth, -1);
    
    float vulkanWidth = engine->screenToVulkan(s, engine->screenWidth, 0) / 2;
    float vulkanHeight = engine->screenToVulkan(s, engine->screenWidth, 0) / 2;

    //float vulkanWidth = s / DEPTH_CONSTANT;
    //float vulkanHeight = s / DEPTH_CONSTANT;

    //std::cout << vulkanWidth << std::endl;
    //std::cout << vulkanHeight << std::endl;
    //std::cout << s << std::endl;
    //std::cout << std::endl;

    //
    std::vector<data::Vertex2D> vertices = {
        {{vulkanX - vulkanWidth, vulkanY - vulkanHeight, z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX + vulkanWidth, vulkanY - vulkanHeight, z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX + vulkanWidth, vulkanY + vulkanHeight, z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX - vulkanWidth, vulkanY + vulkanHeight, z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX - vulkanWidth, vulkanY - vulkanHeight, vulkanWidth * 2 + z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX + vulkanWidth, vulkanY - vulkanHeight, vulkanWidth * 2 + z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX + vulkanWidth, vulkanY + vulkanHeight, vulkanWidth * 2 + z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX - vulkanWidth, vulkanY + vulkanHeight, vulkanWidth * 2 + z, 0.0}, {c.red, c.green, c.blue}}
    };

    //the indices will get add on to the end to m
    std::vector<uint16_t> indices =  {
        2, 3, 0, 0, 1, 2,
        6, 5, 4, 4, 7, 6,
        3, 7, 4, 4, 0, 3,
        7, 3, 2, 2, 6, 7,
        5, 6, 2, 2, 1, 5,
        4, 5, 1, 1, 0, 4
    };

    for (size_t i = 0; i < indices.size(); i++) {
        indices[i] += vertexCount;
    }

    vertexCount += vertices.size();
    indexCount += indices.size();
    objectCount++;

    //
    engine->writeToBuffer(vertices.data(), sizeof(vertices[0]) * vertices.size(), &engine->gpuMemory);
    engine->writeToBuffer(indices.data(), sizeof(indices[0]) * indices.size(), &engine->indexMemory);

    engine->engGraphics.createCommandBuffers(vertexBuffer, indexBuffer, indexCount);

}

Cube::~Cube() {}

void Cube::rotate(float angle) {
    /*
    engine->engGraphics.ubo.model = glm::mat4(
        cos(angle), 0, sin(angle), 0,
        0,          1,          0, 0,
        -sin(angle),0, cos(angle), 0,
        0,          0,          0, 1
    );
    */
}

//-------------------------------------------------------------------------------------------------------------
//Light GameObject  ------------------------------------------------------------------------------------------
Light::Light(int x, int y, int z, Color c, create::Engine* eng) {
    engine = eng;
    //TODO: need to have a check for the existence of a light object and do some default values if it doesnt exist

    //later on the size of the light cube will probably be ~0 (just a point in the scene)
    float cubeSize = 0.2;

    float vulkanX = engine->screenToVulkan(x, engine->screenWidth, -1);
    float vulkanY = engine->screenToVulkan(y, engine->screenWidth, -1);

    std::vector<data::Vertex2D> vertices = {
        {{vulkanX - cubeSize, vulkanY - cubeSize, z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX + cubeSize, vulkanY - cubeSize, z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX + cubeSize, vulkanY + cubeSize, z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX - cubeSize, vulkanY + cubeSize, z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX - cubeSize, vulkanY - cubeSize, cubeSize * 2 + z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX + cubeSize, vulkanY - cubeSize, cubeSize * 2 + z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX + cubeSize, vulkanY + cubeSize, cubeSize * 2 + z, 0.0}, {c.red, c.green, c.blue}},
        {{vulkanX - cubeSize, vulkanY + cubeSize, cubeSize * 2 + z, 0.0}, {c.red, c.green, c.blue}}
    };

    //the indices will get add on to the end to m
    std::vector<uint16_t> indices =  {
        2, 3, 0, 0, 1, 2,
        6, 5, 4, 4, 7, 6,
        3, 7, 4, 4, 0, 3,
        7, 3, 2, 2, 6, 7,
        5, 6, 2, 2, 1, 5,
        4, 5, 1, 1, 0, 4
    };

    //std::cout  << "vertex count : " << vertexCount << std::endl;

    for (size_t i = 0; i < indices.size(); i++) {
        indices[i] += vertexCount;
    }

    //this code should be further abstracted, appears in every gameobject call
    vertexCount += vertices.size();
    indexCount += indices.size();
    objectCount++;

    engine->writeToBuffer(vertices.data(), sizeof(vertices[0]) * vertices.size(), &engine->gpuMemory);
    engine->writeToBuffer(indices.data(), sizeof(indices[0]) * indices.size(), &engine->indexMemory);

    //move to draw call probably
    //std::cout << indexCount << std::endl;
    engine->engGraphics.createCommandBuffers(vertexBuffer, indexBuffer, indexCount);
}

Light::~Light() {}
=======
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
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
