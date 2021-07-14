#include "../inc/engine_draw.hpp"

using namespace draw;

bool EngineDraw::checkPipelineSuitability(uint32_t primitiveCount) {
    if (&engGraphics->graphicsPipeline == nullptr || engGraphics->bindingCount < primitiveCount)  {
        return true;
    }
    return false;
}

void EngineDraw::initialize(std::vector<data::Vertex2D> vertices, graphics::EngineGraphics* userGraphics, create::EngineInit* userInit, 
VkBuffer* vertexBuffer, mem::MaMemory* gpuMemory) {
    
    engGraphics = userGraphics;
    engineInit =  userInit;

    memory = gpuMemory;
    //TODO: can save a lot of performance by doing a check for if the vertex buffer has the same memory allocated, but lets
    //      first see what the performance would look like to see how viable this method is

    //allocate vertices into given buffer
    render(vertices, gpuMemory);
    
    engGraphics->createCommandBuffers(*vertexBuffer, vertices);
    engGraphics->drawFrame();

}
EngineDraw::~EngineDraw() {
    //std::cout << "draw destruction..." << std::endl;
    //vkDeviceWaitIdle(engineInit->device);

    mem::maFreeMemory(memory, memoryData);

    //memoryPool->deallocate(engineInit->device, vertexBuffer, gpuMemory);

    //engGraphics->cleanupRender();
}

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
