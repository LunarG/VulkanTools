/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authors:
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "../configurator.h"

#include <gtest/gtest.h>

struct ApidumpSettingData {
    const char* output_range = "0-0";
    const char* output_format = "text";
    VkBool32 pre_dump = VK_FALSE;
    VkBool32 file = VK_FALSE;
    std::array<const char*, 1> log_filename = {"stdout"};
    /*
    VkBool32 flush = VK_TRUE;
    int32_t name_size = 32;
    VkBool32 show_types = VK_TRUE;
    int32_t type_size = 0;
    VkBool32 timestamp = VK_FALSE;
    VkBool32 show_shader = VK_FALSE;
    VkBool32 detailed = VK_TRUE;
    VkBool32 no_addr = VK_FALSE;
    VkBool32 use_spaces = VK_TRUE;
*/
    std::array<int32_t, 2> indent_size = {4, 8};
    std::array<VkBool32, 1> show_thread_and_frame = {VK_TRUE};
};

TEST(test_export, get) {
    Configurator& configurator = Configurator::Get();

    EXPECT_EQ(configurator.loader_log_enabled, false);
    EXPECT_EQ(configurator.loader_log_messages_flags, GetBit(LOG_ERROR));
}
