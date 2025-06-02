# ~~~
# Copyright (c) 2023-2023 Valve Corporation
# Copyright (c) 2023-2023 LunarG, Inc.
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
if (ANDROID)
    return()
endif()

function(LayerTest NAME)
	set(TEST_FILENAME ./test_${NAME}.cpp)
    set(TEST_NAME test_${NAME}_layer)

    file(GLOB TEST_JSON_FILES ${CMAKE_SOURCE_DIR}/profiles/test/data/)

    add_executable(${TEST_NAME}
                   ${TEST_FILENAME}
                   layer_test_helper.h
                   layer_test_helper.cpp
                   layer_test_main.cpp
                   layer_test_framework.cpp
                   layer_test_framework.h)
    add_dependencies(${TEST_NAME} VkLayer_${NAME})
    target_link_libraries(${TEST_NAME} Vulkan::Headers Vulkan::Loader GTest::gtest GTest::gtest_main Vulkan::LayerSettings)
    target_compile_definitions(${TEST_NAME} PUBLIC LAYER_BINARY_PATH="$<TARGET_FILE_DIR:VkLayer_${NAME}>")
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    set_tests_properties(${TEST_NAME} PROPERTIES ENVIRONMENT
        "VK_LAYER_PATH=$<TARGET_FILE_DIR:VkLayer_${NAME}>"
    )

    set_target_properties(${TEST_NAME} PROPERTIES FOLDER "VkLayer_${NAME}/Test")
endfunction()

set(LAYER_TEST_FILES api_dump monitor screenshot)

foreach(test_item ${LAYER_TEST_FILES})
    # If the target doesn't exist continue.
    if (NOT TARGET "VkLayer_${test_item}")
        continue()
    endif()

    LayerTest(${test_item})
endforeach()
