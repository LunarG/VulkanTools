cmake_minimum_required(VERSION 3.4.1)

# Validation layers could be built with code from
#    github repo   OR
#    ndk's snapshot
set(BUILD_IN_NDK OFF)
if (CMAKE_CURRENT_SOURCE_DIR MATCHES "sources/third_party/vulkan/src")
    set(BUILD_IN_NDK ON)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror \
        -Wno-unused-function -Wno-unused-const-variable \
        -Wno-c++98-compat-pedantic -DVK_PROTOTYPES \
        -DVK_USE_PLATFORM_ANDROID_KHR")

if (BUILD_IN_NDK)
    # NDK directory structure:
    #  $ANDROID_NDK/sources/third_party/
    #        vulkan/src
    get_filename_component(SRC_DIR
        "${ANDROID_NDK}/sources/third_party/vulkan/src" ABSOLUTE)
else ()
    # github directory structure:
    #   ${SRC_DIR}
    #   ${SRC_DIR}/build-android/external
    get_filename_component(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../.."  ABSOLUTE)
endif()
set(COMMON_DIR "${SRC_DIR}/build-android/generated")

set(CMAKE_CXX_clone "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_ANDROID_KHR \
        -fvisibility=hidden")
include_directories(${SRC_DIR}/include
                    ${COMMON_DIR}/include
                    ${SRC_DIR}/layers
                    ${SRC_DIR}/loader)
add_library(layer_utils STATIC
        ${SRC_DIR}/layers/vk_layer_config.cpp
        ${SRC_DIR}/layers/vk_layer_extension_utils.cpp
        ${SRC_DIR}/layers/vk_layer_utils.cpp
        ${SRC_DIR}/layers/generated/vk_format_utils.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_clone}")

# build core_validation layers
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_ANDROID_KHR \
                     -fvisibility=hidden")
add_library(VkLayer_core_validation SHARED
        ${SRC_DIR}/layers/core_validation.cpp
        ${SRC_DIR}/layers/convert_to_renderpass2.cpp
        ${SRC_DIR}/layers/descriptor_sets.cpp
        ${SRC_DIR}/layers/buffer_validation.cpp
        ${SRC_DIR}/layers/shader_validation.cpp
        ${SRC_DIR}/layers/xxhash.c)
target_include_directories(VkLayer_core_validation PRIVATE
        ${SRC_DIR}/include
        ${SRC_DIR}/layers
        ${COMMON_DIR}/include
        ${SRC_DIR}/loader)
target_link_libraries(VkLayer_core_validation PRIVATE
        log layer_utils)

add_library(VkLayer_parameter_validation SHARED
        ${COMMON_DIR}/include/parameter_validation.cpp
        ${SRC_DIR}/layers/parameter_validation_utils.cpp)
target_include_directories(VkLayer_parameter_validation PRIVATE
        ${SRC_DIR}/include
        ${COMMON_DIR}/include
        ${SRC_DIR}/layers
        ${SRC_DIR}/loader)
target_link_libraries(VkLayer_parameter_validation PRIVATE log layer_utils)

add_library(VkLayer_object_tracker SHARED
        ${COMMON_DIR}/include/object_tracker.cpp
        ${COMMON_DIR}/include/layer_chassis_dispatch.cpp
        ${COMMON_DIR}/include/chassis.cpp
        ${SRC_DIR}/layers/object_tracker_utils.cpp)
target_compile_definitions(VkLayer_object_tracker PUBLIC "BUILD_OBJECT_TRACKER")
target_include_directories(VkLayer_object_tracker PRIVATE
        ${SRC_DIR}/include
        ${SRC_DIR}/layers
        ${COMMON_DIR}/include
        ${SRC_DIR}/loader)
target_link_libraries(VkLayer_object_tracker PRIVATE log layer_utils)

add_library(VkLayer_threading SHARED
        ${SRC_DIR}/layers/threading.cpp)
target_include_directories(VkLayer_threading PRIVATE
        ${SRC_DIR}/include
        ${SRC_DIR}/layers
        ${COMMON_DIR}/include
        ${SRC_DIR}/loader)
target_link_libraries(VkLayer_threading PRIVATE log layer_utils)

add_library(VkLayer_unique_objects SHARED
        ${COMMON_DIR}/include/layer_chassis_dispatch.cpp)
        ${COMMON_DIR}/include/chassis.cpp)
target_compile_definitions(VkLayer_object_tracker PUBLIC "LAYER_CHASSIS_CAN_WRAP_HANDLES")
target_include_directories(VkLayer_unique_objects PRIVATE
        ${SRC_DIR}/include
        ${SRC_DIR}/layers
        ${COMMON_DIR}/include
        ${SRC_DIR}/loader)
target_link_libraries(VkLayer_unique_objects PRIVATE log layer_utils)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_clone}")
