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

# Move binary output location to the standard layers directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../layers)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../layers)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../layers)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
#    ${CMAKE_CURRENT_SOURCE_DIR}/../layers
#    ${CMAKE_CURRENT_SOURCE_DIR}/../loader
#    ${CMAKE_CURRENT_SOURCE_DIR}/../include/vulkan
    ${V_LVL_ROOT_DIR}/loader
    ${V_LVL_ROOT_DIR}/layers
    ${CMAKE_CURRENT_BINARY_DIR}
#    ${PROJECT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/${V_LVL_RELATIVE_LOCATION}

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

# Code-generate the layer_factory source files, and add a dependency target for them
set(VLF_BASE_DEPENDENCIES
    ${LVL_SCRIPTS_DIR}/vk.xml
    ${LVL_SCRIPTS_DIR}/generator.py
    ${LVL_SCRIPTS_DIR}/reg.py
    ${VT_SCRIPTS_DIR}/layer_factory_generator.py
    ${VT_SCRIPTS_DIR}/lvl_genvk.py
    )
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/layer_factory.h
    COMMAND ${PYTHON_CMD} ${VT_SCRIPTS_DIR}/lvl_genvk.py -registry ${LVL_SCRIPTS_DIR}/vk.xml layer_factory.h
    DEPENDS ${VLF_BASE_DEPENDENCIES}
    )
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/layer_factory.cpp
    COMMAND ${PYTHON_CMD} ${VT_SCRIPTS_DIR}/lvl_genvk.py -registry ${LVL_SCRIPTS_DIR}/vk.xml layer_factory.cpp
    DEPENDS ${VLF_BASE_DEPENDENCIES}
    )
add_custom_target(generate_vlf DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/layer_factory.cpp ${CMAKE_CURRENT_BINARY_DIR}/layer_factory.h)

# Layer Utils Library
# For Windows, we use a static lib because the Windows loader has a fairly restrictive loader search
# path that can't be easily modified to point it to the same directory that contains the layers.
set(VKLAYER_UTILS_VLF_SOURCES ${V_LVL_ROOT_DIR}/layers/vk_layer_config.cpp ${V_LVL_ROOT_DIR}/layers/vk_layer_extension_utils.cpp ${V_LVL_ROOT_DIR}/layers/vk_layer_utils.cpp ${V_LVL_ROOT_DIR}/layers/vk_format_utils.cpp)
if (WIN32)
    add_library(VkLayer_utils_vlf STATIC ${VKLAYER_UTILS_VLF_SOURCES})
else()
    add_library(VkLayer_utils_vlf SHARED ${VKLAYER_UTILS_VLF_SOURCES})
    if(INSTALL_LVL_FILES)
        install(TARGETS VkLayer_utils_vlf DESTINATION $CMAKE_INSTALL_LIBDIR})
    endif()
endif()
add_dependencies(VkLayer_utils_vlf generate_helper_files)

# Generates a list of subdirectories in a directory.  Used to pick up factory layers and interceptors
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

# Paths for the layer factory json template and the destination for factory layer json files
if (WIN32)
    set (JSON_TEMPLATE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/layer_factory_windows.json)
    set (JSON_DEST_PATH ${CMAKE_CURRENT_BINARY_DIR})
    set (JSON_DECORATED_DEST_PATH ${CMAKE_CURRENT_BINARY_DIR}/../layers/$<CONFIGURATION>)
else()
    set (JSON_TEMPLATE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/layer_factory_linux.json)
    set (JSON_DEST_PATH ${CMAKE_CURRENT_BINARY_DIR}/../layers)
endif()

