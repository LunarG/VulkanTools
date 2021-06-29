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

#include "../setting_int.h"
#include "../layer.h"

#include <gtest/gtest.h>

inline SettingMetaInt* InstantiateInt(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaInt*>(layer.Instantiate(layer.settings, key, SETTING_INT));
}

TEST(test_setting_type_int, init) { EXPECT_EQ(SETTING_INT, SettingMetaInt::TYPE); }

TEST(test_setting_type_int, meta_equal) {
    Layer layer;

    SettingMetaInt* meta0 = InstantiateInt(layer, "key");
    meta0->default_value = 76;
    SettingMetaInt* meta1 = InstantiateInt(layer, "key");
    meta1->default_value = 76;
    EXPECT_EQ(*meta0, *meta1);

    meta1->default_value = 75;
    EXPECT_NE(*meta0, *meta1);
}

TEST(test_setting_type_int, data_equal) {
    Layer layer;

    SettingMetaInt* meta = InstantiateInt(layer, "key");

    SettingDataInt* data0 = Instantiate<SettingDataInt>(meta);
    EXPECT_STREQ(meta->key.c_str(), data0->key.c_str());

    SettingDataInt* data1 = Instantiate<SettingDataInt>(meta);
    EXPECT_STREQ(meta->key.c_str(), data0->key.c_str());

    EXPECT_EQ(*data0, *data1);

    data0->value = 76;
    EXPECT_NE(*data0, *data1);

    data1->value = 76;
    EXPECT_EQ(*data0, *data1);

    SettingMetaInt* metaX = InstantiateInt(layer, "keyX");
    SettingDataInt* dataX = Instantiate<SettingDataInt>(metaX);
    dataX->value = 76;

    EXPECT_NE(*data0, *dataX);
}

TEST(test_setting_type_int, value) {
    Layer layer;

    SettingMetaInt* meta = InstantiateInt(layer, "key");
    EXPECT_STREQ("0", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("0", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataInt* dataA = Instantiate<SettingDataInt>(meta);
    EXPECT_EQ(dataA->value, meta->default_value);

    EXPECT_STREQ("0", dataA->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("0", dataA->Export(EXPORT_MODE_OVERRIDE).c_str());

    meta->default_value = 176;
    EXPECT_STREQ("176", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("176", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataInt* dataB = Instantiate<SettingDataInt>(meta);
    EXPECT_EQ(dataB->value, meta->default_value);

    EXPECT_STREQ("176", dataB->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("176", dataB->Export(EXPORT_MODE_OVERRIDE).c_str());
}
