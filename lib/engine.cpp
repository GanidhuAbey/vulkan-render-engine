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
    gpuMemory = createBuffer(&vertexBuffer, 5e7, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    //create index buffer
    indexMemory = createBuffer(&indexBuffer, 5e7, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
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

//PURPOSE - write data to buffer in device local memory, do NOT use this function for other memory types
//PARAMETERS - dataSize : (the size of the data being given to buffer)
//           - dstBuffer :  (the device local buffer the data will be written to)
//           - data : (the actual data that will be given to the buffer)/
//RETURNS - NONE
void Engine::writeToDeviceBuffer(VkDeviceSize dataSize, VkBuffer dstBuffer, void* data) {
    //create and allocate memory for a temporary cpu readable buffer
    mem::MaMemoryData tempMemoryInfo{};
    mem::MaMemory tempMemory{};
    VkBuffer tempBuffer{};

    tempMemoryInfo = createTempBuffer(dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &tempBuffer, &tempMemory);

    //map data to temporary buffer
    void* pData;
    if (vkMapMemory(engInit.device, tempMemoryInfo.memoryHandle,tempMemoryInfo.offset,tempMemoryInfo.resourceSize, 0, &pData) != VK_SUCCESS) {
        throw std::runtime_error("could not map data to buffer memory");
    }
    memcpy(pData, data, dataSize);
    vkUnmapMemory(engInit.device, tempMemoryInfo.memoryHandle);

    //transfer memory from temp buffer to given device local buffer
    engGraphics.copyBuffer(tempBuffer, dstBuffer, dataSize);

    //destroy buffer and its associated memory
    mem::maDestroyMemory(engInit.device, tempMemory, tempBuffer);
}

//PURPOSE - abstract a bit away from the underlying api when writing to the proper buffer
//PARAMETERS - [VkDeviceSize] dataSize - the size of the data being written to vertex buffer
//           - [void*] data - the data being written to vertex buffer
//RETURNS - NONE
void Engine::writeToVertexBuffer(VkDeviceSize dataSize, void* data) {
    writeToDeviceBuffer(dataSize, vertexBuffer, data);
}

void Engine::writeToIndexBuffer(VkDeviceSize dataSize, void* data) {
    writeToDeviceBuffer(dataSize, indexBuffer, data);
}

//PURPOSE - create and allocate memory for a small cpu-readable buffer
//PARAMETERS  - [VkDeviceSize] dataSize (size of the data that will be allocated to this buffer)
//            - [VkBufferUsageFlags] usage (the type of buffer this buffer will transfer data to)
//            - [VkBuffer*] tempBuffer (memory location for the temp buffer to be created to)
//            - [mem::MaMemory*] tempMemory (memory location for the temp memory to be created to)
//RETURNS - [mem::MaMemoryData] memoryData (struct data giving information on where to allocate the data to)
mem::MaMemoryData Engine::createTempBuffer(VkDeviceSize dataSize, VkBufferUsageFlags usage, VkBuffer* tempBuffer, mem::MaMemory* tempMemory) {
    *tempMemory = createBuffer(tempBuffer, dataSize, usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    //allocate memory for temp buffer
    mem::MaMemoryData memoryData{};
    *tempMemory = mem::maAllocateMemory(*tempMemory, dataSize, &memoryData);

    return memoryData;
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
