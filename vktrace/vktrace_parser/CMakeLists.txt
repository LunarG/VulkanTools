project(vkparser)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    remove_definitions(-DVK_USE_PLATFORM_WIN32_KHR -DWIN32_LEAN_AND_MEAN)
    set(DisplayServer Win32)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    remove_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)

    if (BUILD_WSI_XCB_SUPPORT)
        remove_definitions(-DVK_USE_PLATFORM_XCB_KHR)
    endif()

    if (BUILD_WSI_XLIB_SUPPORT)
        remove_definitions(-DVK_USE_PLATFORM_XLIB_KHR)
    endif()

    if (BUILD_WSI_WAYLAND_SUPPORT)
        remove_definitions(-DVK_USE_PLATFORM_WAYLAND_KHR)
    endif()
else()
    message(FATAL_ERROR "Unsupported Platform!")
endif()

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${SRC_DIR}/cmake/Modules/")
set(GENERATED_FILES_DIR ${CMAKE_BINARY_DIR}/vktrace)

# Run a codegen script to generate vktrace-specific vulkan utils
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VulkanRegistry_DIR}/vk.xml -scripts ${VulkanRegistry_DIR} -o ${GENERATED_FILES_DIR} vktrace_vk_packet_id.h)
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VulkanRegistry_DIR}/vk.xml -scripts ${VulkanRegistry_DIR} -o ${GENERATED_FILES_DIR} vktrace_vk_vk_packets.h)
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VulkanRegistry_DIR}/vk.xml -scripts ${VulkanRegistry_DIR} -o ${GENERATED_FILES_DIR} vkparser_vk_parser_gen.cpp)
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VulkanRegistry_DIR}/vk.xml -scripts ${VulkanRegistry_DIR} -o ${GENERATED_FILES_DIR} api_dump_text.h WORKING_DIRECTORY ${GENERATED_FILES_DIR})
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VulkanRegistry_DIR}/vk.xml -scripts ${VulkanRegistry_DIR} -o ${GENERATED_FILES_DIR} api_dump_html.h WORKING_DIRECTORY ${GENERATED_FILES_DIR})

set(SRC_LIST
    ${SRC_LIST}
    vkparser_main.cpp
    ${Vulkan-ValidationLayers_INCLUDE_DIR}/vk_layer_config.cpp
    ${GENERATED_FILES_DIR}/vkparser_vk_parser_gen.cpp
)

include_directories(
    ${GENERATED_FILES_DIR}
    ${SRC_DIR}
    ${SRC_DIR}/vktrace_common
    ${SRC_DIR}/thirdparty
    ${VULKAN_TOOLS_SOURCE_DIR}/layersvt
    ${VALIDATION_LAYERS_ROOT_DIR}/layers
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${VKTRACE_VULKAN_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Vulkan-ValidationLayers_INCLUDE_DIR}
)

if (${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    add_compiler_flag("-Wno-global-constructors -Wno-used-but-marked-unused")
endif()

# Platform specific compile flags.
if (NOT MSVC)
    add_compiler_flag("-fPIC")
endif()

add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADER_LIST})

add_compiler_flag("-std=c++11")

add_dependencies(${PROJECT_NAME} vktrace_generate_helper_files)

target_link_libraries(${PROJECT_NAME}
    vktrace_common
    ${CMAKE_DL_LIBS}
)

build_options_finalize()

