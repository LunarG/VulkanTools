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

static_assert(SettingMetaList::TYPE == SETTING_LIST, "Invalid type");

TEST(test_setting_type_list, meta_equal) {
    Layer layer;

    SettingMetaList* meta0 = Instantiate<SettingMetaList>(layer, "key");
    SettingMetaList* meta1 = Instantiate<SettingMetaList>(layer, "key");
    EXPECT_EQ(*meta0, *meta1);
}

TEST(test_setting_type_list, data_equal) {
    Layer layer;

    SettingMetaList* meta = Instantiate<SettingMetaList>(layer, "key");

    SettingDataList* data0 = Instantiate<SettingDataList>(meta);
    EXPECT_STREQ(meta->key.c_str(), data0->key.c_str());

    SettingDataList* data1 = Instantiate<SettingDataList>(meta);
    EXPECT_STREQ(meta->key.c_str(), data0->key.c_str());

    EXPECT_EQ(*data0, *data1);
}
