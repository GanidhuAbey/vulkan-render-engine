#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "swapchain_support.hpp"
#include "queue.hpp"
#include "data_formats.hpp"

#include "engine_init.hpp"

#include <cstdlib>
#include <algorithm>
#include <fstream>

#include "glm/glm.hpp"

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace graphics {

class EngineGraphics {
    public:
        VkPipeline graphicsPipeline;
        uint32_t bindingCount = 1;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexMemory;
        uint32_t bufferSize = 0;
        std::vector<data::Vertex2D> oldVertices;
        std::vector<VkCommandBuffer> commandBuffers;
    private:
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        VkRenderPass renderPass;

        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        VkSwapchainKHR swapChain;

        create::EngineInit* engineInit;

        uint32_t attributeCount = 2;

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
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    public:
        //EngineGraphics(EngineInit* initEngine);
        void initialize(create::EngineInit* initEngine);
        ~EngineGraphics();
    
    private:
        void createSwapChain(); //
        void createImageViews(); //
        void createRenderPass(); //
        void createGraphicsPipeline(); //
        void createFrameBuffers(); //
        void createSemaphores();
        void createFences();

    public:
        void createVertexBuffer(std::vector<data::Vertex2D> vertices);
        void createCommandBuffers(std::vector<data::Vertex2D> vertices);
        void drawFrame();
        void cleanupRender();
};

}