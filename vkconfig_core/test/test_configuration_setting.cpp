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

#include "../configuration_setting.h"

#include <gtest/gtest.h>

TEST(test_configuration_setting, equal) {
    ConfigurationSettingValue<SETTING_BOOL, bool> setting_a;
    setting_a.key = "key";
    setting_a.value = true;

    ConfigurationSettingValue<SETTING_BOOL, bool> setting_b;
    setting_b.key = "key";
    setting_b.value = true;

    EXPECT_EQ(setting_a, setting_b);
}