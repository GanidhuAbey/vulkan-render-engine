#include "../inc/engine_draw.hpp"

using namespace draw;

/*
bool EngineDraw::checkPipelineSuitability(uint32_t primitiveCount) {
    if (&engGraphics->graphicsPipeline == nullptr || engGraphics->bindingCount < primitiveCount)  {
        return true;
    }
    return false;
}
*/

<<<<<<< HEAD
void EngineDraw::initialize(VkBuffer indexBuffer, graphics::EngineGraphics* userGraphics, create::EngineInit* userInit, 
=======
void EngineDraw::initialize(std::vector<data::Vertex2D> vertices, std::vector<uint16_t> indices, VkBuffer indexBuffer, graphics::EngineGraphics* userGraphics, create::EngineInit* userInit, 
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
VkBuffer* vertexBuffer, mem::MaMemory* gpuMemory) {
    
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
    //std::cout << "draw destruction..." << std::endl;
    //vkDeviceWaitIdle(engineInit->device);

    //mem::maFreeMemory(memory, memoryData);

    //memoryPool->deallocate(engineInit->device, vertexBuffer, gpuMemory);

    //engGraphics->cleanupRender();
}

/*
void EngineDraw::render(std::vector<data::Vertex2D> vertices, mem::MaMemory* gpuMemory) {
    //allocate memory
    *gpuMemory = mem::maAllocateMemory(*gpuMemory, sizeof(vertices[0]) * vertices.size(), &memoryData);

    void* data;
    //now i should have done everything needed to attach memory to the buffer.
    if (vkMapMemory(engineInit->device, memoryData.memoryHandle, memoryData.offset, memoryData.resourceSize, 0, &data) != VK_SUCCESS) {
        throw std::runtime_error("could not attach data to vertex memory");
    }
    memcpy(data, vertices.data(), memoryData.resourceSize);
    vkUnmapMemory(engineInit->device, memoryData.memoryHandle);
}
*/