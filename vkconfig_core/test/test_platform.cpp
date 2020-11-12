/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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

#include "../platform.h"

#include <gtest/gtest.h>

#include <cstring>

TEST(test_platform, status_type) {
    EXPECT_STREQ("STABLE", GetToken(STATUS_STABLE));
    EXPECT_EQ(STATUS_STABLE, GetStatusType("STABLE"));
}

TEST(test_platform, platform_string) {
    EXPECT_TRUE(std::strcmp("vulkaninfoSDK", GetPlatformString(PLATFORM_STRING_VULKAN_INFO)) == 0 ||
                std::strcmp("vulkaninfo", GetPlatformString(PLATFORM_STRING_VULKAN_INFO)) == 0 ||
                std::strcmp("/usr/local/bin/vulkaninfo", GetPlatformString(PLATFORM_STRING_VULKAN_INFO)) == 0);
}
