#include "../inc/engine.hpp"
#include "common.hpp"

using namespace create;

//define some global variables.
uint16_t indexCount = 0;
uint16_t vertexCount = 0;
uint16_t objectCount = 0;

VkBuffer vertexBuffer;
VkBuffer indexBuffer;

Engine::Engine(int w, int h, const char* title) {
    screenWidth = w;
    screenHeight = h;
    GLFWwindow* window = userWindow.initWindow(w, h, title);
    engInit.initialize(window);
    engGraphics.initialize(&engInit);

    create::QueueData indices(engInit.physicalDevice, engInit.surface);
    queueG = indices.graphicsFamily.value();
    queueP = indices.presentFamily.value();

    //creates vertex buffer
    createVertexBuffer(&gpuMemory);
    createIndexBuffer(&indexMemory);

    std::cout << "hi yah" << std::endl;
    //create uniform buffer
    //uniformMemory = createBuffer(&uniformBuffer, 5e7, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    //update uniform buffer memory
    //writeToBuffer(&engGraphics.ubo, sizeof(engGraphics.ubo), &uniformMemory);

    //engGraphics.createDescriptorSets(uniformBuffer);

    glfWindow = window;

}

Engine::~Engine() {
    mem::maDestroyMemory(engInit.device, gpuMemory);
    mem::maDestroyMemory(engInit.device, indexMemory);
    //mem::maDestroyMemory(engInit.device, uniformMemory, uniformBuffer);
    //nothing to do now, maybe in the future we can add a check to see if memory is leaking
    std::cout << "program exectuted and closed" << std::endl;
}

//TODO: remove this
void Engine::clearScreen() {
    primitiveCount = 0;
}

void Engine::draw() {
    draw::EngineDraw engineDraw;
    engineDraw.initialize(indexBuffer, &engGraphics, &engInit, &vertexBuffer, &gpuMemory);
}

void Engine::destroyUniformData(size_t objIndex) {
    std::cout << "this should only run once" << std::endl;
    mem::maDestroyMemory(engInit.device, uniformBufferData[objIndex]);
}

//PURPOSE - take a given transform and apply it the object specified by the objIndex
//PARAMETERS - [glm::mat4] transform - the transform being applied to the object
//           - [size_t] objIndex - the index representing the order at which the object was created.
//RETURNS - NONE
//NOTES - need functionality to choose which object i am applying the transformation for
//      - a set system could possibly be a more efficient method than using vectors
void Engine::applyTransform(glm::mat4 transform, size_t objIndex, float camera_angle) {
    //add transform to buffer
    UniformBufferObject ubo;
    //ubo.modelToWorld = transform;
    //float lookAtVec = glm::dot(glm::vec3(0.229416,  0.688247, 0.688247), glm::vec3(-0.948683,  0.316228, 0));
    //printf("vec: <%f> \n", lookAtVec);



    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    ubo.modelToWorld = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //ubo.worldToCamera = createCameraMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.worldToCamera = glm::lookAt(glm::vec3(1.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //ubo.projection = glm::perspective(glm::radians(45.0f), engGraphics.swapChainExtent.width / (float) engGraphics.swapChainExtent.height, 0.1f, 10.0f); 
    
    ubo.projection = createPerspectiveProjection(glm::radians(45.0f), engGraphics.swapChainExtent.width / (float) engGraphics.swapChainExtent.height, 0.1f, 10.0f);

    //create buffers if needed
    if (objIndex >= uniformBufferData.size()) {
        //std::cout << objIndex << " oh yea!" << std::endl;
        uniformBufferData.resize(objIndex + 1);
        //need to create new buffer
        mem::MaMemory uniformBufferMemory;
        createUniformBuffer(sizeof(ubo), &uniformBufferMemory);
        uniformBufferData[objIndex] = uniformBufferMemory;
    }

    //finish?

    //the data is now written to memory in a uniform buffer
    //we just need a descriptor pool and set to describe the resource to the gpu
    engGraphics.createDescriptorPools();
    engGraphics.createDescriptorSets(sizeof(UniformBufferObject), uniformBufferData[objIndex].buffer);

    writeToLocalBuffer(sizeof(UniformBufferObject), &uniformBufferData[objIndex], &ubo);

}

