#include "../inc/engine.hpp"

using namespace create;

Engine::Engine(int w, int h, const char* title) {
    screenWidth = w;
    screenHeight = h;
    GLFWwindow* window = userWindow.initWindow(w, h, title);
    engInit.initialize(window);
    engGraphics.initialize(&engInit);

    createVertexBuffer(&vertexBuffer, &gpuMemory);

    glfWindow = window;

}

Engine::~Engine() {
    mem::maDestroyMemory(engInit.device, gpuMemory, vertexBuffer);
    //nothing to do now, maybe in the future we can add a check to see if memory is leaking
    std::cout << "program exectuted and closed" << std::endl;
}

void Engine::drawRect(int x, int y, int w, int h, Color c) {
    //got to convert the screen space coordinates into vulkan space coordinates
    int newRange = vulkanMaxWidth - vulkanMinWidth; //vulkan will convert both width and height to the same range (-1 , 1)
    int oldRangeX = screenWidth - 0;
    float vulkanX = ((x * (newRange))/oldRangeX) - 1;
    int oldRangeY = screenHeight - 0;
    float vulkanY = ((y * (newRange))/oldRangeY) - 1;

    //map the width and height of the rectangle to vulkan space coordinates
    float vulkanWidth = (((float) w * (newRange)) /oldRangeX);
    float vulkanHeight = (((float) h * (newRange)) /oldRangeY);

    //std::cout << "x location in vulkan: " << vulkanX << std::endl;
    //std::cout << "y location in vulkan: " << vulkanY << std::endl;

    //std::cout << "width in vulkan: " << vulkanWidth << std::endl;
    //std::cout << "height in vulkan: " << vulkanHeight << std::endl;

    //create appropriate vertices
    std::vector<data::Vertex2D> vertices = {
        {{vulkanX - vulkanWidth, vulkanY - vulkanHeight}, {c.red, c.green, c.blue}},
        {{vulkanX + vulkanWidth, vulkanY - vulkanHeight}, {c.red, c.green, c.blue}},
        {{vulkanX - vulkanWidth, vulkanY + vulkanHeight}, {c.red, c.green, c.blue}},
        {{vulkanX + vulkanWidth, vulkanY - vulkanHeight}, {c.red, c.green, c.blue}},
        {{vulkanX + vulkanWidth, vulkanY + vulkanHeight}, {c.red, c.green, c.blue}},
        {{vulkanX - vulkanWidth, vulkanY + vulkanHeight}, {c.red, c.green, c.blue}}
    };

    primitiveCount++;

    draw::EngineDraw engineDraw;
    engineDraw.initialize(vertices, &engGraphics, &engInit, &vertexBuffer, &gpuMemory);
}

void Engine::clearScreen() {
    primitiveCount = 0;
}

void Engine::createVertexBuffer(VkBuffer* vertexBuffer, mem::MaMemory* gpuMemory) {
    //the source transfer bit allows this buffer to transfer its data to other buffers that have the destination bit flag.
    *gpuMemory = createBuffer(vertexBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

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
mem::MaMemory Engine::createBuffer(VkBuffer* buffer, VkBufferUsageFlags usage,
VkMemoryPropertyFlags properties) {
    create::QueueData indices(engInit.physicalDevice, engInit.surface);

    mem::MaMemoryInfo memoryInfo{};
    memoryInfo.allocationSize = 1e8;
    memoryInfo.bufferUsage = usage;
    memoryInfo.memoryProperties = properties;
    memoryInfo.queueFamilyIndexCount = indices.graphicsFamily.value();

    mem::MaMemory memoryObj{};
    mem::maCreateMemory(engInit.physicalDevice, engInit.device, &memoryInfo, buffer, &memoryObj);

    return memoryObj;
}