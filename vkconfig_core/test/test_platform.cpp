/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

TEST(test_platform, platform_flags_one) {
    std::vector<std::string> platform_strings;
    platform_strings.push_back(GetToken(PLATFORM_LINUX));

    EXPECT_EQ(PLATFORM_LINUX_BIT, GetPlatformFlags(platform_strings));
}

TEST(test_platform, platform_flags_some) {
    std::vector<std::string> platform_strings;
    platform_strings.push_back(GetToken(PLATFORM_LINUX));
    platform_strings.push_back(GetToken(PLATFORM_MACOS));

    EXPECT_EQ(PLATFORM_LINUX_BIT | PLATFORM_MACOS_BIT, GetPlatformFlags(platform_strings));
}

TEST(test_platform, platform_flags_all) {
    std::vector<std::string> platform_strings;
    for (std::size_t i = 0, n = PLATFORM_COUNT; i < n; ++i) {
        const PlatformType platform_type = static_cast<PlatformType>(i);
        platform_strings.push_back(GetToken(platform_type));
    }

    EXPECT_EQ(PLATFORM_DESKTOP_BIT | PLATFORM_ANDROID_BIT, GetPlatformFlags(platform_strings));
}

TEST(test_platform, platform_to_tokens) {
    EXPECT_EQ(0, GetPlatformFlags(GetPlatformTokens(0)));
    EXPECT_EQ(PLATFORM_LINUX_BIT, GetPlatformFlags(GetPlatformTokens(PLATFORM_LINUX_BIT)));
    EXPECT_EQ(PLATFORM_DESKTOP_BIT, GetPlatformFlags(GetPlatformTokens(PLATFORM_DESKTOP_BIT)));
}

TEST(test_platform, platform_type_windows) {
    EXPECT_STREQ("WINDOWS", GetToken(PLATFORM_WINDOWS));
    EXPECT_EQ(PLATFORM_WINDOWS, GetPlatformType("WINDOWS"));
}

TEST(test_platform, platform_type_linux) {
    EXPECT_STREQ("LINUX", GetToken(PLATFORM_LINUX));
    EXPECT_EQ(PLATFORM_LINUX, GetPlatformType("LINUX"));
}

TEST(test_platform, platform_type_macos) {
    EXPECT_STREQ("MACOS", GetToken(PLATFORM_MACOS));
    EXPECT_EQ(PLATFORM_MACOS, GetPlatformType("MACOS"));
}

TEST(test_platform, platform_type_android) {
    EXPECT_STREQ("ANDROID", GetToken(PLATFORM_ANDROID));
    EXPECT_EQ(PLATFORM_ANDROID, GetPlatformType("ANDROID"));
}

TEST(test_platform, status_type) {
    EXPECT_STREQ("STABLE", GetToken(STATUS_STABLE));
    EXPECT_EQ(STATUS_STABLE, GetStatusType("STABLE"));
}