//PURPOSE - create a projection matrix that adds perspective (lines are not parallel, lines approach eachother the farther they get)
glm::mat4 Engine::createPerspectiveProjection(float vertical_angle, float aspect, float n, float f) {
    //find the distance to the "screen" that will be mapped to
    //the distance to the screen is related to the angle between 'l' and 'r' by the eqn : phi = 2arctan(w/2d)
    //d = w/(2*tan(phi/2)) <-- how can i derive the the neccesarry information to solve this from the parameters used by glm::perspective?
    float c = 1.0/(glm::tan(vertical_angle/2));

    //translate and normalize the vertices to bring it the unit cube form
    glm::mat4 projection_simplified = {
        c/aspect, 0, 0, 0,
        0, c, 0, 0,
        0, 0, -(f+n)/(f-n), -(2*f*n)/(f-n),
        0, 0, -1, 0,
    };

    return glm::transpose(projection_simplified);
    //project the points to a plane
}

//PURPOSE - construct a matrix that maps the clip space to appropriate coordinates and constructs a perspective projection
//NOTES - these parameters can be simplified
//      - somewhat worried about how well this is working
glm::mat4 Engine::createOrthogonalProjection(float left, float right, float top, float bottom, float near, float far) {
    //the given parameters define a cube the governs the space that our input values live in. we must then convert this cube
    //into a normalized cube with a center located at (0, 0, 0). finally we then project this object onto the screen orthogonaly

    //translate the cube
    glm::mat4 translate = {
        1, 0, 0, -(right+left)/2,
        0, 1, 0, -(top+bottom)/2,
        0, 0, 1, -(far+near)/2,
        0, 0, 0, 1,
    };

    //normalize the cube
    glm::mat4 normalize = {
        2/(right-left), 0, 0, 0,
        0, 2/(top-bottom), 0, 0,
        0, 0, 2/(far-near), 0,
        0, 0, 0, 1,
    };

    //project the cube space onto a plane
    glm::mat4 project = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1,
    };

    //multiply these vectors and return the output
    return glm::transpose(project * normalize * translate);
}


//PURPOSE - construct a matrix that transforms objects relative to the camera
//PARAMETERS - [glm::vec3] lookAt - vector representing the point the camera is looking at
//           - [glm::vec3] cameraPos - vector representing the point the camera is located at
//RETURNS - [glm::mat4] worldToCamera - matrix representing the transformation required to get to camera space
//NOTES - for some reason matrix multiplication was not giving me the expected output
glm::mat4 Engine::createCameraMatrix(glm::vec3 lookAt, glm::vec3 cameraPos, glm::vec3 up) {
    //we know we're looking at the point described by the lookAt matrix, but where is the camera?
    //this vector should also represent the distance we need to translate the object by;
    glm::vec3 camera_z = glm::normalize(lookAt - cameraPos);
    glm::vec3 camera_x = glm::normalize(calculateCrossProduct(up, camera_z)); //the axis dont have to be perpendicular to use the cross product

    //so  if i use camera_x and camera_z i can calculate a camera y that is perpendicular to the other two vectors
    glm::vec3 camera_y = calculateCrossProduct(camera_z, camera_x);

    camera_z = -camera_z;

    glm::mat4 modelToWorld = {
        camera_x.x, camera_x.y, camera_x.z, glm::dot(camera_x, -cameraPos),
        camera_y.x, camera_y.y, camera_y.z, glm::dot(camera_y, -cameraPos),
        camera_z.x, camera_z.y, camera_z.z, glm::dot(camera_z, -cameraPos),
        0, 0, 0, 1,
    };

    return glm::transpose(modelToWorld);
}

