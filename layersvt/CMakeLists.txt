cmake_minimum_required (VERSION 2.8.11)

add_custom_target( generate_api_cpp DEPENDS api_dump.cpp )
add_custom_target( generate_api_h DEPENDS api_dump_text.h )

macro(run_vk_helper subcmd)
    add_custom_command(OUTPUT ${ARGN}
        COMMAND ${PYTHON_CMD} ${SCRIPTS_DIR}/vk_helper.py --${subcmd} ${PROJECT_SOURCE_DIR}/include/vulkan/vulkan.h --abs_out_dir ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${SCRIPTS_DIR}/vk_helper.py ${PROJECT_SOURCE_DIR}/include/vulkan/vulkan.h
    )
endmacro()

## VulkanTools has its own layer generator script
macro(run_vk_vtlayer_generate subcmd output)
    add_custom_command(OUTPUT ${output}
        COMMAND ${PYTHON_CMD} ${SCRIPTS_DIR}/vk-vtlayer-generate.py ${DisplayServer} ${subcmd} ${PROJECT_SOURCE_DIR}/include/vulkan/vulkan.h > ${output}
        DEPENDS ${SCRIPTS_DIR}/vk-vtlayer-generate.py ${PROJECT_SOURCE_DIR}/include/vulkan/vulkan.h ${SCRIPTS_DIR}/vulkan.py
    )
endmacro()

macro(run_vk_layer_xml_generate subcmd output)
    add_custom_command(OUTPUT ${output}
        COMMAND ${PYTHON_CMD} ${SCRIPTS_DIR}/vt_genvk.py -registry ${SCRIPTS_DIR}/vk.xml ${output}
        DEPENDS ${SCRIPTS_DIR}/vk.xml ${SCRIPTS_DIR}/generator.py ${SCRIPTS_DIR}/vt_genvk.py ${SCRIPTS_DIR}/reg.py
    )
endmacro()

set(LAYER_JSON_FILES
    VkLayer_api_dump
    VkLayer_basic
    VkLayer_generic
    VkLayer_multi
    VkLayer_screenshot
    )

set(VK_LAYER_RPATH /usr/lib/x86_64-linux-gnu/vulkan/layer:/usr/lib/i386-linux-gnu/vulkan/layer)
set(CMAKE_INSTALL_RPATH ${VK_LAYER_RPATH})

if (WIN32)
    if (NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR))
        if (CMAKE_GENERATOR MATCHES "^Visual Studio.*")
            foreach (config_file ${LAYER_JSON_FILES})
                FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/windows/${config_file}.json src_json)
                FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/${config_file}.json dst_json)
                add_custom_target(${config_file}-json ALL
                    COMMAND copy ${src_json} ${dst_json}
                    VERBATIM
                    )
                add_dependencies(${config_file}-json ${config_file})
            endforeach(config_file)
        else()
            foreach (config_file ${LAYER_JSON_FILES})
                FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/windows/${config_file}.json src_json)
                FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/${config_file}.json dst_json)
                add_custom_target(${config_file}-json ALL
                    COMMAND copy ${src_json} ${dst_json}
                    VERBATIM
                    )
                add_dependencies(${config_file}-json ${config_file})
            endforeach(config_file)
        endif()
    endif()
else()
    # extra setup for out-of-tree builds
    if (NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR))
        foreach (config_file ${LAYER_JSON_FILES})
            add_custom_target(${config_file}-json ALL
                COMMAND ln -sf ${CMAKE_CURRENT_SOURCE_DIR}/linux/${config_file}.json
                VERBATIM
                )
             add_dependencies(${config_file}-json ${config_file})
        endforeach(config_file)
    endif()
endif()

if (WIN32)
    macro(add_vk_layer target)
    add_custom_command(OUTPUT VkLayer_${target}.def
        COMMAND ${PYTHON_CMD} ${SCRIPTS_DIR}/vk-generate.py ${DisplayServer} win-def-file VkLayer_${target} layer > VkLayer_${target}.def
        DEPENDS ${SCRIPTS_DIR}/vk-generate.py ${SCRIPTS_DIR}/vulkan.py generate_api_cpp generate_api_h
    )
    add_library(VkLayer_${target} SHARED ${ARGN} VkLayer_${target}.def)
    target_link_Libraries(VkLayer_${target} VkLayer_utilsvt)
    add_dependencies(VkLayer_${target} generate_vt_helpers)
    endmacro()
else()
    macro(add_vk_layer target)
    add_library(VkLayer_${target} SHARED ${ARGN})
    target_link_Libraries(VkLayer_${target} VkLayer_utilsvt)
    add_dependencies(VkLayer_${target} generate_vt_helpers generate_api_cpp generate_api_h)
    set_target_properties(VkLayer_${target} PROPERTIES LINK_FLAGS "-Wl,-Bsymbolic")
    install(TARGETS VkLayer_${target} DESTINATION ${PROJECT_BINARY_DIR}/install_staging)
    endmacro()
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../loader
    ${CMAKE_CURRENT_SOURCE_DIR}/../layers
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/vulkan
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/../glslang/SPIRV
)

