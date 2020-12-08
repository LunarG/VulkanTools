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

#include "../util.h"
#include "../platform.h"

#include <array>

#include <QDir>

#include <gtest/gtest.h>

TEST(test_util, countof_int_2) {
    const int test_data[]{8, 9};
    static_assert(countof(test_data) == 2, "countof for 2 ints is broken");

    EXPECT_EQ(2, countof(test_data));
}

TEST(test_util, countof_int_1) {
    const int test_data[]{7};
    static_assert(countof(test_data) == 1, "countof for 1 int is broken");

    EXPECT_EQ(1, countof(test_data));
}

TEST(test_util, countof_cstring_3) {
    const char* test_data[]{"GNI", "GNA", "GNE"};
    static_assert(countof(test_data) == 3, "countof for cstring is broken");

    EXPECT_EQ(3, countof(test_data));
}

TEST(test_util, countof_string_3) {
    const std::string test_data[]{"GNI", "GNA", "GNE"};
    static_assert(countof(test_data) == 3, "countof for string is broken");

    EXPECT_EQ(3, countof(test_data));
}

TEST(test_util, countof_array_2) {
    const std::array<int, 2> test_data{6, 7};
    static_assert(countof(test_data) == 2, "countof for array is broken");

    EXPECT_EQ(2, countof(test_data));
}

TEST(test_util, countof_array_3) {
    const std::array<std::string, 3> test_data{"GNI", "GNA", "GNE"};

    EXPECT_EQ(3, countof(test_data));
}

TEST(test_util, countof_vector_2) {
    const std::vector<int> test_data{6, 7};

    EXPECT_EQ(2, countof(test_data));
}

TEST(test_util, countof_vector_3) {
    const std::vector<std::string> test_data{"GNI", "GNA", "GNE"};

    EXPECT_EQ(3, countof(test_data));
}

TEST(test_util_format, int_1) { EXPECT_EQ("Test 1", format("Test %d", 1)); }

TEST(test_util_format, delimited_string) {
    std::string delimited_string;

    AppendString(delimited_string, "A");
    EXPECT_STREQ("A", delimited_string.c_str());

    AppendString(delimited_string, "A");  // A was already added to the delimited string and can only exist once
    EXPECT_STREQ("A", delimited_string.c_str());

    RemoveString(delimited_string, "B");  // B doesn't exist in delimited_string
    EXPECT_STREQ("A", delimited_string.c_str());

    AppendString(delimited_string, "B");
    EXPECT_STREQ("A,B", delimited_string.c_str());

    AppendString(delimited_string, "C");
    EXPECT_STREQ("A,B,C", delimited_string.c_str());

    RemoveString(delimited_string, "B");
    EXPECT_STREQ("A,C", delimited_string.c_str());

    RemoveString(delimited_string, "C");
    EXPECT_STREQ("A", delimited_string.c_str());

    RemoveString(delimited_string, "A");
    EXPECT_STREQ("", delimited_string.c_str());
}

TEST(test_util_format, find) {
    struct Element {
        std::string key;
    };

    std::vector<Element> container;
    EXPECT_EQ(nullptr, FindByKey(container, "D"));

    Element elementA;
    elementA.key = "A";
    Element elementB;
    elementB.key = "B";
    Element elementC;
    elementC.key = "C";

    container.push_back(elementA);
    container.push_back(elementB);
    container.push_back(elementC);

    EXPECT_STREQ("A", FindByKey(container, "A")->key.c_str());
    EXPECT_STREQ("B", FindByKey(container, "B")->key.c_str());
    EXPECT_STREQ("C", FindByKey(container, "C")->key.c_str());
    EXPECT_EQ(nullptr, FindByKey(container, "D"));

    EXPECT_STREQ("A", FindItByKey(container, "A")->key.c_str());
    EXPECT_STREQ("B", FindItByKey(container, "B")->key.c_str());
    EXPECT_STREQ("C", FindItByKey(container, "C")->key.c_str());
    EXPECT_EQ(container.end(), FindItByKey(container, "D"));

    EXPECT_EQ(true, IsFound(container, "A"));
    EXPECT_EQ(true, IsFound(container, "B"));
    EXPECT_EQ(true, IsFound(container, "C"));
    EXPECT_EQ(false, IsFound(container, "D"));
}
