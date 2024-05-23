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

#include "type_log.h"
#include "bitfield.h"
#include "util.h"

#include <array>
#include <cstring>

const char* GetToken(LogType value) {
    static const char* TOKENS[] = {
        "error",    // LOG_ERROR
        "warn",     // LOG_WARN
        "info",     // LOG_INFO
        "debug",    // LOG_DEBUG
        "perf",     // LOG_PERF
        "layer",    // LOG_LAYER
        "implem",   // LOG_IMPLEM
    };
    static_assert(std::size(TOKENS) == LOG_COUNT);

    if (value >= LOG_FIRST && value <= LOG_LAST) {
        return TOKENS[value - LOG_FIRST];
    } else {
        return nullptr;
    }
}

LogType GetLogType(const char* token) {
    for (int i = LOG_FIRST, l = LOG_LAST; i <= l; ++i) {
        const LogType value = static_cast<LogType>(i);
        if (std::strcmp(::GetToken(value), token) == 0) {
            return value;
        }
    }

    return LOG_MESSAGE_INVALID;
}

const char* GetToken(LogBit bit) {
    return GetToken(GetValue(bit));
}

LogBit GetLogBit(const char* token) {
    const LogType value = GetLogType(token);
    if (value >= LOG_FIRST && value <= LOG_LAST) {
        return static_cast<LogBit>(1 << (value - LOG_FIRST));
    } else {
        return static_cast<LogBit>(0);
    }
}

LogBit GetBit(LogType value) {
    if (value >= LOG_FIRST && value <= LOG_LAST) {
        const int bit = GetBit(value, LOG_FIRST);
        return static_cast<LogBit>(bit);
    } else {
        return static_cast<LogBit>(0);
    }
}

LogType GetValue(LogBit bit) {
    const LogType value = GetValue(bit, LOG_FIRST);
    if (value >= LOG_FIRST && value <= LOG_LAST) {
        return value;
    } else {
        return LOG_MESSAGE_INVALID;
    }
}

LogFlags GetLogFlags(const std::vector<std::string>& tokens) {
    LogFlags flags = 0;

    for (std::size_t i = 0, n = tokens.size(); i < n; ++i) {
        LogType value = GetLogType(tokens[i].c_str());
        if (value == LOG_MESSAGE_INVALID) {
            continue;
        }
        flags |= GetBit(value);
    }

    return flags;
}

std::vector<std::string> GetLogTokens(LogFlags flags) {
    std::vector<std::string> result;

    for (int i = LOG_FIRST, l = LOG_LAST; i <= l; ++i) {
        LogType value = static_cast<LogType>(i);
        if (flags & GetBit(value)) {
            result.push_back(GetToken(value));
        }
    }

    return result;
}


LogFlags GetLogFlags(const std::string& string) {
    const std::vector<std::string>& tokens = Split(string, ",");
    return GetLogFlags(tokens);
}

std::string GetLogString(LogFlags flags) {
    const std::vector<std::string>& tokens = GetLogTokens(flags);
    return Merge(tokens, ",");
}

