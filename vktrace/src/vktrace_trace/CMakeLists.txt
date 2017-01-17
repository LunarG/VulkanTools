cmake_minimum_required(VERSION 2.8)
project(vktrace)

include("${SRC_DIR}/build_options.cmake")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SRC_LIST
    ${SRC_LIST}
    vktrace.cpp
    vktrace_process.h
    vktrace_process.cpp
    ../../../layersvt/screenshot_parsing.h
    ../../../layersvt/screenshot_parsing.cpp
)

include_directories(
    ${SRC_DIR}
    ${SRC_DIR}/vktrace_common
    ${SRC_DIR}/vktrace_trace
    ${VULKAN_TOOLS_SOURCE_DIR}/layersvt
)

if (NOT WIN32)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}
    vktrace_common
)

build_options_finalize()
