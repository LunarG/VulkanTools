# ~~~
# Copyright (c) 2026 LunarG, Inc.
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

# Unit tests of the monitor layer. They link the layer's static library and don't need a Vulkan driver, so they also run
# on GitHub's C.I.
set(LAYER_TEST_MAIN "${PROJECT_SOURCE_DIR}/layersvt/test/layer_test_main.cpp")

function(MonitorTest NAME)
    add_executable(${NAME} ${NAME}.cpp ${LAYER_TEST_MAIN})
    target_link_libraries(${NAME} VkLayer_monitor_core GTest::gtest)
    add_test(NAME ${NAME} COMMAND ${NAME})
    set_target_properties(${NAME} PROPERTIES FOLDER "layers/monitor/Test")
endfunction()

MonitorTest(test_monitor_overlay)
target_compile_definitions(test_monitor_overlay PRIVATE MONITOR_SHADER_DIR="${PROJECT_SOURCE_DIR}/layersvt/shaders")

# Swapchain and present plumbing against a fake device
MonitorTest(test_monitor_present)

# The layer's entry points, without a loader or driver
MonitorTest(test_monitor_entry_points)
