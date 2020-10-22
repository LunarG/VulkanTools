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

#include "../configuration_setting.h"

#include <QJsonArray>

#include <gtest/gtest.h>

TEST(test_configuration_setting, equal) {
    ConfigurationSetting setting_a;
    setting_a.key = "key";
    setting_a.value.push_back(true);

    ConfigurationSetting setting_b;
    setting_b.key = "key";
    setting_b.value.push_back(true);

    EXPECT_EQ(setting_a, setting_b);
}

TEST(test_configuration_setting, nequal) {
    ConfigurationSetting setting_a;
    setting_a.key = "key";
    setting_a.value.push_back(true);

    ConfigurationSetting setting_b;
    setting_b.key = "key";
    setting_b.value.push_back(false);

    EXPECT_NE(setting_a, setting_b);
}

TEST(test_configuration_setting, load_string) {
    ConfigurationSetting setting_ref;
    setting_ref.key = "key";
    setting_ref.value.push_back("string");

    QJsonObject json_object;
    json_object.insert("key", "key");
    json_object.insert("value", "string");

    ConfigurationSetting setting_str;
    setting_str.Load(SETTING_STRING, json_object);

    EXPECT_EQ(setting_ref, setting_str);
}

TEST(test_configuration_setting, save_string) {
    ConfigurationSetting setting_ref;
    setting_ref.key = "key";
    setting_ref.value.push_back("string");

    QJsonObject json_object;
    setting_ref.Save(SETTING_STRING, json_object);

    ConfigurationSetting setting_str;
    setting_str.Load(SETTING_STRING, json_object);

    EXPECT_EQ(setting_ref, setting_str);
}

TEST(test_configuration_setting, load_bool) {
    ConfigurationSetting setting_ref;
    setting_ref.key = "key";
    setting_ref.value.push_back(true);

    QJsonObject json_object;
    json_object.insert("key", "key");
    json_object.insert("value", true);

    ConfigurationSetting setting_bool;
    setting_bool.Load(SETTING_BOOL, json_object);

    EXPECT_EQ(setting_ref, setting_bool);
}

TEST(test_configuration_setting, save_bool) {
    ConfigurationSetting setting_ref;
    setting_ref.key = "key";
    setting_ref.value.push_back(true);

    QJsonObject json_object;
    setting_ref.Save(SETTING_BOOL, json_object);

    ConfigurationSetting setting_bool;
    setting_bool.Load(SETTING_BOOL, json_object);

    EXPECT_EQ(setting_ref, setting_bool);
}

TEST(test_configuration_setting, load_int) {
    ConfigurationSetting setting_ref;
    setting_ref.key = "key";
    setting_ref.value.push_back(76);

    QJsonObject json_object;
    json_object.insert("key", "key");
    json_object.insert("value", 76);

    ConfigurationSetting setting_int;
    setting_int.Load(SETTING_INT, json_object);

    EXPECT_EQ(setting_ref, setting_int);
}

TEST(test_configuration_setting, save_int) {
    ConfigurationSetting setting_ref;
    setting_ref.key = "key";
    setting_ref.value.push_back(76);

    QJsonObject json_object;
    setting_ref.Save(SETTING_INT, json_object);

    ConfigurationSetting setting_int;
    setting_int.Load(SETTING_INT, json_object);

    EXPECT_EQ(setting_ref, setting_int);
}

TEST(test_configuration_setting, load_inclusive_list) {
    ConfigurationSetting setting_ref;
    setting_ref.key = "key";
    setting_ref.value.push_back("enum_string");

    QJsonObject json_object;
    json_object.insert("key", "key");

    QJsonArray json_array;
    json_array.append("enum_string");
    json_object.insert("value", json_array);

    ConfigurationSetting setting_array;
    setting_array.Load(SETTING_INCLUSIVE_LIST, json_object);

    EXPECT_EQ(setting_ref, setting_array);
}

TEST(test_configuration_setting, save_inclusive_list) {
    ConfigurationSetting setting_ref;
    setting_ref.key = "key";
    setting_ref.value.push_back("enum_string");

    QJsonObject json_object;
    setting_ref.Save(SETTING_INCLUSIVE_LIST, json_object);

    ConfigurationSetting setting_array;
    setting_array.Load(SETTING_INCLUSIVE_LIST, json_object);

    EXPECT_EQ(setting_ref, setting_array);
}
