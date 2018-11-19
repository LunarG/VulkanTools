
cmake_minimum_required(VERSION 3.4)
project(vkconfig)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets)

if(NOT Qt5_FOUND)
    message("WARNING: vkconfig will be excluded because Qt5 was not found.")
else()
    set(LAYERMGR_SRCS
        active_layers_widget.cpp
        layer_locations_widget.cpp
        layer_manifest.cpp
        layer_settings_widget.cpp
        override_settings.cpp
        vkconfig.cpp
        data.qrc
    )

    if(NOT APPLE)
        if(WIN32)
            add_executable(vkconfig WIN32 ${LAYERMGR_SRCS})
            target_link_libraries(vkconfig Cfgmgr32)
        else()
            add_executable(vkconfig ${LAYERMGR_SRCS})
        endif()
        target_include_directories(vkconfig PRIVATE "${Vulkan_INCLUDE_DIR}")
        target_link_libraries(vkconfig Qt5::Core Qt5::Gui Qt5::Widgets)

        if(UNIX)
            install(TARGETS vkconfig DESTINATION ${CMAKE_INSTALL_BINDIR})
        endif()
    else()
        include(macOS/vkconfig.cmake)
    endif()
endif()
