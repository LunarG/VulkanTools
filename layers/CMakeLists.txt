cmake_minimum_required (VERSION 2.8.11)

set (LOADER_INCLUDE_DIR ${VULKANTOOLS_SOURCE_DIR}/../Vulkan-LoaderAndValidationLayers/include)
macro(run_vk_helper subcmd)
	add_custom_command(OUTPUT ${ARGN}
		COMMAND ${PYTHON_CMD} ${LOADER_PY_DIR}/vk_helper.py --${subcmd} ${LOADER_INCLUDE_DIR}/vulkan/vulkan.h --abs_out_dir ${CMAKE_CURRENT_BINARY_DIR}
		DEPENDS ${LOADER_PY_DIR}/vk_helper.py ${LOADER_INCLUDE_DIR}/vulkan/vulkan.h
	)
endmacro()

macro(run_vk_layer_generate subcmd output)
	add_custom_command(OUTPUT ${output}
		COMMAND ${PYTHON_CMD} ${PROJECT_SOURCE_DIR}/vk-layer-generate.py ${subcmd} ${LOADER_INCLUDE_DIR}/vulkan/vulkan.h > ${output}
		DEPENDS ${PROJECT_SOURCE_DIR}/vk-layer-generate.py ${LOADER_INCLUDE_DIR}/vulkan/vulkan.h ${LOADER_PY_DIR}/vulkan.py
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

if (NOT WIN32)
    # extra setup for out-of-tree builds
    if (NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR))
        foreach (config_file ${LAYER_JSON_FILES})
            add_custom_target(${config_file}-json ALL
                COMMAND ln -sf ${CMAKE_CURRENT_SOURCE_DIR}/linux/${config_file}.json
                VERBATIM
                )
        endforeach(config_file)
    endif()
else()
    if (NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR))
        foreach (config_file ${LAYER_JSON_FILES})
            FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/windows/${config_file}.json src_json)
            FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/${config_file}.json dst_json)
            add_custom_target(${config_file}-json ALL
                COMMAND copy ${src_json} ${dst_json}
                VERBATIM
                )
        endforeach(config_file)
    endif()
endif()

if (WIN32)
    macro(add_vk_layer target)
	add_custom_command(OUTPUT VkLayer_${target}.def
		COMMAND ${PYTHON_CMD} ${LOADER_PY_DIR}/vk-generate.py win-def-file VkLayer_${target} layer > VkLayer_${target}.def
		DEPENDS ${LOADER_PY_DIR}/vk-generate.py ${LOADER_PY_DIR}/vk.py
	)
	add_library(VkLayer_${target} SHARED ${ARGN} VkLayer_${target}.def)
    target_link_Libraries(VkLayer_${target} layer_utils)
	add_dependencies(VkLayer_${target} layer_utils_static)
	add_dependencies(VkLayer_${target} generate_vk_layer_helpers)
	set_target_properties(VkLayer_${target} PROPERTIES LINK_FLAGS "/DEF:${CMAKE_CURRENT_BINARY_DIR}/VkLayer_${target}.def")
    endmacro()
else()
    macro(add_vk_layer target)
	add_library(VkLayer_${target} SHARED ${ARGN})
    target_link_Libraries(VkLayer_${target} layer_utils)
	add_dependencies(VkLayer_${target} generate_vk_layer_helpers)
	set_target_properties(VkLayer_${target} PROPERTIES LINK_FLAGS "-Wl,-Bsymbolic")
	install(TARGETS VkLayer_${target} DESTINATION ${PROJECT_BINARY_DIR}/install_staging)
    endmacro()
endif()

#hacky use layer include directory from loader repo
set (LOADER_LAYER_DIR ${VULKANTOOLS_SOURCE_DIR}/../Vulkan-LoaderAndValidationLayers/layers)
set (LOADER_LOADER_DIR ${VULKANTOOLS_SOURCE_DIR}/../Vulkan-LoaderAndValidationLayers/loader)
include_directories(
        ${LOADER_LAYER_DIR}
        ${LOADER_LOADER_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${LOADER_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

if (WIN32)
    set (CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
    set (CMAKE_C_FLAGS    "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
endif()
if (NOT WIN32)
    set (CMAKE_CXX_FLAGS "-std=c++11")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpointer-arith")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpointer-arith")
endif()

add_custom_command(OUTPUT vk_dispatch_table_helper.h
	COMMAND ${PYTHON_CMD} ${LOADER_PY_DIR}/vk-generate.py dispatch-table-ops layer > vk_dispatch_table_helper.h
	DEPENDS ${LOADER_PY_DIR}/vk-generate.py ${LOADER_PY_DIR}/vulkan.py)

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
)

add_custom_target(generate_vk_layer_helpers DEPENDS
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
)

run_vk_layer_generate(generic generic_layer.cpp)
run_vk_layer_generate(api_dump api_dump.cpp)

# hacky: find layer utils in Loader directory
add_library(layer_utils SHARED ${LOADER_LAYER_DIR}/vk_layer_config.cpp ${LOADER_LAYER_DIR}/vk_layer_extension_utils.cpp)
if (WIN32)
    add_library(layer_utils_static STATIC ${LOADER_LAYER_DIR}/vk_layer_config.cpp ${LOADER_LAYER_DIR}/vk_layer_extension_utils.cpp)
    set_target_properties(layer_utils_static PROPERTIES OUTPUT_NAME layer_utils)
    target_link_libraries(layer_utils)
else()
    install(TARGETS layer_utils DESTINATION ${PROJECT_BINARY_DIR}/install_staging)
endif()

add_vk_layer(basic basic.cpp ${LOADER_LAYER_DIR}/vk_layer_table.cpp)
add_vk_layer(multi multi.cpp ${LOADER_LAYER_DIR}/vk_layer_table.cpp)
add_vk_layer(screenshot screenshot.cpp ${LOADER_LAYER_DIR}/vk_layer_table.cpp)
# generated
add_vk_layer(generic generic_layer.cpp ${LOADER_LAYER_DIR}/vk_layer_table.cpp)
add_vk_layer(api_dump api_dump.cpp ${LOADER_LAYER_DIR}/vk_layer_table.cpp)
