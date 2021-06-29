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

inline SettingMetaBool* InstantiateBool(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaBool*>(layer.Instantiate(layer.settings, key, SETTING_BOOL));
}

TEST(test_setting_type_bool, init) { EXPECT_EQ(SETTING_BOOL, SettingMetaBool::TYPE); }

TEST(test_setting_type_bool, meta_equal) {
    Layer layer;

    SettingMetaBool* meta0 = InstantiateBool(layer, "key");
    meta0->default_value = true;
    SettingMetaBool* meta1 = InstantiateBool(layer, "key");
    meta1->default_value = true;

    EXPECT_EQ(*meta0, *meta1);

    meta1->default_value = false;
    EXPECT_NE(*meta0, *meta1);
}

TEST(test_setting_type_bool, data_equal) {
    Layer layer;

    SettingMetaBool* meta = InstantiateBool(layer, "key");

    SettingDataBool* data0 = Instantiate<SettingDataBool>(meta);
    data0->value = true;
    EXPECT_STREQ(meta->key.c_str(), data0->key.c_str());

    SettingDataBool* data1 = Instantiate<SettingDataBool>(meta);
    data1->value = true;
    EXPECT_STREQ(meta->key.c_str(), data1->key.c_str());

    EXPECT_EQ(*data0, *data1);

    data1->value = false;
    EXPECT_NE(*data0, *data1);
}

TEST(test_setting_type_bool, value) {
    Layer layer;

    SettingMetaBool* meta = InstantiateBool(layer, "key");
    EXPECT_STREQ("false", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("false", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataBool* data0 = Instantiate<SettingDataBool>(meta);
    EXPECT_EQ(data0->value, meta->default_value);

    EXPECT_STREQ("false", data0->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("false", data0->Export(EXPORT_MODE_OVERRIDE).c_str());

    meta->default_value = true;
    EXPECT_STREQ("true", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("true", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataBool* data1 = Instantiate<SettingDataBool>(meta);
    EXPECT_EQ(data1->value, meta->default_value);

    EXPECT_STREQ("true", data1->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("true", data1->Export(EXPORT_MODE_OVERRIDE).c_str());
}
