# ~~~
# Copyright (c) 2020-2026 LunarG, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ~~~

set(CMAKE_AUTORCC ON)

if (NOT DEFINED CMAKE_INSTALL_BINDIR)
    include(GNUInstallDirs)
endif()

function(vkConfigTest NAME)
    set(TEST_FILE ./${NAME}.cpp)
    set(TEST_NAME vkconfig_${NAME})

    file(GLOB FILES_LAYER_JSON ./layers/*.json)
    file(GLOB FILES_JSON ./*.json)
    file(GLOB FILES_TEXT ./*.txt)
    source_group("Test Files" FILES ${FILES_JSON} ${FILES_TEXT} ${FILES_LAYER_JSON})

    add_executable(${TEST_NAME} ${TEST_FILE} ${FILES_JSON} ${FILES_TEXT} resources.qrc)
    set_target_properties(${TEST_NAME} PROPERTIES FOLDER "vkconfig/tests")
    if(Qt6_FOUND)
      target_link_libraries(${TEST_NAME} vkconfig-core Vulkan::LayerSettings GTest::gtest GTest::gtest_main Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)
    endif()

    target_compile_definitions(${TEST_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

    if (RUN_ON_CI)
        target_compile_definitions(${TEST_NAME} PRIVATE RUN_ON_CI)
    endif()

    if(WIN32 AND (QT_TARGET_TYPE STREQUAL STATIC_LIBRARY))
      set_property(TARGET ${TEST_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

vkConfigTest(test_date)
vkConfigTest(test_util)
vkConfigTest(test_version)
vkConfigTest(test_command_line)
vkConfigTest(test_generate_settings)
vkConfigTest(test_json)
vkConfigTest(test_layer)
vkConfigTest(test_layer_manager)
vkConfigTest(test_layer_preset)
vkConfigTest(test_setting)
vkConfigTest(test_setting_type_bool)
vkConfigTest(test_setting_type_bool_numeric)
vkConfigTest(test_setting_type_enum)
vkConfigTest(test_setting_type_file_load)
vkConfigTest(test_setting_type_file_save)
vkConfigTest(test_setting_type_folder_save)
vkConfigTest(test_setting_type_flags)
vkConfigTest(test_setting_type_float)
vkConfigTest(test_setting_type_frames)
vkConfigTest(test_setting_type_group)
vkConfigTest(test_setting_type_int)
vkConfigTest(test_setting_type_list)
vkConfigTest(test_setting_type_string)
vkConfigTest(test_header)
vkConfigTest(test_bitfield)
vkConfigTest(test_parameter)
vkConfigTest(test_path)
vkConfigTest(test_configuration)
vkConfigTest(test_configuration_built_in)
vkConfigTest(test_configuration_manager)
vkConfigTest(test_configurator)
vkConfigTest(test_executable)
vkConfigTest(test_executable_manager)
vkConfigTest(test_override)
vkConfigTest(test_application_singleton)
vkConfigTest(test_type_hide_message)
vkConfigTest(test_type_layer_control)
vkConfigTest(test_type_layer_type)
vkConfigTest(test_type_log)
vkConfigTest(test_type_platform)
vkConfigTest(test_type_status)
vkConfigTest(test_type_tab)
vkConfigTest(test_ui)
vkConfigTest(test_vulkan)



