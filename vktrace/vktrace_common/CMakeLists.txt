project(vktrace_common)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../)

execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VALIDATION_LAYERS_SCRIPTS_DIR}/vk.xml -o ${GENERATED_FILES_DIR} vk_struct_size_helper.h)
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VALIDATION_LAYERS_SCRIPTS_DIR}/vk.xml -o ${GENERATED_FILES_DIR} vk_struct_size_helper.c)

include_directories(
    ${PROJECT_BINARY_DIR}/../..
    ${SRC_DIR}/vktrace_common
    ${SRC_DIR}/thirdparty
    ${CMAKE_BINARY_DIR}/vktrace
    ${CMAKE_BINARY_DIR}/${VALIDATION_LAYERS_RELATIVE_LOCATION}
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    require_pthreads()
endif()

set(SRC_LIST
    ${SRC_LIST}
    vktrace_filelike.c
    vktrace_interconnect.c
    vktrace_platform.c
    vktrace_process.c
    vktrace_settings.c
    vktrace_tracelog.c
    vktrace_trace_packet_utils.c
    vktrace_pageguard_memorycopy.cpp
)

set (CXX_SRC_LIST
     vktrace_pageguard_memorycopy.cpp
)

set_source_files_properties( ${SRC_LIST} PROPERTIES LANGUAGE C)
set_source_files_properties( ${CXX_SRC_LIST} PROPERTIES LANGUAGE CXX)

file( GLOB_RECURSE HDRS *.[h|inl] )

if (NOT MSVC)
    add_c_compiler_flag("-fPIC")
    add_cpp_compiler_flag("-fPIC -std=c++11")
endif()

add_library(${PROJECT_NAME} STATIC ${SRC_LIST} ${CXX_SRC_LIST} ${HDRS})

add_dependencies(${PROJECT_NAME} generate_helper_files)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
target_link_libraries(${PROJECT_NAME}
    Rpcrt4.lib
)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
target_link_Libraries(${PROJECT_NAME}
    dl
    pthread
)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

build_options_finalize()

set_target_properties(vktrace_common PROPERTIES LINKER_LANGUAGE C)
