cmake_minimum_required(VERSION 3.17.2)

project(VULKAN_TOOLS)

set(API_NAME "Vulkan" CACHE STRING "API name")
add_definitions(-DAPI_NAME="${API_NAME}")

add_definitions(-DVK_ENABLE_BETA_EXTENSIONS)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${VULKAN_HEADERS_INSTALL_DIR};${VULKAN_LOADER_INSTALL_DIR};${VULKAN_VALIDATIONLAYERS_INSTALL_DIR};${JSONCPP_INSTALL_DIR})

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

set(VULKANTOOLS_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/scripts)

find_package(VulkanHeaders REQUIRED CONFIG)

find_library(Vulkan_LIBRARY NAMES vulkan vulkan-1)
add_library(Vulkan::Vulkan UNKNOWN IMPORTED)
set_target_properties(Vulkan::Vulkan PROPERTIES
    IMPORTED_LOCATION ${Vulkan_LIBRARY})
target_link_libraries(Vulkan::Vulkan INTERFACE Vulkan::Headers)

find_package(jsoncpp REQUIRED CONFIG)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows" OR
    CMAKE_SYSTEM_NAME STREQUAL "Linux" OR
    CMAKE_SYSTEM_NAME MATCHES "BSD")

    option(BUILD_TESTS "Build tests" ON)
    option(BUILD_APIDUMP "Build api_dump layer" ON)
    option(BUILD_MONITOR "Build monitor layer" ON)
    option(BUILD_SCREENSHOT "Build screenshot layer" ON)
    option(BUILD_VIA "Build via" ON)

    option(BUILD_LAYERMGR "Build Vulkan Configurator" ON)

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

# Define macro used for building vk.xml generated files
function(run_vulkantools_vk_xml_generate dependency output)
    add_custom_command(OUTPUT ${output}
        COMMAND ${PYTHON_CMD} -B ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/vk.xml -scripts ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry ${output}
        DEPENDS ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/vk.xml ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/generator.py ${VULKANTOOLS_SCRIPTS_DIR}/${dependency} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/reg.py
    )
endfunction()

# Define macro used for building video.xml generated files
function(run_vulkantools_video_xml_generate dependency output)
    add_custom_command(OUTPUT ${output}
        COMMAND ${PYTHON_CMD} -B ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/video.xml -scripts ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry ${output}
        DEPENDS ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/video.xml ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/generator.py ${VULKANTOOLS_SCRIPTS_DIR}/${dependency} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/reg.py
    )
endfunction()

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
    include_directories(external)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(external/googletest)

    add_subdirectory(vkconfig_core)
    add_subdirectory(vkconfig)
endif()
