cmake_minimum_required(VERSION 3.10.2)

project(JSON_GEN_LAYER_TESTS)

add_executable(${PROJECT_NAME} main.cpp)

message(STATUS "Using find_package to locate Vulkan")
find_package(Vulkan)
message(STATUS "Vulkan FOUND = ${Vulkan_FOUND}")
message(STATUS "Vulkan Include = ${Vulkan_INCLUDE_DIR}")
message(STATUS "Vulkan Lib = ${Vulkan_LIBRARY}")
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)