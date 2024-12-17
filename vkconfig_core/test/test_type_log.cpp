/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

#include "../type_log.h"

#include <gtest/gtest.h>

static const LogBit LOG_ERROR_BIT = static_cast<LogBit>(1 << (LOG_ERROR - LOG_FIRST));
static const LogBit LOG_WARN_BIT = static_cast<LogBit>(1 << (LOG_WARN - LOG_FIRST));
static const LogBit LOG_LAYER_BIT = static_cast<LogBit>(1 << (LOG_LAYER - LOG_FIRST));

TEST(test_type_log, convert_specific_values) {
    EXPECT_EQ(LOG_ERROR_BIT, GetBit(LOG_ERROR));
    EXPECT_EQ(LOG_ERROR, GetValue(LOG_ERROR_BIT));

    EXPECT_EQ(LOG_WARN_BIT, GetBit(LOG_WARN));
    EXPECT_EQ(LOG_WARN, GetValue(LOG_WARN_BIT));

    EXPECT_EQ(LOG_LAYER_BIT, GetBit(LOG_LAYER));
    EXPECT_EQ(LOG_LAYER, GetValue(LOG_LAYER_BIT));
}

TEST(test_type_log, convert_invalid_value) {
    EXPECT_EQ(0, GetBit(LOG_MESSAGE_INVALID));
    EXPECT_EQ(LOG_MESSAGE_INVALID, GetLogType("unknown"));
    EXPECT_EQ(nullptr, GetToken(LOG_MESSAGE_INVALID));
}

TEST(test_type_log, convert_type_to_token) {
    for (int i = LOG_FIRST, n = LOG_COUNT; i < n; ++i) {
        const LogType value = static_cast<LogType>(i);
        const char* string = GetToken(value);
        EXPECT_EQ(value, GetLogType(string));
    }
}

TEST(test_type_log, convert_bit_to_token) {
    for (int i = LOG_FIRST, n = LOG_COUNT; i < n; ++i) {
        const LogBit bit = GetBit(static_cast<LogType>(i));
        const char* string = GetToken(bit);
        const LogBit back = GetLogBit(string);
        EXPECT_EQ(bit, back);
    }
}

TEST(test_type_log, convert_type_to_bit) {
    for (int i = LOG_FIRST, n = LOG_COUNT; i < n; ++i) {
        const LogType type = static_cast<LogType>(i);
        const LogBit bit = GetBit(type);
        const LogType back = GetValue(bit);
        EXPECT_EQ(back, type);
    }
}

TEST(test_type_log, convert_specific_flags) {
    const std::vector<std::string> TOKENS = {"error", "layer"};

    LogFlags flags = GetLogFlags(TOKENS);
    const std::vector<std::string>& back = GetLogTokens(flags);

    EXPECT_STREQ(TOKENS[0].c_str(), back[0].c_str());
    EXPECT_STREQ(TOKENS[1].c_str(), back[1].c_str());
}

TEST(test_type_log, convert_all_flags_split) {
    LogFlags flags = 0;
    for (int i = LOG_FIRST, l = LOG_LAST; i <= l; ++i) {
        LogType value = static_cast<LogType>(i);
        flags |= GetBit(value);
    }

    const std::vector<std::string>& tokens = GetLogTokens(flags);
    EXPECT_EQ(flags, GetLogFlags(tokens));
}

TEST(test_type_log, convert_all_flags_merge) {
    LogFlags flags = 0;
    for (int i = LOG_FIRST, l = LOG_LAST; i <= l; ++i) {
        LogType value = static_cast<LogType>(i);
        flags |= GetBit(value);
    }

    const std::string& string = GetLogString(flags);
    EXPECT_EQ(flags, GetLogFlags(string));
}

TEST(test_type_log, convert_specific_flags_merge) {
    const std::string cases[] = {"warn", "warn,"};

    for (std::size_t i = 0, n = std::size(cases); i < n; ++i) {
        LogFlags flags = GetLogFlags(cases[i]);
        EXPECT_EQ(flags, GetBit(LOG_WARN));
    }
}

TEST(test_type_log, convert_invalid_flags) {
    const std::vector<std::string> TOKENS = {"error", "unknown", "layer"};

    LogFlags flags = GetLogFlags(TOKENS);
    const std::vector<std::string>& back = GetLogTokens(flags);

    EXPECT_STREQ(TOKENS[0].c_str(), back[0].c_str());
    EXPECT_STREQ(TOKENS[2].c_str(), back[1].c_str());
}
