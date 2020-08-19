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
 * - Richard S. Wright Jr.
 * - Christophe Riccio
 */

#include "../util.h"

#include <array>

#include <gtest/gtest.h>

TEST(test_util_countof, int_2) {
    const int test_data[]{8, 9};
    static_assert(countof(test_data) == 2, "countof for 2 ints is broken");

    EXPECT_EQ(2, countof(test_data));
}

TEST(test_util_countof, int_1) {
    const int test_data[]{7};
    static_assert(countof(test_data) == 1, "countof for 1 int is broken");

    EXPECT_EQ(1, countof(test_data));
}

TEST(test_util_countof, cstring_3) {
    const char* test_data[]{"GNI", "GNA", "GNE"};
    static_assert(countof(test_data) == 3, "countof for cstring is broken");

    EXPECT_EQ(3, countof(test_data));
}

TEST(test_util_countof, string_3) {
    const std::string test_data[]{"GNI", "GNA", "GNE"};
    static_assert(countof(test_data) == 3, "countof for string is broken");

    EXPECT_EQ(3, countof(test_data));
}

TEST(test_util_countof, array_2) {
    const std::array<int, 2> test_data{6, 7};
    static_assert(countof(test_data) == 2, "countof for array is broken");

    EXPECT_EQ(2, countof(test_data));
}

TEST(test_util_countof, array_3) {
    const std::array<std::string, 3> test_data{"GNI", "GNA", "GNE"};

    EXPECT_EQ(3, countof(test_data));
}

TEST(test_util_countof, vector_2) {
    const std::vector<int> test_data{6, 7};

    EXPECT_EQ(2, countof(test_data));
}

TEST(test_util_countof, vector_3) {
    const std::vector<std::string> test_data{"GNI", "GNA", "GNE"};

    EXPECT_EQ(3, countof(test_data));
}

TEST(test_util_format, int_1) { EXPECT_EQ("Test 1", format("Test %d", 1)); }
