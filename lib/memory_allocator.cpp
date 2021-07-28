#include "../inc/memory_allocator.hpp"
using namespace mem;

uint32_t mem::findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

    //uint32_t suitableMemoryForBuffer = 0;
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties == properties)) {
            return i;
        }
    }


    throw std::runtime_error("could not find appropriate memory type");

}

void mem::maCreateMemory(VkPhysicalDevice physicalDevice, VkDevice device, MaMemoryInfo* poolInfo, VkBuffer* buffer, MaMemory* maMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    //bufferInfo.flags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    //TODO: currently hard coding the buffer allocation, this needs to change
    //cannot determine the amount of data the user will use from the beginning so this buffer needs to be able to change size
    bufferInfo.size = poolInfo->allocationSize;
    bufferInfo.usage = poolInfo->bufferUsage;
    //vertex buffer only needs to be accessed by graphics queue right now to draw.
    //NOTE: when i get new computer, maybe worth abstracting this
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 1;
    //NOTE: when getting into compute shading might be worth to abstract this
    bufferInfo.queueFamilyIndexCount = poolInfo->queueFamilyIndexCount;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
        throw std::runtime_error("could not create vertex buffer");
    }

    //bind appropriate memory to the buffer;
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, *buffer, &memRequirements);

    //allocate memory for buffer
    VkMemoryAllocateInfo memoryInfo{};
    memoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryInfo.allocationSize = poolInfo->allocationSize;
    //too lazy to even check if this exists will do later TODO
    memoryInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, poolInfo->memoryProperties);

    VkResult allocResult = vkAllocateMemory(device, &memoryInfo, nullptr, &maMemory->memoryHandle);

    if (allocResult != VK_SUCCESS) {
        std::cout << allocResult << std::endl;
        throw std::runtime_error("could not allocate memory for memory pool");
    }

    if (vkBindBufferMemory(device, *buffer, maMemory->memoryHandle, 0) != VK_SUCCESS) {
        throw std::runtime_error("could not bind allocated memory to buffer");
    }

    maMemory->sizes.resize(1);
    maMemory->offsets.resize(1);

    maMemory->sizes[0] = memRequirements.size;
    maMemory->offsets[0] = 0;


    maMemory->alignmentMultiple = memRequirements.alignment;
}

MaMemory mem::maAllocateMemory(MaMemory maMemory, VkDeviceSize resourceSize, MaMemoryData* memoryData) {
    //find space in buffer
    size_t memoryIndex = 0;
    bool poolFound = false;
    for (size_t i = 0; i < maMemory.sizes.size(); i++) {
        if (maMemory.sizes[i] > resourceSize) {
            //found a place for the memory to be attached
            poolFound = true;
            memoryIndex = i;
            break;
        }
    }
<<<<<<< HEAD
    //if pool found is false, then the buffer can't fit anymore data

    memoryData->memoryHandle = maMemory.memoryHandle;
    memoryData->offset = maMemory.offsets[memoryIndex] - maMemory.alignmentOffset;
    memoryData->resourceSize = resourceSize;
    memoryData->offsetIndex = memoryIndex;



    //maMemory.offsets[memoryIndex] += ceil (static_cast<float>( resourceSize ) / static_cast<float>( maMemory.alignmentMultiple )) * maMemory.alignmentMultiple;
    maMemory.offsets[memoryIndex] += resourceSize;
    //maMemory.alignmentOffset = maMemory.offsets[memoryIndex] - resourceSize;
    maMemory.sizes[memoryIndex] -= resourceSize;

    //std::cout << "resource size: " <<  resourceSize <<  std::endl;
    //std::cout << "offset size: " << maMemory.offsets[0] << std::endl;

=======

    memoryData->memoryHandle = maMemory.memoryHandle;
    memoryData->offset = maMemory.offsets[memoryIndex];
    memoryData->resourceSize = resourceSize;
    memoryData->offsetIndex = memoryIndex;

    maMemory.offsets[memoryIndex] += floor (memoryData->offset / maMemory.alignmentMultiple) * maMemory.alignmentMultiple;
    maMemory.sizes[memoryIndex] -= resourceSize;

>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    return maMemory;

}

void mem::maFreeMemory(MaMemory* maMemory, MaMemoryData memoryData) {
    //instead of actually deallocating the memory and wasting time, allocating it later
    //we'll just tell the user we deallocated the data while keeping it to write new data later.
    maMemory->offsets[memoryData.offsetIndex] = memoryData.offset;
    maMemory->sizes[memoryData.offsetIndex] += memoryData.resourceSize;
}

void mem::maDestroyMemory(VkDevice device, MaMemory maMemory, VkBuffer buffer) {
    vkDeviceWaitIdle(device);
    //free the large chunk of memory
    vkFreeMemory(device, maMemory.memoryHandle, nullptr);

    //destroy the associated buffer
    vkDestroyBuffer(device, buffer, nullptr);
}