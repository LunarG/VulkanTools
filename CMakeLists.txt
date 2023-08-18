cmake_minimum_required(VERSION 3.17.2)

project(VULKAN_TOOLS)

add_subdirectory(scripts)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN "YES")

include(GNUInstallDirs)

find_package(VulkanHeaders REQUIRED CONFIG)

target_compile_definitions(Vulkan::Headers INTERFACE VK_ENABLE_BETA_EXTENSIONS) # Enable beta Vulkan extensions
if(WIN32)
    target_compile_definitions(Vulkan::Headers INTERFACE VK_USE_PLATFORM_WIN32_KHR)
elseif(ANDROID)
    target_compile_definitions(Vulkan::Headers INTERFACE VK_USE_PLATFORM_ANDROID_KHR)
    target_link_libraries(Vulkan::Headers INTERFACE log)
elseif(APPLE)
    target_compile_definitions(Vulkan::Headers INTERFACE VK_USE_PLATFORM_MACOS_MVK VK_USE_PLATFORM_METAL_EXT)
else()
    option(BUILD_WSI_XCB_SUPPORT "Build XCB WSI support" ON)

    # TODO Fix Xlib
    option(BUILD_WSI_XLIB_SUPPORT "Build Xlib WSI support" OFF)

    # TODO: Fix wayland support
    option(BUILD_WSI_WAYLAND_SUPPORT "Build Wayland WSI support" OFF)

    find_package(PkgConfig REQUIRED QUIET) # Use PkgConfig to find Linux system libraries

    if(BUILD_WSI_XCB_SUPPORT)
        pkg_check_modules(XCB REQUIRED QUIET IMPORTED_TARGET xcb)
        target_compile_definitions(Vulkan::Headers INTERFACE VK_USE_PLATFORM_XCB_KHR)
    endif()

    if(BUILD_WSI_XLIB_SUPPORT)
        pkg_check_modules(X11 REQUIRED QUIET IMPORTED_TARGET x11)
        target_compile_definitions(Vulkan::Headers INTERFACE VK_USE_PLATFORM_XLIB_KHR VK_USE_PLATFORM_XLIB_XRANDR_EXT)
    endif()

    if(BUILD_WSI_WAYLAND_SUPPORT)
        pkg_check_modules(WAYlAND_CLIENT QUIET REQUIRED IMPORTED_TARGET wayland-client)
        target_compile_definitions(Vulkan::Headers INTERFACE VK_USE_PLATFORM_WAYLAND_KHR)
    endif()
endif()

# REMOVE: When VVL is replace with utility libraries!
find_file(VVL_INCLUDE_DIR vk_layer_config.h REQUIRED CMAKE_FIND_ROOT_PATH_BOTH)
get_filename_component(VVL_INCLUDE_DIR ${VVL_INCLUDE_DIR} DIRECTORY)
target_include_directories(Vulkan::Headers INTERFACE ${VVL_INCLUDE_DIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON) # Remove when min is 3.26, see CMP0143

option(BUILD_WERROR "Treat compiler warnings as errors")
if (BUILD_WERROR)
    add_compile_options("$<IF:$<CXX_COMPILER_ID:MSVC>,/WX,-Werror>")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    option(BUILD_MOLTENVK_SUPPORT "Build MoltenVK WSI support" ON)
    option(BUILD_METAL_SUPPORT "Build Metal WSI support" OFF)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows" OR
    CMAKE_SYSTEM_NAME STREQUAL "Linux" OR
    CMAKE_SYSTEM_NAME MATCHES "BSD")

    option(BUILD_TESTS "Build tests" ON)
    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_MONITOR "Build monitor layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    option(BUILD_VIA "Build via" ON)

    option(BUILD_LAYERMGR "Build Vulkan Configurator" ON)

elseif(ANDROID)

    # Currently the Android build only build the API dump and screen shot layer.
    # - libVkLayer_api_dump.so
    # - libVkLayer_screenshot.so
    option(BUILD_APIDUMP "Build api_dump layer" ON)
    set(BUILD_MONITOR OFF)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    option(BUILD_DEMOS OFF)
    option(BUILD_VKJSON OFF)
    set(BUILD_VIA OFF)
    set(BUILD_LAYERMGR OFF)

elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")

    option(BUILD_LOADER OFF)
    option(BUILD_TESTS OFF)
    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_MONITOR "Build monitor layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    option(BUILD_DEMOS OFF)
    option(BUILD_VKJSON OFF)
    option(BUILD_VIA "Build VIA" ON)
    option(BUILD_LAYERMGR "Build Vulkan Configurator" ON)

endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_VIA)
    add_subdirectory(via)
endif()

add_subdirectory(vku)
if(BUILD_APIDUMP OR BUILD_MONITOR OR BUILD_SCREENSHOT)
    add_subdirectory(layersvt)
endif()

if(BUILD_LAYERMGR)
    add_subdirectory(vkconfig_core)
    add_subdirectory(vkconfig)
endif()
