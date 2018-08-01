cmake_minimum_required (VERSION 2.8.11)

file(GLOB IMAGES
   "${PROJECT_SOURCE_DIR}/layersvt/images/*"
)
file(COPY ${IMAGES} DESTINATION ${CMAKE_BINARY_DIR}/layersvt/images)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR -DVK_USE_PLATFORM_WIN32_KHX -DWIN32_LEAN_AND_MEAN)
    set(DisplayServer Win32)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR -DVK_USE_PLATFORM_ANDROID_KHX)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (BUILD_WSI_XCB_SUPPORT)
        add_definitions(-DVK_USE_PLATFORM_XCB_KHR -DVK_USE_PLATFORM_XCB_KHX)
        set(DisplayServer Xcb)
    endif()

    if (BUILD_WSI_XLIB_SUPPORT)
        if (NOT DisplayServer)
            set(DisplayServer Xlib)
        endif()
        add_definitions(-DVK_USE_PLATFORM_XLIB_KHR -DVK_USE_PLATFORM_XLIB_KHX)
    endif()

    if (BUILD_WSI_WAYLAND_SUPPORT)
       # TODO Add Wayland Support
       # add_definitions(-DVK_USE_PLATFORM_WAYLAND_KHR)
    endif()

    if (BUILD_WSI_MIR_SUPPORT)
       # TODO Add Mir Support
       # add_definitions(-DVK_USE_PLATFORM_MIR_KHR)
       # include_directories(${MIR_INCLUDE_DIR})
    endif()
else()
    message(FATAL_ERROR "Unsupported Platform!")
endif()

add_custom_target( generate_api_cpp DEPENDS api_dump.cpp )
add_custom_target( generate_api_h DEPENDS api_dump_text.h )
add_custom_target( generate_api_html_h DEPENDS api_dump_html.h )
set_target_properties(generate_api_cpp generate_api_h generate_api_html_h PROPERTIES FOLDER ${VULKANTOOLS_TARGET_FOLDER})

set(TARGET_NAMES
    VkLayer_api_dump
    VkLayer_monitor
    VkLayer_screenshot
    VkLayer_device_simulation
    )

set(VK_LAYER_RPATH /usr/lib/x86_64-linux-gnu/vulkan/layer:/usr/lib/i386-linux-gnu/vulkan/layer)
set(CMAKE_INSTALL_RPATH ${VK_LAYER_RPATH})

if (WIN32)
    macro(add_vk_layer target)
    FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/VkLayer_${target}.def DEF_FILE)
    add_custom_target(copy-${target}-def-file ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DEF_FILE} VkLayer_${target}.def
        VERBATIM
    )
    add_library(VkLayer_${target} SHARED ${ARGN} VkLayer_${target}.def)
    target_link_Libraries(VkLayer_${target} ${VkLayer_utils_LIBRARY})
    add_dependencies(VkLayer_${target} generate_api_cpp generate_api_h generate_api_html_h)
    set_target_properties(copy-${target}-def-file PROPERTIES FOLDER ${VULKANTOOLS_TARGET_FOLDER})
    endmacro()
else()
    macro(add_vk_layer target)
    add_library(VkLayer_${target} SHARED ${ARGN})
    target_link_Libraries(VkLayer_${target} ${VkLayer_utils_LIBRARY})
    add_dependencies(VkLayer_${target} generate_api_cpp generate_api_h generate_api_html_h)
    set_target_properties(VkLayer_${target} PROPERTIES LINK_FLAGS "-Wl,-Bsymbolic")
    install(TARGETS VkLayer_${target} DESTINATION ${CMAKE_INSTALL_LIBDIR})
    endmacro()
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
    ${Vulkan-ValidationLayers_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/../glslang/SPIRV
    ${JSONCPP_INCLUDE_DIR}
)

if (WIN32)
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_CRT_SECURE_NO_WARNINGS")
    set (CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -D_CRT_SECURE_NO_WARNINGS")

    # For VS 2015, which uses compiler version 1900, core_validation.cpp fails with too many objects
    # without either optimizations enabled, or setting the /bigobj compilation option.  Since
    # optimizations are enabled in a release build, this only affects the debug build.  For now,
    # enable /bigobj mode for all debug layer files. An alternative for the future is to split
    # large source files into multiple files which will also alleviate the compilation error.
    if (MSVC AND NOT (MSVC_VERSION LESS 1900))
        set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -D_CRT_SECURE_NO_WARNINGS /bigobj")
        set (CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -D_CRT_SECURE_NO_WARNINGS /bigobj")
    else()
        set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -D_CRT_SECURE_NO_WARNINGS")
        set (CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -D_CRT_SECURE_NO_WARNINGS")
    endif()
else()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpointer-arith")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpointer-arith")
endif()

#VulkanTools layers
run_vulkantools_vk_xml_generate(api_dump_generator.py api_dump.cpp)
run_vulkantools_vk_xml_generate(api_dump_generator.py api_dump_text.h)
run_vulkantools_vk_xml_generate(api_dump_generator.py api_dump_html.h)

add_vk_layer(monitor monitor.cpp vk_layer_table.cpp)
add_vk_layer(screenshot screenshot.cpp screenshot_parsing.h screenshot_parsing.cpp vk_layer_table.cpp)
add_vk_layer(device_simulation device_simulation.cpp vk_layer_table.cpp ${JSONCPP_SOURCE_DIR}/jsoncpp.cpp)
add_vk_layer(api_dump api_dump.cpp vk_layer_table.cpp)

# The output file needs Unix "/" separators or Windows "\" separators
# On top of that, Windows separators actually need to be doubled because the json format uses backslash escapes 
file(TO_NATIVE_PATH "./" RELATIVE_PATH_PREFIX)
string(REPLACE "\\" "\\\\" RELATIVE_PATH_PREFIX "${RELATIVE_PATH_PREFIX}")
 # Run each .json.in file through the generator
# We need to create the generator.cmake script so that the generator can be run at compile time, instead of configure time
# Running at compile time lets us use cmake generator expressions (TARGET_FILE_NAME and TARGET_FILE_DIR, specifically)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/generator.cmake" "configure_file(\"\${INPUT_FILE}\" \"\${OUTPUT_FILE}\")")
foreach(TARGET_NAME ${TARGET_NAMES})
    set(CONFIG_DEFINES
        -DINPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/json/${TARGET_NAME}.json.in"
        -DVK_VERSION="${VulkanHeaders_VERSION_MAJOR}.${VulkanHeaders_VERSION_MINOR}.${VulkanHeaders_VERSION_PATCH}"
    )
    # Get the needed properties from that target
    set(CONFIG_DEFINES ${CONFIG_DEFINES}
        -DOUTPUT_FILE="$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_NAME}.json"
        -DRELATIVE_LAYER_BINARY="${RELATIVE_PATH_PREFIX}$<TARGET_FILE_NAME:${TARGET_NAME}>"
        )
    add_custom_target(${TARGET_NAME}-json ALL COMMAND ${CMAKE_COMMAND} ${CONFIG_DEFINES} -P "${CMAKE_CURRENT_BINARY_DIR}/generator.cmake")
endforeach()
