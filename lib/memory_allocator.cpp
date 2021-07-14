#include "../inc/memory_allocator.hpp"

namespace mem {
    MemoryPool::MemoryPool()  {}
    MemoryPool::~MemoryPool() {}

    MemoryPool::MemoryData MemoryPool::allocate(VkPhysicalDevice physicalDevice, VkDevice device, VkMemoryPropertyFlags properties, VkBuffer& buffer) {

        //get memory requirements for buffer
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        mem::MemoryPool memoryPool;

        //allocate memory for buffer
        VkMemoryAllocateInfo memoryInfo{};
        memoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryInfo.allocationSize = memRequirements.size;
        //too lazy to even check if this exists will do later TODO
        memoryInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);
        
        //calculate the max number of pools we can have.
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
        VkDeviceSize maxCompMemory = memoryProperties.memoryHeaps[memoryInfo.memoryTypeIndex].size;
        
        VkDeviceSize maxAllocCount = floor ( maxCompMemory / allocationSize );
        //make sure we have atleast some extra data not being reserved for vulkan
        if (maxAllocCount > 5) {
            maxAllocCount -= 5;
        }

        /*
        if (vkAllocateMemory(device, &memoryInfo, nullptr, memory) != VK_SUCCESS) {
            throw std::runtime_error("could not allocate memory for memory");
        }
        */
        //instead lets check if we have a memory pool that satisfies the requirements of the buffer and the resource
        //TODO: most likely a more efficient way than this to check if the memory is allocatable
        bool poolFound = false;
        size_t poolIndex = 0;
        size_t poolSizeIndex = 0;
        VkDeviceSize resourceSize = memRequirements.size;
        uint32_t requiredMemoryIndex = memoryInfo.memoryTypeIndex;
        for (size_t i = 0; i < poolCount; i++) {
            //this will run checks on the currently available memory pools to find something that satisfies the requirements
            //of the buffer and the resource
            auto pool = pools[i];
            if (pool.memoryTypeIndex == requiredMemoryIndex) {
                for (size_t j = 0; j < pool.sizes.size(); j++) {
                    if (pool.sizes[j] >= resourceSize) {
                        poolFound = true;
                        poolIndex = i;
                        poolSizeIndex = j;
                        break;
                    }
                }
            }  
        }
        //TODO: need to check if we can allocate any more pools;
        if (!poolFound && poolCount < maxAllocCount) {
            poolCount++;
            pools.resize(poolCount);
            poolIndex = poolCount - 1;

            createMemoryPool(physicalDevice, device, &pools[poolIndex]);
            
        }
        //TODO: the cap is probably much higher than this btw
        //theres probably something else we could do here (discard old memory?)
        else if (poolCount >= maxAllocCount) {
            throw std::runtime_error("ran out of device memory");
        }
        //TODO: now this is where the "deallocation" comes into play, when we cant allocate anymore data, we need to reuse
        //      old data that the user sent a deallocation call for.

        /*
        std::cout << "hello" << std::endl;

        std::cout << "memory: " << pools[0].memory << std::endl;

        std::cout << "memorytypeindex" << pools[0].memoryTypeIndex << std::endl;
        for (size_t i = 0; i < pools[0].offsets.size(); i++) {
            std::cout << pools[0].offsets[i] << ", ";
        }
        std::cout << "\n" << std::endl;
        for (size_t i = 0; i < pools[0].sizes.size(); i++) {
            std::cout << pools[0].sizes[i] << ", ";
        }
        std::cout << "\n" << std::endl;
        */

        MemoryData memoryData{};
        memoryData.memory = pools[poolIndex].memory;
        memoryData.offset = pools[poolIndex].offsets[poolSizeIndex];
        memoryData.resourceSize = resourceSize;
        memoryData.poolIndex = poolIndex;
        memoryData.poolOffsetIndex = poolSizeIndex;
        
        //TODO: should be getting rid of index's where their isnt actually any space left.
        //this should be safe because we've already done the work above to make sure that this data can fit within the space of memory
        //TODO: this is making the assumption that the user will allocate this much data, if they dont their will be problems for the allocator.
        pools[poolIndex].offsets[poolSizeIndex] += memRequirements.alignment;
        pools[poolIndex].sizes[poolSizeIndex] -= resourceSize;

        return memoryData;
    }

    void MemoryPool::deallocate(VkDevice device, VkBuffer buffer, MemoryData memoryData) {
        //instead of actually deallocating the memory and wasting time, allocating it later
        //we'll just tell the user we deallocated the data while keeping it to write new data later.
        pools[memoryData.poolIndex].offsets[memoryData.poolOffsetIndex] = memoryData.offset;
        
        //TODO: this is inefficient as fuck and i dont see it scaling very well
        pools[memoryData.poolIndex].sizes[memoryData.poolOffsetIndex] += memoryData.resourceSize;

        //delete buffer now
        vkDestroyBuffer(device, buffer, nullptr);
    }

    void MemoryPool::createMemoryPool(VkPhysicalDevice physicalDevice, VkDevice device, Pool* pool) {
        //this is completely fresh pool so the size and offset is obvious
        pool->sizes.resize(1);
        pool->offsets.resize(1);

        pool->sizes[0] = allocationSize;
        pool->offsets[0] = 0;

        //allocate a large chunk of memory,
        VkMemoryAllocateInfo memoryInfo{};
        memoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

        //query for the allocation size?
        //to create the staging buffer I need to allocate memory for cpu and gpu?
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

        uint32_t memoryTypeIndex = findMemoryType(physicalDevice, UINT32_MAX, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        //TODO: some way to query the given device to figure out what is the max allocation i can safely make

        memoryInfo.allocationSize = allocationSize;
        memoryInfo.memoryTypeIndex = memoryTypeIndex;

        VkResult allocResult = vkAllocateMemory(device, &memoryInfo, nullptr, &pool->memory);
        if (allocResult != VK_SUCCESS) {
            std::cout << allocResult << std::endl;
            throw std::runtime_error("could not allocate memory for memory pool");
        }      
    }

    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
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

    void destroyMemoryPool(VkDevice device, MemoryPool memoryPools, const VkAllocationCallbacks* pAllocator) {
        auto pools = memoryPools.pools;
        for (size_t i = 0; i < pools.size(); i++) {
            vkFreeMemory(device, pools[i].memory, pAllocator);
        }
    }

}