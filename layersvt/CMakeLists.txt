set(VULKANTOOLS_SCRIPTS_DIR ${PROJECT_SOURCE_DIR}/scripts)

find_package(Python3 REQUIRED)

# Define macro used for building vk.xml generated files
function(run_vulkantools_vk_xml_generate dependency output)
    add_custom_command(OUTPUT ${output}
        COMMAND Python3::Interpreter -B ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/vk.xml -scripts ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry ${output} -removeExtensions VK_NV_cooperative_matrix -removeExtensions VK_QNX_external_memory_screen_buffer
        DEPENDS ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/vk.xml ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/generator.py ${VULKANTOOLS_SCRIPTS_DIR}/${dependency} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/reg.py
    )
endfunction()

# Define macro used for building video.xml generated files
function(run_vulkantools_video_xml_generate dependency output)
    add_custom_command(OUTPUT ${output}
        COMMAND Python3::Interpreter -B ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/video.xml -scripts ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry ${output}
        DEPENDS ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/video.xml ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/generator.py ${VULKANTOOLS_SCRIPTS_DIR}/${dependency} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py ${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry/reg.py
    )
endfunction()

file(GLOB IMAGES
   "${PROJECT_SOURCE_DIR}/layersvt/images/*"
)
file(COPY ${IMAGES} DESTINATION ${CMAKE_BINARY_DIR}/layersvt/images)

if (BUILD_APIDUMP)
    add_custom_target( generate_api_cpp DEPENDS api_dump.cpp )
    add_custom_target( generate_api_text_h DEPENDS api_dump_text.h )
    add_custom_target( generate_api_html_h DEPENDS api_dump_html.h )
    add_custom_target( generate_api_json_h DEPENDS api_dump_json.h )
    add_custom_target( generate_api_video_text_h DEPENDS api_dump_video_text.h )
    add_custom_target( generate_api_video_html_h DEPENDS api_dump_video_html.h )
    add_custom_target( generate_api_video_json_h DEPENDS api_dump_video_json.h )
endif()

if (NOT APPLE)
    if(BUILD_APIDUMP)
        set(TARGET_NAMES ${TARGET_NAMES} VkLayer_api_dump)
    endif()
    if(BUILD_MONITOR)
        set(TARGET_NAMES ${TARGET_NAMES} VkLayer_monitor)
    endif()
    if(BUILD_SCREENSHOT)
        set(TARGET_NAMES ${TARGET_NAMES} VkLayer_screenshot)
    endif()

    set(VK_LAYER_RPATH /usr/lib/x86_64-linux-gnu/vulkan/layer:/usr/lib/i386-linux-gnu/vulkan/layer)
    set(CMAKE_INSTALL_RPATH ${VK_LAYER_RPATH})
else()
    if(BUILD_APIDUMP)
        set(TARGET_NAMES VkLayer_api_dump)
    endif()
endif ()

# REMOVE: When VVL is replace with utility libraries!
find_library(VkLayer_utils_LIBRARY VkLayer_utils HINTS ${VULKAN_VALIDATIONLAYERS_INSTALL_DIR})

if (WIN32)
    macro(add_vk_layer target)
        FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/VkLayer_${target}.def DEF_FILE)
        add_custom_target(copy-${target}-def-file ALL
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DEF_FILE} VkLayer_${target}.def
            VERBATIM
        )
        add_library(VkLayer_${target} SHARED ${ARGN} VkLayer_${target}.def)
        target_link_Libraries(VkLayer_${target} Vulkan::Headers ${VkLayer_utils_LIBRARY})
        set_target_properties(VkLayer_${target} PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
        )
    endmacro()
else()
    macro(add_vk_layer target)
        add_library(VkLayer_${target} SHARED ${ARGN})
        target_link_Libraries(VkLayer_${target} Vulkan::Headers ${VkLayer_utils_LIBRARY})
        if (NOT APPLE)
            set_target_properties(VkLayer_${target} PROPERTIES LINK_FLAGS "-Wl,-Bsymbolic")
        endif ()

        # Search the VVL include directory using the VVL vk_layer_config.h file
        find_file(VVL_INCLUDE_DIR vk_layer_config.h REQUIRED CMAKE_FIND_ROOT_PATH_BOTH)
        get_filename_component(VVL_INCLUDE_DIR ${VVL_INCLUDE_DIR} DIRECTORY)

        install(TARGETS VkLayer_${target} DESTINATION ${CMAKE_INSTALL_LIBDIR})
    endmacro()
