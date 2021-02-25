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

#include "../setting_data.h"

#include <gtest/gtest.h>

#include <memory>

TEST(test_setting_data, equal_int) {
    SettingDataInt data0("data");
    data0.value = 76;
    SettingDataInt data1("data");
    data1.value = 76;

    EXPECT_EQ(data0, data1);

    SettingData* ptr0 = &data0;
    SettingData* ptr1 = &data1;

    EXPECT_EQ(*ptr0, *ptr1);
    EXPECT_EQ(*ptr0, data1);
    EXPECT_EQ(data0, *ptr1);

    SettingDataInt dataX("dataX");
    dataX.value = 76;

    EXPECT_NE(data0, dataX);

    SettingData* ptrX = &dataX;
    EXPECT_NE(*ptr0, *ptrX);
    EXPECT_NE(*ptr0, dataX);
    EXPECT_NE(data0, *ptrX);

    SettingDataInt dataY("data");
    dataY.value = 82;

    EXPECT_NE(data0, dataY);

    SettingData* ptrY = &dataY;
    EXPECT_NE(*ptr0, *ptrY);
    EXPECT_NE(*ptr0, dataY);
    EXPECT_NE(data0, *ptrY);
}

TEST(test_setting_data, equal_int_range) {
    SettingDataIntRange data0("data");
    data0.min_value = 6;
    data0.max_value = 7;
    SettingDataIntRange data1("data");
    data1.min_value = 6;
    data1.max_value = 7;

    EXPECT_EQ(data0, data1);

    SettingData* ptr0 = &data0;
    SettingData* ptr1 = &data1;

    EXPECT_EQ(*ptr0, *ptr1);
    EXPECT_EQ(*ptr0, data1);
    EXPECT_EQ(data0, *ptr1);

    SettingDataIntRange dataX("dataX");
    dataX.min_value = 6;
    dataX.max_value = 7;

    EXPECT_NE(data0, dataX);

    SettingData* ptrX = &dataX;
    EXPECT_NE(*ptr0, *ptrX);
    EXPECT_NE(*ptr0, dataX);
    EXPECT_NE(data0, *ptrX);

    SettingDataIntRange dataY("data");
    dataY.min_value = 5;
    dataY.max_value = 7;

    EXPECT_NE(data0, dataY);

    SettingData* ptrY = &dataY;
    EXPECT_NE(*ptr0, *ptrY);
    EXPECT_NE(*ptr0, dataY);
    EXPECT_NE(data0, *ptrY);
}

TEST(test_setting_data, equal_bool) {
    SettingDataBool data0("data");
    data0.value = true;
    SettingDataBool data1("data");
    data1.value = true;

    EXPECT_EQ(data0, data1);

    SettingData* ptr0 = &data0;
    SettingData* ptr1 = &data1;

    EXPECT_EQ(*ptr0, *ptr1);
    EXPECT_EQ(*ptr0, data1);
    EXPECT_EQ(data0, *ptr1);

    SettingDataBool dataX("dataX");
    dataX.value = true;

    EXPECT_NE(data0, dataX);

    SettingData* ptrX = &dataX;
    EXPECT_NE(*ptr0, *ptrX);
    EXPECT_NE(*ptr0, dataX);
    EXPECT_NE(data0, *ptrX);

    SettingDataBool dataY("data");
    dataY.value = false;

    EXPECT_NE(data0, dataY);

    SettingData* ptrY = &dataY;
    EXPECT_NE(*ptr0, *ptrY);
    EXPECT_NE(*ptr0, dataY);
    EXPECT_NE(data0, *ptrY);
}

TEST(test_setting_data, equal_enum) {
    SettingDataEnum data0("data");
    data0.value = "value";
    SettingDataEnum data1("data");
    data1.value = "value";

    EXPECT_EQ(data0, data1);

    SettingData* ptr0 = &data0;
    SettingData* ptr1 = &data1;

    EXPECT_EQ(*ptr0, *ptr1);
    EXPECT_EQ(*ptr0, data1);
    EXPECT_EQ(data0, *ptr1);

    SettingDataEnum dataX("dataX");
    dataX.value = "value";

    EXPECT_NE(data0, dataX);

    SettingData* ptrX = &dataX;
    EXPECT_NE(*ptr0, *ptrX);
    EXPECT_NE(*ptr0, dataX);
    EXPECT_NE(data0, *ptrX);

    SettingDataEnum dataY("data");
    dataY.value = "valueY";

    EXPECT_NE(data0, dataY);

    SettingData* ptrY = &dataY;
    EXPECT_NE(*ptr0, *ptrY);
    EXPECT_NE(*ptr0, dataY);
    EXPECT_NE(data0, *ptrY);
}

TEST(test_setting_data, equal_flags) {
    std::vector<std::string> value;
    value.push_back("valueA");
    value.push_back("valueB");

    std::vector<std::string> valueX;
    valueX.push_back("valueB");
    valueX.push_back("valueA");

    SettingDataFlags data0("data");
    data0.value = value;
    SettingDataFlags data1("data");
    data1.value = value;

    EXPECT_EQ(data0, data1);

    SettingData* ptr0 = &data0;
    SettingData* ptr1 = &data1;

    EXPECT_EQ(*ptr0, *ptr1);
    EXPECT_EQ(*ptr0, data1);
    EXPECT_EQ(data0, *ptr1);

    SettingDataFlags dataX("dataX");
    dataX.value = value;

    EXPECT_NE(data0, dataX);

    SettingData* ptrX = &dataX;
    EXPECT_NE(*ptr0, *ptrX);
    EXPECT_NE(*ptr0, dataX);
    EXPECT_NE(data0, *ptrX);

    std::vector<std::string> valueY;
    value.push_back("valueB");
    value.push_back("valueC");

    SettingDataFlags dataY("data");
    dataY.value = valueY;

    EXPECT_NE(data0, dataY);

    SettingData* ptrY = &dataY;
    EXPECT_NE(*ptr0, *ptrY);
    EXPECT_NE(*ptr0, dataY);
    EXPECT_NE(data0, *ptrY);
}
