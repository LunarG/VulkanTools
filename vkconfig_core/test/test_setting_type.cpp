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

#include "../setting_type.h"

#include <gtest/gtest.h>

TEST(test_setting_type, is_enum_true1) { EXPECT_EQ(true, IsEnum(SETTING_ENUM)); }

TEST(test_setting_type, is_enum_true2) { EXPECT_EQ(true, IsEnum(SETTING_FLAGS)); }

TEST(test_setting_type, is_enum_false) { EXPECT_EQ(false, IsEnum(SETTING_STRING)); }

TEST(test_setting_type, get_setting_token) { EXPECT_STREQ("STRING", GetSettingTypeToken(SETTING_STRING)); }

TEST(test_setting_type, get_setting_type) { EXPECT_EQ(SETTING_STRING, GetSettingType("STRING")); }

TEST(test_setting_type, get_setting_save_file) { EXPECT_TRUE(IsPath(SETTING_SAVE_FILE)); }

TEST(test_setting_type, get_setting_load_file) { EXPECT_TRUE(IsPath(SETTING_LOAD_FILE)); }

TEST(test_setting_type, get_setting_save_folder) { EXPECT_TRUE(IsPath(SETTING_SAVE_FOLDER)); }

TEST(test_setting_type, get_setting_string) { EXPECT_TRUE(!IsPath(SETTING_STRING)); }

TEST(test_setting_type, get_setting_bool) { EXPECT_TRUE(!IsPath(SETTING_BOOL)); }

TEST(test_setting_type, get_setting_state_hidden) {
    EXPECT_EQ(SETTING_VIEW_HIDDEN, GetSettingView("HIDDEN"));
    EXPECT_STREQ("HIDDEN", GetSettingViewToken(SETTING_VIEW_HIDDEN));
}

TEST(test_setting_type, get_setting_state_standard) {
    EXPECT_EQ(SETTING_VIEW_STANDARD, GetSettingView("STANDARD"));
    EXPECT_STREQ("STANDARD", GetSettingViewToken(SETTING_VIEW_STANDARD));
}
