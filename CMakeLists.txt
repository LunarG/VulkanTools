# ~~~
# Copyright (c) 2020-2025 LunarG, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ~~~

cmake_minimum_required(VERSION 3.22.1)

project(VULKAN_TOOLS LANGUAGES CXX)

add_subdirectory(scripts)

find_package(VulkanHeaders REQUIRED CONFIG)
find_package(VulkanLoader CONFIG)
find_package(VulkanUtilityLibraries REQUIRED CONFIG)
find_package(valijson REQUIRED CONFIG)

find_package(Qt6 COMPONENTS Core Gui Widgets Network QUIET)
find_package(Qt5 COMPONENTS Core Gui Widgets Network QUIET)
if(Qt6_FOUND)
  get_target_property(QT_TARGET_TYPE Qt6::Core TYPE)
elseif(Qt5_FOUND)
  get_target_property(QT_TARGET_TYPE Qt5::Core TYPE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN "YES")

include(GNUInstallDirs)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(BUILD_WERROR "Treat compiler warnings as errors")
if (BUILD_WERROR)
    add_compile_options("$<IF:$<CXX_COMPILER_ID:MSVC>,/WX,-Werror>")
endif()

option(BUILD_TESTS "Build tests")
option(RUN_ON_GITHUB "Build only tests that can run on Github" OFF)

if(BUILD_TESTS)
    if (WIN32 AND (QT_TARGET_TYPE STREQUAL STATIC_LIBRARY))
        message("WARNING: Static Qt detected! Please ensure googletest is built with update_deps.sh options: '--gtest-shared-libs=OFF' and '--gtest-force-shared-crt=OFF'")
    endif()
    enable_testing()
    find_package(GTest REQUIRED CONFIG)
endif()

# NOTE: Our custom code generation target isn't desirable for system package managers or add_subdirectory users.
# So this target needs to be off by default to avoid obtuse build errors or patches.
option(VT_CODEGEN "Enable vulkantools code generation")
if (VT_CODEGEN)
    find_package(Python3 REQUIRED)
    add_custom_target(vt_codegen
        COMMAND Python3::Interpreter "${CMAKE_SOURCE_DIR}/scripts/generate_source.py"
            "${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry"
            --incremental --generated-version ${VulkanHeaders_VERSION}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/layersvt/generated
    )

    file(GLOB VT_GENERATED_FILES "${CMAKE_SOURCE_DIR}/layersvt/generated/*.cpp" "${CMAKE_SOURCE_DIR}/layersvt/generated/*.h")
    add_custom_command(OUTPUT ${VT_GENERATED_FILES}
        COMMAND Python3::Interpreter "${CMAKE_SOURCE_DIR}/scripts/generate_source.py"
            "${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry"
            --incremental --generated-version ${VulkanHeaders_VERSION}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/layersvt/generated
        DEPENDS ${CMAKE_SOURCE_DIR}/scripts/api_dump_generator.py ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/vk.xml
    )

    add_custom_target(vt_codegen_as_needed DEPENDS ${VT_GENERATED_FILES})
endif()

if (MSVC)
    add_compile_options("$<$<CONFIG:Release>:/Zi>")
    add_link_options("$<$<CONFIG:Release>:/DEBUG:FULL>")
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Windows|Linux|BSD")

    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_MONITOR "Build monitor layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    option(BUILD_LAYERMGR "Build Vulkan Configurator" ON)

elseif(ANDROID)

    # Currently the Android build only build the API dump and screen shot layer.
    # - libVkLayer_api_dump.so
    # - libVkLayer_screenshot.so
    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    set(BUILD_MONITOR OFF)
    set(BUILD_LAYERMGR OFF)

elseif (APPLE)

    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    set(BUILD_MONITOR OFF)

    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        option(BUILD_LAYERMGR "Build Vulkan Configurator" ON)
    endif()

endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_APIDUMP OR BUILD_MONITOR OR BUILD_SCREENSHOT)
    message(STATUS "INFO: Building Vulkan Layers")
    add_subdirectory(layersvt)
endif()

if(BUILD_LAYERMGR)
    if(NOT Qt6_FOUND AND NOT Qt5_FOUND)
        message("WARNING: Vulkan Configurator will be excluded from the build because Qt6 or Qt5 was not found. Please add Qt6 or Qt5 into the PATH environment variable")
    else()
        if(Qt6_FOUND)
            if(Qt6_VERSION VERSION_LESS 6.2)
                message("WARNING: Qt 6.5 or newer is recommanded to build Vulkan Configurator but the found version was Qt " ${Qt6_VERSION})
            endif()
        elseif(Qt5_FOUND)
            message("WARNING: Qt 6.5 or newer is recommanded to build Vulkan Configurator but the found version was Qt " ${Qt5_VERSION})
        endif()

        message(STATUS "INFO: Building Vulkan Configurator")

        if (WIN32)
            # Minimize what Windows.h leaks
            add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
        endif()

        add_subdirectory(vkconfig_core)
        add_subdirectory(vkconfig_cmd)
        add_subdirectory(vkconfig_gui)

        if(WIN32 AND BUILD_TESTS AND (QT_TARGET_TYPE STREQUAL STATIC_LIBRARY))
            set_property(TARGET vkconfig-gui vkconfig-cmd vkconfig-core PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
            message(STATUS "INFO: vkconfig will link against static runtime")
        endif()

    endif()
endif()
