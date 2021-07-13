cmake_minimum_required(VERSION 3.16.3)
project(renderEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES lib/*.cpp)

#engine composer
add_library(
    required_libs
    ${SOURCES}
)

find_library(
    vulkan
    VULKAN_SDK
)

find_library(
    glfw
    GLFW
)

add_executable(cmake-good main.cpp)



target_link_libraries(cmake-good PRIVATE required_libs)
target_link_libraries(cmake-good PRIVATE vulkan)
target_link_libraries(cmake-good PRIVATE glfw)









