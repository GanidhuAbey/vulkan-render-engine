#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//#include "engine_graphics.hpp"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

//100 mb of data per allocation
//TODO: need to create a information struct in the style of vulkan to let user manipulate this data
//VkDeviceSize allocationSize = 5e8;

namespace mem {
struct MaMemoryInfo {
    VkDeviceSize bufferSize;
    VkDeviceSize allocationSize;
    VkBufferUsageFlags bufferUsage;
    VkMemoryPropertyFlags memoryProperties;
    uint32_t queueFamilyIndexCount;

};

struct MaMemory {
    VkDeviceMemory memoryHandle;
    std::vector<VkDeviceSize> sizes;
    std::vector<VkDeviceSize> offsets;
    VkDeviceSize alignmentMultiple = 0;
<<<<<<< HEAD
<<<<<<< HEAD
    VkDeviceSize alignmentOffset = 0;
    VkDeviceSize offsetAmt = 0;
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
};

struct MaMemoryData {
    VkDeviceMemory memoryHandle;
    VkDeviceSize offset;
    VkDeviceSize resourceSize;
    size_t offsetIndex;
};

uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

void maCreateMemory(VkPhysicalDevice physicalDevice, VkDevice device, MaMemoryInfo* poolInfo, VkBuffer* buffer, MaMemory* maMemory);
MaMemory maAllocateMemory(MaMemory maMemory, VkDeviceSize resourceSize, MaMemoryData* memoryData);
void maFreeMemory(MaMemory* maMemory, MaMemoryData memoryData);
void maDestroyMemory(VkDevice device, MaMemory maMemory, VkBuffer buffer);

} // namespace mem