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

#include "../type_platform.h"

#include <gtest/gtest.h>

#include <cstring>

TEST(test_platform, platform_flags_one) {
    std::vector<std::string> platform_strings = GetPlatformTokens(PLATFORM_LINUX_BIT);
    int flags = GetPlatformFlags(platform_strings);
    EXPECT_EQ(PLATFORM_LINUX_BIT, flags);
}

TEST(test_platform, platform_flags_some) {
    std::vector<std::string> platform_strings = GetPlatformTokens(PLATFORM_LINUX_BIT | PLATFORM_MACOS_BIT);
    int flags = GetPlatformFlags(platform_strings);
    EXPECT_EQ(PLATFORM_LINUX_BIT | PLATFORM_MACOS_BIT, flags);
}

TEST(test_platform, platform_flags_all) {
    std::vector<std::string> platform_strings = GetPlatformTokens(PLATFORM_DESKTOP_BIT | PLATFORM_MOBILE_BIT);
    int flags = GetPlatformFlags(platform_strings);
    EXPECT_EQ(PLATFORM_DESKTOP_BIT | PLATFORM_MOBILE_BIT, flags);
}

TEST(test_platform, platform_to_tokens) {
    EXPECT_EQ(0, GetPlatformFlags(GetPlatformTokens(0)));
    EXPECT_EQ(PLATFORM_LINUX_BIT, GetPlatformFlags(GetPlatformTokens(PLATFORM_LINUX_BIT)));
    EXPECT_EQ(PLATFORM_DESKTOP_BIT, GetPlatformFlags(GetPlatformTokens(PLATFORM_DESKTOP_BIT)));
}
