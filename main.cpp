//Questions: What exactly can you do we pNext?
//               - was used one in extending instance create info to make sure that the debugger had the ability to log what happened with it.
//               - i think it allows the user to basically extend the functionalaity of the vulkan object being created but extending it into other created vulkan objects
//                 so in the instance/debug example I extended the ability of the instance object to also give information to the Debug Utils

//Add some safety checks to see if Vulkan is enabled
//include GLFW and vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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


//TODO: can i change this to a #define?
//screen size, maybe move to a header
const uint32_t WIDTH  = 800;
const uint32_t HEIGHT = 600;

//this is the standard validation layer that is included in the vulkan sdk
//if more validation layers are needed then its best to add them here
//(i think)
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

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

    //Window surface which vulkan draws on
    VkSurfaceKHR surface;

private:
    void initWindow() {
        //initializes glfw
        //add check for initialize failure
        glfwInit();

        //turn off opengl context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        //turn off ability to resize window
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        //create a basic window
        window = glfwCreateWindow(WIDTH, HEIGHT, "vulkan renderer", nullptr, nullptr);
    }

    void initVulkan() {
        createInstance();
        setupDebugMessenger();
        //pick graphics card that meet required vulkan features
        //TODO: add a method for the user to control this?
        pickPhysicalDevice();

        createLogicalDevice();
    }

    void mainLoop() {
        //loop continues till a close request is made to window
        while (!glfwWindowShouldClose(window)) {
            //this function will process events, it seems that their is no need to catch the event
            glfwPollEvents();
        }
    }

    void cleanup() {
        //if debug mode is on then destroy debug messengar
        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }
        vkDestroyDevice(device, nullptr);
        //destroy vulkan instance
        vkDestroyInstance(instance, nullptr);

        //end window processes
        glfwDestroyWindow(window);
        glfwTerminate();

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
        
        //setup debug layers and extensions for instance
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

    void createLogicalDevice() {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        VkDeviceQueueCreateInfo createQInfo{};
        createQInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        //.value() can access the actual type from std::optional<type> variables
        createQInfo.queueFamilyIndex = indices.graphicsFamily.value();
        createQInfo.queueCount = 1;

        //because we're only dealing with 1 queue giving a float value to this parameter is the same as providing an array of one value
        float queuePriority = 1.0f;
        createQInfo.pQueuePriorities = &queuePriority;

        //this defined a struct of features that we can use, right now its set to all VK_FALSE meaning no features are needed
        //later on we will need geometry shaders from these device features (which is why we already queried for it in the pickPhysicalDevice() option)
        VkPhysicalDeviceFeatures deviceFeatures{};

        //filling in the struct for the logical device
        VkDeviceCreateInfo createLInfo{};
        createLInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        //this is written as an array in the documentation, but that is because their can be multiple queues and logical devices. In our case their is only a single value
        createLInfo.pQueueCreateInfos = &createQInfo;
        createLInfo.queueCreateInfoCount = 1;

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

        if (vkCreateDevice(physicalDevice, &createLInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("could not create logical device");
        }

        //TODO: not sure where to put this...
        //grab queue from device
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);

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

        QueueFamilyIndices indices = findQueueFamilies(device);

        //the '.' operator is used when dealing with the actual object not a pointer to it.
        //can use the isComplete() function but want to keep the above note
        if (indices.graphicsFamily.has_value() == false) {
            return 0;
        }

        return score;
    }

    struct QueueFamilyIndices {
        //now until graphicsFamily is assigned a value it will appear as if a value does not exist when graphicsFamily.has_value() is called
        std::optional<uint32_t> graphicsFamily;

        bool isComplete() {
            return graphicsFamily.has_value();
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
