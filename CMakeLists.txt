cmake_minimum_required(VERSION 3.10.2)

enable_testing()

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")

project (VULKAN_TOOLS)

set(API_NAME "Vulkan" CACHE STRING "API name")
add_definitions(-DAPI_NAME="${API_NAME}")

add_definitions(-DVK_ENABLE_BETA_EXTENSIONS)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Define GNU standard installation directories.
include(GNUInstallDirs)

# Set a better default install location for Windows only if the user did not provide one.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND WIN32)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "default install path" FORCE)
endif()

# Find Vulkan Headers, Loader, and ValidationLayers
# Search order:
#  User-supplied CMAKE_PREFIX_PATH containing paths to the header and/or loader install dirs
#  CMake options VULKAN_HEADERS_INSTALL_DIR, VULKAN_LOADER_INSTALL_DIR, and/or VULKAN_VALIDATIONLAYERS_INSTALL_DIR
#  Env vars VULKAN_HEADERS_INSTALL_DIR, VULKAN_LOADER_INSTALL_DIR, and/or VULKAN_VALIDATIONLAYERS_INSTALL_DIR
#  Fallback to FindVulkan operation using SDK install or system installed components.
set(VULKAN_HEADERS_INSTALL_DIR "HEADERS-NOTFOUND" CACHE PATH "Absolute path to a Vulkan-Headers install directory")
set(VULKAN_LOADER_INSTALL_DIR "LOADER-NOTFOUND" CACHE PATH "Absolute path to a Vulkan-Loader install directory")
if(WIN32 AND "${VULKAN_LOADER_INSTALL_DIR}" STREQUAL "LOADER-NOTFOUND")
    if(CMAKE_CL_64)
        set(VULKAN_LOADER_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/x64")
    else()
        set(VULKAN_LOADER_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/x86")
    endif()
endif()

set(VULKAN_VALIDATIONLAYERS_INSTALL_DIR "VALIDATIONLAYERS-NOTFOUND" CACHE PATH "Absolute path to Vulkan-ValidationLayers directory")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${VULKAN_HEADERS_INSTALL_DIR};${VULKAN_LOADER_INSTALL_DIR};${VULKAN_VALIDATIONLAYERS_INSTALL_DIR};$ENV{VULKAN_HEADERS_INSTALL_DIR};$ENV{VULKAN_LOADER_INSTALL_DIR};$ENV{VULKAN_VALIDATIONLAYERS_INSTALL_DIR})
message(STATUS "Using find_package to locate Vulkan")
find_package(Vulkan)
message(STATUS "Vulkan FOUND = ${Vulkan_FOUND}")
message(STATUS "Vulkan Include = ${Vulkan_INCLUDE_DIR}")
message(STATUS "Vulkan Lib = ${Vulkan_LIBRARY}")

if(NOT VULKAN_HEADERS_INSTALL_DIR AND NOT DEFINED ENV{VULKAN_HEADERS_INSTALL_DIR})
    message(FATAL_ERROR "Must define location of Vulkan-Headers install -- see BUILD.md")
endif()

# Cmake command line option overrides environment variable
if(NOT VULKAN_HEADERS_INSTALL_DIR)
    set(VULKAN_HEADERS_INSTALL_DIR $ENV{VULKAN_HEADERS_INSTALL_DIR})
endif()
message(STATUS "Using Vulkan-Headers install located at ${VULKAN_HEADERS_INSTALL_DIR}")

if(NOT VULKAN_LOADER_INSTALL_DIR AND NOT DEFINED ENV{VULKAN_LOADER_INSTALL_DIR})
    message(FATAL_ERROR "Must define location of Vulkan-Loader install -- see BUILD.md")
endif()

# Cmake command line option overrides environment variable
if(NOT VULKAN_LOADER_INSTALL_DIR)
    set(VULKAN_LOADER_INSTALL_DIR $ENV{VULKAN_LOADER_INSTALL_DIR})
endif()
message(STATUS "Using Vulkan-Loader install located at ${VULKAN_LOADER_INSTALL_DIR}")

if(NOT VULKAN_VALIDATIONLAYERS_INSTALL_DIR AND NOT DEFINED ENV{VULKAN_VALIDATIONLAYERS_INSTALL_DIR})
    message(FATAL_ERROR "Must define location of Vulkan-ValidationLayers install -- see BUILD.md")
endif()

# Cmake command line option overrides environment variable
if(NOT VULKAN_VALIDATIONLAYERS_INSTALL_DIR)
    set(VULKAN_VALIDATIONLAYERS_INSTALL_DIR $ENV{VULKAN_VALIDATIONLAYERS_INSTALL_DIR})
endif()
message(STATUS "Using Vulkan-ValidationLayers install located at ${VULKAN_VALIDATIONLAYERS_INSTALL_DIR}")

file(TO_CMAKE_PATH "${VULKAN_VALIDATIONLAYERS_INSTALL_DIR}" VULKAN_VALIDATIONLAYERS_INSTALL_DIR)
set(Vulkan-ValidationLayers_INCLUDE_DIR "${VULKAN_VALIDATIONLAYERS_INSTALL_DIR}/include/vulkan")
set(Vulkan-ValidationLayers_LIBRARY_DIR "${VULKAN_VALIDATIONLAYERS_INSTALL_DIR}/lib")
find_library(VkLayer_utils_LIBRARY VkLayer_utils HINTS ${Vulkan-ValidationLayers_LIBRARY_DIR})

# Check for python 3.6 or greater
# Cannot use find_package(PythonInterp 3.6 REQUIRED) because it isn't smart enough to ignore an older /usr/bin/python3
# Cannot use FindPython3 because it isn't available in CMake 3.10
# This will go away when we start committing the generated files to this repo
foreach(PYTHON_MIN_VERSION RANGE 6 20)
    find_package(PythonInterp "3.${PYTHON_MIN_VERSION}" EXACT)
    if(PythonInterp_FOUND)
        set (PYTHON_CMD ${PYTHON_EXECUTABLE})
        break()
    endif()
    unset(PYTHON_EXECUTABLE CACHE)
endforeach(PYTHON_MIN_VERSION)
if(NOT PythonInterp_FOUND)
    message(FATAL_ERROR "Could not find Python >= 3.6")
endif()

# Enable cmake folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(VULKANTOOLS_TARGET_FOLDER vt_cmake_targets)

option(USE_CCACHE "Use ccache" OFF)
if (USE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    endif(CCACHE_FOUND)
endif()

option(BUILD_WERROR "Treat compiler warnings as errors" ON)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME MATCHES "BSD")
    set(FALLBACK_CONFIG_DIRS "/etc/xdg" CACHE STRING
        "Search path to use when XDG_CONFIG_DIRS is unset or empty or the current process is SUID/SGID. Default is freedesktop compliant.")
    set(FALLBACK_DATA_DIRS "/usr/local/share:/usr/share" CACHE STRING
        "Search path to use when XDG_DATA_DIRS is unset or empty or the current process is SUID/SGID. Default is freedesktop compliant.")

    if(BUILD_WERROR)
        if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
            add_compile_options(-Werror)
        endif()
    endif()

    include(FindPkgConfig)
    option(BUILD_WSI_XCB_SUPPORT "Build XCB WSI support" ON)
    option(BUILD_WSI_XLIB_SUPPORT "Build Xlib WSI support" ON)
    option(BUILD_WSI_WAYLAND_SUPPORT "Build Wayland WSI support" ON)

    if (BUILD_WSI_XCB_SUPPORT)
        find_package(XCB REQUIRED)
    endif()

    if (BUILD_WSI_XLIB_SUPPORT)
        find_package(X11 REQUIRED)
    endif()

    if (BUILD_WSI_WAYLAND_SUPPORT)
        find_package(Wayland REQUIRED)
        include_directories(${WAYLAND_CLIENT_INCLUDE_DIR})
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    option(BUILD_MOLTENVK_SUPPORT "Build MoltenVK WSI support" ON)
    option(BUILD_METAL_SUPPORT "Build Metal WSI support" OFF)
endif()

set (VULKAN_SDK $ENV{VULKAN_SDK})

# NOTE: jsoncpp is located in SUBPROJECTS_DIR.
set (SUBPROJECTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/submodules)

set(VULKANTOOLS_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/scripts)

# Header file for CMake settings
find_package(VulkanHeaders)
include_directories(${VulkanHeaders_INCLUDE_DIR} ${PROJECT_BINARY_DIR}/${VALIDATION_LAYERS_RELATIVE_LOCATION})

# The vulkan loader search is:
#     User-supplied setting of CMAKE_PREFIX_PATH
#     VULKAN_LOADER_INSTALL_DIR defined via cmake option
#     VULKAN_LOADER_INSTALL_DIR defined via environment variable
#     Default findVulkan operation if the VULKAN_SDK environment variable is defined
set(VULKAN_LOADER_INSTALL_DIR "LOADER-NOTFOUND" CACHE PATH "Absolute path to a Vulkan-Loader install directory")
if (VULKAN_LOADER_INSTALL_DIR)
    message(STATUS "VULKAN_LOADER_INSTALL_DIR specified, using find_package to locate Vulkan")
elseif(ENV{VULKAN_LOADER_INSTALL_DIR})
    message(STATUS "VULKAN_LOADER_INSTALL_DIR environment variable specified, using find_package to locate Vulkan")
endif()
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${VULKAN_LOADER_INSTALL_DIR};${VULKAN_HEADERS_INSTALL_DIR};$ENV{VULKAN_LOADER_INSTALL_DIR};$ENV{VULKAN_HEADERS_INSTALL_DIR})
find_package(Vulkan)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows" OR
    CMAKE_SYSTEM_NAME STREQUAL "Linux" OR
    CMAKE_SYSTEM_NAME MATCHES "BSD")

    option(BUILD_TESTS "Build tests" ON)
    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_MONITOR "Build monitor layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    option(BUILD_VLF "Build layer_factory" ON)
    option(BUILD_VIA "Build via" ON)

    option(BUILD_LAYERMGR "Build Vulkan Configurator" ON)

elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")

    option(BUILD_LOADER OFF)
    option(BUILD_TESTS OFF)
    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_MONITOR "Build monitor layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    option(BUILD_VLF OFF)
    option(BUILD_DEMOS OFF)
    option(BUILD_VKJSON OFF)
    option(BUILD_VIA "Build VIA" ON)
    option(BUILD_LAYERMGR "Build Vulkan Configurator" ON)

endif()

find_path(JSONCPP_INCLUDE_DIR json/json.h HINTS "${SUBPROJECTS_DIR}/jsoncpp/dist"
                                                   "${SUBPROJECTS_DIR}/JsonCpp/dist"
                                                   "${SUBPROJECTS_DIR}/JsonCPP/dist"
                                                   "${SUBPROJECTS_DIR}/JSONCPP/dist"
                                                   "${CMAKE_SOURCE_DIR}/../jsoncpp/dist"
                                             DOC "Path to jsoncpp/dist/json/json.h")

find_path(JSONCPP_SOURCE_DIR jsoncpp.cpp HINTS "${SUBPROJECTS_DIR}/jsoncpp/dist"
                                                   "${SUBPROJECTS_DIR}/JsonCpp/dist"
                                                   "${SUBPROJECTS_DIR}/JsonCPP/dist"
                                                   "${SUBPROJECTS_DIR}/JSONCPP/dist"
                                                   "${CMAKE_SOURCE_DIR}/../jsoncpp/dist"
                                             DOC "Path to jsoncpp/dist/jsoncpp.cpp")

