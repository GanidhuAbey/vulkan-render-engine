#include "engine_graphics.hpp"
#include "common.hpp"

using namespace graphics;

void EngineGraphics::recreateSwapChain() {
    //probably shouldn't recreate any of these till the device has caught up to the most recent call
    vkDeviceWaitIdle(engineInit->device);

    create::SwapChainSupport details(engineInit->physicalDevice, engineInit->surface);
    VkSurfaceFormatKHR surfaceFormat = chooseSwapChainFormat(details.formats);
    createSwapChain(); //
    //createSwapChain(VK_FORMAT_D16_UNORM, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, &depthChain, &depthImages);

    //vkDestroySwapchainKHR(device, swapChain, nullptr);
    //vkDestroyPipeline(device, graphicsPipeline, nullptr);
    //vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

    //vkDestroyRenderPass(device, renderPass, nullptr);

    //createImageViews(&swapChainDepthImageViews, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, swapChainImageFormat); //create depth images

    //recreate the depth data
    createDepthResources();

    createColorImageViews(); //creates color images
    createRenderPass(); //
    //createGraphicsPipeline(); //
    createFrameBuffers(); //
    createCommandBuffers(vertexBuffer, indexBuffer, indexCount); //
}

void EngineGraphics::cleanupSwapChain(bool destroyAll) {
    vkDeviceWaitIdle(engineInit->device);

    vkFreeCommandBuffers(engineInit->device, engineInit->commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    for (const auto& framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(engineInit->device, framebuffer, nullptr);
    }
    for (size_t i = 0; i < swapChainImages.size(); i++) {
        vkDestroyImageView(engineInit->device, swapChainColorImageViews[i], nullptr);
        //vkDestroyImageView(engineInit->device, swapChainDepthImageViews[i], nullptr);
    }
    vkDestroyImageView(engineInit->device, depthImageView, nullptr);
    vkFreeMemory(engineInit->device, depthMemory, nullptr);
    vkDestroyImage(engineInit->device, depthImage, nullptr);

    if (destroyAll) {
        vkDestroySwapchainKHR(engineInit->device, swapChain, nullptr);
        vkDestroyPipeline(engineInit->device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(engineInit->device, pipelineLayout, nullptr);
        vkDestroyRenderPass(engineInit->device, renderPass, nullptr);
    } else {
        vkDestroySwapchainKHR(engineInit->device, swapChain, nullptr);
        vkDestroyRenderPass(engineInit->device, renderPass, nullptr);
    }
}

VkPipelineShaderStageCreateInfo EngineGraphics::fillShaderStageStruct(VkShaderStageFlagBits stage, VkShaderModule shaderModule) {
    VkPipelineShaderStageCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.stage = stage;
    createInfo.module = shaderModule;
    createInfo.pName = "main";

    return createInfo;
}

VkShaderModule EngineGraphics::createShaderModule(std::vector<char> shaderCode) {
    VkShaderModule shaderModule;

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = static_cast<uint32_t>( shaderCode.size() );

    const uint32_t* shaderFormatted = reinterpret_cast<const uint32_t*>(shaderCode.data());

    createInfo.pCode = shaderFormatted;

    if (vkCreateShaderModule(engineInit->device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("could not create shader module");
    }

    return shaderModule;
}

//Searches for the best available format (color space and color support for pixels)
VkSurfaceFormatKHR EngineGraphics::chooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    //could do some system to rank the other formats, but honestly i have no idea what the difference are so
    return availableFormats[0];
}

//find the best swap chain method, that is the method by which the images being presented to the screen that images being drawn are being swapped.
VkPresentModeKHR EngineGraphics::chooseSwapChainPresentation(const std::vector<VkPresentModeKHR>& availablePresentations) {
    for (const auto& availablePresentation : availablePresentations) {
        //the most ideal method is VK_PRESENT_MODE_MAILBOX_KHR which pushes images into a queue where they are presented one by one to the display on each refresh
        //of the monitor, and when the queue is full it overides the move recent image added to to the queue
        if (availablePresentation == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentation;
        }
    }

    //apparently this is guaranteed to be available.
    //this presentation method pushes images into a queue to be displayed as well but when the queue is full it blocks other images from being added till
    //there is space in the queue.
    return VK_PRESENT_MODE_FIFO_KHR;
}


VkExtent2D EngineGraphics::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(engineInit->window, &width, &height);

        VkExtent2D surfaceExtent {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        //by using clamp we can guarantee that surfaceExtent will be within the max and min values
        surfaceExtent.width = std::clamp(surfaceExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        surfaceExtent.height = std::clamp(surfaceExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return surfaceExtent;
    }
}

void EngineGraphics::initialize(create::EngineInit* initEngine) {
    engineInit = initEngine;

    createSwapChain(); //
    createDepthResources();
    createColorImageViews(); //creates color images
    createRenderPass(); //
    createDescriptorSetLayout();
    createGraphicsPipeline(); //
    createFrameBuffers(); //
    createSemaphores();
    createFences();
}

EngineGraphics::~EngineGraphics() {
    vkDeviceWaitIdle(engineInit->device);

    std::cout << "graphics destruction..." << std::endl;

    for (size_t i = 0; i < descriptorPools.size(); i++) {
        vkDestroyDescriptorPool(engineInit->device, descriptorPools[i], nullptr);
    }
    vkDestroyDescriptorSetLayout(engineInit->device, setLayout,  nullptr);

    //destroy depth data
    vkDestroyImageView(engineInit->device, depthImageView, nullptr);
    vkFreeMemory(engineInit->device, depthMemory, nullptr);
    vkDestroyImage(engineInit->device, depthImage, nullptr);

    //vkFreeMemory(engineInit->device, vertexMemory, nullptr);
    //vkDestroyBuffer(engineInit->device, vertexBuffer, nullptr);

    //vkFreeCommandBuffers(engineInit->device, engineInit->commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

    for (const auto& framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(engineInit->device, framebuffer, nullptr);
    }
    for (size_t i = 0; i < swapChainImages.size(); i++) {
        vkDestroyImageView(engineInit->device, swapChainColorImageViews[i], nullptr);
        //vkDestroyImageView(engineInit->device, swapChainDepthImageViews[i], nullptr);
    }

    //TODO: rewriting code here already found in cleanupSwapChain()

    vkDestroySwapchainKHR(engineInit->device, swapChain, nullptr);
    vkDestroyPipeline(engineInit->device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(engineInit->device, pipelineLayout, nullptr);
    vkDestroyRenderPass(engineInit->device, renderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(engineInit->device, imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(engineInit->device, renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(engineInit->device, inFlightFences[i], nullptr);
    }


    //delete engineInit;
}

void EngineGraphics::createSwapChain() {
    create::SwapChainSupport details(engineInit->physicalDevice, engineInit->surface);

    VkSurfaceCapabilitiesKHR surfaceCapabilities{};
    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(engineInit->physicalDevice, engineInit->surface, &surfaceCapabilities) != VK_SUCCESS) {
        throw std::runtime_error("could not retrieve surface capabilities");
    }

    VkSurfaceFormatKHR surfaceFormat = chooseSwapChainFormat(details.formats);
    VkPresentModeKHR presentMode = chooseSwapChainPresentation(details.presentModes);
    VkExtent2D extent = chooseSwapExtent(details.capabilities);

    //dont try max, the swap chain can always create more than this, we're just giving the absolute minimum it can produce without crashing.
    //if we set to max it will probably always crash.
    uint32_t imageQueue = details.capabilities.minImageCount + 1;

    uint32_t maxCount = details.capabilities.maxImageCount;
    if (maxCount > 0 && imageQueue > maxCount) {
        imageQueue = maxCount;
    }

    //create swapchain vulkan object
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.surface = engineInit->surface;
    createInfo.minImageCount = imageQueue;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.presentMode = presentMode;
    //TODO: try with false later
    createInfo.clipped = VK_TRUE;

    create::QueueData indices(engineInit->physicalDevice, engineInit->surface);

    uint32_t queueIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily.value() != indices.presentFamily.value()) {


        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    //TODO: can save resources if an old swap chain can be handed over here.
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(engineInit->device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("could not initialize the swap chain");
    };

    //grab swapchain images
    uint32_t numImages = 0;
    vkGetSwapchainImagesKHR(engineInit->device, swapChain, &numImages, nullptr);

    //i dont think numImages can ever be 0 so this should be fine
    //if an error does occur it should probably get caught in the validation layer before we get undefined behaviour
    swapChainImages.resize(numImages);
    vkGetSwapchainImagesKHR(engineInit->device, swapChain, &numImages, swapChainImages.data());


    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

void EngineGraphics::createDepthResources() {
    createDepthImage();
    createImageMemory(depthImage);
    createImageView(VK_FORMAT_D16_UNORM, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImage, VK_IMAGE_ASPECT_DEPTH_BIT, &depthImageView);

}

void EngineGraphics::createDepthImage() {
    createImage(VK_FORMAT_D16_UNORM, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, &depthImage);
}

void EngineGraphics::createImage(VkFormat format, VkImageUsageFlags usage,  VkImage* image) {

    create::QueueData indices(engineInit->physicalDevice, engineInit->surface);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext = nullptr;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = format;
    VkExtent3D newExtent{};
    newExtent.width = swapChainExtent.width;
    newExtent.height = swapChainExtent.height;
    newExtent.depth = 1; //no clue how this setting will affect things
    imageInfo.extent = newExtent;
    //imageInfo.extent = swapChainExtent;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.queueFamilyIndexCount = 1;
    imageInfo.pQueueFamilyIndices = &indices.graphicsFamily.value();
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    vkCreateImage(engineInit->device, &imageInfo, nullptr, image);
}

void EngineGraphics::createImageMemory(VkImage image) {
    VkMemoryRequirements memoryReq{};
    vkGetImageMemoryRequirements(engineInit->device, image, &memoryReq);

    //create some memory for the image
    VkMemoryAllocateInfo memoryAlloc{};
    memoryAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAlloc.allocationSize = memoryReq.size;

    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(engineInit->physicalDevice, &memoryProperties);

    VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    uint32_t memoryIndex = 0;
    //uint32_t suitableMemoryForBuffer = 0;
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (memoryReq.memoryTypeBits & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties == properties)) {
            memoryIndex = i;
            break;
        }
    }



    memoryAlloc.memoryTypeIndex = memoryIndex;

    vkAllocateMemory(engineInit->device, &memoryAlloc, nullptr, &depthMemory);

    vkBindImageMemory(engineInit->device, depthImage, depthMemory, 0);
}

void EngineGraphics::createImageView(VkFormat format, VkImageUsageFlags usage, VkImage image, VkImageAspectFlags aspectFlag, VkImageView* imageView) {
    VkImageViewUsageCreateInfo usageInfo{};
    usageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO;
    usageInfo.usage = usage;

    //setup create struct for image views
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.pNext = &usageInfo;
    createInfo.image = image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = format;


    //this changes the colour output of the image, currently set to standard colours
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    //deciding on how many layers are in the image, and if we're using any mipmap levels.
    //TODO: come back here when you know what those mean
    //layers are used for steroscopic 3d applications in which you would provide multiple images to each eye, creating a 3D effect.
    //mipmap levels are an optimization made so that lower quality textures are used when further away to save resources.
    createInfo.subresourceRange.aspectMask = aspectFlag;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(engineInit->device, &createInfo, nullptr, imageView) != VK_SUCCESS) {
        throw std::runtime_error("one of the image views could not be created");
    }

}

//TODO:create two sets of image views so that i can upload depth data
void EngineGraphics::createColorImageViews() {
    swapChainColorImageViews.resize(swapChainImages.size());

    for (int i = 0; i < swapChainColorImageViews.size(); i++) {
        VkImageViewUsageCreateInfo usageInfo{};
        usageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO;
        usageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        //setup create struct for image views
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.pNext = &usageInfo;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;


        //this changes the colour output of the image, currently set to standard colours
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        //deciding on how many layers are in the image, and if we're using any mipmap levels.
        //TODO: come back here when you know what those mean
        //layers are used for steroscopic 3d applications in which you would provide multiple images to each eye, creating a 3D effect.
        //mipmap levels are an optimization made so that lower quality textures are used when further away to save resources.
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(engineInit->device, &createInfo, nullptr, &swapChainColorImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("one of the image views could not be created");
        }
    }
}
//TODO: can most likely abstract the attachment creation process
//NOTE: input attachments we feed into the shader must be bound to the pipeline in the descriptor set
void EngineGraphics::createRenderPass() {
    //create a depth attachment and a depth subpass
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = VK_FORMAT_D16_UNORM;//format must be a depth/stencil format
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    //colour buffer is a buffer for the colour data at each pixel in the framebuffer, obviously important for actually drawing to screen
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    //when an image is being rendered to this is asking whether to clear everything that was on the image or store it to be readable.
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    //this is too make after the rendering too screen is done makes sure the rendered contents aren't cleared and are still readable.
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    //TODO: dont know much about stencilling, seems to be something about colouring in the image from a different layer?
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    //TODO: what does the undefined here mean? that it can be anything?
    //"The initialLayout specifies which layout the image will have before the render pass begins" - vulkan tutorial
    //"Using VK_IMAGE_LAYOUT_UNDEFINED for initialLayout means that we don't care what previous layout the image was in" - vulkan tutorial
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; //the other subpass does not affect the layout of the image this subpass uses.
    //the final layout most likely means what layout the image should be transferred to at the end
    //and since we wont to present to the screen this would probably always remain as this
    //"The finalLayout specifies the layout to automatically transition to when the render pass finishes" - vulkan tutorial
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    //these are subpasses you can make in the render pass to add things depending on the framebuffer in previous passes.
    //i'd assume that if you were to use these for things like post-processing you wouldn't be able to clear the image on load like
    //we do here
    VkAttachmentReference colorAttachmentRef{};
    //this refers to where the VkAttachment is and since we only have one the '0' would point to it.
    colorAttachmentRef.attachment = 0;
    //our framebuffer only has a color buffer attached to it so this layout will help optimize it
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription depthSubpass{};
    depthSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    depthSubpass.pDepthStencilAttachment = &depthAttachmentRef;

    //creating the actual subpass using the reference we created above
    VkSubpassDescription colorSubpass{};
    colorSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    colorSubpass.colorAttachmentCount = 1;
    colorSubpass.pColorAttachments = &colorAttachmentRef;
    colorSubpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkRenderPassCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount = 2;
    VkAttachmentDescription attachments[2] = {colorAttachment, depthAttachment};
    createInfo.pAttachments = attachments;
    createInfo.subpassCount = 1;
    VkSubpassDescription subpasses[1] = {colorSubpass};
    createInfo.pSubpasses = subpasses;
    //createInfo.dependencyCount = 1;
    //createInfo.pDependencies = &dependency;


    if (vkCreateRenderPass(engineInit->device, &createInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("could not create render pass");
    }
}

void EngineGraphics::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(engineInit->device, &layoutInfo, nullptr, &setLayout) != VK_SUCCESS) {
        throw std::runtime_error("could not create descriptor set");
    }
}

void EngineGraphics::createDescriptorPools() {
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(swapChainImages.size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());
    //poolInfo.flags = 0;

    size_t currentSize = descriptorPools.size();
    descriptorPools.resize(currentSize + 1);
    if (vkCreateDescriptorPool(engineInit->device, &poolInfo, nullptr, &descriptorPools[currentSize]) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}
//TODO: this buffer parameter is useless
void EngineGraphics::createDescriptorSets(VkDeviceSize bufferSize, VkBuffer buffer) {
    std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), setLayout);

    VkDescriptorSetAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool = descriptorPools[descriptorPools.size() - 1];
    allocateInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
    allocateInfo.pSetLayouts = layouts.data();

    size_t currentSize = descriptorSets.size();
    descriptorSets.resize(currentSize + 1);
    descriptorSets[currentSize].resize(swapChainImages.size());
    if (vkAllocateDescriptorSets(engineInit->device, &allocateInfo, descriptorSets[currentSize].data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = bufferSize;

        VkWriteDescriptorSet writeInfo{};
        writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeInfo.dstBinding = 0;
        writeInfo.dstSet = descriptorSets[currentSize][i];
        writeInfo.descriptorCount = 1;
        writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeInfo.pBufferInfo = &bufferInfo;
        writeInfo.dstArrayElement = 0;

        vkUpdateDescriptorSets(engineInit->device, 1, &writeInfo, 0, nullptr);
    }

}

void EngineGraphics::createGraphicsPipeline()  {
    //load in the appropriate shader code for a triangle
    auto vertShaderCode = engineInit->readFile("shaders/vert.spv");
    auto fragShaderCode = engineInit->readFile("shaders/frag.spv");

    //convert the shader code into a vulkan object
    VkShaderModule vertShader = createShaderModule(vertShaderCode);
    VkShaderModule fragShader = createShaderModule(fragShaderCode);

    //create shader stage of the graphics pipeline
    VkPipelineShaderStageCreateInfo createVertShaderInfo = fillShaderStageStruct(VK_SHADER_STAGE_VERTEX_BIT, vertShader);
    VkPipelineShaderStageCreateInfo createFragShaderInfo = fillShaderStageStruct(VK_SHADER_STAGE_FRAGMENT_BIT, fragShader);

    const VkPipelineShaderStageCreateInfo shaderStages[] = {createVertShaderInfo, createFragShaderInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = bindingCount;

    VkVertexInputBindingDescription bindingDescrip{};
    bindingDescrip.binding = 0;
    bindingDescrip.stride = sizeof(data::Vertex2D);
    bindingDescrip.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescrip;

    vertexInputInfo.vertexAttributeDescriptionCount = 2;

    VkVertexInputAttributeDescription posAttribute{};
    posAttribute.location = 0;
    posAttribute.binding = 0;
    posAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    posAttribute.offset = 0;

    VkVertexInputAttributeDescription colorAttribute{};
    colorAttribute.location = 1;
    colorAttribute.binding = 0;
    colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    colorAttribute.offset = offsetof(data::Vertex2D, color);

    VkVertexInputAttributeDescription attributeDescriptions[] = {posAttribute, colorAttribute};

    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = swapChainExtent.width;
    viewport.height = swapChainExtent.height;
    viewport.minDepth = 0.0;
    viewport.maxDepth = 1.0;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportInfo{};

    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.viewportCount = 1;
    viewportInfo.pViewports = &viewport;
    viewportInfo.scissorCount = 1;
    viewportInfo.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
    rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationInfo.depthClampEnable = VK_FALSE;
    rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    //TODO: try to enable the wideLines gpu feature
    rasterizationInfo.lineWidth = 1.0f;
    rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
    rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationInfo.depthBiasEnable = VK_FALSE;
    rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
    rasterizationInfo.depthBiasClamp = 0.0f; // Optional
    rasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendInfo.logicOpEnable = VK_FALSE;
    colorBlendInfo.attachmentCount = 1;
    colorBlendInfo.pAttachments = &colorBlendAttachment;

    VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
    depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilInfo.pNext = nullptr;
    depthStencilInfo.flags = 0;
    depthStencilInfo.depthTestEnable = VK_TRUE;
    depthStencilInfo.depthWriteEnable = VK_TRUE;
    depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilInfo.stencilTestEnable = VK_FALSE;

    float blendValues[4] = {0.0, 0.0, 0.0, 0.5};
    colorBlendInfo.blendConstants[0] = blendValues[0];
    colorBlendInfo.blendConstants[1] = blendValues[1];
    colorBlendInfo.blendConstants[2] = blendValues[2];
    colorBlendInfo.blendConstants[3] = blendValues[3];

    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicInfo{};
    dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicInfo.dynamicStateCount = 2;
    dynamicInfo.pDynamicStates = dynamicStates;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &setLayout;

    if (vkCreatePipelineLayout(engineInit->device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("could not create pipeline layout");
    }

    VkGraphicsPipelineCreateInfo createGraphicsPipelineInfo{};
    createGraphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    createGraphicsPipelineInfo.stageCount = 2;
    createGraphicsPipelineInfo.pStages = shaderStages;
    createGraphicsPipelineInfo.pVertexInputState = &vertexInputInfo;
    createGraphicsPipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    createGraphicsPipelineInfo.pViewportState = &viewportInfo;
    createGraphicsPipelineInfo.pRasterizationState = &rasterizationInfo;
    createGraphicsPipelineInfo.pMultisampleState = &multisampling;
    createGraphicsPipelineInfo.pColorBlendState = &colorBlendInfo;
    createGraphicsPipelineInfo.pDepthStencilState  = &depthStencilInfo;
    createGraphicsPipelineInfo.pDynamicState = &dynamicInfo;
    createGraphicsPipelineInfo.layout = pipelineLayout;
    createGraphicsPipelineInfo.renderPass = renderPass;
    createGraphicsPipelineInfo.subpass = 0;

    if (vkCreateGraphicsPipelines(engineInit->device, VK_NULL_HANDLE, 1, &createGraphicsPipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("could not create graphics pipeline");
    }

    //destroy the used shader object
    vkDestroyShaderModule(engineInit->device, vertShader, nullptr);
    vkDestroyShaderModule(engineInit->device, fragShader, nullptr);

}

void EngineGraphics::createFrameBuffers() {
    //get the number of images we need to create framebuffers for
    size_t imageNum = swapChainImages.size();
    //resize framebuffer vector to fit as many frame buffers as images
    swapChainFramebuffers.resize(imageNum);

    //iterate through all the frame buffers
    for (int i = 0; i < imageNum; i++) {
        //create a frame buffer
        VkFramebufferCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.renderPass = renderPass;
        //we only want one image per frame buffer
        createInfo.attachmentCount = 2;
        //they put the image view in a separate array for some reason
        VkImageView imageViews[2] = {swapChainColorImageViews[i], depthImageView};
        createInfo.pAttachments = imageViews;
        createInfo.width = swapChainExtent.width;
        createInfo.height = swapChainExtent.height;
        createInfo.layers = 1;

        if (vkCreateFramebuffer(engineInit->device, &createInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("could not create a frame buffer");
        }
    }
}
//TODO: use seperate command pool for memory optimizations
void EngineGraphics::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size) {
    //create command buffer
    VkCommandBuffer transferBuffer;

    VkCommandBufferAllocateInfo bufferAllocate{};
    bufferAllocate.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    bufferAllocate.commandPool = engineInit->commandPool;
    bufferAllocate.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    bufferAllocate.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(engineInit->device, &bufferAllocate, &transferBuffer) != VK_SUCCESS) {
        throw std::runtime_error("could not allocate memory for transfer buffer");
    }

    //begin command buffer
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(transferBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("one of the command buffers failed to begin");
    }

    //transfer between buffers
    VkBufferCopy copyData{};
    copyData.srcOffset = 0;
    //TODO: need to allocate memory and choose a proper offset for this
    copyData.dstOffset = dstOffset;
    copyData.size = size;

    vkCmdCopyBuffer(transferBuffer,
        srcBuffer,
        dstBuffer,
        1,
        &copyData
    );

    //end command buffer
    if (vkEndCommandBuffer(transferBuffer) != VK_SUCCESS) {
        throw std::runtime_error("could not create succesfully end transfer buffer");
    };

    //destroy transfer buffer, shouldnt need it after copying the data.
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &transferBuffer;

    vkQueueSubmit(engineInit->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(engineInit->graphicsQueue);

    vkFreeCommandBuffers(engineInit->device, engineInit->commandPool, 1, &transferBuffer);
}

void EngineGraphics::createCommandBuffers(VkBuffer buffer, VkBuffer indBuffer, size_t indexCount) {
    vertexBuffer = buffer;
    indexBuffer = indBuffer;
    //allocate memory for command buffer, you have to create a draw command for each image
    commandBuffers.resize(swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo bufferAllocate{};
    bufferAllocate.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    bufferAllocate.commandPool = engineInit->commandPool;
    bufferAllocate.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    uint32_t imageCount =  static_cast<uint32_t>( commandBuffers.size() );
    bufferAllocate.commandBufferCount = imageCount;

    if (vkAllocateCommandBuffers(engineInit->device, &bufferAllocate, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("could not allocate memory for command buffers");
    }

    //push all my command buffers into an exectute stage.
    std::cout << "y" << std::endl;
    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("one of the command buffers failed to begin");
        }
        //begin a render pass so that we can draw to the appropriate framebuffer
        VkRenderPassBeginInfo renderInfo{};
        renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderInfo.renderPass = renderPass;
        renderInfo.framebuffer = swapChainFramebuffers[i];
        VkRect2D renderArea{};
        renderArea.offset = VkOffset2D {0, 0};
        renderArea.extent = swapChainExtent;
        renderInfo.renderArea = renderArea;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0};
        clearValues[1].depthStencil = {1.0, 0};
        renderInfo.clearValueCount = static_cast<uint32_t>( clearValues.size() );
        renderInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i],  &renderInfo, VK_SUBPASS_CONTENTS_INLINE);
        //run first subpass here?
        //the question is what does the first subpass do?

        //now after the first pass is complete we move on to the next subpass
        //vkCmdNextSubpass(commandBuffers[i], VK_SUBPASS_CONTENTS_INLINE);

        //add commands to command buffer
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        VkViewport newViewport = {};
        newViewport.x = 0;
        newViewport.y = 0;
        newViewport.width = swapChainExtent.width;
        newViewport.height = swapChainExtent.height;
        newViewport.minDepth = 0.0;
        newViewport.maxDepth = 1.0;
        vkCmdSetViewport(commandBuffers[i], 0, 1, &newViewport);

        VkRect2D newScissor{};
        newScissor.offset = {0, 0};
        newScissor.extent = swapChainExtent;
        vkCmdSetScissor(commandBuffers[i], 0, 1, &newScissor);

        //time for the draw calls
        const VkDeviceSize offsets[] = {0, offsetof(data::Vertex2D, color)};
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &buffer, offsets);
        vkCmdBindIndexBuffer(commandBuffers[i], indBuffer, 0, VK_INDEX_TYPE_UINT16);

        //draw first object (cube)
        std::cout << "descriptor count: " << descriptorSets[0].size() << std::endl;
        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[0][i], 0, nullptr);
        vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indexCount), 1, 0, 0, 0);
        //vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(6), 1, 36, 0, 0);
        //vkCmdNextSubpass(commandBuffers[i], VK_SUBPASS_CONTENTS_INLINE);
        vkCmdEndRenderPass(commandBuffers[i]);

        //end commands to go to execute stage
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)  {
            throw std::runtime_error("could not end command buffer");
        }
    }
}

