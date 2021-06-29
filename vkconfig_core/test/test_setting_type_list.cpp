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

#include "../setting_list.h"
#include "../layer.h"

#include <gtest/gtest.h>

inline SettingMetaList* InstantiateList(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaList*>(layer.Instantiate(layer.settings, key, SETTING_LIST));
}

TEST(test_setting_type_list, init) { EXPECT_EQ(SETTING_LIST, SettingMetaList::TYPE); }

TEST(test_setting_type_list, meta_equal) {
    Layer layer;

    SettingMetaList* meta0 = InstantiateList(layer, "key");
    SettingMetaList* meta1 = InstantiateList(layer, "key");
    EXPECT_EQ(*meta0, *meta1);
}

TEST(test_setting_type_list, data_equal) {
    Layer layer;

    SettingMetaList* meta = InstantiateList(layer, "key");

    SettingDataList* data0 = Instantiate<SettingDataList>(meta);
    EXPECT_STREQ(meta->key.c_str(), data0->key.c_str());

    SettingDataList* data1 = Instantiate<SettingDataList>(meta);
    EXPECT_STREQ(meta->key.c_str(), data0->key.c_str());

    EXPECT_EQ(*data0, *data1);
}

TEST(test_setting_type_list, value) {
    EnabledNumberOrString A("A");
    EnabledNumberOrString B("B");
    B.enabled = true;
    EnabledNumberOrString C("C");
    C.enabled = false;

    EnabledNumberOrString D("D");
    EnabledNumberOrString E("E");
    E.enabled = true;
    EnabledNumberOrString F("F");
    F.enabled = false;

    Layer layer;

    SettingMetaList* meta = InstantiateList(layer, "key");
    meta->default_value.push_back(A);
    meta->default_value.push_back(B);
    meta->default_value.push_back(C);
    EXPECT_STREQ("A,B", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("A,B", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataList* dataA = Instantiate<SettingDataList>(meta);
    EXPECT_EQ(dataA->value, meta->default_value);

    EXPECT_STREQ("A,B", dataA->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("A,B", dataA->Export(EXPORT_MODE_OVERRIDE).c_str());

    meta->default_value.clear();
    meta->default_value.push_back(D);
    meta->default_value.push_back(E);
    meta->default_value.push_back(F);
    EXPECT_STREQ("D,E", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("D,E", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataList* dataC = Instantiate<SettingDataList>(meta);
    EXPECT_EQ(dataC->value, meta->default_value);

    EXPECT_STREQ("D,E", dataC->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("D,E", dataC->Export(EXPORT_MODE_OVERRIDE).c_str());
}

void LoadVUIDs(std::vector<NumberOrString>& value);

TEST(test_setting_type_list, validation_list) {
    std::vector<NumberOrString> list;
    LoadVUIDs(list);

    EXPECT_TRUE(!list.empty());
}