find_library(JSONCPP_LIB NAMES jsoncpp HINTS ${JSONCPP_SEARCH_PATH} )

if (WIN32)
    add_library(jsoncpp         STATIC IMPORTED)
    find_library(JSONCPP_DLIB NAMES jsoncpp
                 HINTS ${JSONCPP_DEBUG_SEARCH_PATH} )
    set_target_properties(jsoncpp PROPERTIES
                         IMPORTED_LOCATION       "${JSONCPP_LIB}"
                         IMPORTED_LOCATION_DEBUG "${JSONCPP_DLIB}")
endif()

# Define macro used for building vkxml generated files
macro(run_vulkantools_vk_xml_generate dependency output)
    add_custom_command(OUTPUT ${output}
    COMMAND ${PYTHON_CMD} -B ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VulkanRegistry_DIR}/vk.xml -scripts ${VulkanRegistry_DIR} ${output}
        -removeExtensions VK_KHR_video_queue -removeExtensions VK_KHR_video_decode_queue -removeExtensions VK_KHR_video_encode_queue -removeExtensions VK_EXT_video_decode_h264
        -removeExtensions VK_EXT_video_decode_h265 -removeExtensions VK_EXT_video_encode_h264 -removeExtensions VK_EXT_video_encode_h265
    DEPENDS ${VulkanRegistry_DIR}/vk.xml ${VulkanRegistry_DIR}/generator.py ${VULKANTOOLS_SCRIPTS_DIR}/${dependency} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py ${VulkanRegistry_DIR}/reg.py
    )
endmacro()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_VLF)
    add_subdirectory(layer_factory)
endif()

if(BUILD_VIA)
    add_subdirectory(via)
endif()

add_subdirectory(vku)
if(BUILD_APIDUMP OR BUILD_MONITOR OR BUILD_SCREENSHOT)
    add_subdirectory(layersvt)
endif()

if(BUILD_LAYERMGR)
    include_directories(external)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(external/googletest)

    add_subdirectory(vkconfig_core)
    add_subdirectory(vkconfig)
endif()

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
