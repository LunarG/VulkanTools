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

TEST(test_util, format_int_1) { EXPECT_EQ("Test 1", format("Test %d", 1)); }

TEST(test_util, format_strings_list) {
    std::vector<std::string> list;

    AppendString(list, "A");
    EXPECT_TRUE(IsStringFound(list, "A"));
    EXPECT_FALSE(IsStringFound(list, "B"));
    EXPECT_EQ(1, list.size());

    AppendString(list, "A");  // A was already added to the delimited string and can only exist once
    EXPECT_TRUE(IsStringFound(list, "A"));
    EXPECT_FALSE(IsStringFound(list, "B"));
    EXPECT_EQ(1, list.size());

    RemoveString(list, "B");  // B doesn't exist in delimited_string
    EXPECT_TRUE(IsStringFound(list, "A"));
    EXPECT_FALSE(IsStringFound(list, "B"));
    EXPECT_EQ(1, list.size());

    AppendString(list, "B");
    EXPECT_TRUE(IsStringFound(list, "A"));
    EXPECT_TRUE(IsStringFound(list, "B"));
    EXPECT_EQ(2, list.size());

    AppendString(list, "C");
    EXPECT_TRUE(IsStringFound(list, "A"));
    EXPECT_TRUE(IsStringFound(list, "B"));
    EXPECT_TRUE(IsStringFound(list, "C"));
    EXPECT_EQ(3, list.size());

    RemoveString(list, "B");
    EXPECT_TRUE(IsStringFound(list, "A"));
    EXPECT_FALSE(IsStringFound(list, "B"));
    EXPECT_TRUE(IsStringFound(list, "C"));
    EXPECT_EQ(2, list.size());

    RemoveString(list, "B");
    EXPECT_TRUE(IsStringFound(list, "A"));
    EXPECT_FALSE(IsStringFound(list, "B"));
    EXPECT_TRUE(IsStringFound(list, "C"));
    EXPECT_EQ(2, list.size());

    RemoveString(list, "C");
    EXPECT_TRUE(IsStringFound(list, "A"));
    EXPECT_FALSE(IsStringFound(list, "B"));
    EXPECT_FALSE(IsStringFound(list, "C"));
    EXPECT_EQ(1, list.size());

    RemoveString(list, "A");
    EXPECT_FALSE(IsStringFound(list, "A"));
    EXPECT_FALSE(IsStringFound(list, "B"));
    EXPECT_FALSE(IsStringFound(list, "C"));
    EXPECT_EQ(0, list.size());
}

TEST(test_util, split_merge_delimiter) {
    std::vector<std::string> delimiters;
    delimiters.push_back(",");
    delimiters.push_back(";");
    delimiters.push_back(":");
    delimiters.push_back(" ");
    delimiters.push_back("< >");
    delimiters.push_back("[{()}]");

    for (std::size_t i = 0, n = delimiters.size(); i < n; ++i) {
        const std::string delimiter = delimiters[i];
        const std::string merge = format("pouet%sgni%sa%sgnu", delimiter.c_str(), delimiter.c_str(), delimiter.c_str());
        const std::vector<std::string> split = Split(merge, delimiter);

        EXPECT_EQ(4, split.size());

        EXPECT_STREQ("pouet", split[0].c_str());
        EXPECT_STREQ("gni", split[1].c_str());
        EXPECT_STREQ("a", split[2].c_str());
        EXPECT_STREQ("gnu", split[3].c_str());

        EXPECT_STREQ(merge.c_str(), Merge(split, delimiter).c_str());
    }
}

TEST(test_util, split_merge_end) {
    std::vector<std::string> delimiters;
    delimiters.push_back(",");
    delimiters.push_back(";");
    delimiters.push_back(":");
    delimiters.push_back(" ");
    delimiters.push_back("< >");
    delimiters.push_back("[{()}]");

    for (std::size_t i = 0, n = delimiters.size(); i < n; ++i) {
        const std::string delimiter = delimiters[i];
        const std::string merge = format("pouet%sgni%sa%s", delimiter.c_str(), delimiter.c_str(), delimiter.c_str());
        const std::vector<std::string> split = Split(merge, delimiter);

        EXPECT_EQ(3, split.size());

        EXPECT_STREQ("pouet", split[0].c_str());
        EXPECT_STREQ("gni", split[1].c_str());
        EXPECT_STREQ("a", split[2].c_str());
    }
}

TEST(test_util, format_find) {
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

    EXPECT_EQ(true, IsFound(container, "A"));
    EXPECT_EQ(true, IsFound(container, "B"));
    EXPECT_EQ(true, IsFound(container, "C"));
    EXPECT_EQ(false, IsFound(container, "D"));
}

TEST(test_util, to_lower_case) {
    EXPECT_STREQ("string", ToLowerCase("string").c_str());
    EXPECT_STREQ(" string", ToLowerCase(" string").c_str());
    EXPECT_STREQ(" string ", ToLowerCase(" string ").c_str());
    EXPECT_STREQ("string", ToLowerCase("sTRing").c_str());
    EXPECT_STREQ("string", ToLowerCase("String").c_str());
    EXPECT_STREQ("string76", ToLowerCase("StrinG76").c_str());
    EXPECT_STREQ("str ing", ToLowerCase("Str inG").c_str());
}

