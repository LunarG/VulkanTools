cmake_minimum_required(VERSION 3.17.2)

project(VULKAN_TOOLS LANGUAGES CXX)

add_subdirectory(scripts)

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

find_package(VulkanHeaders REQUIRED CONFIG)

find_package(VulkanLoader CONFIG)

find_package(VulkanUtilityLibraries REQUIRED CONFIG)

find_package(valijson REQUIRED CONFIG)

option(BUILD_TESTS "Build tests")
option(RUN_ON_GITHUB "Build only tests that can run on Github" OFF)
option(BUILD_EXPERIMENTAL "Build tests for debugging layers" OFF)

if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED CONFIG)
endif()

# NOTE: Our custom code generation target isn't desirable for system package managers or add_subdirectory users.
# So this target needs to be off by default to avoid obtuse build errors or patches.
option(VT_CODEGEN "Enable vulkantools code generation")
if (VT_CODEGEN)
    find_package(Python3 REQUIRED)
    add_custom_target(vt_codegen
        COMMAND Python3::Interpreter "${VULKAN_TOOLS_SOURCE_DIR}/scripts/generate_source.py"
            "${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry"
            --incremental --generated-version ${VulkanHeaders_VERSION}
        WORKING_DIRECTORY ${VULKAN_TOOLS_SOURCE_DIR}/layersvt/generated
    )
endif()

if (MSVC)
    add_compile_options("$<$<CONFIG:Release>:/Zi>")
    add_link_options("$<$<CONFIG:Release>:/DEBUG:FULL>")
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Windows|Linux|BSD")

    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_MONITOR "Build monitor layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)

    option(BUILD_VIA "Build via" ON)
    option(BUILD_LAYERMGR "Build Vulkan Configurator" ON)

elseif(ANDROID)
    # https://gitlab.kitware.com/cmake/cmake/issues/18787
    # https://github.com/android-ndk/ndk/issues/463
    if (CMAKE_VERSION VERSION_LESS "3.21")
        message(FATAL_ERROR "Android build requires at least CMake 3.21!")
    endif()

    # Currently the Android build only build the API dump and screen shot layer.
    # - libVkLayer_api_dump.so
    # - libVkLayer_screenshot.so
    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    set(BUILD_MONITOR OFF)

    set(BUILD_VIA OFF)
    set(BUILD_LAYERMGR OFF)

elseif (APPLE)

    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    set(BUILD_MONITOR OFF)

    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        option(BUILD_VIA "Build via" ON)
        option(BUILD_LAYERMGR "Build Vulkan Configurator" ON)
    endif()

endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_VIA)
    add_subdirectory(via)
endif()

if(BUILD_APIDUMP OR BUILD_MONITOR OR BUILD_SCREENSHOT)
    message(STATUS "INFO: Building Vulkan Layers")
    add_subdirectory(layersvt)
endif()

if(BUILD_LAYERMGR)
    message(STATUS "INFO: Building Vulkan Configurator")

    if (WIN32)
        # Minimize what Windows.h leaks
        add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
    endif()

    add_subdirectory(vkconfig_core)
    add_subdirectory(vkconfig)
    if(BUILD_EXPERIMENTAL)
        add_subdirectory(vkconfig3)
    endif()
endif()
