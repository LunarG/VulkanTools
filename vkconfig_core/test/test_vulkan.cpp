/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

#include "../vulkan_info.h"

#include <gtest/gtest.h>

TEST(test_vulkan, string) {
    const VulkanSystemInfo& vulkan_system_info = BuildVulkanSystemInfo();

    const std::string version_a("1.1.130");
    const std::string version_b("1.2.145");

    EXPECT_EQ(version_a, Version(version_a.c_str()).str());
    EXPECT_EQ(version_b, Version(version_b.c_str()).str());
}
