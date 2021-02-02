cmake_minimum_required(VERSION 3.10.2)
project(vkconfig)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 COMPONENTS Core Gui Widgets Network QUIET)

if(Qt5_FOUND)
    file(GLOB FILES_SOURCE ./*.cpp)
    file(GLOB FILES_HEADER ./*.h)

    file(GLOB FILES_CONFIGURATIONS ./configurations/*.json)
    source_group("Configurations Files" FILES ${FILES_CONFIGURATIONS})

    file(GLOB FILES_LAYERS_130 ./layers/130/*.json)
    source_group("Layers 130 Files" FILES ${FILES_LAYERS_130})

    file(GLOB FILES_LAYERS_135 ./layers/135/*.json)
    source_group("Layers 135 Files" FILES ${FILES_LAYERS_135})

    file(GLOB FILES_LAYERS_141 ./layers/141/*.json)
    source_group("Layers 141 Files" FILES ${FILES_LAYERS_141})

    file(GLOB FILES_LAYERS_148 ./layers/148/*.json)
    source_group("Layers 148 Files" FILES ${FILES_LAYERS_148})

    file(GLOB FILES_LAYERS_154 ./layers/154/*.json)
    source_group("Layers 154 Files" FILES ${FILES_LAYERS_154})

    file(GLOB FILES_LAYERS_162 ./layers/162/*.json)
    source_group("Layers 162 Files" FILES ${FILES_LAYERS_162})

    file(GLOB FILES_LAYERS_LATEST ./layers/latest/*.json)
    source_group("Layers Latest Files" FILES ${FILES_LAYERS_LATEST})

    set(FILES_RESOURCES 
        ${FILES_CONFIGURATIONS}
        ${FILES_LAYERS_130}
        ${FILES_LAYERS_135}
        ${FILES_LAYERS_141}
        ${FILES_LAYERS_148}
        ${FILES_LAYERS_154}
        ${FILES_LAYERS_162}
        ${FILES_LAYERS_LATEST})

    set(FILES_ALL ${FILES_SOURCE} ${FILES_HEADER} ${FILES_RESOURCES})

    add_definitions(-DQT_NO_DEBUG_OUTPUT)
    add_definitions(-DQT_NO_WARNING_OUTPUT)

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
