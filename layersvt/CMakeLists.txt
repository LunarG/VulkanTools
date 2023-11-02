file(GLOB IMAGES
   "${PROJECT_SOURCE_DIR}/layersvt/images/*"
)
file(COPY ${IMAGES} DESTINATION ${CMAKE_BINARY_DIR}/layersvt/images)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR -DVK_USE_PLATFORM_WIN32_KHX -DWIN32_LEAN_AND_MEAN -DNOMINMAX)
    set(DisplayServer Win32)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME MATCHES "BSD")
    if (BUILD_WSI_XCB_SUPPORT)
        add_definitions(-DVK_USE_PLATFORM_XCB_KHR -DVK_USE_PLATFORM_XCB_KHX)
        set(DisplayServer Xcb)
    endif()

    if (BUILD_WSI_XLIB_SUPPORT)
        if (NOT DisplayServer)
            set(DisplayServer Xlib)
        endif()
        add_definitions(-DVK_USE_PLATFORM_XLIB_KHR -DVK_USE_PLATFORM_XLIB_KHX)
    endif()

    if (BUILD_WSI_WAYLAND_SUPPORT)
       # TODO Add Wayland Support
       # add_definitions(-DVK_USE_PLATFORM_WAYLAND_KHR)
    endif()
elseif(APPLE)
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        add_compile_definitions(VK_USE_PLATFORM_MACOS_MVK)
    endif()
    if (IOS)
        add_compile_definitions(VK_USE_PLATFORM_IOS_MVK)
    endif()

    # TODO: Add Metal support. Requires objective C/C++.
    # add_compile_definitions(VK_USE_PLATFORM_METAL_EXT)
else()
    message(FATAL_ERROR "Unsupported Platform!")
endif()

if (BUILD_APIDUMP)
    add_custom_target( generate_api_cpp DEPENDS api_dump.cpp )
    add_custom_target( generate_api_text_h DEPENDS api_dump_text.h )
    add_custom_target( generate_api_html_h DEPENDS api_dump_html.h )
    add_custom_target( generate_api_json_h DEPENDS api_dump_json.h )
    add_custom_target( generate_api_video_text_h DEPENDS api_dump_video_text.h )
    add_custom_target( generate_api_video_html_h DEPENDS api_dump_video_html.h )
    add_custom_target( generate_api_video_json_h DEPENDS api_dump_video_json.h )

    set_target_properties(generate_api_cpp generate_api_text_h generate_api_html_h generate_api_json_h
        generate_api_video_text_h generate_api_video_html_h generate_api_video_json_h
         PROPERTIES FOLDER VkLayer_api_dump/${VULKANTOOLS_TARGET_FOLDER})
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

if (WIN32)
    macro(add_vk_layer target)
        FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/VkLayer_${target}.def DEF_FILE)
        add_custom_target(copy-${target}-def-file ALL
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DEF_FILE} VkLayer_${target}.def
            VERBATIM
        )
        add_library(VkLayer_${target} SHARED ${ARGN} VkLayer_${target}.def)
        target_link_Libraries(VkLayer_${target} Vulkan::Headers Vulkan::UtilityHeaders Vulkan::LayerSettings ${VkLayer_utils_LIBRARY})
        set_target_properties(VkLayer_${target} PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
        )
        set_target_properties(copy-${target}-def-file PROPERTIES FOLDER "VkLayer_${target}")
        set_target_properties(VkLayer_${target} PROPERTIES FOLDER "VkLayer_${target}")
    endmacro()
else()
    macro(add_vk_layer target)
        add_library(VkLayer_${target} SHARED ${ARGN})
        target_link_Libraries(VkLayer_${target} Vulkan::Headers Vulkan::UtilityHeaders Vulkan::LayerSettings ${VkLayer_utils_LIBRARY})
        if (NOT APPLE)
            set_target_properties(VkLayer_${target} PROPERTIES LINK_FLAGS "-Wl,-Bsymbolic")
        endif ()
        if (ANDROID)
            target_link_Libraries(VkLayer_${target} log)
        endif()
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
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    if (MSVC)
        add_compile_options(/bigobj)
    endif()
else()
    add_compile_options(-Wpointer-arith)
endif()

find_package(Python3 REQUIRED)

set(VULKANTOOLS_SCRIPTS_DIR "${VULKAN_TOOLS_SOURCE_DIR}/scripts")
set(VULKAN_REGISTRY "${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry")

# Define macro used for building vk.xml generated files
function(run_vulkantools_vk_xml_generate dependency output)
    add_custom_command(OUTPUT ${output}
        COMMAND Python3::Interpreter -B ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VULKAN_REGISTRY}/vk.xml -scripts ${VULKAN_REGISTRY} ${output}
        DEPENDS ${VULKAN_REGISTRY}/vk.xml ${VULKAN_REGISTRY}/generator.py ${VULKANTOOLS_SCRIPTS_DIR}/${dependency} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py ${VULKAN_REGISTRY}/reg.py
    )
endfunction()

