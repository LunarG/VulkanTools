cmake_minimum_required (VERSION 2.8.11)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR -DVK_USE_PLATFORM_WIN32_KHX -DWIN32_LEAN_AND_MEAN)
    set(DisplayServer Win32)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR -DVK_USE_PLATFORM_ANDROID_KHX)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (BUILD_WSI_XCB_SUPPORT)
        add_definitions(-DVK_USE_PLATFORM_XCB_KHR -DVK_USE_PLATFORM_XCB_KHX)
    endif()

    if (BUILD_WSI_XLIB_SUPPORT)
       add_definitions(-DVK_USE_PLATFORM_XLIB_KHR -DVK_USE_PLATFORM_XLIB_KHX -DVK_USE_PLATFORM_XLIB_XRANDR_EXT)
    endif()

    if (BUILD_WSI_WAYLAND_SUPPORT)
       add_definitions(-DVK_USE_PLATFORM_WAYLAND_KHR -DVK_USE_PLATFORM_WAYLAND_KHX)
    endif()

    if (BUILD_WSI_MIR_SUPPORT)
        add_definitions(-DVK_USE_PLATFORM_MIR_KHR -DVK_USE_PLATFORM_MIR_KHX)
        include_directories(${MIR_INCLUDE_DIR})
    endif()
else()
    message(FATAL_ERROR "Unsupported Platform!")
endif()

set(LAYER_JSON_FILES VkLayer_layer_factory)

if (WIN32)
    if (CMAKE_GENERATOR MATCHES "^Visual Studio.*")
        foreach (config_file ${LAYER_JSON_FILES})
            FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/windows/${config_file}.json src_json)
            FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/${config_file}.json dst_json)
            add_custom_target(${config_file}-json ALL
                COMMAND copy ${src_json} ${dst_json}
                VERBATIM
                )
        endforeach(config_file)
    else()
        foreach (config_file ${LAYER_JSON_FILES})
            FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/windows/${config_file}.json src_json)
            FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/${config_file}.json dst_json)
            add_custom_target(${config_file}-json ALL
                COMMAND copy ${src_json} ${dst_json}
                VERBATIM
                )
        endforeach(config_file)
    endif()
else()
    # extra setup for out-of-tree builds
    if (NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR))
        foreach (config_file ${LAYER_JSON_FILES})
            add_custom_target(${config_file}-json ALL
                COMMAND ln -sf ${CMAKE_CURRENT_SOURCE_DIR}/linux/${config_file}.json
                VERBATIM
                )
        endforeach(config_file)
        # Add link to vk_validation_error_messages.h in build dir for scripts to pick up
        add_custom_target(vk_validation_error_messages ALL
            COMMAND ln -sf ${CMAKE_CURRENT_SOURCE_DIR}/vk_validation_error_messages.h
            VERBATIM
            )
    endif()
endif()
# If a layer has a direct dependency on a project with the same name, use it.
if ((Win32) OR (NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)))
    foreach (config_file ${LAYER_JSON_FILES})
        add_dependencies(${config_file}-json ${config_file})
    endforeach(config_file)
endif()

# Add targets for JSON file install on Linux.
# Need to remove the "./" from the library path before installing to /etc.
if(UNIX)
    if(INSTALL_LVL_FILES)
        foreach (config_file ${LAYER_JSON_FILES})
            add_custom_target(${config_file}-staging-json ALL
                COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/staging-json
                COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/linux/${config_file}.json ${CMAKE_CURRENT_BINARY_DIR}/staging-json
                COMMAND sed -i -e "/\"library_path\":/s$./libVkLayer$libVkLayer$" ${CMAKE_CURRENT_BINARY_DIR}/staging-json/${config_file}.json
                VERBATIM
                DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linux/${config_file}.json
                )
            install(FILES ${CMAKE_CURRENT_BINARY_DIR}/staging-json/${config_file}.json DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/vulkan/explicit_layer.d)
        endforeach(config_file)
    endif()
endif()

#if (WIN32)
#    macro(add_vk_layer target)
#    FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/VkLayer_${target}.def DEF_FILE)
#    add_custom_target(copy-${target}-def-file ALL
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DEF_FILE} VkLayer_${target}.def
#        VERBATIM
#    )
#    add_library(VkLayer_${target} SHARED ${ARGN} VkLayer_${target}.def)
#    add_dependencies(VkLayer_${target} generate_helper_files)
#    target_link_Libraries(VkLayer_${target} VkLayer_utils)
#    add_dependencies(VkLayer_${target} generate_helper_files VkLayer_utils)
#    endmacro()
#else()
#    macro(add_vk_layer target)
#    add_library(VkLayer_${target} SHARED ${ARGN})
#    target_link_Libraries(VkLayer_${target} VkLayer_utils)
#    add_dependencies(VkLayer_${target} generate_helper_files VkLayer_utils)
#    set_target_properties(VkLayer_${target} PROPERTIES LINK_FLAGS "-Wl,-Bsymbolic,--exclude-libs,ALL")
#    if(INSTALL_LVL_FILES)
#        install(TARGETS VkLayer_${target} DESTINATION ${CMAKE_INSTALL_LIBDIR})
#    endif()
#    endmacro()
#endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../loader
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/vulkan
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_PROJECT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
)

if (WIN32)
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_CRT_SECURE_NO_WARNINGS")
    set (CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -D_CRT_SECURE_NO_WARNINGS")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -D_CRT_SECURE_NO_WARNINGS")
    set (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -D_CRT_SECURE_NO_WARNINGS")
    set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -D_CRT_SECURE_NO_WARNINGS /bigobj")
    set (CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -D_CRT_SECURE_NO_WARNINGS /bigobj")
    # Turn off transitional "changed behavior" warning message for Visual Studio versions prior to 2015.
    # The changed behavior is that constructor initializers are now fixed to clear the struct members.
    add_compile_options("$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,19>>:/wd4351>")
else()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpointer-arith -Wno-unused-function -Wno-sign-compare")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpointer-arith -Wno-unused-function -Wno-sign-compare")
endif()

run_vk_xml_generate(layer_factory_generator.py layer_factory.h)
run_vk_xml_generate(layer_factory_generator.py layer_factory.cpp)

add_dependencies(VkLayer_utils generate_helper_files)

set (CMAKE_LAYER_FACTORY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/layer_factory)
file(GLOB LAYER_FACTORY_SOURCES ${CMAKE_LAYER_FACTORY_SOURCE_DIR}/*.h ${CMAKE_LAYER_FACTORY_SOURCE_DIR}/*.cpp)

add_vk_layer(layer_factory layer_factory.cpp layer_factory.h ../vk_layer_table.cpp ${LAYER_FACTORY_SOURCES})
