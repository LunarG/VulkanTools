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

#include "../setting_float.h"
#include "../layer.h"

#include <gtest/gtest.h>

inline SettingMetaFloat* InstantiateFloat(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaFloat*>(layer.Instantiate(layer.settings, key, SETTING_FLOAT));
}

TEST(test_setting_type_float, init) { EXPECT_EQ(SETTING_FLOAT, SettingMetaFloat::TYPE); }

TEST(test_setting_type_float, meta_equal) {
    Layer layer;

    SettingMetaFloat* meta0 = InstantiateFloat(layer, "key");
    meta0->default_value = 7.6f;
    SettingMetaFloat* meta1 = InstantiateFloat(layer, "key");
    meta1->default_value = 7.6f;
    EXPECT_EQ(*meta0, *meta1);

    meta1->default_value = 7.5f;
    EXPECT_NE(*meta0, *meta1);
}

TEST(test_setting_type_float, data_equal) {
    Layer layer;

    SettingMetaFloat* meta = InstantiateFloat(layer, "key");

    SettingDataFloat* data0 = Instantiate<SettingDataFloat>(meta);
    EXPECT_STREQ(meta->key.c_str(), data0->key.c_str());

    SettingDataFloat* data1 = Instantiate<SettingDataFloat>(meta);
    EXPECT_STREQ(meta->key.c_str(), data1->key.c_str());

    EXPECT_EQ(*data0, *data1);

    data0->value = 7.6f;
    EXPECT_NE(*data0, *data1);

    data1->value = 7.6f;
    EXPECT_EQ(*data0, *data1);

    SettingMetaFloat* metaX = InstantiateFloat(layer, "keyX");
    SettingDataFloat* dataX = Instantiate<SettingDataFloat>(metaX);
    dataX->value = 7.6f;

    EXPECT_NE(*data0, *dataX);
}

TEST(test_setting_type_float, value) {
    Layer layer;

    SettingMetaFloat* meta = InstantiateFloat(layer, "key");
    meta->precision = 2;
    meta->width = 3;
    meta->default_value = 1.75555555f;
    EXPECT_STREQ("1.76", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("1.76", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataFloat* dataA = Instantiate<SettingDataFloat>(meta);
    EXPECT_EQ(dataA->value, meta->default_value);

    EXPECT_STREQ("1.76", dataA->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("1.76", dataA->Export(EXPORT_MODE_OVERRIDE).c_str());

    meta->default_value = 1.72222222f;
    EXPECT_STREQ("1.72", meta->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("1.72", meta->Export(EXPORT_MODE_OVERRIDE).c_str());

    SettingDataFloat* dataB = Instantiate<SettingDataFloat>(meta);
    EXPECT_EQ(dataB->value, meta->default_value);

    EXPECT_STREQ("1.72", dataB->Export(EXPORT_MODE_DOC).c_str());
    EXPECT_STREQ("1.72", dataB->Export(EXPORT_MODE_OVERRIDE).c_str());
}

TEST(test_setting_type_float, process_input_float_no_error) {
    Layer layer;

    SettingMetaFloat* meta = InstantiateFloat(layer, "key");
    SettingDataFloat* data = Instantiate<SettingDataFloat>(meta);

    EXPECT_EQ(SETTING_INPUT_NO_ERROR, data->ProcessInput("1.0"));
}

TEST(test_setting_type_float, process_input_float_error_empty) {
    Layer layer;

    SettingMetaFloat* meta = InstantiateFloat(layer, "key");
    SettingDataFloat* data = Instantiate<SettingDataFloat>(meta);

    EXPECT_EQ(SETTING_INPUT_ERROR_EMPTY, data->ProcessInput(""));
}

TEST(test_setting_type_float, process_input_float_error_syntax) {
    Layer layer;

    SettingMetaFloat* meta = InstantiateFloat(layer, "key");
    SettingDataFloat* data = Instantiate<SettingDataFloat>(meta);

    EXPECT_EQ(SETTING_INPUT_ERROR_SYNTAX, data->ProcessInput("1,0"));
}

TEST(test_setting_type_float, process_input_float_error_sementics) {
    Layer layer;

    SettingMetaFloat* meta = InstantiateFloat(layer, "key");
    meta->max_value = 2.0;
    meta->min_value = 1.0;

    SettingDataFloat* data = Instantiate<SettingDataFloat>(meta);

    EXPECT_EQ(SETTING_INPUT_ERROR_SEMENTICS, data->ProcessInput("4.0"));
}
