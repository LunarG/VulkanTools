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

#include "../version.h"

#include <string>

#include <gtest/gtest.h>

TEST(test_version, string) {
    const std::string version_a("1.1.130");
    const std::string version_b("1.2.145");

    EXPECT_EQ(version_a, Version(version_a.c_str()).str());
    EXPECT_EQ(version_b, Version(version_b.c_str()).str());
}

TEST(test_version, string_ctr) { EXPECT_EQ(Version(1, 1, 130), Version("1.1.130")); }

TEST(test_version, compare) {
    const std::string version_a("1.1.130");
    const std::string version_b("1.2.135");
    const std::string version_c("1.2.145");

    EXPECT_TRUE(version_a == version_a);
    EXPECT_TRUE(version_b == version_b);
    EXPECT_TRUE(version_c == version_c);

    EXPECT_TRUE(version_a != version_b);
    EXPECT_TRUE(version_b != version_c);
    EXPECT_TRUE(version_a != version_c);

    EXPECT_TRUE(version_a < version_b);
    EXPECT_TRUE(version_b < version_c);
    EXPECT_TRUE(version_a < version_c);

    EXPECT_TRUE(version_a <= version_a);
    EXPECT_TRUE(version_b <= version_b);
    EXPECT_TRUE(version_c <= version_c);

    EXPECT_TRUE(version_a <= version_b);
    EXPECT_TRUE(version_b <= version_c);
    EXPECT_TRUE(version_a <= version_c);

    EXPECT_TRUE(version_b > version_a);
    EXPECT_TRUE(version_c > version_b);
    EXPECT_TRUE(version_c > version_a);

    EXPECT_TRUE(version_a >= version_a);
    EXPECT_TRUE(version_b >= version_b);
    EXPECT_TRUE(version_c >= version_c);

    EXPECT_TRUE(version_b >= version_a);
    EXPECT_TRUE(version_c >= version_b);
    EXPECT_TRUE(version_c >= version_a);
}

TEST(test_version, major) {
    const char* version_a("1.1.130");
    const char* version_b("1.2.145");

    EXPECT_EQ(1, Version(version_a).GetMajor());
    EXPECT_EQ(1, Version(version_b).GetMajor());
}

TEST(test_version, minor) {
    const char* version_a("1.1.130");
    const char* version_b("1.2.145");

    EXPECT_EQ(1, Version(version_a).GetMinor());
    EXPECT_EQ(2, Version(version_b).GetMinor());
}

TEST(test_version, patch) {
    const char* version_a("1.1.130");
    const char* version_b("1.2.145");

    EXPECT_EQ(130, Version(version_a).GetPatch());
    EXPECT_EQ(145, Version(version_b).GetPatch());
}
