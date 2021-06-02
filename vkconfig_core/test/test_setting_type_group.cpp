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

#include "../setting_group.h"
#include "../layer.h"

#include <gtest/gtest.h>

static_assert(SettingMetaGroup::TYPE == SETTING_GROUP, "Invalid type");

TEST(test_setting_type_group, meta_equal) {
    Layer layer;

    SettingMetaGroup* meta0 = Instantiate<SettingMetaGroup>(layer, "key");
    SettingMetaGroup* meta1 = Instantiate<SettingMetaGroup>(layer, "key");
    EXPECT_EQ(*meta0, *meta1);
}

TEST(test_setting_type_group, data_equal) {
    Layer layer;

    SettingMetaGroup* meta = Instantiate<SettingMetaGroup>(layer, "key");

    SettingDataGroup* data0 = Instantiate<SettingDataGroup>(meta);
    EXPECT_STREQ(meta->key.c_str(), data0->key.c_str());

    SettingDataGroup* data1 = Instantiate<SettingDataGroup>(meta);
    EXPECT_STREQ(meta->key.c_str(), data1->key.c_str());

    EXPECT_EQ(*data0, *data1);
}