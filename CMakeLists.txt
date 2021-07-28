cmake_minimum_required(VERSION 3.16.3)
project(renderEngine VERSION 1.0.0)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_CXX_STANDARD 17)
SET(QTCP_PROJECT_DIR ../)

include_directories(inc/)
file(GLOB SOURCES lib/*.cpp)

#engine composer
add_library(
    required_libs
    ${SOURCES}
)

add_library(
    shaderc_combined
    SHARED
    IMPORTED
)

find_library(
    vulkan
    VULKAN_SDK
)

find_library(
    glfw
    GLFW
)

add_compile_options(-pthread)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE required_libs)
target_link_libraries(main PRIVATE vulkan)
target_link_libraries(main PRIVATE glfw)









