#define VERSION_1 0;

#include "../inc/engine_init.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//this is the standard validation layer that is included in the vulkan sdk
//if more validation layers are needed then its best to add them here
//(i think)
<<<<<<< HEAD
<<<<<<< HEAD
const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

using namespace create;

bool EngineInit::checkValidationLayerSupport()
{
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

using namespace create;

bool EngineInit::checkValidationLayerSupport() {
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    uint32_t layerCount = 0;

    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

<<<<<<< HEAD
<<<<<<< HEAD
    for (const char *layerName : validationLayers)
    {
        bool layerFound = false;
        for (const auto &availableLayer : availableLayers)
        {
            if (strcmp(layerName, availableLayer.layerName) == 0)
            {
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    for (const char* layerName : validationLayers) {
        bool layerFound = false;
        for (const auto& availableLayer : availableLayers) {
            if (strcmp(layerName, availableLayer.layerName) == 0) {
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
                layerFound = true;
                break;
            }
        }
<<<<<<< HEAD
<<<<<<< HEAD
        if (!layerFound)
        {
=======
        if (!layerFound) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
        if (!layerFound) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
            return false;
        }
    }
    return true;
}

//checks if the needed device extensions are supported
<<<<<<< HEAD
<<<<<<< HEAD
bool EngineInit::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t deviceExtCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtCount, nullptr);

    if (deviceExtCount == 0)
    {
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
bool EngineInit::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t deviceExtCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtCount, nullptr);

    if (deviceExtCount == 0) {
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
        return false;
    }

    std::vector<VkExtensionProperties> supportedDeviceExtensions(deviceExtCount);

    vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtCount, supportedDeviceExtensions.data());

<<<<<<< HEAD
<<<<<<< HEAD
    for (const auto &deviceExt1 : deviceExtensions)
    {
        bool found = false;
        for (const auto &deviceExt2 : supportedDeviceExtensions)
        {
            if (strcmp(deviceExt1, deviceExt2.extensionName))
            {
                found = true;
            }
        }
        if (!found)
        {
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    for (const auto& deviceExt1 : deviceExtensions) {
        bool found = false;
        for (const auto& deviceExt2 : supportedDeviceExtensions) {
            if (strcmp(deviceExt1, deviceExt2.extensionName)) {
                found = true;
            }
        }
        if (!found) {
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
            return false;
        }
    }
    return true;
}

<<<<<<< HEAD
<<<<<<< HEAD
//TODO: update to vulkan version 1.2
int EngineInit::rateDeviceSuitability(VkPhysicalDevice device)
{
=======
int EngineInit::rateDeviceSuitability(VkPhysicalDevice device) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
int EngineInit::rateDeviceSuitability(VkPhysicalDevice device) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    int score = 0;

    //grab data of device
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

<<<<<<< HEAD
<<<<<<< HEAD
    VkPhysicalDeviceVulkan12Features vulkan12Features{};
    vulkan12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    //vulkan12Features.separateDepthStencilLayouts = VK_TRUE;

    //VkPhysicalDeviceFeatures2 deviceFeatures{};
    //deviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    //deviceFeatures.pNext = &vulkan12Features;
    //eviceFeatures.features = vulkan10Features;

    //vkGetPhysicalDeviceFeatures2(device, &deviceFeatures);

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
    }

=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    //NOTE: their is also a different limit for 3D image dimensions
    score += deviceProperties.limits.maxImageDimension2D;

    //needed to draw images.
<<<<<<< HEAD
<<<<<<< HEAD
    if (!deviceFeatures.geometryShader)
    {
        return 0;
    }

    //check if neede7d extensions are supported by device
    if (!checkDeviceExtensionSupport(device))
    {
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    if (!deviceFeatures.geometryShader) {
        return 0;
    }
    
    //check if neede7d extensions are supported by device
    if (!checkDeviceExtensionSupport(device)) {
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
        return 0;
    }

    QueueData indices(device, surface);

    //the '.' operator is used when dealing with the actual object not a pointer to it.
    //can use the isComplete() function but want to keep the above note
<<<<<<< HEAD
<<<<<<< HEAD
    if (!indices.isComplete())
    {
=======
    if (!indices.isComplete()) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
    if (!indices.isComplete()) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
        return 0;
    }

    //check if the binding count is atleast 1
    //TODO: this is a kind of check you do when you know what you'll be drawing,
    //but we should figure out some general parameters to make sure the device we pull can draw at most things
<<<<<<< HEAD
<<<<<<< HEAD
    if (deviceProperties.limits.maxVertexInputBindings < 1 || deviceProperties.limits.maxVertexInputBindingStride < sizeof(data::Vertex2D))
    {
        return 0;
    }
    score += 10 * deviceProperties.limits.maxVertexInputBindings;
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    //std::cout << sizeof(Vertex) <<  " : " << deviceProperties.limits.maxVertexInputBindings << std::endl;
    if (deviceProperties.limits.maxVertexInputBindings < 1 || deviceProperties.limits.maxVertexInputBindingStride < sizeof(data::Vertex2D)) {
        return 0;
    }
    score += 10*deviceProperties.limits.maxVertexInputBindings;
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9

    //check swapchain support
    bool extensionsSupported = checkDeviceExtensionSupport(device);
    bool swapChainAdequate = false;
<<<<<<< HEAD
<<<<<<< HEAD
    if (extensionsSupported)
    {
=======
    if (extensionsSupported) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
    if (extensionsSupported) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
        SwapChainSupport details(device, surface);
        swapChainAdequate = !details.formats.empty() && !details.presentModes.empty();
    }

<<<<<<< HEAD
<<<<<<< HEAD
    if (!extensionsSupported || !swapChainAdequate)
    {
        return 0;
    }

    //check if seperate depth/stencil passes are acceptable

    return score;
}

std::vector<const char *> EngineInit::getRequiredExtensions()
{
    uint32_t glfwExtensionsCount = 0;
    const char **glfwExtensions;
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    if (!extensionsSupported || !swapChainAdequate) {
        return 0;
    }

    return score;
}

std::vector<const char*> EngineInit::getRequiredExtensions()  {
    uint32_t glfwExtensionsCount = 0;
    const char** glfwExtensions;
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

    //Now add a function setupDebugMessenger to be called from initVulkan right after createInstance:const char** to  std::vector<const char*>
<<<<<<< HEAD
<<<<<<< HEAD
    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

    //check if debug is enabled
    if (enableValidationLayers)
    {
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

    //check if debug is enabled
    if (enableValidationLayers) {
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
        //add VK_EXT_debug_utils
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

<<<<<<< HEAD
<<<<<<< HEAD
//challenge function to compare extensions returned by glfw versus the supported extensions
bool EngineInit::checkSupportedExtensions(std::vector<const char *> requiredExtensions)
{
=======
    //challenge function to compare extensions returned by glfw versus the supported extensions
bool EngineInit::checkSupportedExtensions(std::vector<const char*> requiredExtensions) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
    //challenge function to compare extensions returned by glfw versus the supported extensions
bool EngineInit::checkSupportedExtensions(std::vector<const char*> requiredExtensions) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
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

<<<<<<< HEAD
<<<<<<< HEAD
    //generally the amounted of extensions we're dealing with is small so it should be fine to use a double iteration
    uint32_t includedExtensions = 0;
    for (const auto &requiredExtension : requiredExtensions)
    {
        bool extensionFound = false;
        for (const auto &extension : supportedExtensions)
        {
            if (strcmp(requiredExtension, extension.extensionName) == 0)
            {
                extensionFound = true;
            }
        }
        if (!extensionFound)
        {
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9

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
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
            return false;
        }
    }

    return true;
}

//the '&' in function arguements acts largely like a '*' except its entry cannot be null
<<<<<<< HEAD
<<<<<<< HEAD
void EngineInit::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
=======
void EngineInit::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
void EngineInit::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debug::debugCallback;
}

<<<<<<< HEAD
<<<<<<< HEAD
void EngineInit::initialize(GLFWwindow *userWindow)
{
=======
void EngineInit::initialize(GLFWwindow* userWindow) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
void EngineInit::initialize(GLFWwindow* userWindow) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    window = userWindow;
    createInstance();
    createSurface();
    setupDebugMessenger();
    pickPhysicalDevice();
    createLogicalDevice();
    createCommandPool();
}

<<<<<<< HEAD
<<<<<<< HEAD
EngineInit::~EngineInit()
{
=======
EngineInit::~EngineInit() {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
EngineInit::~EngineInit() {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    vkDeviceWaitIdle(device);

    std::cout << "engine instance destruction.." << std::endl;

<<<<<<< HEAD
<<<<<<< HEAD
    if (enableValidationLayers)
    {
        debug::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
    vkDestroyCommandPool(device, commandPool, nullptr);
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    if (enableValidationLayers) {
        debug::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
    vkDestroyCommandPool(device,  commandPool, nullptr);
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9

    vkDestroyDevice(device, nullptr);

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
}

<<<<<<< HEAD
<<<<<<< HEAD
void EngineInit::setupDebugMessenger()
{
    if (!enableValidationLayers)
        return;

    //fill in object struct
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};

=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
void EngineInit::setupDebugMessenger() {
    if (!enableValidationLayers) return;

    //fill in object struct
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    populateDebugMessengerCreateInfo(createInfo);

    //functions that are part of extensions (like this one) are  not not loaded automatically.
    //inorder to load functions like these we have to use vkGetInstanceProcAddr
    //create object
<<<<<<< HEAD
<<<<<<< HEAD
    if (debug::CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("could not create debug messenger");
    }
}

void EngineInit::createInstance()
{
    //check if debug mode is on and if user device has accessed to the validation layers they set in validationLayers
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    if (debug::CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("could not create debug messenger");
    }
    
}

void EngineInit::createInstance() {
    //check if debug mode is on and if user device has accessed to the validation layers they set in validationLayers
    if (enableValidationLayers && !checkValidationLayerSupport()) {
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
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
<<<<<<< HEAD
<<<<<<< HEAD
    appInfo.apiVersion = VK_API_VERSION_1_2;
=======
    appInfo.apiVersion =  VK_API_VERSION_1_0;
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
    appInfo.apiVersion =  VK_API_VERSION_1_0;
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9

    VkInstanceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

<<<<<<< HEAD
<<<<<<< HEAD
    std::vector<const char *> extensions = getRequiredExtensions();

    //TODO: move all this into the checkSupportedExtension()
    //check supported extensions
    if (!checkSupportedExtensions(extensions))
    {
        throw std::runtime_error("required vulkan extensions are not supported!");
    }

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    //if not place inside the if statement, this debug info will run out of scope and be freed from memory, when it
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    std::vector<const char*> extensions = getRequiredExtensions();

    //TODO: move all this into the checkSupportedExtension()
    //check supported extensions
    if (!checkSupportedExtensions(extensions)) {
        throw std::runtime_error("required vulkan extensions are not supported!");
    }

    createInfo.enabledExtensionCount = static_cast<uint32_t>( extensions.size() );
    createInfo.ppEnabledExtensionNames = extensions.data();

    //if not place inside the if statement, this debug info will run out of scope and be freed from memory, when it 
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    //is still needed to be able to create an instance.
    VkDebugUtilsMessengerCreateInfoEXT createDebugInfo{};

    //setup debug layers and extensions for instance#define VK_USE_PLATFORM_WIN32_KHR
<<<<<<< HEAD
<<<<<<< HEAD
    if (enableValidationLayers)
    {
        //convert size int to uint32_t
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(createDebugInfo);
        //do you need to do this?
        //TODO: see how removing this cast changes things once we draw triangle
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&createDebugInfo;
    }
    //don't need to do this but it will probably be useful to be as descriptive as possible
    else
    {
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
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
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    //TODO: "pAllocator is not NULL" not sure if possible to make parameter nullptr
    //in future vulkan versions
<<<<<<< HEAD
<<<<<<< HEAD
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance");
    }
}
void EngineInit::createSurface()
{
    //glfw can do all this for me plus all other platforms and i won't to decide between anything

    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("surface could not be created");
    }
}
void EngineInit::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance");
    }
}
void EngineInit::createSurface() {
    //glfw can do all this for me plus all other platforms and i won't to decide between anything

    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("surface could not be created");
    }
}
void EngineInit::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
        throw std::runtime_error("no GPU's on this machine supports vulkan");
    }

    //makes int the key and VkPhysicalDevice the value, multimap is like a dictionary with a key-value pair, but multimap also keeps the values sorted by the keys.
    std::multimap<int, VkPhysicalDevice> candidates;

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

<<<<<<< HEAD
<<<<<<< HEAD
    for (const auto &device : devices)
    {
=======
    for (const auto& device : devices) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
    for (const auto& device : devices) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

<<<<<<< HEAD
<<<<<<< HEAD
    if (candidates.rbegin()->first > 0)
    {
        //this device is suitable
        physicalDevice = candidates.rbegin()->second;
    }
    else
    {
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    if (candidates.rbegin()->first > 0) {
        //this device is suitable
        physicalDevice = candidates.rbegin()->second;
    }
    else {
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
        throw std::runtime_error("could not find a suitable GPU");
    }

    //findQueueFamilies(physicalDevice);
}
<<<<<<< HEAD
<<<<<<< HEAD
void EngineInit::createLogicalDevice()
{
=======
void EngineInit::createLogicalDevice() {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
void EngineInit::createLogicalDevice() {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    QueueData indices(physicalDevice, surface);

    //Load Queues
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> queueIndices = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    //iterate through queuefamilies and fill in struct data
    float queuePriority = 1.0f;
<<<<<<< HEAD
<<<<<<< HEAD
    for (uint32_t i : queueIndices)
    {
=======
    for (uint32_t i : queueIndices) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
    for (uint32_t i : queueIndices) {
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
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

    //TODO: move this somehwere else
    VkPhysicalDeviceProperties deviceProperties{};
<<<<<<< HEAD
<<<<<<< HEAD
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    //filling in the struct for the logical device
    VkDeviceCreateInfo createLInfo{};
    createLInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createLInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    vkGetPhysicalDeviceProperties(physicalDevice,  &deviceProperties);
    
    //filling in the struct for the logical device
    VkDeviceCreateInfo createLInfo{};
    createLInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createLInfo.queueCreateInfoCount = static_cast<uint32_t>( queueCreateInfos.size() );
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    //documentation writes this as an array but when only dealing with a single queue, you can just put a single value and its fine.
    createLInfo.pQueueCreateInfos = queueCreateInfos.data();
    createLInfo.pEnabledFeatures = &deviceFeatures;

    //NOTE: creating logical devices also allows us to enable extensions to extend the functionality of Vulkan
    //examples of this would be VK_KHR_swapchain which is a device specific extension designed to present rendered images to the screen.

    //this part is not required for newer versions of vulkan, but older versions still rely on this
<<<<<<< HEAD
<<<<<<< HEAD
    if (enableValidationLayers)
    {
        //set up device layers for this logical device
        createLInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createLInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    if (enableValidationLayers) {
        //set up device layers for this logical device
        createLInfo.enabledLayerCount = static_cast<uint32_t>( validationLayers.size() );
        createLInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
        createLInfo.enabledLayerCount = 0;
    }

    //enable VK_KHR_SWAPCHAIN extension
<<<<<<< HEAD
<<<<<<< HEAD
    createLInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createLInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (vkCreateDevice(physicalDevice, &createLInfo, nullptr, &device) != VK_SUCCESS)
    {
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    createLInfo.enabledExtensionCount = static_cast<uint32_t>( deviceExtensions.size() );
    createLInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (vkCreateDevice(physicalDevice, &createLInfo, nullptr, &device) != VK_SUCCESS) {
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
        throw std::runtime_error("could not create logical device");
    }

    //TODO: not sure where to put this...
    //grab queue from device
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
<<<<<<< HEAD
<<<<<<< HEAD
}

void EngineInit::createCommandPool()
{
=======
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9

}

void EngineInit::createCommandPool() {
<<<<<<< HEAD
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    QueueData queueFamilyIndices(physicalDevice, surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    poolInfo.flags = 0; // Optional

<<<<<<< HEAD
<<<<<<< HEAD
    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("could not create command pool");
    }
}
=======
    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)  {
        throw std::runtime_error("could not create command pool");
    }
}    
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)  {
        throw std::runtime_error("could not create command pool");
    }
}    
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
