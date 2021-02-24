cmake_minimum_required(VERSION 3.10.2)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR -DVK_USE_PLATFORM_WIN32_KHX -DWIN32_LEAN_AND_MEAN -DNOMINMAX)
    set(DisplayServer Win32)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME MATCHES "BSD")
    if (BUILD_WSI_XCB_SUPPORT)
        add_definitions(-DVK_USE_PLATFORM_XCB_KHR -DVK_USE_PLATFORM_XCB_KHX)
    endif()

    if (BUILD_WSI_XLIB_SUPPORT)
       add_definitions(-DVK_USE_PLATFORM_XLIB_KHR -DVK_USE_PLATFORM_XLIB_KHX -DVK_USE_PLATFORM_XLIB_XRANDR_EXT)
    endif()

    if (BUILD_WSI_WAYLAND_SUPPORT)
       add_definitions(-DVK_USE_PLATFORM_WAYLAND_KHR -DVK_USE_PLATFORM_WAYLAND_KHX)
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
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Vulkan-ValidationLayers_INCLUDE_DIR}
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
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wpointer-arith -Wno-unused-function -Wno-sign-compare")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpointer-arith -Wno-unused-function -Wno-sign-compare")
endif()

run_vulkantools_vk_xml_generate(layer_factory_generator.py layer_factory.h)
run_vulkantools_vk_xml_generate(layer_factory_generator.py layer_factory.cpp)
add_custom_target(generate_vlf DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/layer_factory.cpp ${CMAKE_CURRENT_BINARY_DIR}/layer_factory.h)
set_target_properties(generate_vlf PROPERTIES FOLDER ${VULKANTOOLS_TARGET_FOLDER})

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
set (JSON_TEMPLATE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/layer_factory.json.in)
if (WIN32)
    set (JSON_DEST_PATH ${CMAKE_CURRENT_BINARY_DIR})
else()
    set (JSON_DEST_PATH ${CMAKE_CURRENT_BINARY_DIR}/../layers)
endif()



set (CMAKE_LAYER_FACTORY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SUBDIRLIST(ST_SUBDIRS ${CMAKE_LAYER_FACTORY_SOURCE_DIR})

# json file creation

# The output file needs Unix "/" separators or Windows "\" separators
# On top of that, Windows separators actually need to be doubled because the json format uses backslash escapes
file(TO_NATIVE_PATH "./" RELATIVE_PATH_PREFIX)
string(REPLACE "\\" "\\\\" RELATIVE_PATH_PREFIX "${RELATIVE_PATH_PREFIX}")

# Run json.in file through the generator
# We need to create the generator.cmake script so that the generator can be run at compile time, instead of configure time
# Running at compile time lets us use cmake generator expressions (TARGET_FILE_NAME and TARGET_FILE_DIR, specifically)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/generator.cmake" "configure_file(\"\${INPUT_FILE}\" \"\${OUTPUT_FILE}\")")
foreach(SUBDIR ${ST_SUBDIRS})
    set(TARGET_NAME VkLayer_${SUBDIR})
    set(CONFIG_DEFINES
        -DINPUT_FILE=${JSON_TEMPLATE_PATH}
        -DVK_VERSION="${VulkanHeaders_VERSION_MAJOR}.${VulkanHeaders_VERSION_MINOR}.${VulkanHeaders_VERSION_PATCH}"
    )
    set(CONFIG_DEFINES ${CONFIG_DEFINES}
        -DOUTPUT_FILE="$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_NAME}.json"
        -DRELATIVE_LAYER_BINARY="${RELATIVE_PATH_PREFIX}$<TARGET_FILE_NAME:${TARGET_NAME}>"
        -DLAYER_NAME="VK_LAYER_LUNARG_${SUBDIR}"
    )
    if(CMAKE_CURRENT_BINARY_DIR MATCHES ".*layer_factory.*")
        if(WIN32)
            add_custom_target(${TARGET_NAME}-json ALL
                    COMMAND ${CMAKE_COMMAND} ${CONFIG_DEFINES} -P "${CMAKE_CURRENT_BINARY_DIR}/generator.cmake"
            )
        else()
            add_custom_target(${TARGET_NAME}-json ALL
                    COMMAND ${CMAKE_COMMAND} ${CONFIG_DEFINES} -P "${CMAKE_CURRENT_BINARY_DIR}/generator.cmake"
                    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/../layers/staging-json
                    COMMAND ${CMAKE_COMMAND} -E copy ${JSON_DEST_PATH}/VkLayer_${SUBDIR}.json ${CMAKE_CURRENT_BINARY_DIR}/../layers/staging-json
                    COMMAND sed -i -e "/library_path.:/s=./libVkLayer=libVkLayer=" ${CMAKE_CURRENT_BINARY_DIR}/../layers/staging-json/VkLayer_${SUBDIR}.json
            )
        endif()
    endif()

    if (WIN32)
        macro(add_factory_layer target subdir)
        # Read in def file template, update with new layer name and write to destination
        file(READ ${CMAKE_CURRENT_SOURCE_DIR}/VkLayer_layer_factory.def def_file_template)
        string(REPLACE "layer_factory" ${target} target_def_file ${def_file_template})
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/VkLayer_${target}.def ${target_def_file})
        # Edit json template and copy to build\layers dir at cmake time
        file(READ "${JSON_TEMPLATE_PATH}" json_file_template)
        string(REPLACE "layer_factory" "${target}" target_json_file "${json_file_template}")
        file(TO_NATIVE_PATH ${JSON_DEST_PATH}/VkLayer_${target}.json dst_json)
        file(WRITE ${dst_json} ${target_json_file})

        add_library(VkLayer_${target} SHARED ${ARGN} VkLayer_${target}.def)
        target_link_Libraries(VkLayer_${target} ${VkLayer_utils_LIBRARY})
        target_include_directories(VkLayer_${target} PRIVATE ${subdir})
        endmacro()
    else()
        macro(add_factory_layer target subdir)
        # Create custom target for the copies so that json template is copied and edited at compile time
        file(READ "${JSON_TEMPLATE_PATH}" json_file_template)
        string(REPLACE "layer_factory" "${target}" target_json_file "${json_file_template}")
        file(TO_NATIVE_PATH ${JSON_DEST_PATH}/VkLayer_${target}.json dst_json)
        file(WRITE ${dst_json} ${target_json_file})
        add_library(VkLayer_${target} SHARED ${ARGN})
        target_link_Libraries(VkLayer_${target} ${VkLayer_utils_LIBRARY})
        target_include_directories(VkLayer_${target} PRIVATE ${subdir})
        set_target_properties(VkLayer_${target} PROPERTIES LINK_FLAGS "-Wl,-Bsymbolic,--exclude-libs,ALL")
        endmacro()
    endif()

