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

#include "../setting_frames.h"
#include "../layer.h"

#include <gtest/gtest.h>

inline SettingMetaFrames* InstantiateFrames(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaFrames*>(layer.Instantiate(layer.settings, key, SETTING_FRAMES));
}

TEST(test_setting_type_frames, init) { EXPECT_EQ(SETTING_FRAMES, SettingMetaFrames::TYPE); }

TEST(test_setting_type_frames, meta_equal) {
    Layer layer;

    SettingMetaFrames* meta0 = InstantiateFrames(layer, "key");
    meta0->default_value = "6-7";
    SettingMetaFrames* meta1 = InstantiateFrames(layer, "key");
    meta1->default_value = "6-7";
    EXPECT_EQ(*meta0, *meta1);

    meta1->default_value = "5-7";
    EXPECT_NE(*meta0, *meta1);
}

TEST(test_setting_type_frames, data_equal) {
    Layer layer;

    SettingMetaFrames* meta = InstantiateFrames(layer, "key");

    SettingDataFrames* data0 = Instantiate<SettingDataFrames>(meta);
    EXPECT_STREQ(meta->key.c_str(), data0->key.c_str());

    SettingDataFrames* data1 = Instantiate<SettingDataFrames>(meta);
    EXPECT_STREQ(meta->key.c_str(), data1->key.c_str());

    EXPECT_EQ(*data0, *data1);

    data0->value = "6-7";
    EXPECT_NE(*data0, *data1);

    data1->value = "6-7";
    EXPECT_EQ(*data0, *data1);

    SettingMetaFrames* metaX = InstantiateFrames(layer, "keyX");
    SettingDataFrames* dataX = Instantiate<SettingDataFrames>(metaX);
    dataX->value = "6-7";

    EXPECT_NE(*data0, *dataX);
}

TEST(test_setting_frames, process_input_frames_no_error) {
    Layer layer;

    SettingMetaFrames* meta = InstantiateFrames(layer, "key");
    SettingDataFrames* data = Instantiate<SettingDataFrames>(meta);

    EXPECT_EQ(SETTING_INPUT_NO_ERROR, data->ProcessInput("1-2,9"));
}

TEST(test_setting_frames, process_input_frames_error_empty) {
    Layer layer;

    SettingMetaFrames* meta = InstantiateFrames(layer, "key");
    SettingDataFrames* data = Instantiate<SettingDataFrames>(meta);

    EXPECT_EQ(SETTING_INPUT_ERROR_EMPTY, data->ProcessInput(""));
}

TEST(test_setting_frames, process_input_frames_error_syntax) {
    Layer layer;

    SettingMetaFrames* meta = InstantiateFrames(layer, "key");
    SettingDataFrames* data = Instantiate<SettingDataFrames>(meta);

    EXPECT_EQ(SETTING_INPUT_ERROR_SYNTAX, data->ProcessInput("a"));
}