if (WIN32)
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_CRT_SECURE_NO_WARNINGS")
    set (CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -D_CRT_SECURE_NO_WARNINGS")

    # For VS 2015, which uses compiler version 1900, core_validation.cpp fails with too many objects
    # without either optimizations enabled, or setting the /bigobj compilation option.  Since
    # optimizations are enabled in a release build, this only affects the debug build.  For now,
    # enable /bigobj mode for all debug layer files. An alternative for the future is to split
    # large source files into multiple files which will also alleviate the compilation error.
    if (MSVC AND NOT (MSVC_VERSION LESS 1900))
        set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -D_CRT_SECURE_NO_WARNINGS /bigobj")
        set (CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -D_CRT_SECURE_NO_WARNINGS /bigobj")
    else()
        set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -D_CRT_SECURE_NO_WARNINGS")
        set (CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -D_CRT_SECURE_NO_WARNINGS")
    endif()
else()
    set (CMAKE_CXX_FLAGS "-std=c++11")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpointer-arith")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpointer-arith")
endif()

add_custom_command(OUTPUT vk_dispatch_table_helper.h
    COMMAND ${PYTHON_CMD} ${SCRIPTS_DIR}/vk-generate.py ${DisplayServer} dispatch-table-ops layer > vk_dispatch_table_helper.h
    DEPENDS ${SCRIPTS_DIR}/vk-generate.py ${SCRIPTS_DIR}/vulkan.py)

run_vk_helper(gen_enum_string_helper vk_enum_string_helper.h)
run_vk_helper(gen_struct_wrappers
    vk_struct_string_helper.h
    vk_struct_string_helper_cpp.h
    vk_struct_string_helper_no_addr.h
    vk_struct_string_helper_no_addr_cpp.h
    vk_struct_size_helper.h
    vk_struct_size_helper.c
    vk_struct_wrappers.h
    vk_struct_wrappers.cpp
    vk_safe_struct.h
# Don't list vk_safe_struct.cpp as OUTPUT to avoid duplicate builds.
# If listed here use of it for add_library will cause it to be created
# independently of custom target generate_vk_layer_helpers .
# That breaks parallel builds.
#   vk_safe_struct.cpp
)

# Let gen_struct_wrappers really create vk_safe_struct.cpp
add_custom_command(OUTPUT vk_safe_struct.cpp
    COMMAND echo defer making vk_safe_struct.cpp
)

set_source_files_properties(
    vk_struct_string_helper.h
    vk_struct_string_helper_cpp.h
    vk_struct_string_helper_no_addr.h
    vk_struct_string_helper_no_addr_cpp.h
    vk_struct_size_helper.h
    vk_struct_size_helper.c
    vk_struct_wrappers.h
    vk_struct_wrappers.cpp
    vk_safe_struct.h
    vk_safe_struct.cpp
    PROPERTIES GENERATED TRUE)

add_custom_target(generate_vt_helpers DEPENDS
    vk_dispatch_table_helper.h
    vk_enum_string_helper.h
    vk_struct_string_helper.h
    vk_struct_string_helper_no_addr.h
    vk_struct_string_helper_cpp.h
    vk_struct_string_helper_no_addr_cpp.h
    vk_struct_size_helper.h
    vk_struct_size_helper.c
    vk_struct_wrappers.h
    vk_struct_wrappers.cpp
    vk_safe_struct.h
    vk_safe_struct.cpp
)

#VulkanTools layers
run_vk_vtlayer_generate(generic generic_layer.cpp)
run_vk_layer_xml_generate(ApiDump api_dump.cpp)
run_vk_layer_xml_generate(ApiDump api_dump_text.h)

# Layer Utils Library
# For Windows, we use a static lib because the Windows loader has a fairly restrictive loader search
# path that can't be easily modified to point it to the same directory that contains the layers.
if (WIN32)
    add_library(VkLayer_utilsvt STATIC ../layers/vk_layer_config.cpp ../layers/vk_layer_extension_utils.cpp ../layers/vk_layer_utils.cpp)
else()
    add_library(VkLayer_utilsvt SHARED ../layers/vk_layer_config.cpp ../layers/vk_layer_extension_utils.cpp ../layers/vk_layer_utils.cpp)
    install(TARGETS VkLayer_utilsvt DESTINATION ${PROJECT_BINARY_DIR}/install_staging)
endif()

# VulkanTools layers
add_vk_layer(basic basic.cpp ../layers/vk_layer_table.cpp)
add_vk_layer(multi multi.cpp ../layers/vk_layer_table.cpp)
# generated
add_vk_layer(generic generic_layer.cpp ../layers/vk_layer_table.cpp)
add_vk_layer(api_dump api_dump.cpp ../layers/vk_layer_table.cpp)
add_vk_layer(screenshot screenshot.cpp ../layers/vk_layer_table.cpp)