endforeach()

# Create the layer_factory_linux.json file
if(UNIX)
    set(CONFIG_DEFINES
        -DINPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/layer_factory.json.in
        -DLAYER_NAME="VK_LAYER_LUNARG_layer_factory"
        -DRELATIVE_LAYER_BINARY="./libVkLayer_layer_factory.so"
        -DVK_VERSION="${VulkanHeaders_VERSION_MAJOR}.${VulkanHeaders_VERSION_MINOR}.${VulkanHeaders_VERSION_PATCH}"
        -DOUTPUT_FILE="layer_factory_linux.json"
    )
    add_custom_target(layer_factory_linux-json ALL
        COMMAND ${CMAKE_COMMAND} ${CONFIG_DEFINES} -P "${CMAKE_CURRENT_BINARY_DIR}/generator.cmake"
        )
endif()

set (CMAKE_LAYER_FACTORY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SUBDIRLIST(ST_SUBDIRS ${CMAKE_LAYER_FACTORY_SOURCE_DIR})

# Loop through all subdirectories, creating a factory-based layer for each. For each factory layer, create a dependency link on
# the previous layer in order to serialize their builds.
set(dep_chain generate_vlf)
FOREACH(subdir ${ST_SUBDIRS})
    file(GLOB INTERCEPTOR_SOURCES ${CMAKE_LAYER_FACTORY_SOURCE_DIR}/${subdir}/*.h ${CMAKE_LAYER_FACTORY_SOURCE_DIR}/${subdir}/*.cpp)
    add_factory_layer(${subdir} ${CMAKE_LAYER_FACTORY_SOURCE_DIR}/${subdir} layer_factory.cpp layer_factory.h ${Vulkan-ValidationLayers_INCLUDE_DIR}/xxhash.c ${INTERCEPTOR_SOURCES})
    add_dependencies(VkLayer_${subdir} ${dep_chain})
    set(dep_chain VkLayer_${subdir})
ENDFOREACH()

# Add targets for JSON file install. Try to follow the same convention as the Khronos Vulkan-ValidationLayers repository to maintain
# a coherent directory topology in the install path.
if(WIN32)
    foreach (factory_layer ${ST_SUBDIRS})
        set(config_file VkLayer_${factory_layer})
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${config_file}.json DESTINATION ${CMAKE_INSTALL_LIBDIR})
    endforeach(factory_layer)
elseif(UNIX)
    foreach (factory_layer ${ST_SUBDIRS})
        set(config_file VkLayer_${factory_layer})
        add_custom_target(
            ${config_file}-staging-json ALL
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/../layers/staging-json
            COMMAND ${CMAKE_COMMAND} -E copy ${JSON_DEST_PATH}/${config_file}.json ${CMAKE_CURRENT_BINARY_DIR}/../layers/staging-json
            COMMAND sed -i -e "/.library_path.:/s=./libVkLayer=libVkLayer=" ${CMAKE_CURRENT_BINARY_DIR}/../layers/staging-json/${config_file}.json
            VERBATIM
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../layers/${config_file}.json
        )

        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/../layers/staging-json/${config_file}.json DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/vulkan/explicit_layer.d)
        install(TARGETS ${config_file} DESTINATION ${CMAKE_INSTALL_LIBDIR})
    endforeach(factory_layer)
endif()
