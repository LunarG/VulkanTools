cmake_minimum_required(VERSION 3.17.2)

project(VULKAN_TOOLS)

add_subdirectory(scripts)

set(API_NAME "Vulkan" CACHE STRING "API name")
add_definitions(-DAPI_NAME="${API_NAME}")

add_definitions(-DVK_ENABLE_BETA_EXTENSIONS)

include(GNUInstallDirs)

set(Vulkan-ValidationLayers_INCLUDE_DIR "${VULKAN_VALIDATIONLAYERS_INSTALL_DIR}/include/vulkan")
set(Vulkan-ValidationLayers_LIBRARY_DIR "${VULKAN_VALIDATIONLAYERS_INSTALL_DIR}/lib")
find_library(VkLayer_utils_LIBRARY VkLayer_utils HINTS ${Vulkan-ValidationLayers_LIBRARY_DIR})

# Enable cmake folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(VULKANTOOLS_TARGET_FOLDER vt_cmake_targets)

option(BUILD_WERROR "Treat compiler warnings as errors" ON)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME MATCHES "BSD")
    if(BUILD_WERROR)
        if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
            add_compile_options(-Werror)
        endif()
    endif()

    option(BUILD_WSI_XCB_SUPPORT "Build XCB WSI support" ON)
    option(BUILD_WSI_XLIB_SUPPORT "Build Xlib WSI support" ON)
    option(BUILD_WSI_WAYLAND_SUPPORT "Build Wayland WSI support" ON)

    find_package(PkgConfig REQUIRED QUIET) # Use PkgConfig to find Linux system libraries

    if (BUILD_WSI_XCB_SUPPORT)
        pkg_check_modules(XCB REQUIRED QUIET IMPORTED_TARGET xcb)
    endif()

    if (BUILD_WSI_XLIB_SUPPORT)
        pkg_check_modules(X11 REQUIRED QUIET IMPORTED_TARGET x11)
    endif()

    if (BUILD_WSI_WAYLAND_SUPPORT)
        pkg_check_modules(WAYLAND_CLIENT REQUIRED QUIET IMPORTED_TARGET wayland-client)
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    option(BUILD_MOLTENVK_SUPPORT "Build MoltenVK WSI support" ON)
    option(BUILD_METAL_SUPPORT "Build Metal WSI support" OFF)
endif()

find_package(VulkanHeaders REQUIRED CONFIG)

find_library(Vulkan_LIBRARY NAMES vulkan vulkan-1)
add_library(Vulkan::Vulkan UNKNOWN IMPORTED)
set_target_properties(Vulkan::Vulkan PROPERTIES
    IMPORTED_LOCATION ${Vulkan_LIBRARY})
target_link_libraries(Vulkan::Vulkan INTERFACE Vulkan::Headers)

find_package(valijson REQUIRED CONFIG)

find_package(jsoncpp REQUIRED CONFIG)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows" OR
    CMAKE_SYSTEM_NAME STREQUAL "Linux" OR
    CMAKE_SYSTEM_NAME MATCHES "BSD")

    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_MONITOR "Build monitor layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    option(BUILD_VIA "Build via" ON)

    option(BUILD_LAYERMGR "Build Vulkan Configurator" ON)

elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")

    option(BUILD_LOADER OFF)
    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_MONITOR "Build monitor layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    option(BUILD_DEMOS OFF)
    option(BUILD_VKJSON OFF)
    option(BUILD_VIA "Build VIA" ON)
    option(BUILD_LAYERMGR "Build Vulkan Configurator" ON)

endif()

option(BUILD_TESTS "Build tests")
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)

    find_package(GTest REQUIRED CONFIG)
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
