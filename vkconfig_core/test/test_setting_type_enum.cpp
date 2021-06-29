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

#include "../setting_flags.h"
#include "../layer.h"

#include <gtest/gtest.h>

inline SettingMetaEnum* InstantiateEnum(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaEnum*>(layer.Instantiate(layer.settings, key, SETTING_ENUM));
}

TEST(test_setting_type_enum, init) { EXPECT_EQ(SETTING_ENUM, SettingMetaEnum::TYPE); }

TEST(test_setting_type_enum, meta_equal) {
    Layer layer;

    SettingMetaEnum* meta0 = InstantiateEnum(layer, "key");
    meta0->default_value = "value";
    SettingMetaEnum* meta1 = InstantiateEnum(layer, "key");
    meta1->default_value = "value";
    EXPECT_EQ(*meta0, *meta1);

    meta1->default_value = "data";
    EXPECT_NE(*meta0, *meta1);
}

TEST(test_setting_type_enum, data_equal) {
    Layer layer;

    SettingMetaEnum* meta = InstantiateEnum(layer, "key");

    SettingDataEnum* data0 = Instantiate<SettingDataEnum>(meta);
    EXPECT_EQ(SETTING_ENUM, data0->type);
    data0->value = "value";

    SettingDataEnum* data1 = Instantiate<SettingDataEnum>(meta);
    EXPECT_EQ(SETTING_ENUM, data1->type);
    data1->value = "value";

    EXPECT_EQ(*data0, *data1);
}

TEST(test_setting_type_enum, value) {
    Layer layer;

    SettingMetaEnum* meta = InstantiateEnum(layer, "key");
    meta->default_value = "A";
    EXPECT_STREQ("A", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("A", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataEnum* dataA = Instantiate<SettingDataEnum>(meta);
    EXPECT_EQ(dataA->value, meta->default_value);

    EXPECT_STREQ("A", dataA->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("A", dataA->Export(EXPORT_MODE_OVERRIDE).c_str());

    meta->default_value = "B";
    EXPECT_STREQ("B", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("B", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataEnum* dataB = Instantiate<SettingDataEnum>(meta);
    EXPECT_EQ(dataB->value, meta->default_value);

    EXPECT_STREQ("B", dataB->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("B", dataB->Export(EXPORT_MODE_OVERRIDE).c_str());
}

TEST(test_setting_type_enum, is_supported_enum_value_default) {
    SettingEnumValue value;

    EXPECT_EQ(true, IsSupported(&value));
}

TEST(test_setting_type_enum, is_supported_enum_value_null) {
    const SettingEnumValue* value = nullptr;

    EXPECT_EQ(false, IsSupported(value));
}

TEST(test_setting_type_enum, is_supported_enum_value_hidden) {
    SettingEnumValue value;
    value.view = SETTING_VIEW_HIDDEN;

    EXPECT_EQ(false, IsSupported(&value));
}

TEST(test_setting_type_enum, is_supported_enum_value_platform) {
    SettingEnumValue value;
    value.platform_flags = 0;

    EXPECT_EQ(false, IsSupported(&value));
}
