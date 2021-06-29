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

inline SettingMetaFlags* InstantiateFlags(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaFlags*>(layer.Instantiate(layer.settings, key, SETTING_FLAGS));
}

TEST(test_setting_type_flags, init) { EXPECT_EQ(SETTING_FLAGS, SettingMetaFlags::TYPE); }

TEST(test_setting_type_flags, meta_equal) {
    Layer layer;

    SettingMetaFlags* meta0 = InstantiateFlags(layer, "key");
    SettingMetaFlags* meta1 = InstantiateFlags(layer, "key");

    EXPECT_EQ(*meta0, *meta1);

    std::vector<std::string> valueX;
    valueX.push_back("valueA");
    valueX.push_back("valueB");

    meta0->default_value = valueX;
    EXPECT_NE(*meta0, *meta1);

    meta1->default_value = valueX;
    EXPECT_EQ(*meta0, *meta1);

    std::vector<std::string> valueY;
    valueY.push_back("valueB");
    valueY.push_back("valueC");

    meta1->default_value = valueY;
    EXPECT_NE(*meta0, *meta1);

    std::vector<std::string> valueZ;
    valueZ.push_back("valueB");
    valueZ.push_back("valueA");

    meta1->default_value = valueZ;
    EXPECT_EQ(*meta0, *meta1);
}

TEST(test_setting_type_flags, data_equal) {
    Layer layer;

    SettingMetaFlags* metaA = InstantiateFlags(layer, "keyA");
    SettingMetaFlags* metaB = InstantiateFlags(layer, "keyB");
    EXPECT_NE(*metaA, *metaB);

    std::vector<std::string> valueX;
    valueX.push_back("valueA");
    valueX.push_back("valueB");

    std::vector<std::string> valueY;
    valueY.push_back("valueB");
    valueY.push_back("valueC");

    SettingDataFlags* data0 = Instantiate<SettingDataFlags>(metaA);
    EXPECT_STREQ(metaA->key.c_str(), data0->key.c_str());

    SettingDataFlags* data1 = Instantiate<SettingDataFlags>(metaA);
    EXPECT_STREQ(metaA->key.c_str(), data1->key.c_str());

    EXPECT_EQ(*data0, *data1);

    data0->value = valueX;
    EXPECT_NE(*data0, *data1);

    data1->value = valueX;
    EXPECT_EQ(*data0, *data1);

    data1->value = valueY;
    EXPECT_NE(*data0, *data1);
}

TEST(test_setting_type_flags, value) {
    Layer layer;

    SettingMetaFlags* meta = InstantiateFlags(layer, "key");
    meta->default_value.push_back("A");
    meta->default_value.push_back("B");
    EXPECT_STREQ("A,B", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("A,B", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataFlags* dataA = Instantiate<SettingDataFlags>(meta);
    EXPECT_EQ(dataA->value, meta->default_value);

    EXPECT_STREQ("A,B", dataA->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("A,B", dataA->Export(EXPORT_MODE_OVERRIDE).c_str());

    meta->default_value.clear();
    meta->default_value.push_back("C");
    meta->default_value.push_back("D");
    EXPECT_STREQ("C,D", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("C,D", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataFlags* dataC = Instantiate<SettingDataFlags>(meta);
    EXPECT_EQ(dataC->value, meta->default_value);

    EXPECT_STREQ("C,D", dataC->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("C,D", dataC->Export(EXPORT_MODE_OVERRIDE).c_str());
}
