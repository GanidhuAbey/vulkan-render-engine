//DOCUMENTATION: https://vulkan.lunarg.com/doc/view/latest/windows/apispec.html#_vkcmdbeginquery3

//Questions: What exactly can you do we pNext?
//               - was used one in extending instance create info to make sure that the debugger had the ability to log what happened with it.
//               - i think it allows the user to basically extend the functionalaity of the vulkan object being created but extending it into other created vulkan objects
//                 so in the instance/debug example I extended the ability of the instance object to also give information to the Debug Utils

//Add some safety checks to see if Vulkan is enabled
//include GLFW and vulkan
#define VK_USE_PLATFORM_XCB_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

//uncomment this line to disable debug mode
//#define NDEBUG

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <typeinfo>
#include <string.h>
#include <string>
#include <map>
#include <optional>
#include <set>
#include <algorithm>
#include <fstream>



//TODO: can i change this to a #define?
//screen size, maybe move to a header
const uint32_t WIDTH  = 800;
const uint32_t HEIGHT = 600;

//how many images can be rendered at the same time
const int MAX_FRAMES_IN_FLIGHT = 2;

//this is the standard validation layer that is included in the vulkan sdk
//if more validation layers are needed then its best to add them here
//(i think)
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

void glfwErrorCallback(int code, const char* description) {
    std::cerr << "GLFW Error " << code << ": " << description << std::endl;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
const VkAllocationCallbacks* pAllocator, 
VkDebugUtilsMessengerEXT* pDebugMessenger) {
    //basically casting the return of vkGetInstanceProcAddr to a pointer function of the function being loaded.
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    //check if function successfully loaded
    if (func != nullptr) {
        //why dont I need messengar?
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        //TODO: figure out best error message
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

}


void DestroyDebugUtilsMessengerEXT(VkInstance instance,
VkDebugUtilsMessengerEXT pDebugMessenger,
const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != nullptr) {
        func(instance, pDebugMessenger, pAllocator);
    }
}

//this class can be moved to a header
class HelloTriangleApplication {
public:
    void run() {
        //setup glfw window
        initWindow();
       
        //this initializes any of the vulkan related objects we will need
        initVulkan();
        //this is where the frames are rendered and updated using the vulkan objects
        mainLoop();
        //once the window is closed the vulkan objects are freed from memory here
        cleanup();
    }
private:
    //glfw window
    GLFWwindow* window;
    //vulkan instance
    VkInstance instance;
    //debug extension messenger
    VkDebugUtilsMessengerEXT debugMessenger;
    //physical device
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    //logical device
    VkDevice device;
    //Graphics Queue
    VkQueue graphicsQueue;
    //Presentation Queue
    VkQueue presentQueue;
    //Window surface which vulkan draws on
    VkSurfaceKHR surface;
    //SwapChain: handles an image queue allowing one image to be presented to the display while simultaneously rendering another image in the background
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    //this will basically grab data from the actual images and will be what interacts with our code to update the images
    std::vector<VkImageView> swapChainImageViews;

    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;

    VkPipeline graphicsPipeline;

    //there is a frame buffer assosicated with each image, the frame buffer has the colour buffer attachment which we will use to actually colour the
    //triangle we are presenting
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

private:
    void initWindow() {
        //initializes glfw
        //add check for initialize failure
        glfwInit();

        //setup debug callback if user enabled debug layers.
        if (enableValidationLayers) {
            glfwSetErrorCallback(glfwErrorCallback);
        }

        //turn off opengl context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        //turn off ability to resize window
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        //create a basic window
        window = glfwCreateWindow(WIDTH, HEIGHT, "vulkan renderer", nullptr, nullptr);
    }

    void initVulkan() {
        createInstance();
        createSurface();
        setupDebugMessenger();
        //pick graphics card that meet required vulkan features
        //TODO: add a method for the user to control this?
        pickPhysicalDevice();
        createLogicalDevice();
        createSwapChain();
        createImageViews();
        createRenderPass();
        createGraphicsPipeline();
        createFrameBuffers();
        createCommandPool();
        createCommandBuffers();
        createSemaphores();
        createFences();
    }