//PURPOSE - calculates the cross product of two vectors
//PARAMETERS - [glm::vec3] v (the first vector)
//           - [glm::vec3] w (the second vector)
//RETURNS - [glm::vec3] the cross product of the two given vectors
glm::vec3 Engine::calculateCrossProduct(glm::vec3 v, glm::vec3 w) {
    //[x, v1, w1]
    //[z, v3, w3]
    //determinant = x*(v2*w3 - w2*v3) - y*(v1*w3 - w1*v3) + z(v1*w2 - w1*v2)

    return {
        (v.y*w.z - w.y*v.z),
        -(v.x*w.z - w.x*v.z),
        (v.x*w.y - w.x*v.y),
    };
}

//PURPOSE - create a buffer with the uniform bit active that is located in cpu-readable memory
//PARAMETERS - [VkDeviceSize] dataSize (the size of the data that will be allocated to this buffer)
//             [mem::MaMemory*] pMemory (pointer to the MaMemory obj that will contain the neccesary data on the buffer
//RETURNS - NONE
void Engine::createUniformBuffer(VkDeviceSize dataSize, mem::MaMemory* pMemory) {
    mem::MaBufferCreateInfo bufferInfo{};
    bufferInfo.size = dataSize;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 1;
    bufferInfo.pQueueFamilyIndices = &queueG;
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    mem::maCreateBuffer(engInit.physicalDevice, engInit.device, &bufferInfo, pMemory);
}

//PURPOSE - write data to buffer in cpu-readable memory
//PARAMETERS - [VkDeviceSize] dataSize (size of the data being allocated)
//           - [mem::MaMemory*] pMemory (pointer to the memory where the data is being allocated)
//           - [void*] data (the information being allocated)
//RETURNS - NONE
void Engine::writeToLocalBuffer(VkDeviceSize dataSize, mem::MaMemory* pMemory, void* data) {
    mem::maAllocateMemory(dataSize, pMemory);
    mem::maMapMemory(engInit.device, dataSize, pMemory, data);
}

//PURPOSE - write data to buffer in device local memory, do NOT use this function for other memory types
//PARAMETERS - dataSize : (the size of the data being given to buffer)
//           - dstBuffer :  (the device local buffer the data will be written to)
//           - data : (the actual data that will be given to the buffer)/
//RETURNS - NONE
void Engine::writeToDeviceBuffer(VkDeviceSize dataSize, mem::MaMemory* pMemory, void* data) {
    //create and allocate memory for a temporary cpu readable buffer
    mem::MaMemoryData tempMemoryInfo{};
    mem::MaMemory tempMemory{};

    createTempBuffer(dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &tempMemory);
    mem::maMapMemory(engInit.device, dataSize, &tempMemory, data);

    //allocate memory in the device local buffer
    mem::maAllocateMemory(dataSize, pMemory);

    //transfer memory from temp buffer to given device local buffer
    engGraphics.copyBuffer(tempMemory.buffer, pMemory->buffer, pMemory->offset, dataSize);

    //make sure to state that the memory allocation is complete (maybe worth being implemented not by the user)
    pMemory->allocate = false;

    //destroy buffer and its associated memory
    mem::maDestroyMemory(engInit.device, tempMemory);

    std::cout <<  "hellow as up" << std::endl;
}

//PURPOSE - abstract a bit away from the underlying api when writing to the proper buffer
//PARAMETERS - [VkDeviceSize] dataSize - the size of the data being written to vertex buffer
//           - [void*] data - the data being written to vertex buffer
//RETURNS - NONE
void Engine::writeToVertexBuffer(VkDeviceSize dataSize, void* data) {
    writeToDeviceBuffer(dataSize, &gpuMemory, data);
}

void Engine::writeToIndexBuffer(VkDeviceSize dataSize, void* data) {
    writeToDeviceBuffer(dataSize, &indexMemory, data);
}

//PURPOSE - when function is called the render commands are updated so the neweset vertices are being drawn.
//PARAMETERS - [uint16_t] indexCount - a count of how many indices the the renderer has to draw
//RETURNS - NONE
void Engine::addToRenderQueue(uint16_t indexCount) {
    std::cout << "x" << std::endl;
    engGraphics.createCommandBuffers(gpuMemory.buffer, indexMemory.buffer, indexCount);
}

