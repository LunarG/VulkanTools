PROJECT(vktracevulkan)
cmake_minimum_required(VERSION 2.8)

#include(FindPkgConfig)

# if Vktrace is being built as part of a vulkan driver build, then use that target instead of the locally-commited binary.
    set(VKTRACE_VULKAN_DRIVER_DIR ${CMAKE_SOURCE_DIR})
    set(VKTRACE_VULKAN_INCLUDE_DIR ${V_LVL_ROOT_DIR}/include/vulkan)
    set(VKTRACE_VULKAN_HEADER ${V_LVL_ROOT_DIR}/include/vulkan/vulkan.h)
    #set(VKTRACE_VULKAN_LUNARG_DEBUG_MARKER_HEADER ${VKTRACE_VULKAN_INCLUDE_DIR}/vk_lunarg_debug_marker.h)

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

