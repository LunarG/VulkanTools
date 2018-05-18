file(GLOB TEXTURES
   "${PROJECT_SOURCE_DIR}/via/images/*"
)
file(COPY ${TEXTURES} DESTINATION ${CMAKE_BINARY_DIR}/via/images)


if(DEFINED LOADER_REPO_ROOT AND NOT LOADER_REPO_ROOT STREQUAL "NOT_DEFINED")
    message(STATUS "Using user-supplied path to locate Vulkan")
    if(WIN32)
        set (LOADER_SEARCH_PATHS
            "${LOADER_REPO_ROOT}/${BUILDTGT_DIR}/loader/${DEBUG_DECORATION}"
            "${LOADER_REPO_ROOT}/${BUILDTGT_DIR}/loader/${RELEASE_DECORATION}"
            )
    elseif(UNIX)
        set (LOADER_SEARCH_PATHS
            "${LOADER_REPO_ROOT}/${BUILDTGT_DIR}/loader"
            )
    endif()
    find_library(LIBVK NAMES vulkan vulkan-1
        HINTS ${LOADER_SEARCH_PATHS}
        )
    message(STATUS "Found Vulkan: ${LIBVK}")
else()
    message(STATUS "Using find_package to locate Vulkan")
    find_package(Vulkan)
    set (LIBVK "Vulkan::Vulkan")
endif()

if(WIN32)
    # For Windows, since 32-bit and 64-bit items can co-exist, we build each in its own build directory.
    # 32-bit target data goes in build32, and 64-bit target data goes into build.  So, include/link the
    # appropriate data at build time.
    if (CMAKE_CL_64)
        set (BUILDTGT_DIR build)
    else ()
        set (BUILDTGT_DIR build32)
    endif()

    # Use static MSVCRT libraries
    foreach(configuration in CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO
                             CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${configuration} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${configuration} "${${configuration}}")
        endif()
    endforeach()

    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES")

else()

    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    if (BUILD_WSI_XCB_SUPPORT)
        find_package(XCB REQUIRED)

        include_directories(${XCB_INCLUDE_DIRS})
        link_libraries(${XCB_LIBRARIES})
    endif()
    if (BUILD_WSI_XLIB_SUPPORT)
        find_package(X11 REQUIRED)

        include_directories(${X11_INCLUDE_DIRS})
        link_libraries(${X11_LIBRARIES})
    endif()
    if (BUILD_WSI_WAYLAND_SUPPORT)
        find_package(Wayland REQUIRED)

        include_directories(${WAYLAND_CLIENT_INCLUDE_DIR})
        link_libraries(${WAYLAND_CLIENT_LIBRARIES})
    endif()

    # Disable the RPATH for VIA because we want it to use
    # the environment setup by the user
    SET(CMAKE_SKIP_BUILD_RPATH  TRUE)

    link_libraries(${API_LOWERCASE} m)

endif()

add_executable(via via.cpp ${JSONCPP_SOURCE_DIR}/jsoncpp.cpp)
target_include_directories(via PUBLIC ${JSONCPP_INCLUDE_DIR})
target_link_libraries(via ${LIBVK} ${LIBRARIES})
if(WIN32)
    target_link_libraries(via version shlwapi Cfgmgr32)
endif()
if(UNIX)
    install(TARGETS via DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
