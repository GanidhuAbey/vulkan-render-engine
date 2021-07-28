#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "swapchain_support.hpp"
#include "queue.hpp"
#include "data_formats.hpp"
#include "memory_allocator.hpp"

#include "engine_init.hpp"

#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace graphics {

class EngineGraphics {
    public:
        VkPipeline graphicsPipeline;
        uint32_t bindingCount = 1;
        uint32_t bufferSize = 0;
        std::vector<data::Vertex2D> oldVertices;
        std::vector<VkCommandBuffer> commandBuffers;

        struct UniformBufferObject {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };

        std::vector<UniformBufferObject> ubos;

    private:
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        VkRenderPass renderPass;

        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainColorImageViews;
        std::vector<VkImageView> swapChainDepthImageViews;

        VkSwapchainKHR swapChain;

        create::EngineInit* engineInit;

        uint32_t attributeCount = 2;

        VkDescriptorSetLayout setLayout;
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSet;
        VkPipelineLayout pipelineLayout;

        std::vector<VkFramebuffer> swapChainFramebuffers;

        //this one signals when an image has been presented and is available for rendering
        std::vector<VkSemaphore> imageAvailableSemaphores;
        //this one signals when an image has finished been rendered to and is awaiting presentation
        std::vector<VkSemaphore> renderFinishedSemaphores;
        //this fence signals when a frame has run through and returned from being rendered and presented
        //so that the current frame can be drawn again.
        //this prevents the cpu from idly forcing draw calls while images are not available.
        std::vector<VkFence> inFlightFences;
        size_t currentFrame = 0;
        //adds another check that the image we are about to send to flight, is not an image that was rendered in
        //the last frame (if it was then we wait the fence to return a signal before we continue) and then we dont waste
        //energy
        std::vector<VkFence> imagesInFlight;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<mem::MaMemory> uniformMemories;

        std::vector<data::Vertex2D> verts;
        std::vector<uint16_t> indexes;

        

    private:
        void recreateSwapChain();
        void cleanupSwapChain(bool destroyAll);
        VkPipelineShaderStageCreateInfo fillShaderStageStruct(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
        VkShaderModule createShaderModule(std::vector<char> shaderCode);
        static std::vector<char> readFile(const std::string& filename);
        VkSurfaceFormatKHR chooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapChainPresentation(const std::vector<VkPresentModeKHR>& availablePresentations);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void fillRequiredValues(VkPhysicalDevice physicalDeviceUser, VkDevice deviceUser, VkSurfaceKHR surfaceUser);
        void updateUniformBuffers(uint32_t nextImage);

    public:
        //EngineGraphics(EngineInit* initEngine);
        void initialize(create::EngineInit* initEngine);
        ~EngineGraphics();
    
    private:
        void createSwapChain(); //
        std::vector<VkImage> createImages(size_t imageNum);
        void createImageViews(VkImageUsageFlags usage, VkFormat format);  //
        void createRenderPass(); //
        void createDescriptorSetLayout();
        void createDescriptorPools();
        void createGraphicsPipeline(); //
        void createFrameBuffers(); //
        void createSemaphores();
        void createFences();

    public:
        void createCommandBuffers(VkBuffer buffer, VkBuffer indexBuffer, size_t indexCount);
        void drawFrame();
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void createDescriptorSets(VkBuffer buffer);
        void createUniformBuffer(size_t bufferCount);
};

}