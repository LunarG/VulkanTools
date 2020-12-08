/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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

#include "../layer.h"
#include "../util.h"

#include <gtest/gtest.h>

TEST(test_layer, collect_settings) {
    std::vector<LayerSettingMeta> meta;

    EXPECT_TRUE(CollectDefaultSettingData(std::vector<LayerSettingMeta>()).empty());

    LayerSettingMeta meta0;
    meta0.key = "key0";
    meta0.default_value = "value0";
    meta.push_back(meta0);

    std::vector<LayerSettingData> data = CollectDefaultSettingData(meta);

    LayerSettingData* data0 = FindByKey(data, "key0");
    EXPECT_STREQ("value0", data0->value.c_str());
}
