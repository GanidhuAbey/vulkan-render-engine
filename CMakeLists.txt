cmake_minimum_required(VERSION 3.16.3)
project(renderEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

#engine composer
add_library(
    engine_main
    inc/engine.hpp
    lib/engine.cpp
)

#engine window
add_library(
    engine_window
    inc/eng_window.hpp
    lib/eng_window.cpp
)

#engine initializer
add_library(
    engine_init
    inc/engine_init.hpp
    lib/engine_init.cpp
)

#engine graphics pipeline
add_library(
    engine_graphics
    inc/engine_graphics.hpp
    lib/engine_graphics.cpp
)

#error setup
add_library(
    error_setup
    inc/error_setup.hpp
    lib/error_setup.cpp
)

add_library(
    queue_data
    inc/queue.hpp
    lib/queue.cpp
)

add_library(
    swapchain_support
    inc/swapchain_support.hpp
    lib/swapchain_support.cpp
)

add_executable(cmake-good main.cpp)



target_link_libraries(cmake-good PRIVATE engine_main)
target_link_libraries(cmake-good PRIVATE engine_window)
target_link_libraries(cmake-good PRIVATE engine_init)
target_link_libraries(cmake-good PRIVATE engine_graphics)
target_link_libraries(cmake-good PRIVATE error_setup)
target_link_libraries(cmake-good PRIVATE queue_data)
target_link_libraries(cmake-good PRIVATE swapchain_support)

target_link_libraries(cmake-good PRIVATE -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi)