# Define macro used for building video.xml generated files
function(run_vulkantools_video_xml_generate dependency output)
    add_custom_command(OUTPUT ${output}
        COMMAND Python3::Interpreter -B ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VULKAN_REGISTRY}/video.xml -scripts ${VULKAN_REGISTRY} ${output}
        DEPENDS ${VULKAN_REGISTRY}/video.xml ${VULKAN_REGISTRY}/generator.py ${VULKANTOOLS_SCRIPTS_DIR}/${dependency} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py ${VULKAN_REGISTRY}/reg.py
    )
endfunction()

#VulkanTools layers
if(BUILD_APIDUMP)
    run_vulkantools_vk_xml_generate(api_dump_generator.py api_dump.cpp)
    run_vulkantools_vk_xml_generate(api_dump_generator.py api_dump_text.h)
    run_vulkantools_vk_xml_generate(api_dump_generator.py api_dump_html.h)
    run_vulkantools_vk_xml_generate(api_dump_generator.py api_dump_json.h)
    run_vulkantools_video_xml_generate(api_dump_generator.py api_dump_video_text.h)
    run_vulkantools_video_xml_generate(api_dump_generator.py api_dump_video_html.h)
    run_vulkantools_video_xml_generate(api_dump_generator.py api_dump_video_json.h)

    add_vk_layer(api_dump api_dump.cpp api_dump.h vk_layer_table.cpp vk_layer_table.h VkLayer_api_dump.def api_dump_layer.md VkLayer_api_dump.json.in ../scripts/api_dump_generator.py)
    add_dependencies(VkLayer_api_dump generate_api_cpp generate_api_text_h generate_api_html_h generate_api_json_h
        generate_api_video_text_h generate_api_video_html_h generate_api_video_json_h)
endif ()

if (NOT APPLE)
    if(BUILD_MONITOR)
        add_vk_layer(monitor monitor.cpp vk_layer_table.cpp vk_layer_table.h VKLayer_monitor.def monitor_layer.md VkLayer_monitor.json.in)
    endif ()
    if(BUILD_SCREENSHOT)
        add_vk_layer(screenshot screenshot.cpp screenshot_parsing.h screenshot_parsing.cpp screenshot_parsing.h vk_layer_table.cpp vk_layer_table.h VkLayer_screenshot.def screenshot_layer.md VkLayer_screenshot.json.in)
    endif ()
endif ()

if (ANDROID)
    message(DEBUG "Android doesn't use json manifests.")
    return()
endif()

# The output file needs Unix "/" separators or Windows "\" separators
# Windows paths need to start with ".\", Unix paths do not.
# Windows separators actually need to be doubled because the json format uses backslash escapes
file(TO_NATIVE_PATH "./" RELATIVE_PATH_PREFIX)
string(REPLACE "\\" "\\\\" RELATIVE_PATH_PREFIX "${RELATIVE_PATH_PREFIX}")
# Run each .json.in file through the generator
# We need to create the generator.cmake script so that the generator can be run at compile time, instead of configure time
# Running at compile time lets us use cmake generator expressions (TARGET_FILE_NAME and TARGET_FILE_DIR, specifically)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/generator.cmake" "configure_file(\"\${INPUT_FILE}\" \"\${OUTPUT_FILE}\" @ONLY)")
foreach(TARGET_NAME ${TARGET_NAMES})
    set(CONFIG_DEFINES
        -DINPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.json.in"
        -DVK_VERSION="${VulkanHeaders_VERSION}"
    )
    # Get the needed properties from that target
    set(CONFIG_DEFINES ${CONFIG_DEFINES}
        -DOUTPUT_FILE="$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_NAME}.json"
        -DRELATIVE_LAYER_BINARY="${RELATIVE_PATH_PREFIX}$<TARGET_FILE_NAME:${TARGET_NAME}>"
        )

    add_custom_target(${TARGET_NAME}-json ALL COMMAND ${CMAKE_COMMAND} ${CONFIG_DEFINES} -P "${CMAKE_CURRENT_BINARY_DIR}/generator.cmake")
    set_target_properties(${TARGET_NAME}-json PROPERTIES FOLDER "${TARGET_NAME}")

    # Add target for JSON file install.
    if(NOT WIN32)
        # Add targets for JSON file install on Linux.
        # Need to remove the "./" from the library path before installing
        add_custom_target(${TARGET_NAME}-staging-json ALL
            COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/staging-json
            COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.json ${CMAKE_CURRENT_BINARY_DIR}/staging-json
            COMMAND sed -i -e "/.library_path.:/s=./libVkLayer=libVkLayer=" ${CMAKE_CURRENT_BINARY_DIR}/staging-json/${TARGET_NAME}.json
            VERBATIM
            DEPENDS ${TARGET_NAME}-json
            )
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/staging-json/${TARGET_NAME}.json DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/vulkan/explicit_layer.d)
    endif()

endforeach()

if (BUILD_TESTS_DEBUG)
    add_subdirectory(test)
endif ()