    void mainLoop() {
        //loop continues till a close request is made to window
        while (!glfwWindowShouldClose(window)) {
            //this function will process events, it seems that their is no need to catch the event
            glfwPollEvents();
            drawFrame();
        }
        //going to wait for the device processes to finish before moving on.
        vkDeviceWaitIdle(device);
    }

    void cleanup() {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }
        //reset command  buffers TODO: move somewhere else when we abstract the draw calls
        if (vkResetCommandPool(device, commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT) != VK_SUCCESS) {
            throw std::runtime_error("could not reset command pool");
        }

        vkDestroyCommandPool(device, commandPool, nullptr);
        //if debug mode is on then destroy debug messengar
        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }
        for (const auto& framebuffer : swapChainFramebuffers) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        for (const auto& imageView : swapChainImageViews) {
            vkDestroyImageView(device, imageView, nullptr);
        }


        vkDestroySwapchainKHR(device, swapChain, nullptr);

        vkDestroyPipeline(device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

        vkDestroyRenderPass(device, renderPass, nullptr);

        vkDestroyDevice(device, nullptr);

        vkDestroySurfaceKHR(instance, surface, nullptr);
        //destroy vulkan instance
        vkDestroyInstance(instance, nullptr);

        //end window processes
        glfwDestroyWindow(window);
        glfwTerminate();

    }
    void drawFrame() {
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
        //allocate memory to store next image
        uint32_t nextImage;

        if (vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &nextImage) != VK_SUCCESS) {
            throw std::runtime_error("could not aquire next image in swapchain");
        }

        if (imagesInFlight[nextImage] != VK_NULL_HANDLE) {
            vkWaitForFences(device, 1, &imagesInFlight[nextImage], VK_TRUE, UINT64_MAX);
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

        vkResetFences(device, 1, &inFlightFences[currentFrame]);

        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) !=  VK_SUCCESS) {
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

        if (vkQueuePresentKHR(presentQueue, &presentInfo) != VK_SUCCESS) {
            throw std::runtime_error("could not create presentation queue");
        }
        
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void createInstance() {
        //check if debug mode is on and if user device has accessed to the validation layers they set in validationLayers
        //std::cout << (enableValidationLayers && !checkValidationLayerSupport()) << std::endl;
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("the requested validation layers are not available!");
        }

        //fill struct with data on application
        //helps graphics card optimize by knowing more about the application
        VkApplicationInfo appInfo{};

        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";

        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

        //this will create a vulkan 1.0 version which is used in the tutorial.
        //TODO: try a higher version of vulkan to see how the program changes
        appInfo.apiVersion =  VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        std::vector<const char*> extensions = getRequiredExtensions();

        //TODO: move all this into the checkSupportedExtension()
        //check supported extensions
        bool supported = checkSupportedExtensions(extensions);
        if (!supported) {
            throw std::runtime_error("required vulkan extensions are not supported!");
        }

        createInfo.enabledExtensionCount = static_cast<uint32_t>( extensions.size() );
        createInfo.ppEnabledExtensionNames = extensions.data();

        //if not place inside the if statement, this debug info will run out of scope and be freed from memory, when it 
        //is still needed to be able to create an instance.
        VkDebugUtilsMessengerCreateInfoEXT createDebugInfo{};

        //setup debug layers and extensions for instance#define VK_USE_PLATFORM_WIN32_KHR
        if (enableValidationLayers) {
            //convert size int to uint32_t
            createInfo.enabledLayerCount = static_cast<uint32_t>( validationLayers.size() );
            createInfo.ppEnabledLayerNames = validationLayers.data();


            populateDebugMessengerCreateInfo(createDebugInfo);
            //do you need to do this?
            //TODO: see how removing this cast changes things once we draw triangle
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &createDebugInfo;
        }
        //don't need to do this but it will probably be useful to be as descriptive as possible
        else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        //TODO: "pAllocator is not NULL" not sure if possible to make parameter nullptr
        //in future vulkan versions
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance");
        }
    }

    void createSurface() {
        //glfw can do all this for me plus all other platforms and i won't to decide between anything

        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("surface could not be created");
        }
    }

    void setupDebugMessenger() {
        if (!enableValidationLayers) return;

        //fill in object struct
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        
        populateDebugMessengerCreateInfo(createInfo);

        //functions that are part of extensions (like this one) are  not not loaded automatically.
        //inorder to load functions like these we have to use vkGetInstanceProcAddr
        //create object
        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("could not create debug messenger");
        }
        
    }

    void pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("no GPU's on this machine supports vulkan");
        }

        //makes int the key and VkPhysicalDevice the value, multimap is like a dictionary with a key-value pair, but multimap also keeps the values sorted by the keys.
        std::multimap<int, VkPhysicalDevice> candidates;

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            int score = rateDeviceSuitabilty(device);
            candidates.insert(std::make_pair(score, device));
        }

        if (candidates.rbegin()->first > 0) {
            //this device is suitable
            physicalDevice = candidates.rbegin()->second;
        }
        else {
            throw std::runtime_error("could not find a suitable GPU");
        }

        //findQueueFamilies(physicalDevice);
    }

    void createLogicalDevice() {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        //Load Queues
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> queueIndices = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        //iterate through queuefamilies and fill in struct data
        float queuePriority = 1.0f;
        for (uint32_t i : queueIndices) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = i;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        }

        //this defined a struct of features that we can use, right now its set to all VK_FALSE meaning no features are needed
        //later on we will need geometry shaders from these device features (which is why we already queried for it in the pickPhysicalDevice() option)
        VkPhysicalDeviceFeatures deviceFeatures{};

        //filling in the struct for the logical device
        VkDeviceCreateInfo createLInfo{};
        createLInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createLInfo.queueCreateInfoCount = static_cast<uint32_t>( queueCreateInfos.size() );
        //documentation writes this as an array but when only dealing with a single queue, you can just put a single value and its fine.
        createLInfo.pQueueCreateInfos = queueCreateInfos.data();
        createLInfo.pEnabledFeatures = &deviceFeatures;

        //NOTE: creating logical devices also allows us to enable extensions to extend the functionality of Vulkan
        //examples of this would be VK_KHR_swapchain which is a device specific extension designed to present rendered images to the screen.

        //this part is not required for newer versions of vulkan, but older versions still rely on this
        if (enableValidationLayers) {
            //set up device layers for this logical device
            createLInfo.enabledLayerCount = static_cast<uint32_t>( validationLayers.size() );
            createLInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else {
            createLInfo.enabledLayerCount = 0;
        }

        //enable VK_KHR_SWAPCHAIN extension
        createLInfo.enabledExtensionCount = static_cast<uint32_t>( deviceExtensions.size() );
        createLInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (vkCreateDevice(physicalDevice, &createLInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("could not create logical device");
        }

        //TODO: not sure where to put this...
        //grab queue from device
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

    }

    void createSwapChain() {
        SwapChainSupportDetails details = querySwapChainSupport(physicalDevice);

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
        createInfo.surface = surface;
        createInfo.minImageCount = imageQueue;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.presentMode = presentMode;
        //TODO: try with false later
        createInfo.clipped = VK_TRUE;

        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
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
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
            throw std::runtime_error("could not initialize the swap chain");
        };

        //grab swapchain images
        uint32_t numImages = 0;
        vkGetSwapchainImagesKHR(device, swapChain, &numImages, nullptr);

        //i dont think numImages can ever be 0 so this should be fine
        //if an error does occur it should probably get caught in the validation layer before we get undefined behaviour
        swapChainImages.resize(numImages);
        vkGetSwapchainImagesKHR(device, swapChain, &numImages, swapChainImages.data());


        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;

    }

    void createImageViews() {
        swapChainImageViews.resize(swapChainImages.size());

        for (int i = 0; i < swapChainImageViews.size(); i++) {
            //setup create struct for image views
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
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

            if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("one of the image views could not be created");
            }
        }
    }
    void createRenderPass() {
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
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        //the final layout most likely means what layout the image should be transferred to at the end
        //and since we wont to present to the screen this would probably always remain as this
        //"The finalLayout specifies the layout to automatically transition to when the render pass finishes" - vulkan tutorial
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        //these are subpasses you can make in the render pass to add things depending on the framebuffer in previous passes.
        //i'd assume that if you were to use these for things like post-processing you wouldn't be able to clear the image on load like
        //we do here
        VkAttachmentReference colorAttachmentRef{};
        //this refers to where the VkAttachment is and since we only have one the '0' would point to it.
        colorAttachmentRef.attachment = 0;
        //our framebuffer only has a color buffer attached to it so this layout will help optimize it
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        //creating the actual subpass using the reference we created above
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        
        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = &colorAttachment;
        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &subpass;
        createInfo.dependencyCount = 1;
        createInfo.pDependencies = &dependency;



        if (vkCreateRenderPass(device, &createInfo, nullptr, &renderPass) != VK_SUCCESS) {
            throw std::runtime_error("could not create render pass");
        }

    }

    void createGraphicsPipeline()  {
        //load in the appropriate shader code for a triangle
        auto vertShaderCode = readFile("shaders/spirv/vert.spv");
        auto fragShaderCode = readFile("shaders/spirv/frag.spv");

        //convert the shader code into a vulkan object
        VkShaderModule vertShader = createShaderModule(vertShaderCode);
        VkShaderModule fragShader = createShaderModule(fragShaderCode);

        //create shader stage of the graphics pipeline
        VkPipelineShaderStageCreateInfo createVertShaderInfo = fillShaderStageStruct(VK_SHADER_STAGE_VERTEX_BIT, vertShader);
        VkPipelineShaderStageCreateInfo createFragShaderInfo = fillShaderStageStruct(VK_SHADER_STAGE_FRAGMENT_BIT, fragShader);

        const VkPipelineShaderStageCreateInfo shaderStages[] = {createVertShaderInfo, createFragShaderInfo};

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;

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
        rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
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

        float blendValues[4] = {0.0, 0.0, 0.0, 0.5};
        colorBlendInfo.blendConstants[0] = blendValues[0];
        colorBlendInfo.blendConstants[1] = blendValues[1];
        colorBlendInfo.blendConstants[2] = blendValues[2];
        colorBlendInfo.blendConstants[3] = blendValues[3];

        VkDynamicState dynamicStates[] = {
            VK_DYNAMIC_STATE_LINE_WIDTH,
            VK_DYNAMIC_STATE_VIEWPORT
        };

        VkPipelineDynamicStateCreateInfo dynamicInfo{};
        dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicInfo.dynamicStateCount = 2;
        dynamicInfo.pDynamicStates = dynamicStates;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
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
        createGraphicsPipelineInfo.pDynamicState = nullptr;
        createGraphicsPipelineInfo.layout = pipelineLayout;
        createGraphicsPipelineInfo.renderPass = renderPass;
        createGraphicsPipelineInfo.subpass = 0; 

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &createGraphicsPipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("could not create graphics pipeline");
        }

        //destroy the used shader object
        vkDestroyShaderModule(device, vertShader, nullptr);
        vkDestroyShaderModule(device, fragShader, nullptr);

        //std::cout << vertShaderCode.size() << std::endl;
    }

    void createFrameBuffers() {
        //get the number of images we need to create framebuffers for
        size_t imageNum = swapChainImageViews.size();
        //resize framebuffer vector to fit as many frame buffers as images
        swapChainFramebuffers.resize(imageNum);

        //iterate through all the frame buffers
        for (int i = 0; i < imageNum; i++) {
            //create a frame buffer
            VkFramebufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            createInfo.renderPass = renderPass;
            //we only want one image per frame buffer
            createInfo.attachmentCount = 1;
            //they put the image view in a separate array for some reason
            createInfo.pAttachments = &swapChainImageViews[i];
            createInfo.width = swapChainExtent.width;
            createInfo.height = swapChainExtent.height;
            createInfo.layers = 1;

            if (vkCreateFramebuffer(device, &createInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("could not create a frame buffer");
            }
        }
    }

    void createCommandPool() {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        poolInfo.flags = 0; // Optional

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)  {
            throw std::runtime_error("could not create command pool");
        }
    }

    void createCommandBuffers() {
        //allocate memory for command buffer, you have to create a draw command for each image
        commandBuffers.resize(swapChainFramebuffers.size());
        
        VkCommandBufferAllocateInfo bufferAllocate{};
        bufferAllocate.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        bufferAllocate.commandPool = commandPool;
        bufferAllocate.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        uint32_t imageCount =  static_cast<uint32_t>( commandBuffers.size() );
        bufferAllocate.commandBufferCount = imageCount;

        if (vkAllocateCommandBuffers(device, &bufferAllocate, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("could not allocate memory for command buffers");
        }

        //push all my command buffers into an exectute stage.
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

            VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
            renderInfo.clearValueCount = 1;
            renderInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(commandBuffers[i],  &renderInfo, VK_SUBPASS_CONTENTS_INLINE);

            //add commands to command buffer
            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

            //time for the draw calls
            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);

            //end commands to go to execute stage
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)  {
                throw std::runtime_error("could not end command buffer");
            }
        }
    }
    void createSemaphores() {
        //create required sephamores
        VkSemaphoreCreateInfo semaphoreBegin{};
        semaphoreBegin.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
            if (vkCreateSemaphore(device, &semaphoreBegin, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS || 
            vkCreateSemaphore(device, &semaphoreBegin, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS) {
                throw std::runtime_error("could not create semaphore ready signal");
            }       
    }

    void createFences() {
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);
        VkFenceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateFence(device, &createInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create fences");
            };

        }
    }

    VkPipelineShaderStageCreateInfo fillShaderStageStruct(VkShaderStageFlagBits stage, VkShaderModule shaderModule) {
        VkPipelineShaderStageCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo.stage = stage;
        createInfo.module = shaderModule;
        createInfo.pName = "main";

        return createInfo;
    }

    VkShaderModule createShaderModule(std::vector<char> shaderCode) {
        VkShaderModule shaderModule;
        
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = static_cast<uint32_t>( shaderCode.size() );
        
        const uint32_t* shaderFormatted = reinterpret_cast<const uint32_t*>(shaderCode.data());

        createInfo.pCode = shaderFormatted;

        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("could not create shader module");
        }

        return shaderModule;
    }

    static std::vector<char> readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary); 

        if (!file.is_open()) {
            throw std::runtime_error("could not open file");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        //std::cout << fileSize << std::endl;

        file.close();

        return buffer;
    }

    int rateDeviceSuitabilty(VkPhysicalDevice device) {
        int score = 0;

        //grab data of device
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;

        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }
        
        //NOTE: their is also a different limit for 3D image dimensions
        score += deviceProperties.limits.maxImageDimension2D;

        //needed to draw images.
        if (!deviceFeatures.geometryShader) {
            return 0;
        }
        
        //check if needed extensions are supported by device
        if (!checkDeviceExtensionSupport(device)) {
            return 0;
        }

        QueueFamilyIndices indices = findQueueFamilies(device);

        //the '.' operator is used when dealing with the actual object not a pointer to it.
        //can use the isComplete() function but want to keep the above note
        if (!indices.isComplete()) {
            return 0;
        }

        //check swapchain support
        bool extensionsSupported = checkDeviceExtensionSupport(device);
        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails details = querySwapChainSupport(device);
            swapChainAdequate = !details.formats.empty() && !details.presentModes.empty();
        }

        if (!extensionsSupported || !swapChainAdequate) {
            return 0;
        }

        return score;
    }
    //checks if the needed device extensions are supported
    bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t deviceExtCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtCount, nullptr);

        if (deviceExtCount == 0) {
            return false;
        }

        std::vector<VkExtensionProperties> supportedDeviceExtensions(deviceExtCount);

        vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtCount, supportedDeviceExtensions.data());

        for (const auto& deviceExt1 : deviceExtensions) {
            bool found = false;
            for (const auto& deviceExt2 : supportedDeviceExtensions) {
                if (strcmp(deviceExt1, deviceExt2.extensionName)) {
                    found = true;
                }
            }
            if (!found) {
                return false;
            }
        }
        return true;
    }
    //data struct to hold information on the capabilities of the swapchain
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device)  {
        SwapChainSupportDetails swapchainDetails;

        //get surface capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapchainDetails.capabilities);

        //get formats
        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount > 0) {
            swapchainDetails.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapchainDetails.formats.data());
        }
        //get presentation modes
        uint32_t modeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, nullptr);

        if (modeCount > 0) {
            swapchainDetails.presentModes.resize(modeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, swapchainDetails.presentModes.data());
        }

        return swapchainDetails;
    }

    //Searches for the best available format (color space and color support for pixels)
    VkSurfaceFormatKHR chooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        //could do some system to rank the other formats, but honestly i have no idea what the difference are so
        return availableFormats[0];
    }

    //find the best swap chain method, that is the method by which the images being presented to the screen that images being drawn are being swapped.
    VkPresentModeKHR chooseSwapChainPresentation(const std::vector<VkPresentModeKHR>& availablePresentations) {
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


    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

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

    //data struct to hold information on needed queues
    struct QueueFamilyIndices {
        //now until graphicsFamily is assigned a value it will appear as if a value does not exist when graphicsFamily.has_value() is called
        std::optional<uint32_t> graphicsFamily;
        //presentation queues are sometimes not held in the same queue as graphics, therefore we need to query for presentation queues as well
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        //logic to fill indices struct up
        uint32_t queueCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queueFamilies.data());

        //NOTE: the reason for the const auto& is to specify that the data being read from the for loop is NOT being modified
        int i = 0;
        
        for (const auto& queue : queueFamilies) {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }
            if (queue.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }
            if (indices.isComplete()) {
                break;
            }
            i++;
        }

        return indices;
    }

    //challenge function to compare extensions returned by glfw versus the supported extensions
    bool checkSupportedExtensions(std::vector<const char*> requiredExtensions) {
        //debugging: print out all supported extensions
        /*
        std::cout << "supported extensions:" << std::endl;
        for (const auto& extension : supportedExtensions) {
            std::cout << '\t' << extension.extensionName << std::endl;
        }
        */

        //grab the number of supported extensions
        uint32_t extensionCount = 0;
        //do layers have extensions associated with it?
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        //create vector to hold all supported extension properties
        std::vector<VkExtensionProperties> supportedExtensions(extensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, supportedExtensions.data());


        //generally the amounted of extensions we're dealing with is small so it should be fine to use a double iteration
        uint32_t includedExtensions = 0;
        for (const auto& requiredExtension : requiredExtensions) {
            bool extensionFound = false;
            for (const auto& extension : supportedExtensions) {
                if (strcmp(requiredExtension, extension.extensionName) == 0) {
                    extensionFound = true;
                }
            }
            if (!extensionFound) {
                return false;
            }
        }

        return true;
	}

    bool checkValidationLayerSupport() {
        uint32_t layerCount = 0;

        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;
            for (const auto& availableLayer : availableLayers) {
                if (strcmp(layerName, availableLayer.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound) {
                return false;
            }
        }
        return true;
    }

    std::vector<const char*> getRequiredExtensions()  {
        uint32_t glfwExtensionsCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

        //Now add a function setupDebugMessenger to be called from initVulkan right after createInstance:const char** to  std::vector<const char*>
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

        //check if debug is enabled
        if (enableValidationLayers) {
            //add VK_EXT_debug_utils
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }
    //the '&' in function arguements acts largely like a '*' except its entry cannot be null
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

        //arrow is used in pointers of objects/structs
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }   
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
