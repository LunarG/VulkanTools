cmake_minimum_required(VERSION 3.10.2)
project(vkconfig)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 COMPONENTS Core Gui Widgets Network QUIET)

if(Qt5_FOUND)
    file(GLOB FILES_SOURCE ./*.cpp)
    file(GLOB FILES_HEADER ./*.h)
    file(GLOB FILES_DOCS ./*.md)

    source_group("Docs Files" FILES ${FILES_DOCS})

    set(FILES_ALL ${FILES_SOURCE} ${FILES_HEADER})

    add_definitions(-DQT_NO_DEBUG_OUTPUT)
    add_definitions(-DQT_NO_WARNING_OUTPUT)

    if(NOT APPLE)
        add_library(vkconfig_core STATIC ${FILES_ALL})

        if(WIN32)
            add_definitions(-D_CRT_SECURE_NO_WARNINGS)
            target_link_libraries(vkconfig_core Cfgmgr32)
        endif()

        target_include_directories(vkconfig_core PRIVATE "${Vulkan_INCLUDE_DIR}")
        target_link_libraries(vkconfig_core Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
        target_compile_definitions(vkconfig_core PRIVATE ${VKCONFIG_DEFINITIONS})

        add_subdirectory(test)
    endif()
endif()
