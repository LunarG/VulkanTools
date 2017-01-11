PROJECT(vktracevulkan)
cmake_minimum_required(VERSION 2.8)

#include(FindPkgConfig)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(PYTHON_CMD "python3")
else()
    set(PYTHON_CMD "py")
endif()


# if Vktrace is being built as part of a vulkan driver build, then use that target instead of the locally-commited binary.
#if (TARGET vulkan)
#    message(STATUS "Using external Vulkan header and library.")
#    set(VKTRACE_VULKAN_LIB vulkan)
    set(VKTRACE_VULKAN_DRIVER_DIR ${CMAKE_SOURCE_DIR})
    set(VKTRACE_VULKAN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/vulkan)
    set(VKTRACE_VULKAN_HEADER ${CMAKE_SOURCE_DIR}/include/vulkan/vulkan.h)
    #set(VKTRACE_VULKAN_LUNARG_DEBUG_MARKER_HEADER ${VKTRACE_VULKAN_INCLUDE_DIR}/vk_lunarg_debug_marker.h)
#else()
    # Use a locally committed vulkan header and binary
#    message(STATUS "Using Vktrace-supplied Vulkan header and library.")
#    set(VKTRACE_VULKAN_DRIVER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vulkan)
#    set(VKTRACE_VULKAN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vulkan/include)
#    set(VKTRACE_VULKAN_HEADER ${VKTRACE_VULKAN_INCLUDE_DIR}/vulkan/vulkan.h)
#    set(VKTRACE_VULKAN_DEBUG_REPORT_LUNARG_HEADER ${VKTRACE_VULKAN_INCLUDE_DIR}/vk_debug_report_lunarg.h)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
   if (CMAKE_GENERATOR MATCHES "^Visual Studio.*")
      set(VKTRACE_VULKAN_LIB
         ${CMAKE_BINARY_DIR}/loader/${CMAKE_CFG_INTDIR}/${API_LOWERCASE}-1.lib
         )
   else()
      set(VKTRACE_VULKAN_LIB
         ${CMAKE_BINARY_DIR}/loader/${API_LOWERCASE}-1.lib
         )
   endif()
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR
    ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set(VKTRACE_VULKAN_LIB
        ${CMAKE_BINARY_DIR}/loader/lib${API_LOWERCASE}.so
    )

endif()

message(STATUS "VKTRACE_VULKAN_LIB = " ${VKTRACE_VULKAN_LIB})
#message(STATUS "VKTRACE_VULKAN_DRIVER_DIR = " ${VKTRACE_VULKAN_DRIVER_DIR})
#message(STATUS "VKTRACE_VULKAN_HEADER = " ${VKTRACE_VULKAN_HEADER})

# Run a codegen script to generate utilities that are vulkan-specific, dependent on the vulkan header files, and may be shared by the tracer, replayer, or debugger.
# Generally, these are likely to be things that SHOULD be provided by the vulkan SDK.
set(VKTRACE_VULKAN_CODEGEN_UTILS "vulkan/codegen_utils")
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${VKTRACE_VULKAN_CODEGEN_UTILS})

# generate files for vulkan.h
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SCRIPTS_DIR}/lvl_genvk.py -registry ${SCRIPTS_DIR}/vk.xml -o ${CMAKE_CURRENT_SOURCE_DIR}/${VKTRACE_VULKAN_CODEGEN_UTILS} vk_struct_size_helper.h)
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SCRIPTS_DIR}/lvl_genvk.py -registry ${SCRIPTS_DIR}/vk.xml -o ${CMAKE_CURRENT_SOURCE_DIR}/${VKTRACE_VULKAN_CODEGEN_UTILS} vk_struct_size_helper.c)
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SCRIPTS_DIR}/lvl_genvk.py -registry ${SCRIPTS_DIR}/vk.xml -o ${CMAKE_CURRENT_SOURCE_DIR}/${VKTRACE_VULKAN_CODEGEN_UTILS} vk_enum_string_helper.h)

# generate files for vk_lunarg_debug_marker.h
#execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SCRIPTS_DIR}/vk_helper.py --gen_enum_string_helper ${VKTRACE_VULKAN_LUNARG_DEBUG_MARKER_HEADER} --abs_out_dir ${CMAKE_CURRENT_SOURCE_DIR}/${VKTRACE_VULKAN_CODEGEN_UTILS})

# Run a codegen script to generate vktrace-specific vulkan utils
set(CODEGEN_VKTRACE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/codegen_vktrace_utils")
file(MAKE_DIRECTORY ${CODEGEN_VKTRACE_DIR})
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VKTRACE_VULKAN_DIR}/vktrace_generate.py AllPlatforms vktrace-packet-id VK_VERSION_1_0 OUTPUT_FILE ${CODEGEN_VKTRACE_DIR}/vktrace_vk_packet_id.h)
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VKTRACE_VULKAN_DIR}/vktrace_generate.py AllPlatforms vktrace-core-trace-packets VK_VERSION_1_0 OUTPUT_FILE ${CODEGEN_VKTRACE_DIR}/vktrace_vk_vk_packets.h)
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VKTRACE_VULKAN_DIR}/vktrace_generate.py AllPlatforms vktrace-ext-trace-packets vk_lunarg_debug_marker OUTPUT_FILE ${CODEGEN_VKTRACE_DIR}/vktrace_vk_vk_lunarg_debug_marker_packets.h)

# Directories which actually contain vulkan-specific vktrace plugins.
add_subdirectory(vkreplay/)
# Only build vktraceviewer when Qt5 is available
if (Qt5_FOUND AND BUILD_VKTRACEVIEWER)
    add_subdirectory(vktraceviewer/)
endif()
