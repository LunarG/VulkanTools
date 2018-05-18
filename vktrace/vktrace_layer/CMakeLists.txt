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

# Run a codegen script to generate vktrace-specific vulkan utils
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VALIDATION_LAYERS_SCRIPTS_DIR}/vk.xml -o ${GENERATED_FILES_DIR} vktrace_vk_packet_id.h)
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VALIDATION_LAYERS_SCRIPTS_DIR}/vk.xml -o ${GENERATED_FILES_DIR} vktrace_vk_vk_packets.h)

execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VALIDATION_LAYERS_SCRIPTS_DIR}/vk.xml -o ${GENERATED_FILES_DIR} vktrace_vk_vk.h)
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VALIDATION_LAYERS_SCRIPTS_DIR}/vk.xml -o ${GENERATED_FILES_DIR} vktrace_vk_vk.cpp)

execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VALIDATION_LAYERS_SCRIPTS_DIR}/vk.xml -o ${GENERATED_FILES_DIR} vk_struct_size_helper.h)
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VALIDATION_LAYERS_SCRIPTS_DIR}/vk.xml -o ${GENERATED_FILES_DIR} vk_struct_size_helper.c)

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
    ${CMAKE_BINARY_DIR}/${VALIDATION_LAYERS_RELATIVE_LOCATION}/layers
    ${VALIDATION_LAYERS_ROOT_DIR}/Vulkan-Headers/include
    ${VALIDATION_LAYERS_ROOT_DIR}/Vulkan-Headers/include/vulkan
)

# copy/link layer json file into build/layersvt directory
if (NOT WIN32)
    # extra setup for out-of-tree builds
    if (NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR))
        add_custom_target(vktrace_layer-json ALL
            COMMAND ln -sf ${CMAKE_CURRENT_SOURCE_DIR}/linux/VkLayer_vktrace_layer.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}../layersvt/
            VERBATIM
            )
    endif()
    # Add targets for JSON file install on Linux.
    # Need to remove the "../vktrace" from the library path before installing to /etc.
    set (LAYER_JSON_FILES VkLayer_vktrace_layer)
    foreach (config_file ${LAYER_JSON_FILES})
        add_custom_target(${config_file}-staging-json ALL
            COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/staging-json
            COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/linux/${config_file}.json ${CMAKE_CURRENT_BINARY_DIR}/staging-json
            COMMAND sed -i -e "/\"library_path\":/s$../vktrace/libVkLayer$libVkLayer$" ${CMAKE_CURRENT_BINARY_DIR}/staging-json/${config_file}.json
            VERBATIM
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linux/${config_file}.json
            )
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/staging-json/${config_file}.json DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/vulkan/explicit_layer.d)
    endforeach(config_file)
else()
    if (NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR))
        FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/windows/VkLayer_vktrace_layer.json src_json)
        if (CMAKE_GENERATOR MATCHES "^Visual Studio.*")
            FILE(TO_NATIVE_PATH ${PROJECT_BINARY_DIR}/../../layersvt/$<CONFIGURATION>/VkLayer_vktrace_layer.json dst_json)
        else()
            FILE(TO_NATIVE_PATH ${PROJECT_BINARY_DIR}/../../layersvt/VkLayer_vktrace_layer.json dst_json)
        endif()

        add_custom_target(vktrace_layer-json ALL
            COMMAND copy ${src_json} ${dst_json}
            VERBATIM
            )
        add_dependencies(vktrace_layer-json VkLayer_vktrace_layer)
        set_target_properties(vktrace_layer-json PROPERTIES FOLDER ${VULKANTOOLS_TARGET_FOLDER})
    endif()
endif()

add_library(${PROJECT_NAME} SHARED ${SRC_LIST} ${HDR_LIST})

add_dependencies(${PROJECT_NAME} generate_helper_files)

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