TEST(test_util, to_upper_case) {
    EXPECT_STREQ("STRING", ToUpperCase("string").c_str());
    EXPECT_STREQ(" STRING", ToUpperCase(" string").c_str());
    EXPECT_STREQ(" STRING ", ToUpperCase(" string ").c_str());
    EXPECT_STREQ("STRING", ToUpperCase("sTRing").c_str());
    EXPECT_STREQ("STRING", ToUpperCase("String").c_str());
    EXPECT_STREQ("STRING76", ToUpperCase("StrinG76").c_str());
    EXPECT_STREQ("STR ING", ToUpperCase("Str inG").c_str());
}

TEST(test_util, is_number) {
    EXPECT_EQ(true, IsNumber("0123456789"));
    EXPECT_EQ(true, IsNumber("0x1F"));
    EXPECT_EQ(true, IsNumber("-0x1F"));
    EXPECT_EQ(true, IsNumber("0x1adf"));
    EXPECT_EQ(true, IsNumber("-0x48e"));
    EXPECT_EQ(true, IsNumber("-0x3AC7e"));

    EXPECT_EQ(false, IsNumber("01234c56789"));
    EXPECT_EQ(false, IsNumber("$%#&@()-_[]{}"));
}

TEST(test_util, is_float) {
    EXPECT_EQ(true, IsFloat("1.0"));
    EXPECT_EQ(true, IsFloat("-1.0"));
    EXPECT_EQ(true, IsFloat("1"));
    EXPECT_EQ(true, IsFloat("-1"));
    EXPECT_EQ(true, IsFloat("1."));
    EXPECT_EQ(true, IsFloat("-1."));
    EXPECT_EQ(false, IsFloat("1.0f"));
    EXPECT_EQ(false, IsFloat("-1.0f"));
}

TEST(test_util, is_frames) {
    EXPECT_EQ(true, IsFrames("0"));
    EXPECT_EQ(true, IsFrames("0-2"));
    EXPECT_EQ(true, IsFrames("0,2"));
    EXPECT_EQ(true, IsFrames("0-2,6,7"));
    EXPECT_EQ(true, IsFrames("0-2,6-7"));
    EXPECT_EQ(true, IsFrames("0,2,6,7"));
    EXPECT_EQ(true, IsFrames("1-2,60,70"));
    EXPECT_EQ(true, IsFrames("10-20,60,70"));
    EXPECT_EQ(true, IsFrames("1-8-2"));
    EXPECT_EQ(true, IsFrames("1-8-2,0"));
    EXPECT_EQ(true, IsFrames("1-8-2,10-20-5"));
    EXPECT_EQ(true, IsFrames("1-8,10-20-5"));
    EXPECT_EQ(true, IsFrames("1-8-2,10-20-1"));
    EXPECT_EQ(true, IsFrames("1,2,3,4"));

    EXPECT_EQ(false, IsFrames("1,"));
    EXPECT_EQ(false, IsFrames("-1"));
    EXPECT_EQ(false, IsFrames("1-"));
    EXPECT_EQ(false, IsFrames("1--4"));
    EXPECT_EQ(false, IsFrames("1-4-"));
    EXPECT_EQ(false, IsFrames("1,,4"));
    EXPECT_EQ(false, IsFrames("1,-4"));
    EXPECT_EQ(false, IsFrames(",-76"));
    EXPECT_EQ(false, IsFrames("76,-"));
    EXPECT_EQ(false, IsFrames("76,-82"));
    EXPECT_EQ(false, IsFrames("1-8-2-1"));
}

TEST(test_util, number_or_string_ctr) {
    NumberOrString expected0;
    expected0.key = "key";
    expected0.number = 0;

    EXPECT_EQ(expected0, NumberOrString("key"));

    NumberOrString expected1;
    expected1.key = "";
    expected1.number = 76;

    EXPECT_EQ(expected1, NumberOrString("76"));

    NumberOrString expected2;
    expected2.key = "";
    expected2.number = 76;

    EXPECT_EQ(expected2, NumberOrString(76));
}

TEST(test_util, number_or_string_cmp) {
    EXPECT_TRUE(NumberOrString("B") < NumberOrString("C"));
    EXPECT_FALSE(NumberOrString("D") < NumberOrString("C"));
    EXPECT_FALSE(NumberOrString("C") < NumberOrString("C"));

    EXPECT_TRUE(NumberOrString(76) < NumberOrString(82));
    EXPECT_FALSE(NumberOrString(76) < NumberOrString(75));
    EXPECT_FALSE(NumberOrString(76) < NumberOrString(76));
}

