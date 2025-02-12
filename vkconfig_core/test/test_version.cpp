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

#include "../version.h"

#include <string>

#include <gtest/gtest.h>

TEST(test_version, string) {
    const std::string version_a("1.1.130");
    const std::string version_b("1.2.145");
    const std::string version_c("1.3.298.0");
    const std::string version_d("1.4");

    EXPECT_EQ(version_a, Version(version_a.c_str()).str());
    EXPECT_EQ(version_b, Version(version_b.c_str()).str());
    EXPECT_EQ(version_c, Version(version_c.c_str()).str());
    EXPECT_EQ(version_d, Version(version_d.c_str()).str());
}

TEST(test_version, string_ctr) {
    EXPECT_EQ(Version(2, 0), Version("2"));
    EXPECT_EQ(Version(1, 4), Version("1.4"));
    EXPECT_EQ(Version(1, 1, 130), Version("1.1.130"));
    EXPECT_EQ(Version(1, 4, 304, 1), Version("1.4.304.1"));
}

TEST(test_version, compare_2) {
    const Version version_a("1.1");
    const Version version_b("1.2");
    const Version version_c("2.0");
    const Version version_d("2.1");

    EXPECT_TRUE(version_a == version_a);
    EXPECT_TRUE(version_b == version_b);
    EXPECT_TRUE(version_c == version_c);

    EXPECT_TRUE(version_a != version_b);
    EXPECT_TRUE(version_b != version_c);
    EXPECT_TRUE(version_a != version_c);
    EXPECT_TRUE(version_a != version_d);

    EXPECT_TRUE(version_a < version_b);
    EXPECT_TRUE(version_b < version_c);
    EXPECT_TRUE(version_a < version_c);
    EXPECT_TRUE(version_a < version_d);

    EXPECT_TRUE(version_a <= version_a);
    EXPECT_TRUE(version_b <= version_b);
    EXPECT_TRUE(version_c <= version_c);
    EXPECT_TRUE(version_d <= version_d);

    EXPECT_TRUE(version_a <= version_b);
    EXPECT_TRUE(version_b <= version_c);
    EXPECT_TRUE(version_b <= version_d);
    EXPECT_TRUE(version_c <= version_d);

    EXPECT_TRUE(version_b > version_a);
    EXPECT_TRUE(version_c > version_b);
    EXPECT_TRUE(version_c > version_a);
    EXPECT_TRUE(version_d > version_b);

    EXPECT_TRUE(version_a >= version_a);
    EXPECT_TRUE(version_b >= version_b);
    EXPECT_TRUE(version_c >= version_c);
    EXPECT_TRUE(version_d >= version_d);

    EXPECT_TRUE(version_b >= version_a);
    EXPECT_TRUE(version_c >= version_b);
    EXPECT_TRUE(version_c >= version_a);
    EXPECT_TRUE(version_d >= version_b);
}

TEST(test_version, compare_3) {
    const Version version_a("1.1.130");
    const Version version_b("1.2.135");
    const Version version_c("1.2.145");

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

TEST(test_version, compare_4) {
    const Version version_a("1.2.135.1");
    const Version version_b("1.2.145.0");
    const Version version_c("1.2.145.1");

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

TEST(test_version, compare_mix) {
    const Version version_a("1.1");
    const Version version_b("1.1.135");
    const Version version_c("1.1.135.1");

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
