cmake_minimum_required(VERSION 3.16.3)
project(renderEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
SET(QTCP_PROJECT_DIR ../)

include_directories(inc/)
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

add_executable(main main.cpp)



target_link_libraries(main PRIVATE required_libs)
target_link_libraries(main PRIVATE vulkan)
target_link_libraries(main PRIVATE glfw)









