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

#include "../json.h"

#include <QJsonArray>

#include <gtest/gtest.h>

TEST(test_json, read_object) {
    QJsonObject json_object_expected;
    json_object_expected.insert("object_key", "object_value");

    QJsonArray json_array_expected;
    json_array_expected.append(json_object_expected);
    json_array_expected.append(json_object_expected);

    QJsonObject json_root;
    json_root.insert("key", json_array_expected);

    QJsonArray json_array = ReadArray(json_root, "key");

    EXPECT_EQ(json_array_expected, json_array);
}

TEST(test_json, read_array) {
    QJsonObject json_object_expected;
    json_object_expected.insert("object_key", "object_value");

    QJsonObject json_root;
    json_root.insert("key", json_object_expected);

    QJsonObject json_object = ReadObject(json_root, "key");

    EXPECT_EQ(json_object_expected, json_object);
}

TEST(test_json, read_string_value) {
    QJsonObject json_object;
    json_object.insert("key", "value");

    EXPECT_STREQ("value", ReadStringValue(json_object, "key").c_str());
}

TEST(test_json, read_string_array) {
    QJsonArray json_array;
    json_array.append("value0");
    json_array.append("value1");

    QJsonObject json_object;
    json_object.insert("key", json_array);

    const std::vector<std::string> values = ReadStringArray(json_object, "key");

    EXPECT_STREQ("value0", values[0].c_str());
    EXPECT_STREQ("value1", values[1].c_str());
}

TEST(test_json, read_string_single) {
    QJsonObject json_object;
    json_object.insert("key", "value");

    EXPECT_STREQ("value", ReadString(json_object, "key").c_str());
}

TEST(test_json, read_string_multiple) {
    QJsonArray json_array;
    json_array.append("value0");
    json_array.append("value1");

    QJsonObject json_object;
    json_object.insert("key", json_array);

    EXPECT_STREQ("value0,value1", ReadString(json_object, "key").c_str());
}

TEST(test_json, read_int_value) {
    QJsonObject json_object;
    json_object.insert("key", 76);

    EXPECT_EQ(76, ReadIntValue(json_object, "key"));
}

TEST(test_json, read_bool_value_true) {
    QJsonObject json_object;
    json_object.insert("key", true);

    EXPECT_EQ(true, ReadBoolValue(json_object, "key"));
}

TEST(test_json, read_bool_value_false) {
    QJsonObject json_object;
    json_object.insert("key", false);

    EXPECT_EQ(false, ReadBoolValue(json_object, "key"));
}
