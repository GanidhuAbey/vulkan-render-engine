#include "../inc/engine_draw.hpp"

using namespace draw;

void EngineDraw::initialize(VkBuffer indexBuffer, graphics::EngineGraphics* userGraphics, create::EngineInit* userInit, VkBuffer* vertexBuffer, mem::MaMemory* gpuMemory) {

    engGraphics = userGraphics;
    engineInit =  userInit;

    memory = gpuMemory;
    //TODO: can save a lot of performance by doing a check for if the vertex buffer has the same memory allocated, but lets
    //      first see what the performance would look like to see how viable this method is

    //allocate vertices into given buffer
    //render(vertices.data(), sizeof(vertices[0]) * vertices.size(), gpuMemory);

    //std::cout << "so this works?" << std::endl;

    engGraphics->drawFrame();

}
EngineDraw::~EngineDraw() {
}