//PURPOSE - create and allocate memory for a small cpu-readable buffer
//PARAMETERS  - [VkDeviceSize] dataSize (size of the data that will be allocated to this buffer)
//            - [VkBufferUsageFlags] usage (the type of buffer this buffer will transfer data to)
//            - [VkBuffer*] tempBuffer (memory location for the temp buffer to be created to)
//            - [mem::MaMemory*] tempMemory (memory location for the temp memory to be created to)
//RETURNS - [mem::MaMemoryData] memoryData (struct data giving information on where to allocate the data to)
void Engine::createTempBuffer(VkDeviceSize dataSize, VkBufferUsageFlags usage, mem::MaMemory* tempMemory) {
    mem::MaBufferCreateInfo bufferInfo{};
    bufferInfo.size = dataSize;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 1;
    bufferInfo.pQueueFamilyIndices = &queueG;
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    mem::maCreateBuffer(engInit.physicalDevice, engInit.device, &bufferInfo, tempMemory);

    //allocate memory for temp buffer
    mem::maAllocateMemory(dataSize, tempMemory);

    std::cout << "memory is allocated? : " << tempMemory->allocate << std::endl;
}

//TODO: put output variables as the last parameters
/*
mem::MaMemory Engine::createBuffer(VkBuffer* buffer, VkDeviceSize memorySize, VkBufferUsageFlags usage,
VkMemoryPropertyFlags properties) {
    create::QueueData indices(engInit.physicalDevice, engInit.surface);

    mem::MaMemoryInfo memoryInfo{};
    memoryInfo.allocationSize = memorySize;
    memoryInfo.bufferUsage = usage;
    memoryInfo.memoryProperties = properties;
    memoryInfo.queueFamilyIndexCount = indices.graphicsFamily.value();

    mem::MaMemory memoryObj{};

    mem::MaBufferCreateInfo bufferInfo{};
    bufferInfo.
    //mem::maCreateMemory(engInit.physicalDevice, engInit.device, &memoryInfo, buffer, &memoryObj);

    //return memoryObj;
}
*/
//PURPOSE - create a vertex buffer and assigned 5e7 bytes of device local memory
//PARAMETERS - [mem::MaMemory*] pMemory (a data struct containing information on the memory associated with the buffer)
//RETURNS - NONE
void Engine::createVertexBuffer(mem::MaMemory* pMemory) {
    //create::QueueData indices(engInit.physicalDevice, engInit.surface);

    mem::MaBufferCreateInfo bufferInfo{};
    bufferInfo.size = 5e7;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 1;
    bufferInfo.pQueueFamilyIndices = &queueG;
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    mem::maCreateBuffer(engInit.physicalDevice, engInit.device, &bufferInfo, pMemory);
}

//PURPOSE - create a index buffer and assigned 5e7 bytes of device local memory
//PARAMETERS - [mem::MaMemory*] pMemory (a data struct containing information on the memory associated with the buffer)
//RETURNS - NONE
void Engine::createIndexBuffer(mem::MaMemory* pMemory) {
    //create::QueueData indices(engInit.physicalDevice, engInit.surface);

    mem::MaBufferCreateInfo bufferInfo{};
    bufferInfo.size = 5e7;
    bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 1;
    bufferInfo.pQueueFamilyIndices = &queueG;
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    mem::maCreateBuffer(engInit.physicalDevice, engInit.device, &bufferInfo, pMemory);
}


float Engine::screenToVulkan(int screenCoord, int screenSize, int vulkanMin) {
    const float vulkanMaxWidth = 1;
    const float vulkanMinWidth = -1;

    float newRange = vulkanMaxWidth - vulkanMinWidth; //vulkan will convert both width and height to the same range (-1 , 1)
    float oldRange = screenSize - 0;
    float vulkanCoord = ((screenCoord * (newRange)) / oldRange) + vulkanMin;

    //std::cout << (screenCoord * newRange) << std::endl;

    return vulkanCoord;
}