TEST(test_util, number_or_string_list) {
    std::vector<NumberOrString> list;
    EXPECT_FALSE(IsValueFound(list, NumberOrString(76)));
    EXPECT_FALSE(IsValueFound(list, NumberOrString(75)));
    EXPECT_EQ(0, list.size());

    AppendValue(list, NumberOrString(76));
    EXPECT_TRUE(IsValueFound(list, NumberOrString(76)));
    EXPECT_FALSE(IsValueFound(list, NumberOrString(75)));
    EXPECT_EQ(1, list.size());

    AppendValue(list, NumberOrString(76));
    EXPECT_TRUE(IsValueFound(list, NumberOrString(76)));
    EXPECT_FALSE(IsValueFound(list, NumberOrString(75)));
    EXPECT_EQ(1, list.size());

    RemoveValue(list, NumberOrString(76));
    EXPECT_FALSE(IsValueFound(list, NumberOrString(76)));
    EXPECT_FALSE(IsValueFound(list, NumberOrString(75)));
    EXPECT_EQ(0, list.size());
}

TEST(test_util, enabled_number_or_string_ctr) {
    EnabledNumberOrString expected0;
    expected0.key = "key";
    expected0.number = 0;

    EXPECT_EQ(expected0, EnabledNumberOrString("key"));

    EnabledNumberOrString expected1;
    expected1.key = "";
    expected1.number = 76;

    EXPECT_EQ(expected1, EnabledNumberOrString("76"));

    EnabledNumberOrString expected2;
    expected2.key = "";
    expected2.number = 76;

    EXPECT_EQ(expected2, EnabledNumberOrString(76));
}

TEST(test_util, enabled_number_or_string_equal) {
    EnabledNumberOrString expected0A;
    expected0A.key = "key";
    expected0A.number = 0;
    expected0A.enabled = true;

    EXPECT_EQ(expected0A, EnabledNumberOrString("key"));

    EnabledNumberOrString expected0B;
    expected0B.key = "key";
    expected0B.number = 0;
    expected0B.enabled = false;

    EXPECT_EQ(expected0B, EnabledNumberOrString("key"));
    EXPECT_EQ(expected0B, expected0A);

    EnabledNumberOrString expected1A;
    expected1A.key = "";
    expected1A.number = 76;
    expected1A.enabled = true;

    EXPECT_EQ(expected1A, EnabledNumberOrString("76"));

    EnabledNumberOrString expected1B;
    expected1B.key = "";
    expected1B.number = 76;
    expected1B.enabled = false;

    EXPECT_EQ(expected1B, EnabledNumberOrString("76"));
    EXPECT_EQ(expected1B, expected1A);

    EnabledNumberOrString expected2A;
    expected2A.key = "";
    expected2A.number = 76;
    expected2A.enabled = true;

    EXPECT_EQ(expected2A, EnabledNumberOrString(76));
    EXPECT_EQ(expected2A, expected1A);

    EnabledNumberOrString expected2B;
    expected2B.key = "";
    expected2B.number = 76;
    expected2B.enabled = false;

    EXPECT_EQ(expected2B, EnabledNumberOrString(76));
    EXPECT_EQ(expected2B, expected2A);
    EXPECT_EQ(expected2B, expected1B);
}

TEST(test_util, enabled_number_or_string_cmp) {
    EXPECT_TRUE(EnabledNumberOrString("B") < EnabledNumberOrString("C"));
    EXPECT_FALSE(EnabledNumberOrString("D") < EnabledNumberOrString("C"));
    EXPECT_FALSE(EnabledNumberOrString("C") < EnabledNumberOrString("C"));

    EXPECT_TRUE(EnabledNumberOrString(76) < EnabledNumberOrString(82));
    EXPECT_FALSE(EnabledNumberOrString(76) < EnabledNumberOrString(75));
    EXPECT_FALSE(EnabledNumberOrString(76) < EnabledNumberOrString(76));
}

TEST(test_util, enabled_number_or_string_list) {
    std::vector<EnabledNumberOrString> list;
    EXPECT_FALSE(IsValueFound(list, EnabledNumberOrString(76)));
    EXPECT_EQ(0, list.size());

    list.push_back(EnabledNumberOrString(76));
    EXPECT_TRUE(IsValueFound(list, EnabledNumberOrString(76)));
    EXPECT_FALSE(IsValueFound(list, EnabledNumberOrString(75)));
    EXPECT_EQ(1, list.size());

    RemoveValue(list, EnabledNumberOrString(76));
    EXPECT_FALSE(IsValueFound(list, EnabledNumberOrString(76)));
    EXPECT_FALSE(IsValueFound(list, EnabledNumberOrString(75)));
    EXPECT_EQ(0, list.size());
}

TEST(test_util, enabled_number_or_string_convert) {
    std::vector<NumberOrString> list;
    AppendValue(list, NumberOrString(76));
    AppendValue(list, NumberOrString(75));
    AppendValue(list, NumberOrString("key"));
    AppendValue(list, NumberOrString(82));
    EXPECT_EQ(4, list.size());

    QStringList qlist = ConvertValues(list);
    EXPECT_EQ(4, qlist.size());

    QStringList expected;
    expected.push_back("76");
    expected.push_back("75");
    expected.push_back("key");
    expected.push_back("82");

    EXPECT_EQ(expected, qlist);
}
