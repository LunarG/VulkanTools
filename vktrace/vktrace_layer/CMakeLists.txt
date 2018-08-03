cmake_minimum_required(VERSION 2.8)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
   if (BUILD_WSI_XCB_SUPPORT)
       set(ENV{VULKAN_WSI} "Xcb")
       find_library(XCB-KEYSYMS xcb-keysyms)
   elseif (BUILD_WSI_XLIB_SUPPORT)
       set(ENV{VULKAN_WSI} "Xlib")
   elseif (BUILD_WSI_WAYLAND_SUPPORT)
       set(ENV{VULKAN_WSI} "Wayland")
   else()
       # Mir WSI Case
       set(ENV{VULKAN_WSI} "Mir")
   endif()
endif()

project(VkLayer_vktrace_layer)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../)

set(TARGET_NAMES VkLayer_vktrace_layer)

if (WIN32)
    # Put VkLayer_vktrace_layer.dll in the same directory as vktrace.exe
    # so that vktrace.exe can find VkLayer_vktrace_layer.dll.
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../../layersvt/)
endif()

set(SRC_LIST
    ${SRC_LIST}
    vktrace_lib.c
    vktrace_lib_pagestatusarray.cpp
    vktrace_lib_pageguardmappedmemory.cpp
    vktrace_lib_pageguardcapture.cpp
    vktrace_lib_pageguard.cpp
    vktrace_lib_trace.cpp
    vktrace_lib_trim.cpp
    vktrace_lib_trim_generate.cpp
    vktrace_lib_trim_statetracker.cpp
    vktrace_vk_exts.cpp
    ${GENERATED_FILES_DIR}/vktrace_vk_vk.cpp
)

set_source_files_properties( ${SRC_LIST} PROPERTIES LANGUAGE CXX)

set(HDR_LIST
    vktrace_lib_helpers.h
    vktrace_lib_trim.h
    vktrace_lib_trim_generate.h
    vktrace_lib_trim_statetracker.h
    vktrace_lib_pagestatusarray.h
    vktrace_lib_pageguardmappedmemory.h
    vktrace_lib_pageguardcapture.h
    vktrace_lib_pageguard.h
    vktrace_vk_exts.h
)

# def file - needed for Windows 32-bit so that vk functions names aren't mangled
if (WIN32)
    if (NOT(CMAKE_GENERATOR MATCHES "Win64"))
        set (HDR_LIST ${HDR_LIST} VkLayer_vktrace_layer.def)
    endif()
endif()

include_directories(
    ${SRC_DIR}/vktrace_common
    ${SRC_DIR}/vktrace_trace
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${VKTRACE_VULKAN_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}
    ${GENERATED_FILES_DIR}
    ${Vulkan-ValidationLayers_INCLUDE_DIR}
)

if (NOT WIN32)
    set (GIPA VK_LAYER_LUNARG_vktraceGetInstanceProcAddr)
    set (GDPA VK_LAYER_LUNARG_vktraceGetDeviceProcAddr)
else()
    set (GIPA __HOOKED_vktraceGetInstanceProcAddr)
    set (GDPA __HOOKED_vktraceGetDeviceProcAddr)
endif()

# json file creation

# The output file needs Unix "/" separators or Windows "\" separators.
# On top of that, Windows separators actually need to be doubled because the json format uses backslash escapes.
file(TO_NATIVE_PATH "./" RELATIVE_PATH_PREFIX)
string(REPLACE "\\" "\\\\" RELATIVE_PATH_PREFIX "${RELATIVE_PATH_PREFIX}")
 # Run each .json.in file through the generator.
