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

#include "../setting_bool.h"
#include "../layer.h"

#include <gtest/gtest.h>

inline SettingMetaBoolNumeric* InstantiateBoolNum(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaBoolNumeric*>(layer.Instantiate(layer.settings, key, SETTING_BOOL_NUMERIC_DEPRECATED));
}

TEST(test_setting_type_bool_num, init) { EXPECT_EQ(SETTING_BOOL_NUMERIC_DEPRECATED, SettingMetaBoolNumeric::TYPE); }

TEST(test_setting_type_bool_num, meta_equal) {
    Layer layer;

    SettingMetaBoolNumeric* meta0 = InstantiateBoolNum(layer, "key");
    meta0->default_value = true;
    SettingMetaBoolNumeric* meta1 = InstantiateBoolNum(layer, "key");
    meta1->default_value = true;

    EXPECT_EQ(*meta0, *meta1);

    meta1->default_value = false;
    EXPECT_NE(*meta0, *meta1);
}

TEST(test_setting_type_bool_num, data_equal) {
    Layer layer;

    SettingMetaBoolNumeric* meta = InstantiateBoolNum(layer, "key");

    SettingDataBoolNumeric* data0 = Instantiate<SettingDataBoolNumeric>(meta);
    data0->value = true;
    EXPECT_STREQ(meta->key.c_str(), data0->key.c_str());

    SettingDataBoolNumeric* data1 = Instantiate<SettingDataBoolNumeric>(meta);
    data1->value = true;
    EXPECT_STREQ(meta->key.c_str(), data1->key.c_str());

    EXPECT_EQ(*data0, *data1);

    data1->value = false;
    EXPECT_NE(*data0, *data1);
}

TEST(test_setting_type_bool_num, value) {
    Layer layer;

    SettingMetaBoolNumeric* meta = InstantiateBoolNum(layer, "key");
    EXPECT_STREQ("0", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("0", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataBoolNumeric* data0 = Instantiate<SettingDataBoolNumeric>(meta);
    EXPECT_EQ(data0->value, meta->default_value);

    EXPECT_STREQ("0", data0->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("0", data0->Export(EXPORT_MODE_OVERRIDE).c_str());

    meta->default_value = true;
    EXPECT_STREQ("1", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("1", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataBoolNumeric* data1 = Instantiate<SettingDataBoolNumeric>(meta);
    EXPECT_EQ(data1->value, meta->default_value);

    EXPECT_STREQ("1", data1->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("1", data1->Export(EXPORT_MODE_OVERRIDE).c_str());
}
