file(GLOB IMAGES "${PROJECT_SOURCE_DIR}/layersvt/images/*")
file(COPY ${IMAGES} DESTINATION ${CMAKE_BINARY_DIR}/layersvt/images)

if(WIN32)
    add_compile_definitions(VK_USE_PLATFORM_WIN32_KHR)
    # Minimize what Windows.h leaks
    add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
elseif(ANDROID)
    add_compile_definitions(VK_USE_PLATFORM_ANDROID_KHR)
elseif(APPLE)
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        add_compile_definitions(VK_USE_PLATFORM_MACOS_MVK)
    endif()
    if (IOS)
        add_compile_definitions(VK_USE_PLATFORM_IOS_MVK)
    endif()
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux|BSD|DragonFly|GNU")
    option(BUILD_WSI_XCB_SUPPORT "Build XCB WSI support" ON)

    find_package(PkgConfig REQUIRED)

    # TODO: The monitor layer is the only layer that requires XCB support
    if (BUILD_WSI_XCB_SUPPORT)
        pkg_check_modules(XCB REQUIRED QUIET IMPORTED_TARGET xcb)
        add_compile_definitions(VK_USE_PLATFORM_XCB_KHR)
    endif()
else()
    message(FATAL_ERROR "Unsupported Platform!")
endif()

if (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    if (MSVC)
        add_compile_options(/bigobj)
    endif()
else()
    add_compile_options(-Wpointer-arith)
endif()

if(BUILD_APIDUMP)
    add_custom_target(generate_api_cpp DEPENDS api_dump.cpp )
    add_custom_target(generate_api_text_h DEPENDS api_dump_text.h )
    add_custom_target(generate_api_html_h DEPENDS api_dump_html.h )
    add_custom_target(generate_api_json_h DEPENDS api_dump_json.h )
    add_custom_target(generate_api_video_text_h DEPENDS api_dump_video_text.h )
    add_custom_target(generate_api_video_html_h DEPENDS api_dump_video_html.h )
    add_custom_target(generate_api_video_json_h DEPENDS api_dump_video_json.h )

    find_package(Python3 REQUIRED)

    set(VULKANTOOLS_SCRIPTS_DIR "${VULKAN_TOOLS_SOURCE_DIR}/scripts")
    set(VULKAN_REGISTRY "${VULKAN_HEADERS_INSTALL_DIR}/${CMAKE_INSTALL_DATADIR}/vulkan/registry")

    # Define macro used for building vk.xml generated files
    function(run_vulkantools_generate xml_file dependency output)
        add_custom_command(OUTPUT ${output}
            COMMAND Python3::Interpreter -B ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py -registry ${VULKAN_REGISTRY}/${xml_file} -scripts ${VULKAN_REGISTRY} ${output}
            DEPENDS ${VULKAN_REGISTRY}/${xml_file} ${VULKAN_REGISTRY}/generator.py ${VULKANTOOLS_SCRIPTS_DIR}/${dependency} ${VULKANTOOLS_SCRIPTS_DIR}/vt_genvk.py ${VULKAN_REGISTRY}/reg.py
        )
    endfunction()

    run_vulkantools_generate(vk.xml api_dump_generator.py api_dump.cpp)
    run_vulkantools_generate(vk.xml api_dump_generator.py api_dump_text.h)
    run_vulkantools_generate(vk.xml api_dump_generator.py api_dump_html.h)
    run_vulkantools_generate(vk.xml api_dump_generator.py api_dump_json.h)
    run_vulkantools_generate(video.xml api_dump_generator.py api_dump_video_text.h)
    run_vulkantools_generate(video.xml api_dump_generator.py api_dump_video_html.h)
    run_vulkantools_generate(video.xml api_dump_generator.py api_dump_video_json.h)

    if(IOS)
        add_library(VkLayer_api_dump SHARED)
    else()
        add_library(VkLayer_api_dump MODULE)
    endif()

    target_sources(VkLayer_api_dump PRIVATE
        api_dump.cpp
        api_dump.h
        vk_layer_table.cpp
        vk_layer_table.h
        api_dump_layer.md
        json/VkLayer_api_dump.json.in
        ../scripts/api_dump_generator.py
    )

    target_include_directories(VkLayer_api_dump PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
    )

    if(CMAKE_SYSTEM_NAME MATCHES "Linux|BSD|DragonFly|GNU")
        target_compile_definitions(VkLayer_api_dump PRIVATE VK_USE_PLATFORM_XLIB_KHR)

        # Add Wayland API dump support
        # target_compile_definitions(VkLayer_api_dump PRIVATE VK_USE_PLATFORM_XLIB_KHR)
    endif()

    add_dependencies(VkLayer_api_dump
        generate_api_cpp
        generate_api_text_h
        generate_api_html_h
        generate_api_json_h
        generate_api_video_text_h
        generate_api_video_html_h
        generate_api_video_json_h
    )

    target_compile_definitions(VkLayer_api_dump PRIVATE VK_ENABLE_BETA_EXTENSIONS)
endif ()

if(BUILD_MONITOR)
    add_library(VkLayer_monitor MODULE)
    target_sources(VkLayer_monitor PRIVATE
        monitor.cpp
        vk_layer_table.cpp
        vk_layer_table.h
        monitor_layer.md
        json/VkLayer_monitor.json.in
    )
endif ()

if(BUILD_SCREENSHOT)
    add_library(VkLayer_screenshot MODULE)
    target_sources(VkLayer_screenshot PRIVATE
        screenshot.cpp
        screenshot_parsing.h
        screenshot_parsing.cpp
        screenshot_parsing.h
        vk_layer_table.cpp
        vk_layer_table.h
        screenshot_layer.md
        json/VkLayer_screenshot.json.in
    )
endif()

if (BUILD_TESTS_DEBUG)
    add_subdirectory(test)
endif()

list(APPEND TOOL_LAYERS "VkLayer_api_dump" "VkLayer_screenshot" "VkLayer_monitor")
foreach(layer ${TOOL_LAYERS})
    if (NOT TARGET "${layer}")
        continue()
    endif()

    target_link_Libraries(${layer} PRIVATE Vulkan::Headers Vulkan::UtilityHeaders Vulkan::LayerSettings)

    if (ANDROID)
        target_link_Libraries(${layer} PRIVATE log)
    endif()

    target_include_directories(${layer} PRIVATE .)

    if (MSVC)
        target_link_options(${layer} PRIVATE /DEF:${CMAKE_CURRENT_SOURCE_DIR}/${layer}.def)
    elseif(MINGW)
        target_sources(${layer} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${layer}.def)
    endif()

    if (APPLE)
        # IOS and APPLE can both be true (FYI)
        if(IOS)
            set_target_properties(${layer} PROPERTIES
		FRAMEWORK			TRUE
		MACOSX_FRAMEWORK_IDENTIFIER 	com.khronos.${layer}
            )
        else()
            set_target_properties(${layer} PROPERTIES SUFFIX ".dylib")
        endif()
    endif()

    if (ANDROID)
        install(TARGETS ${layer} DESTINATION ${CMAKE_INSTALL_LIBDIR})
        continue()
    endif()

    set(INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/json/${layer}.json.in")
    set(INTERMEDIATE_FILE "${CMAKE_CURRENT_BINARY_DIR}/json/intermediate-${layer}.json")
    set(OUTPUT_FILE_FINAL_NAME "${layer}.json")
    set(LAYER_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
    if (WIN32)
        set(LAYER_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})
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