void EngineGraphics::createSemaphores() {
    //create required sephamores
    VkSemaphoreCreateInfo semaphoreBegin{};
    semaphoreBegin.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        if (vkCreateSemaphore(engineInit->device, &semaphoreBegin, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateSemaphore(engineInit->device, &semaphoreBegin, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS) {
            throw std::runtime_error("could not create semaphore ready signal");
        }
}

void EngineGraphics::createFences() {
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);
    VkFenceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateFence(engineInit->device, &createInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create fences");
        };

    }
}

/*
void EngineGraphics::updateUniformBuffers(uint32_t nextImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    size_t imageCount = swapChainImages.size();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    //can define
    ubos.resize(uniformBuffers.size());
    //ubos[nextImage].model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubos[nextImage].view = glm::lookAt(glm::vec3(1.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubos[nextImage].proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 10.0f);
    ubos[nextImage].proj[1][1] *= -1;

    ubos[nextImage + imageCount].view = glm::lookAt(glm::vec3(1.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubos[nextImage + imageCount].proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 10.0f);
    ubos[nextImage + imageCount].proj[1][1] *= -1;


    ubos[nextImage].model = glm::mat4(
        cos(time), -sin(time), 0, 0,
        sin(time), cos(time), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );

    ubos[nextImage + swapChainImages.size()].model = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );


    //std::cout << ubo.translation[2][0] << std::endl;
    std::cout << "\n" << std::endl;
    for (size_t i = 0; i < 4; i++)  {
        std::cout << "[";
        for (size_t j = 0; j < 4; j++) {
            std::cout << ubo.view[i][j] << ", ";
        }
        std::cout << "]" << std::endl;
    }
    //std::cout << "uniform buffer size: " << uniformBuffers.size() << std::endl;
    //std::cout << imageCount << std::endl;
    for (size_t i = 0; i < (uniformMemories.size()/imageCount); i++) {
        void* data;
        vkMapMemory(engineInit->device, uniformMemories[nextImage + (imageCount * i)].memoryHandle, 0, sizeof(ubos[nextImage + (imageCount * i)]), 0, &data);
            memcpy(data, &ubos[nextImage + (imageCount * i)], sizeof(UniformBufferObject));
        vkUnmapMemory(engineInit->device, uniformMemories[nextImage + (imageCount * i)].memoryHandle);
    }
}
*/