# We need to create the generator.cmake script so that the generator can be run at compile time, instead of configure time.
# Running at compile time lets us use cmake generator expressions (TARGET_FILE_NAME and TARGET_FILE_DIR, specifically).
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/generator.cmake" "configure_file(\"\${INPUT_FILE}\" \"\${OUTPUT_FILE}\")")
foreach(TARGET_NAME ${TARGET_NAMES})
    set(CONFIG_DEFINES
        -DGIPA="${GIPA}"
        -DGDPA="${GDPA}"
        -DINPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/json/${TARGET_NAME}.json.in"
        -DVK_VERSION="${VulkanHeaders_VERSION_MAJOR}.${VulkanHeaders_VERSION_MINOR}.${VulkanHeaders_VERSION_PATCH}"
    )
    set(CONFIG_DEFINES ${CONFIG_DEFINES}
        -DOUTPUT_FILE="${CMAKE_CURRENT_BINARY_DIR}/../../layersvt/${TARGET_NAME}.json"
        -DRELATIVE_LAYER_BINARY="${RELATIVE_PATH_PREFIX}$<TARGET_FILE_NAME:${TARGET_NAME}>"
    )
    add_custom_target(${TARGET_NAME}-json ALL COMMAND ${CMAKE_COMMAND} ${CONFIG_DEFINES} -P "${CMAKE_CURRENT_BINARY_DIR}/generator.cmake")
endforeach()

#if (NOT WIN32)
#    # extra setup for out-of-tree builds
#    #if (NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR))
#    #    add_custom_target(vktrace_layer-json ALL
#    #        COMMAND ln -sf ${CMAKE_CURRENT_SOURCE_DIR}/linux/VkLayer_vktrace_layer.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}../layersvt/
#    #        VERBATIM
#    #        )
#    #endif()
#    # Add targets for JSON file install on Linux.
#    # Need to remove the "../vktrace" from the library path before installing to /etc.
#    set (LAYER_JSON_FILES VkLayer_vktrace_layer)
#    foreach (config_file ${LAYER_JSON_FILES})
#        add_custom_target(${config_file}-staging-json ALL
#            COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/staging-json
#            COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/linux/${config_file}.json ${CMAKE_CURRENT_BINARY_DIR}/staging-json
#            COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/linux/${config_file}.json ${CMAKE_CURRENT_BINARY_DIR}/staging-json
#            COMMAND sed -i -e "/\"library_path\":/s$../vktrace/libVkLayer$libVkLayer$" ${CMAKE_CURRENT_BINARY_DIR}/staging-json/${config_file}.json
#            VERBATIM
#            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/json/${config_file}.json.in
#            )
#        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/staging-json/${config_file}.json DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/vulkan/explicit_layer.d)
#    endforeach(config_file)
#else()
#    if (NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR))
#        FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/windows/VkLayer_vktrace_layer.json src_json)
#        if (CMAKE_GENERATOR MATCHES "^Visual Studio.*")
#            FILE(TO_NATIVE_PATH ${PROJECT_BINARY_DIR}/../../layersvt/$<CONFIGURATION>/VkLayer_vktrace_layer.json dst_json)
#        else()
#            FILE(TO_NATIVE_PATH ${PROJECT_BINARY_DIR}/../../layersvt/VkLayer_vktrace_layer.json dst_json)
#        endif()
#
#        add_custom_target(vktrace_layer-json ALL
#            COMMAND copy ${src_json} ${dst_json}
#            VERBATIM
#            )
#        add_dependencies(vktrace_layer-json VkLayer_vktrace_layer)
#        set_target_properties(vktrace_layer-json PROPERTIES FOLDER ${VULKANTOOLS_TARGET_FOLDER})
#    endif()
#endif()

add_library(${PROJECT_NAME} SHARED ${SRC_LIST} ${HDR_LIST})

add_dependencies(${PROJECT_NAME} vktrace_generate_helper_files)

# Set Compiler Flags, Libraries
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(OS_TRACER_LIBS
        -shared
        -ldl
    )
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    set(OS_TRACER_LIBS)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
target_link_libraries(${PROJECT_NAME}
    vktrace_common
    ${VKTRACE_VULKAN_LIB}
    ${OS_TRACER_LIBS}
)
else()
target_link_libraries(${PROJECT_NAME}
    vktrace_common
    xcb
    xcb-keysyms
    ${VKTRACE_VULKAN_LIB}
    ${OS_TRACER_LIBS}
)
install(TARGETS VkLayer_vktrace_layer DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

build_options_finalize()

set_target_properties(VkLayer_vktrace_layer PROPERTIES LINKER_LANGUAGE C)