endif()

if (APPLE)
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_BINARY_DIR}
    )
else ()
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_BINARY_DIR}
    )
endif()

if (WIN32)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS /bigobj")
    set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS /bigobj")
else()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpointer-arith")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpointer-arith")
endif()

# VulkanTools layers
if (BUILD_APIDUMP)
    run_vulkantools_vk_xml_generate(api_dump_generator.py api_dump.cpp)
    run_vulkantools_vk_xml_generate(api_dump_generator.py api_dump_text.h)
    run_vulkantools_vk_xml_generate(api_dump_generator.py api_dump_html.h)
    run_vulkantools_vk_xml_generate(api_dump_generator.py api_dump_json.h)
    run_vulkantools_video_xml_generate(api_dump_generator.py api_dump_video_text.h)
    run_vulkantools_video_xml_generate(api_dump_generator.py api_dump_video_html.h)
    run_vulkantools_video_xml_generate(api_dump_generator.py api_dump_video_json.h)

    add_vk_layer(api_dump api_dump.cpp vk_layer_table.cpp ../vku/vk_layer_settings.cpp ../vku/vk_layer_settings.h)
    add_dependencies(VkLayer_api_dump generate_api_cpp generate_api_text_h generate_api_html_h generate_api_json_h
        generate_api_video_text_h generate_api_video_html_h generate_api_video_json_h)
endif ()

if (NOT APPLE)
    if(BUILD_MONITOR)
        add_vk_layer(monitor monitor.cpp vk_layer_table.cpp ../vku/vk_layer_settings.cpp ../vku/vk_layer_settings.h)
    endif ()
    if(BUILD_SCREENSHOT)
        add_vk_layer(screenshot screenshot.cpp screenshot_parsing.h screenshot_parsing.cpp vk_layer_table.cpp ../vku/vk_layer_settings.cpp ../vku/vk_layer_settings.h)
    endif ()
endif ()

if (ANDROID)
    message(DEBUG "Android doesn't use json manifests.")
    return()
endif()

foreach(layer ${TARGET_NAMES})
    set(INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${layer}.json.in")
    set(INTERMEDIATE_FILE "${CMAKE_CURRENT_BINARY_DIR}/json/intermediate-${layer}.json")
    set(OUTPUT_FILE_FINAL_NAME "${layer}.json")
    set(LAYER_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
    if (WIN32)
        set(LAYER_INSTALL_DIR ${CMAKE_INSTALL_BINDIR}) # WIN32/MINGW expect the dll in the `bin` dir, this matches our WIN32 SDK process
    endif()

    if (WIN32)
        set(JSON_LIBRARY_PATH ".\\\\${layer}.dll")
    elseif(APPLE)
        set(JSON_LIBRARY_PATH "./lib${layer}.dylib")
    else()
        set(JSON_LIBRARY_PATH "./lib${layer}.so")
    endif()

    set(JSON_VERSION ${VulkanHeaders_VERSION})

    configure_file(${INPUT_FILE} ${INTERMEDIATE_FILE} @ONLY)

    add_custom_command(TARGET ${layer} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${INTERMEDIATE_FILE} $<TARGET_FILE_DIR:${layer}>/${OUTPUT_FILE_FINAL_NAME}
    )

    if (UNIX)
        set(UNIX_INTERMEDIATE_FILE "${CMAKE_CURRENT_BINARY_DIR}/json/unix_install_${layer}.json")

        if(APPLE)
            set(JSON_LIBRARY_PATH "lib${layer}.dylib")
        else()
            set(JSON_LIBRARY_PATH "lib${layer}.so")
        endif()

        configure_file(${INPUT_FILE} ${UNIX_INTERMEDIATE_FILE} @ONLY)

        install(FILES ${UNIX_INTERMEDIATE_FILE} DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/vulkan/explicit_layer.d RENAME ${OUTPUT_FILE_FINAL_NAME})
    endif()

    if (WIN32)
        install(FILES ${INTERMEDIATE_FILE} DESTINATION ${LAYER_INSTALL_DIR} RENAME ${OUTPUT_FILE_FINAL_NAME})
    endif()
    if (MSVC)
        install(FILES $<TARGET_PDB_FILE:${layer}> DESTINATION ${LAYER_INSTALL_DIR})
    endif()

    install(TARGETS ${layer} DESTINATION ${LAYER_INSTALL_DIR})
endforeach()
