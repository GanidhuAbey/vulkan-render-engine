#include "../inc/engine_draw.hpp"

namespace draw {
    bool EngineDraw::checkPipelineSuitability(uint32_t primitiveCount) {
        if (&engGraphics->graphicsPipeline == nullptr || engGraphics->bindingCount < primitiveCount)  {
            return true;
        }
        return false;
    }

    void EngineDraw::initialize(std::vector<data::Vertex2D> vertices, graphics::EngineGraphics* userGraphics, mem::MemoryPool* userPool, 
        create::EngineInit* userInit, uint32_t primitiveCount) {
        engGraphics = userGraphics;
        engineInit =  userInit;
        memoryPool = userPool;
        //check if pipeline exists
        if (checkPipelineSuitability(primitiveCount)) {
            //create new graphics pipeline
        }

        //TODO: can save a lot of performance by doing a check for if the vertex buffer has the same memory allocated, but lets
        //      first see what the performance would look like to see how viable this method is

        //check if vertex buffer exists and if its appropriates
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
        createVertexBuffer(vertices, bufferSize, &vertexBuffer, &gpuMemory);
        
        engGraphics->createCommandBuffers(vertexBuffer, vertices);
        engGraphics->drawFrame();

    }
    EngineDraw::~EngineDraw() {
        //std::cout << "draw destruction..." << std::endl;
        vkDeviceWaitIdle(engineInit->device);

        memoryPool->deallocate(engineInit->device, vertexBuffer, gpuMemory);

        //engGraphics->cleanupRender();
    }

    //TODO: put output variables as the last parameters
    mem::MemoryPool::MemoryData EngineDraw::createBuffer(VkBuffer* buffer, VkDeviceSize bufferSize, VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties) {
        
        
        create::QueueData indices(engineInit->physicalDevice, engineInit->surface);

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        //bufferInfo.flags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        //TODO: currently hard coding the buffer allocation, this needs to change
        //cannot determine the amount of data the user will use from the beginning so this buffer needs to be able to change size
        bufferInfo.size = bufferSize;
        bufferInfo.usage = usage;
        //vertex buffer only needs to be accessed by graphics queue right now to draw.
        //NOTE: when i get new computer, maybe worth abstracting this
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        bufferInfo.queueFamilyIndexCount = 1;
        //NOTE: when getting into compute shading might be worth to abstract this
        bufferInfo.queueFamilyIndexCount = indices.graphicsFamily.value();  

        if (vkCreateBuffer(engineInit->device, &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
            throw std::runtime_error("could not create vertex buffer");
        }

        mem::MemoryPool::MemoryData memoryData = memoryPool->allocate(engineInit->physicalDevice, engineInit->device, properties, *buffer);

        //now bind buffer and pass this data further back to the create vertex buffer function
        if (vkBindBufferMemory(engineInit->device, *buffer, memoryData.memory, memoryData.offset) != VK_SUCCESS) {
            throw std::runtime_error("could not bind allocated memory to buffer");
        }

        return memoryData;
    }

    void EngineDraw::createVertexBuffer(std::vector<data::Vertex2D> vertices, VkDeviceSize bufferSize, VkBuffer* vertexBuffer, mem::MemoryPool::MemoryData* gpuMemory) {
        //create CPU accesible buffer
        VkBuffer cpuVertexBuffer;

        //the source transfer bit allows this buffer to transfer its data to other buffers that have the destination bit flag.
        mem::MemoryPool::MemoryData cpuMemory = createBuffer(&cpuVertexBuffer, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        void* data;
        //now i should have done everything needed to attach memory to the buffer.
        if (vkMapMemory(engineInit->device, cpuMemory.memory, cpuMemory.offset, bufferSize, 0, &data) != VK_SUCCESS) {
            throw std::runtime_error("could not attach data to vertex memory");
        }
        memcpy(data, vertices.data(), bufferSize);
        vkUnmapMemory(engineInit->device, cpuMemory.memory);

        //the transfer usage flag specifies that the buffer can be the destination of a transfer command
        //you can essentially copy data from other buffers into this buffer.

        //the property usage flag designates that only the gpu can read and write to this buffer, the cpu cannot touch this buffer in anyway.
        *gpuMemory = createBuffer(vertexBuffer, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        engGraphics->copyBuffer(cpuVertexBuffer, *vertexBuffer, bufferSize);

        //destroy temporary buffer
        memoryPool->deallocate(engineInit->device, cpuVertexBuffer, cpuMemory);
    }

}