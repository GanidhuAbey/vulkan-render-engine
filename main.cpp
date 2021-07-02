//Add some safety checks to see if Vulkan is enabled
//include GLFW and vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <typeinfo>
#include <string.h>

//TODO: can i change this to a #define?
//screen size, maybe move to a header
const uint32_t WIDTH  = 800;
const uint32_t HEIGHT = 600;

//this class can be moved to a header
class HelloTriangleApplication {
public:
    void run() {
        //setup glfw window
        initwindow();
        //this initializes any of the vulkan related objects we will need
        initvulkan();
        //this is where the frames are rendered and updated using the vulkan objects
        mainloop();
        //once the window is closed the vulkan objects are freed from memory here
        cleanup();
    }
private:
    GLFWwindow* window;
    VkInstance instance;

private:
    void initwindow() {
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

    void initvulkan() {
        createInstance();
    }

    void mainloop() {
        //loop continues till a close request is made to window
        while (!glfwWindowShouldClose(window)) {
            //this function will process events, it seems that their is no need to catch the event
            glfwPollEvents();
        }
    }

    void cleanup() {
        //destroy vulkan instance
        vkDestroyInstance(instance, nullptr);

        //end window processes
        glfwDestroyWindow(window);
        glfwTerminate();

    }

    void createInstance() {
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

        uint32_t glfwExtensionsCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

        createInfo.enabledExtensionCount = glfwExtensionsCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        //grab the number of supported extensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        //create vector to hold all supported extension properties
        std::vector<VkExtensionProperties> extensions(extensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        //check supported extensions
        bool supported = checkSupportedExtensions(extensions, glfwExtensions, glfwExtensionsCount);
        if (!supported) {
            std::runtime_error("required vulkan extensions are not supported!");
        }

        //TODO: "pAllocator is not NULL" not sure if possible to make parameter nullptr
        //in future vulkan versions
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            std::runtime_error("failed to create instance");
        }
    }

    //challenge function to compare extensions returned by glfw versus the supported extensions
    bool checkSupportedExtensions(std::vector<VkExtensionProperties> supportedExtensions, const char** glfwExtensions, uint32_t extCount) {
        //print supported extensions
        std::cout << "available supported extensions:\n";

        for (const auto& extension : supportedExtensions) {
            std::cout << '\t' << extension.extensionName << '\n';
        }

        std::cout << "extensions returned by glfw: \n";
        //print extensions given by glfw
        for (int i = 0; i < extCount; i++) {
            std::cout << '\t' << glfwExtensions[i] << '\n';
        }

        //generally the amounted of extensions we're dealing with is small so it should be fine to use a double iteration
        uint32_t includedExtensions = 0;
        for (int i = 0; i < extCount; i++) {
            const char* checkExt = glfwExtensions[i];
            for (const auto& extension : supportedExtensions) {
                if (strcmp(glfwExtensions[0], extension.extensionName) == 0) {
                    includedExtensions++;
                    std::cout << "extension was supported" << std::endl;
                }
            }
        }

        if (includedExtensions == extCount) {
            return true;
        }
        return false;
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
