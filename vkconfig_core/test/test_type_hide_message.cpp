/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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

#include "../type_hide_message.h"

#include <gtest/gtest.h>

static const HideMessageBit HIDE_MESSAGE_NEED_APPLICATION_RESTART_BIT =
    static_cast<HideMessageBit>(1 << (HIDE_MESSAGE_NEED_APPLICATION_RESTART - HIDE_MESSAGE_FIRST));
static const HideMessageBit HIDE_MESSAGE_USE_SYSTEM_TRAY_BIT =
    static_cast<HideMessageBit>(1 << (HIDE_MESSAGE_USE_SYSTEM_TRAY - HIDE_MESSAGE_FIRST));
static const HideMessageBit HIDE_MESSAGE_WARN_CORE_SHADER_IGNORE_BIT =
    static_cast<HideMessageBit>(1 << (HIDE_MESSAGE_WARN_CORE_SHADER_IGNORE - HIDE_MESSAGE_FIRST));

TEST(test_type_hide_message, convert_specific_values) {
    EXPECT_EQ(HIDE_MESSAGE_NEED_APPLICATION_RESTART_BIT, GetBit(HIDE_MESSAGE_NEED_APPLICATION_RESTART));
    EXPECT_EQ(HIDE_MESSAGE_NEED_APPLICATION_RESTART, GetValue(HIDE_MESSAGE_NEED_APPLICATION_RESTART_BIT));

    EXPECT_EQ(HIDE_MESSAGE_USE_SYSTEM_TRAY_BIT, GetBit(HIDE_MESSAGE_USE_SYSTEM_TRAY));
    EXPECT_EQ(HIDE_MESSAGE_USE_SYSTEM_TRAY, GetValue(HIDE_MESSAGE_USE_SYSTEM_TRAY_BIT));

    EXPECT_EQ(HIDE_MESSAGE_WARN_CORE_SHADER_IGNORE_BIT, GetBit(HIDE_MESSAGE_WARN_CORE_SHADER_IGNORE));
    EXPECT_EQ(HIDE_MESSAGE_WARN_CORE_SHADER_IGNORE, GetValue(HIDE_MESSAGE_WARN_CORE_SHADER_IGNORE_BIT));
}

TEST(test_type_hide_message, convert_invalid_value) {
    EXPECT_EQ(0, GetBit(HIDE_MESSAGE_INVALID));
    EXPECT_EQ(HIDE_MESSAGE_INVALID, GetHideMessageType("UNKNOWN_VALUE"));
    EXPECT_EQ(nullptr, GetToken(HIDE_MESSAGE_INVALID));
}

TEST(test_type_hide_message, convert_type_to_token) {
    for (int i = HIDE_MESSAGE_FIRST, n = HIDE_MESSAGE_COUNT; i < n; ++i) {
        const HideMessageType value = static_cast<HideMessageType>(i);
        const char* string = GetToken(value);
        EXPECT_EQ(value, GetHideMessageType(string));
    }
}

TEST(test_type_hide_message, convert_bit_to_token) {
    for (int i = HIDE_MESSAGE_FIRST, n = HIDE_MESSAGE_COUNT; i < n; ++i) {
        const HideMessageBit bit = static_cast<HideMessageBit>(1 << (i - HIDE_MESSAGE_FIRST));
        const char* string = GetToken(bit);
        EXPECT_EQ(bit, GetHideMessageBit(string));
    }
}

TEST(test_type_hide_message, convert_type_to_bit) {
    for (int i = HIDE_MESSAGE_FIRST, n = HIDE_MESSAGE_COUNT; i < n; ++i) {
        const HideMessageType type = static_cast<HideMessageType>(i);
        const HideMessageBit bit = GetBit(type);
        const HideMessageType back = GetValue(bit);
        EXPECT_EQ(back, type);
    }
}

TEST(test_type_hide_message, convert_specific_flags) {
    const std::vector<std::string> TOKENS = {"HIDE_MESSAGE_NEED_APPLICATION_RESTART", "HIDE_MESSAGE_WARN_MISSING_LAYERS_IGNORE"};

    HideMessageFlags flags = GetHideMessageFlags(TOKENS);
    const std::vector<std::string>& back = GetHideMessageTokens(flags);

    EXPECT_STREQ(TOKENS[0].c_str(), back[0].c_str());
    EXPECT_STREQ(TOKENS[1].c_str(), back[1].c_str());
}

TEST(test_type_hide_message, convert_all_flags_split) {
    HideMessageFlags flags = 0;
    for (int i = HIDE_MESSAGE_FIRST, l = HIDE_MESSAGE_LAST; i <= l; ++i) {
        HideMessageType value = static_cast<HideMessageType>(i);
        flags |= GetBit(value);
    }

    const std::vector<std::string>& tokens = GetHideMessageTokens(flags);
    EXPECT_EQ(flags, GetHideMessageFlags(tokens));
}

TEST(test_type_hide_message, convert_all_flags_merge) {
    HideMessageFlags flags = 0;
    for (int i = HIDE_MESSAGE_FIRST, l = HIDE_MESSAGE_LAST; i <= l; ++i) {
        HideMessageType value = static_cast<HideMessageType>(i);
        flags |= GetBit(value);
    }

    const std::string& string = GetHideMessageString(flags);
    EXPECT_EQ(flags, GetHideMessageFlags(string));
}

TEST(test_type_hide_message, convert_specific_flags_merge) {
    const std::string cases[] = {"HIDE_MESSAGE_NEED_APPLICATION_RESTART", "HIDE_MESSAGE_NEED_APPLICATION_RESTART,"};

    for (std::size_t i = 0, n = std::size(cases); i < n; ++i) {
        HideMessageFlags flags = GetHideMessageFlags(cases[i]);
        EXPECT_EQ(flags, GetBit(HIDE_MESSAGE_NEED_APPLICATION_RESTART));
    }
}

TEST(test_type_hide_message, convert_invalid_flags) {
    const std::vector<std::string> TOKENS = {"HIDE_MESSAGE_NEED_APPLICATION_RESTART", "UNKNOWN_VALUE",
                                             "HIDE_MESSAGE_WARN_CORE_SHADER_IGNORE"};

    HideMessageFlags flags = GetHideMessageFlags(TOKENS);
    const std::vector<std::string>& back = GetHideMessageTokens(flags);

    EXPECT_STREQ(TOKENS[0].c_str(), back[0].c_str());
    EXPECT_STREQ(TOKENS[2].c_str(), back[1].c_str());
}
