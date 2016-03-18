cmake_minimum_required(VERSION 2.8)

project(vktraceviewer_vk)

include("${SRC_DIR}/build_options.cmake")

find_package(Qt5 COMPONENTS Widgets Gui Core Svg QUIET)

if(NOT Qt5_FOUND)
# After Qt5.3 is installed, you may need to add the following to the cmake command line:
# -DQt5_DIR=/home/<username>/Qt/5.3/gcc_64/lib/cmake/Qt5
message(WARNING "WARNING: vktraceviewer_vk will be excluded because Qt5 was not found.")
else()

set(SRC_LIST
    ${SRC_LIST}
    vktraceviewer_vk.cpp
    vktraceviewer_vk_settings.cpp
    vktraceviewer_vk_qcontroller.cpp
    vktraceviewer_vk_qfile_model.cpp
    ${SRC_DIR}/vktrace_viewer/vktraceviewer_QReplayWorker.cpp
    ${SRC_DIR}/vktrace_replay/vkreplay_factory.cpp
#    ${GLV_VULKAN_DIR}/vulkan/codegen_utils/vk_struct_string_helper.c
#    ${GLV_VULKAN_DIR}/vulkan/codegen_utils/vk_wsi_lunarg_struct_string_helper.c
)

# This should only contain headers that define a QOBJECT
# Typically that means just headers for UI objects
set(UI_HEADER_LIST
    vktraceviewer_vk_qcontroller.h
    vktraceviewer_vk_qfile_model.h
    vktraceviewer_vk_qgroupframesproxymodel.h
    ${SRC_DIR}/vktrace_viewer/vktraceviewer_qgroupthreadsproxymodel.h
    ${SRC_DIR}/vktrace_viewer/vktraceviewer_qimageviewer.h
    ${SRC_DIR}/vktrace_viewer/vktraceviewer_qsvgviewer.h
    ${SRC_DIR}/vktrace_viewer/vktraceviewer_QTraceFileModel.h
    ${SRC_DIR}/vktrace_viewer/vktraceviewer_QReplayWidget.h
    ${SRC_DIR}/vktrace_viewer/vktraceviewer_QReplayWorker.h
)

set(HDR_LIST
    vktraceviewer_vk_settings.h
    vktraceviewer_vk_qgroupframesproxymodel.h
    ${SRC_DIR}/vktrace_viewer/vktraceviewer_qgroupthreadsproxymodel.h
    ${SRC_DIR}/vktrace_viewer/vktraceviewer_controller.h
    ${SRC_DIR}/vktrace_replay/vkreplay_factory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/codegen_utils/vk_enum_string_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/codegen_utils/vk_struct_string_helper.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/codegen_utils/vk_wsi_lunarg_enum_string_helper.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/codegen_utils/vk_wsi_lunarg_struct_string_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/codegen_utils/vk_wsi_swapchain_enum_string_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/codegen_utils/vk_wsi_swapchain_struct_string_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/codegen_utils/vk_wsi_device_swapchain_enum_string_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/codegen_utils/vk_wsi_device_swapchain_struct_string_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/codegen_utils/vk_debug_report_lunarg_enum_string_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/codegen_utils/vk_debug_report_lunarg_struct_string_helper.h
    ${CODEGEN_VKTRACE_DIR}/vktrace_vk_packet_id.h
    ${CODEGEN_VKTRACE_DIR}/vktrace_vk_vk_packets.h
    # ${CODEGEN_VKTRACE_DIR}/vktrace_vk_vk_wsi_lunarg_packets.h
    ${CODEGEN_VKTRACE_DIR}/vktrace_vk_vk_debug_report_lunarg_packets.h
)

include_directories(
    ${CODEGEN_VKTRACE_DIR}
    ${SRC_DIR}/vktrace_common
    ${SRC_DIR}/vktrace_viewer
    ${SRC_DIR}/vktrace_replay
    ${SRC_DIR}/thirdparty
    ${VKTRACE_VULKAN_DIR}/${CODEGEN_VKTRACE_DIR}
    ${VKTRACE_VULKAN_INCLUDE_DIR}
    ${VKTRACE_VULKAN_DIR}/vulkan/codegen_utils
    ${Qt5Widgets_INCLUDE_DIRS}
)

QT5_WRAP_CPP(QT_GEN_HEADER_MOC_LIST ${UI_HEADER_LIST})

add_library(${PROJECT_NAME} SHARED ${SRC_LIST} ${HDR_LIST}
    ${QT_GEN_HEADER_MOC_LIST}
)

target_link_libraries(${PROJECT_NAME} 
    Qt5::Widgets
    Qt5::Core
    Qt5::Svg
    ${VKTRACE_VULKAN_LIB}
    vktrace_common
)

build_options_finalize()

endif(NOT Qt5_FOUND)