if (WIN32)
    macro(add_factory_layer target subdir)
    # Read in def file template, update with new layer name and write to destination
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/VkLayer_layer_factory.def def_file_template)
    string(REPLACE "layer_factory" ${target} target_def_file ${def_file_template})
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/VkLayer_${target}.def ${target_def_file})
    add_library(VkLayer_${target} SHARED ${ARGN} ${CMAKE_CURRENT_BINARY_DIR}/layer_factory.cpp ${V_LVL_ROOT_DIR}/layers/vk_layer_table.cpp VkLayer_${target}.def)
    # Edit json template and copy to build\layers dir at cmake time
    file(READ "${JSON_TEMPLATE_PATH}" json_file_template)
    string(REPLACE "layer_factory" "${target}" target_json_file "${json_file_template}")
    file(TO_NATIVE_PATH ${JSON_DEST_PATH}/VkLayer_${target}.json dst_json)
    file(WRITE ${dst_json} ${target_json_file})
    # Copy target json file from build/layers to final directory at compile time
    file(TO_NATIVE_PATH ${JSON_DECORATED_DEST_PATH}/VkLayer_${target}.json decorated_dst_json)
    add_custom_target(${target}json-copy ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${dst_json} ${decorated_dst_json}
        VERBATIM
        )
    target_link_Libraries(VkLayer_${target} VkLayer_utils_vlf)
    target_include_directories(VkLayer_${target} PRIVATE ${subdir})
    add_dependencies(VkLayer_${target} generate_vlf generate_helper_files VkLayer_utils_vlf)
    add_dependencies(VkLayer_${target} generate_helper_files VkLayer_utils_vlf)
    endmacro()
else()
    macro(add_factory_layer target subdir)
    # Create custom target for the copies so that json template is copied and edited at compile time
    file(READ "${JSON_TEMPLATE_PATH}" json_file_template)
    string(REPLACE "layer_factory" "${target}" target_json_file "${json_file_template}")
    file(TO_NATIVE_PATH ${JSON_DEST_PATH}/VkLayer_${target}.json dst_json)
    file(WRITE ${dst_json} ${target_json_file})
    add_library(VkLayer_${target} SHARED ${ARGN} ${CMAKE_CURRENT_BINARY_DIR}/layer_factory.cpp ${V_LVL_ROOT_DIR}/layers/vk_layer_table.cpp)
    target_link_Libraries(VkLayer_${target} VkLayer_utils_vlf)
    target_include_directories(VkLayer_${target} PRIVATE ${subdir})
    add_dependencies(VkLayer_${target} generate_vlf generate_helper_files VkLayer_utils_vlf)
    set_target_properties(VkLayer_${target} PROPERTIES LINK_FLAGS "-Wl,-Bsymbolic")
    if(INSTALL_LVL_FILES)
        install(TARGETS VkLayer_${target} DESTINATION ${CMAKE_INSTALL_LIBDIR})
    endif()
    endmacro()
endif()

set (CMAKE_LAYER_FACTORY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SUBDIRLIST(ST_SUBDIRS ${CMAKE_LAYER_FACTORY_SOURCE_DIR})

# Loop through all subdirectories, creating a factory-based layer for each. For each factory layer, create a dependency link on
# the previous layer in order to serialize their builds.
set(dep_chain generate_vlf)
FOREACH(subdir ${ST_SUBDIRS})
    file(GLOB INTERCEPTOR_SOURCES ${CMAKE_LAYER_FACTORY_SOURCE_DIR}/${subdir}/*.h ${CMAKE_LAYER_FACTORY_SOURCE_DIR}/${subdir}/*.cpp)
    add_factory_layer(${subdir} ${CMAKE_LAYER_FACTORY_SOURCE_DIR}/${subdir} ${INTERCEPTOR_SOURCES})
    add_dependencies(VkLayer_${subdir} ${dep_chain})
    set(dep_chain VkLayer_${subdir})
ENDFOREACH()

# Add targets for JSON file install on Linux.
# Need to remove the "./" from the library path before installing to /etc.
if(UNIX)
    if(INSTALL_LVL_FILES)
        foreach (factory_layer ${ST_SUBDIRS})
            set(config_file VkLayer_${factory_layer})
            add_custom_target(${config_file}-staging-json ALL
                COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/../layers/staging-json
                COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/../layers/${config_file}.json ${CMAKE_CURRENT_BINARY_DIR}/../layers/staging-json
                COMMAND sed -i -e "/\"library_path\":/s$./libVkLayer$libVkLayer$" ${CMAKE_CURRENT_BINARY_DIR}/../layers/staging-json/${config_file}.json
                VERBATIM
                DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../layers/${config_file}.json
                )
            install(FILES ${CMAKE_CURRENT_BINARY_DIR}/../layers/staging-json/${config_file}.json DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/vulkan/explicit_layer.d)
        endforeach(factory_layer)
    endif()
endif()
