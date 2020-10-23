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

#include "../setting_value.h"

#include <gtest/gtest.h>

TEST(test_setting_value, equal) {
    SettingValue setting_a;
    SettingValue setting_b;

    EXPECT_EQ(setting_a, setting_b);
}

TEST(test_setting_value, nequal) {
    SettingValue setting_a;
    SettingValue setting_b;
    setting_b.push_back("gni");

    EXPECT_NE(setting_a, setting_b);
}

TEST(test_setting_value, string) {
    SettingValue setting_a;
    setting_a.push_back("string");

    SettingValue setting_b;
    setting_b.push_back("string");

    EXPECT_EQ(setting_a, setting_b);
}

TEST(test_setting_value, int) {
    SettingValue setting_a;
    setting_a.push_back(76);

    SettingValue setting_b;
    setting_b.push_back(76);

    EXPECT_EQ(setting_a, setting_b);
}

TEST(test_setting_value, bool) {
    SettingValue setting_a;
    setting_a.push_back(true);

    SettingValue setting_b;
    setting_b.push_back(true);

    EXPECT_EQ(setting_a, setting_b);
}

TEST(test_setting_value, remove_done) {
    SettingValue value;
    value.push_back("string");

    EXPECT_EQ(true, value.remove("string"));
    EXPECT_TRUE(value.empty());
}

TEST(test_setting_value, remove_fail) {
    SettingValue value;
    value.push_back("stringA");
    value.push_back("stringB");

    EXPECT_EQ(false, value.remove("string"));
    EXPECT_EQ(2, value.size());
}

TEST(test_setting_value, remove_fail_empty) {
    SettingValue value;

    EXPECT_EQ(false, value.remove("string"));
    EXPECT_TRUE(value.empty());
}
