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

static_assert(SettingMetaEnum::TYPE == SETTING_ENUM, "Invalid type");

TEST(test_setting_type_enum, meta_equal) {
    Layer layer;

    SettingMetaEnum* meta0 = Instantiate<SettingMetaEnum>(layer, "key");
    meta0->default_value = "value";
    SettingMetaEnum* meta1 = Instantiate<SettingMetaEnum>(layer, "key");
    meta1->default_value = "value";
    EXPECT_EQ(*meta0, *meta1);

    meta1->default_value = "data";
    EXPECT_NE(*meta0, *meta1);
}

TEST(test_setting_type_enum, data_equal) {
    Layer layer;

    SettingMetaEnum* meta = Instantiate<SettingMetaEnum>(layer, "key");

    SettingDataEnum* data0 = Instantiate<SettingDataEnum>(meta);
    EXPECT_EQ(SETTING_ENUM, data0->type);
    data0->value = "value";

    SettingDataEnum* data1 = Instantiate<SettingDataEnum>(meta);
    EXPECT_EQ(SETTING_ENUM, data1->type);
    data1->value = "value";

    EXPECT_EQ(*data0, *data1);
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
