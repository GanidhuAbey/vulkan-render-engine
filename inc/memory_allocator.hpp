#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "engine_graphics.hpp"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

const uint32_t ALLOCATION_BARRIER = 600000000;

namespace mem {
    class MemoryPool {
        public:
            //TODO: offset should probably be VkDeviceSize
            struct MemoryData {
                VkDeviceSize offset;
                VkDeviceMemory memory;
                VkDeviceSize resourceSize;
                size_t poolIndex;
                size_t poolOffsetIndex;
            };

            struct Pool {
                std::vector<VkDeviceSize> sizes;
                std::vector<VkDeviceSize> offsets;
                uint32_t memoryTypeIndex;
                VkDeviceMemory memory;
            };

            std::vector<Pool> pools;
            size_t poolCount = 0;
            //100 mb of data per allocation
            //TODO: need to create a information struct in the style of vulkan to let user manipulate this data
            VkDeviceSize allocationSize = 1e8;

        public:
            //this would handle allocating the pool for you
            MemoryPool();
            //this would free all the memory pools allocated
            ~MemoryPool();
            //this would allocate your desired memory for you
            MemoryData allocate(VkPhysicalDevice physicalDevice, VkDevice device, VkMemoryPropertyFlags properties, VkBuffer& buffer);
            //this would deallocate your desired memory for you
            void deallocate(VkDevice device, VkBuffer buffer, MemoryData memoryData);

        private:
            void createMemoryPool(VkPhysicalDevice physicalDevice, VkDevice device, Pool* pool);
            void destroyMemoryPool(VkDevice device, MemoryPool memoryPool);
    };

    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void destroyMemoryPool(VkDevice device, MemoryPool memoryPools, const VkAllocationCallbacks* pAllocator);

}