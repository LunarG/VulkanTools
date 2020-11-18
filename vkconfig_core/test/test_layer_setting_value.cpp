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

#include "../layer_setting_value.h"

#include <gtest/gtest.h>

TEST(test_setting_value, equal) {
    SettingValue setting_a;
    SettingValue setting_b;

    EXPECT_EQ(setting_a, setting_b);
}

TEST(test_setting_value, nequal) {
    SettingValue setting_a;
    SettingValue setting_b;
    setting_b.Append("gni");

    EXPECT_NE(setting_a, setting_b);
}

TEST(test_setting_value, string) {
    SettingValue setting_a;
    setting_a.Append("string");

    SettingValue setting_b;
    setting_b.Append("string");

    EXPECT_EQ(setting_a, setting_b);
}

TEST(test_setting_value, int) {
    SettingValue setting_a;
    setting_a.Append(76);

    SettingValue setting_b;
    setting_b.Append(76);

    EXPECT_EQ(setting_a, setting_b);
}

TEST(test_setting_value, bool) {
    SettingValue setting_a;
    setting_a.Append(true);

    SettingValue setting_b;
    setting_b.Append(true);

    EXPECT_EQ(setting_a, setting_b);
}

TEST(test_setting_value, remove_done) {
    SettingValue default_value;
    default_value.Append("string");

    EXPECT_EQ(true, default_value.Remove("string"));
    EXPECT_TRUE(default_value.Empty());
}

TEST(test_setting_value, remove_fail) {
    SettingValue default_value;
    default_value.Append("stringA");
    default_value.Append("stringB");

    EXPECT_EQ(false, default_value.Remove("string"));
    EXPECT_EQ(2, default_value.Size());
}

TEST(test_setting_value, remove_fail_empty) {
    SettingValue default_value;

    EXPECT_EQ(false, default_value.Remove("string"));
    EXPECT_TRUE(default_value.Empty());
}

TEST(test_setting_value, load_string) {
    const SettingValue setting_value_ctor("key", "value");

    QJsonObject json_object;
    json_object.insert("key", "value");
    SettingValue setting_value_load;
    const bool result = setting_value_load.Load("key", SETTING_STRING, json_object);

    EXPECT_TRUE(result);
    EXPECT_EQ(setting_value_ctor, setting_value_load);
}

TEST(test_setting_value, save_string) {
    const SettingValue setting_value_ctor("key", "value");

    QJsonObject json_object;
    SettingValue setting_value_save = setting_value_ctor;
    const bool result_save = setting_value_save.Save("key", SETTING_STRING, json_object);

    EXPECT_TRUE(result_save);
    EXPECT_EQ(setting_value_ctor, setting_value_save);

    SettingValue setting_value_load;
    const bool result_load = setting_value_load.Load("key", SETTING_STRING, json_object);

    EXPECT_TRUE(result_load);
    EXPECT_EQ(setting_value_ctor, setting_value_load);
}

TEST(test_setting_value, load_int) {
    const SettingValue setting_value_ctor("key", 76);

    QJsonObject json_object;
    json_object.insert("key", 76);
    SettingValue setting_value_load;
    const bool result = setting_value_load.Load("key", SETTING_INT, json_object);

    EXPECT_TRUE(result);
    EXPECT_EQ(setting_value_ctor, setting_value_load);
}

TEST(test_setting_value, save_int) {
    const SettingValue setting_value_ctor("key", 76);

    QJsonObject json_object;
    SettingValue setting_value_save = setting_value_ctor;
    const bool result_save = setting_value_save.Save("key", SETTING_INT, json_object);

    EXPECT_TRUE(result_save);
    EXPECT_EQ(setting_value_ctor, setting_value_save);

    SettingValue setting_value_load;
    const bool result_load = setting_value_load.Load("key", SETTING_INT, json_object);

    EXPECT_TRUE(result_load);
    EXPECT_EQ(setting_value_ctor, setting_value_load);
}

TEST(test_setting_value, load_bool) {
    const SettingValue setting_value_ctor("key", true);

    QJsonObject json_object;
    json_object.insert("key", true);
    SettingValue setting_value_load;
    const bool result = setting_value_load.Load("key", SETTING_BOOL, json_object);

    EXPECT_TRUE(result);
    EXPECT_EQ(setting_value_ctor, setting_value_load);
}

TEST(test_setting_value, save_bool) {
    const SettingValue setting_value_ctor("key", true);

    QJsonObject json_object;
    SettingValue setting_value_save = setting_value_ctor;
    const bool result_save = setting_value_save.Save("key", SETTING_BOOL, json_object);

    EXPECT_TRUE(result_save);
    EXPECT_EQ(setting_value_ctor, setting_value_save);

    SettingValue setting_value_load;
    const bool result_load = setting_value_load.Load("key", SETTING_BOOL, json_object);

    EXPECT_TRUE(result_load);
    EXPECT_EQ(setting_value_ctor, setting_value_load);
}
