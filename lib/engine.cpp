#include "../inc/engine.hpp"
#include "common.hpp"

using namespace create;

//define some global variables.
uint16_t indexCount = 0;
uint16_t vertexCount = 0;
uint16_t objectCount = 0;

VkBuffer vertexBuffer;
VkBuffer indexBuffer;

Engine::Engine(int w, int h, const char* title) {
    screenWidth = w;
    screenHeight = h;
    GLFWwindow* window = userWindow.initWindow(w, h, title);
    engInit.initialize(window);
    engGraphics.initialize(&engInit);

    //creates vertex buffer
    gpuMemory = createBuffer(&vertexBuffer, 5e7, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    //create index buffer
    indexMemory = createBuffer(&indexBuffer, 5e7, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    //create uniform buffer
    //uniformMemory = createBuffer(&uniformBuffer, 5e7, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    //update uniform buffer memory
    //writeToBuffer(&engGraphics.ubo, sizeof(engGraphics.ubo), &uniformMemory);

    //engGraphics.createDescriptorSets(uniformBuffer);

    glfWindow = window;

}

Engine::~Engine() {
    mem::maDestroyMemory(engInit.device, gpuMemory, vertexBuffer);
    mem::maDestroyMemory(engInit.device, indexMemory, indexBuffer);
    //mem::maDestroyMemory(engInit.device, uniformMemory, uniformBuffer);
    //nothing to do now, maybe in the future we can add a check to see if memory is leaking
    std::cout << "program exectuted and closed" << std::endl;
}

//TODO: remove this
void Engine::clearScreen() {
    primitiveCount = 0;
}

void Engine::draw() {
    draw::EngineDraw engineDraw;
    engineDraw.initialize(indexBuffer, &engGraphics, &engInit, &vertexBuffer, &gpuMemory);
}

void Engine::writeToBuffer(void* data, VkDeviceSize dataSize, mem::MaMemory* memory) {
    *memory = mem::maAllocateMemory(*memory, dataSize, &memoryData);

    std::cout << "offset  factors : " << memoryData.offset - memory->offsetAmt << std::endl;

    void* pData;
    if (vkMapMemory(engInit.device, memoryData.memoryHandle, memoryData.offset, memoryData.resourceSize, 0, &pData) != VK_SUCCESS) {
        throw std::runtime_error("could not map data to buffer memory");
    }
    memcpy(pData, data, dataSize);
    vkUnmapMemory(engInit.device, memoryData.memoryHandle);

    std::cout << "hey hey hey" << std::endl;
}

void Engine::createVertexBuffer(VkBuffer* vertexBuffer, mem::MaMemory* gpuMemory) {
    //the source transfer bit allows this buffer to transfer its data to other buffers that have the destination bit flag.

    /*
    void* data;
    //now i should have done everything needed to attach memory to the buffer.
    if (vkMapMemory(engineInit->device, cpuMemory.memory, cpuMemory.offset, bufferSize, 0, &data) != VK_SUCCESS) {
        throw std::runtime_error("could not attach data to vertex memory");
    }
    memcpy(data, vertices.data(), bufferSize);
    vkUnmapMemory(engineInit->device, cpuMemory.memory);
    */
}

//TODO: put output variables as the last parameters
mem::MaMemory Engine::createBuffer(VkBuffer* buffer, VkDeviceSize memorySize, VkBufferUsageFlags usage,
VkMemoryPropertyFlags properties) {
    create::QueueData indices(engInit.physicalDevice, engInit.surface);

    mem::MaMemoryInfo memoryInfo{};
    memoryInfo.allocationSize = memorySize;
    memoryInfo.bufferUsage = usage;
    memoryInfo.memoryProperties = properties;
    memoryInfo.queueFamilyIndexCount = indices.graphicsFamily.value();

    mem::MaMemory memoryObj{};
    mem::maCreateMemory(engInit.physicalDevice, engInit.device, &memoryInfo, buffer, &memoryObj);

    return memoryObj;
}

float Engine::screenToVulkan(int screenCoord, int screenSize, int vulkanMin) {
    const float vulkanMaxWidth = 1;
    const float vulkanMinWidth = -1;

    float newRange = vulkanMaxWidth - vulkanMinWidth; //vulkan will convert both width and height to the same range (-1 , 1)
    float oldRange = screenSize - 0;
    float vulkanCoord = ((screenCoord * (newRange)) / oldRange) + vulkanMin;

    //std::cout << (screenCoord * newRange) << std::endl;

    return vulkanCoord;
}
