cmake_minimum_required(VERSION 3.10.2)
project(vktrace)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../)

set(SRC_LIST
    ${SRC_LIST}
    vktrace.cpp
    vktrace_process.h
    vktrace_process.cpp
    ${SRC_DIR}/../layersvt/screenshot_parsing.h
    ${SRC_DIR}/../layersvt/screenshot_parsing.cpp
)

include_directories(
    ${SRC_DIR}
    ${SRC_DIR}/vktrace_common
    ${SRC_DIR}/vktrace_trace
    ${VULKAN_TOOLS_SOURCE_DIR}/layersvt
    ${CMAKE_BINARY_DIR}
    ${Vulkan-ValidationLayers_INCLUDE_DIR}
    ${GENERATED_FILES_DIR}
)

if (NOT WIN32)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

add_executable(${PROJECT_NAME} ${SRC_LIST})

add_dependencies(${PROJECT_NAME} vktrace_generate_helper_files)

target_link_libraries(${PROJECT_NAME}
    vktrace_common
)

build_options_finalize()
if(UNIX)
    install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

