if (MINGW)
    return()
endif()

file(GLOB TEXTURES "${PROJECT_SOURCE_DIR}/via/images/*")
file(COPY ${TEXTURES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/images)

add_executable(vkvia)

target_sources(vkvia PRIVATE
    via.cpp
    via_system.hpp
    via_system.cpp
)

if(WIN32)
    target_sources(vkvia PRIVATE
        via_system_windows.hpp
        via_system_windows.cpp
    )
elseif(APPLE)
    target_sources(vkvia PRIVATE
        via_system_macos.hpp
        via_system_macos.cpp
    )
elseif(UNIX)
    target_sources(vkvia PRIVATE
        via_system_linux.hpp
        via_system_linux.cpp
        via_system_bsd.hpp
        via_system_bsd.cpp
    )
endif()

if (WIN32)
    target_compile_definitions(vkvia PRIVATE VIA_WINDOWS_TARGET)
elseif(APPLE)
    target_compile_definitions(vkvia PRIVATE VIA_MACOS_TARGET)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_compile_definitions(vkvia PRIVATE VIA_LINUX_TARGET)
elseif(CMAKE_SYSTEM_NAME MATCHES "BSD")
    target_compile_definitions(vkvia PRIVATE VIA_BSD_TARGET)
endif()

# Disable the RPATH for VIA because we want it to use the environment setup by the user
set_target_properties(vkvia PROPERTIES SKIP_BUILD_RPATH TRUE)

if(${CMAKE_CXX_COMPILER_ID} MATCHES "(GNU|Clang)")

    target_compile_options(vkvia PRIVATE
        -Wall
        -Wextra
        -Wno-unused-parameter
        -Wno-missing-field-initializers
        -fno-strict-aliasing
        -fno-builtin-memcmp
        -fno-rtti
    )

elseif (MSVC)
    target_compile_definitions(vkvia PRIVATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
endif()

# https://github.com/LunarG/VulkanTools/issues/1908
find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(jsoncpp REQUIRED IMPORTED_TARGET jsoncpp)
    target_link_libraries(vkvia PRIVATE PkgConfig::jsoncpp)
else()
    find_package(jsoncpp CONFIG)
    target_link_libraries(vkvia PRIVATE jsoncpp_static)
endif()

target_link_libraries(vkvia PRIVATE
    Vulkan::Headers
    valijson
    ${CMAKE_DL_LIBS}
    $<TARGET_NAME_IF_EXISTS:Vulkan::Loader>
    $<TARGET_NAME_IF_EXISTS:PkgConfig::XCB>
    $<TARGET_NAME_IF_EXISTS:PkgConfig::X11>
    $<TARGET_NAME_IF_EXISTS:PkgConfig::WAYlAND_CLIENT>
)

install(TARGETS vkvia)
