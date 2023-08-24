file(GLOB TEXTURES
   "${PROJECT_SOURCE_DIR}/via/images/*"
)
file(COPY ${TEXTURES} DESTINATION ${CMAKE_BINARY_DIR}/via/images)

if(WIN32)

    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES")
    add_definitions(-DVIA_WINDOWS_TARGET)

else()
    if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers")
        set(COMMON_COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} -fno-strict-aliasing -fno-builtin-memcmp")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_FLAGS} -fno-rtti")
    endif()
    # jsoncpp has intentional fallthrough
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_FLAGS} -std=c++11 -Wno-implicit-fallthrough")

    if(APPLE)
        add_definitions(-DVIA_MACOS_TARGET)
    elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
        add_definitions(-DVIA_LINUX_TARGET)
    elseif(CMAKE_SYSTEM_NAME MATCHES "BSD")
	    add_definitions(-DVIA_BSD_TARGET)
    endif()

    # Disable the RPATH for VIA because we want it to use
    # the environment setup by the user
    SET(CMAKE_SKIP_BUILD_RPATH  TRUE)
endif()

if(WIN32)
add_executable(vkvia
                  via.cpp
                  via_system.hpp
                  via_system.cpp
                  via_system_windows.hpp
                  via_system_windows.cpp)
elseif(APPLE)
add_executable(vkvia
                  via.cpp
                  via_system.hpp
                  via_system.cpp
                  via_system_macos.hpp
                  via_system_macos.cpp)
elseif(UNIX)
add_executable(vkvia
                  via.cpp
                  via_system.hpp
                  via_system.cpp
                  via_system_linux.hpp
                  via_system_linux.cpp
                  via_system_bsd.hpp
                  via_system_bsd.cpp)
endif()

target_link_libraries(vkvia PRIVATE
    Vulkan::Headers
    Vulkan::Vulkan
    jsoncpp_static
    valijson
    ${CMAKE_DL_LIBS}
    $<TARGET_NAME_IF_EXISTS:PkgConfig::XCB>
    $<TARGET_NAME_IF_EXISTS:PkgConfig::X11>
    $<TARGET_NAME_IF_EXISTS:PkgConfig::WAYlAND_CLIENT>
)

install(TARGETS vkvia)
