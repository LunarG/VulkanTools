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

#include "../header.h"

#include <gtest/gtest.h>

TEST(test_header, get_setting_state_hidden) {
    EXPECT_EQ(SETTING_VIEW_HIDDEN, GetSettingView("HIDDEN"));
    EXPECT_STREQ("HIDDEN", GetToken(SETTING_VIEW_HIDDEN));
}

TEST(test_header, get_setting_state_standard) {
    EXPECT_EQ(SETTING_VIEW_STANDARD, GetSettingView("STANDARD"));
    EXPECT_STREQ("STANDARD", GetToken(SETTING_VIEW_STANDARD));
}

TEST(test_header, get_setting_state_advanced) {
    EXPECT_EQ(SETTING_VIEW_ADVANCED, GetSettingView("ADVANCED"));
    EXPECT_STREQ("ADVANCED", GetToken(SETTING_VIEW_ADVANCED));
}