void EngineGraphics::drawFrame() {
    //don't bother drawing frames when the window isn't there
    if (glfwGetWindowAttrib(engineInit->window, GLFW_ICONIFIED) == 1) {
        return;
    }

    vkWaitForFences(engineInit->device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    //allocate memory to store next image
    uint32_t nextImage;

    VkResult result = vkAcquireNextImageKHR(engineInit->device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &nextImage);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        cleanupSwapChain(false);
        //VkSwapchainKHR oldSwapChain = swapChain;
        recreateSwapChain();

        return;
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("could not aquire image from swapchain");
    }

    //updateUniformBuffers(nextImage);

    if (imagesInFlight[nextImage] != VK_NULL_HANDLE) {
        vkWaitForFences(engineInit->device, 1, &imagesInFlight[nextImage], VK_TRUE, UINT64_MAX);
        //imagesInFlight[nextImage] = VK_NULL_HANDLE;
    }
    // Mark the image as now being in use by this frame
    imagesInFlight[nextImage] = inFlightFences[currentFrame];

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    //add appropriate command buffer
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[nextImage];
    //signal to send when this command buffer has executed.
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(engineInit->device, 1, &inFlightFences[currentFrame]);

    if (vkQueueSubmit(engineInit->graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) !=  VK_SUCCESS) {
        throw std::runtime_error("could not submit command buffer to queue");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &nextImage;
    presentInfo.pResults = nullptr;

    VkResult presentResult = vkQueuePresentKHR(engineInit->presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        cleanupSwapChain(false);
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
