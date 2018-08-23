cmake_minimum_required(VERSION 2.8)
project(vkreplay)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    set(XCB_DISPLAY_LIB vkdisplay_xcb)

    set(XCB_SRC_LIST
        vkreplay_vkdisplay_xcb.cpp
    )

    set (XCB_HDR_LIST
        vkreplay_vkdisplay.h
    )

    add_library(${XCB_DISPLAY_LIB} SHARED ${XCB_SRC_LIST} ${XCB_HDR_LIST})

    target_link_libraries(${XCB_DISPLAY_LIB}
        xcb
        vktrace_common
    )

    set(WAYLAND_DISPLAY_LIB vkdisplay_wayland)

    set(WAYLAND_SRC_LIST
        vkreplay_vkdisplay_wayland.cpp
    )

    set (WAYLAND_HDR_LIST
        vkreplay_vkdisplay.h
    )

    add_library(${WAYLAND_DISPLAY_LIB} SHARED ${WAYLAND_SRC_LIST} ${WAYLAND_HDR_LIST})

    target_link_libraries(${WAYLAND_DISPLAY_LIB}
        ${WAYLAND_CLIENT_LIBRARIES}
        vktrace_common
    )

    # Make sure the exe directory is searched when loading libraries with dlopen
    SET(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN'")
endif()


set(SRC_LIST
    ${SRC_LIST}
    vkreplay.cpp
    vkreplay_settings.cpp
    vkreplay_vkreplay.cpp
    vkreplay_vkdisplay.cpp
    ${GENERATED_FILES_DIR}/vkreplay_vk_replay_gen.cpp
    vkreplay_factory.h
    vkreplay_seq.h
    vkreplay_window.h
    vkreplay_main.cpp
    vkreplay_seq.cpp
    vkreplay_factory.cpp
    ${SRC_DIR}/../layersvt/screenshot_parsing.cpp
)

set (HDR_LIST
    vkreplay.h
    vkreplay_settings.h
    vkreplay_vkreplay.h
    ${SRC_DIR}/../layersvt/screenshot_parsing.h
    ${GENERATED_FILES_DIR}/vkreplay_vk_objmapper.h
    ${GENERATED_FILES_DIR}/vktrace_vk_packet_id.h
    ${GENERATED_FILES_DIR}/vktrace_vk_vk_packets.h
)

include_directories(
    codegen
    ${SRC_DIR}/vktrace_common
    ${SRC_DIR}/thirdparty
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${VULKAN_TOOLS_SOURCE_DIR}/layersvt
    ${GENERATED_FILES_DIR}
    ${VKTRACE_VULKAN_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Vulkan-ValidationLayers_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${HDR_LIST})

add_dependencies(${PROJECT_NAME} vktrace_generate_helper_files)

target_link_libraries(${PROJECT_NAME}
    ${LIBRARIES}
    Vulkan::Vulkan
    vktrace_common
)

build_options_finalize()
if(UNIX)
    install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(TARGETS ${XCB_DISPLAY_LIB} DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(TARGETS ${WAYLAND_DISPLAY_LIB} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
