cmake_minimum_required(VERSION 3.16.3)
project(renderEngine VERSION 1.0.0)

<<<<<<< HEAD
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
set(CMAKE_CXX_STANDARD 17)
SET(QTCP_PROJECT_DIR ../)

include_directories(inc/)
file(GLOB SOURCES lib/*.cpp)

#engine composer
add_library(
    required_libs
    ${SOURCES}
)

<<<<<<< HEAD
add_library(
    shaderc_combined
    SHARED
    IMPORTED
)

=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
find_library(
    vulkan
    VULKAN_SDK
)

find_library(
    glfw
    GLFW
)

<<<<<<< HEAD
add_compile_options(-pthread)

add_executable(main main.cpp)

=======
add_executable(main main.cpp)



>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
target_link_libraries(main PRIVATE required_libs)
target_link_libraries(main PRIVATE vulkan)
target_link_libraries(main PRIVATE glfw)









