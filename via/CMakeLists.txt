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

# Needed for VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
target_compile_definitions(vkvia PRIVATE VK_ENABLE_BETA_EXTENSIONS)

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

    if (APPLE)
        # vkvia currently uses sprintf which is deprecated for security reasons
        target_compile_options(vkvia PRIVATE -Wno-deprecated-declarations)
    endif()

elseif (MSVC)
    target_compile_definitions(vkvia PRIVATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
endif()

# NOTE: jsoncpp hasn't had updates in a bit and isn't getting fixes.
# may be worth using nlohmann/json in the future which gets more updates
# and has better CMake support.
find_package(jsoncpp CONFIG)
if (TARGET jsoncpp_static)
    target_link_libraries(vkvia PRIVATE jsoncpp_static)
    
# Support using jsoncpp.pc but only for UNIX platforms.
# And only if UPDATE_DEPS is disabled.
elseif (UNIX AND NOT UPDATE_DEPS)

    # https://github.com/LunarG/VulkanTools/issues/1908
    find_package(PkgConfig)
    if (PKG_CONFIG_FOUND)
        pkg_check_modules(jsoncpp REQUIRED IMPORTED_TARGET jsoncpp)
        target_link_libraries(vkvia PRIVATE PkgConfig::jsoncpp)
    endif()

endif()

target_link_libraries(vkvia PRIVATE
    Vulkan::Headers
    valijson
    ${CMAKE_DL_LIBS}
    $<TARGET_NAME_IF_EXISTS:Vulkan::Loader>
)

install(TARGETS vkvia)
