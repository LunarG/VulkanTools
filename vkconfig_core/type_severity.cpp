/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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

#include "type_severity.h"

#include <array>
#include <cstring>

const char* GetToken(SeverityType value) {
    static const char* TOKENS[] = {
        "INFORMATION",  // SEVERITY_INFORMATION
        "WARNING",      // SEVERITY_WARNING
        "CRITICAL",     // SEVERITY_CRITICAL
        "QUESTION"      // SEVERITY_QUESTION
    };
    static_assert(std::size(TOKENS) == SEVERITY_COUNT, "The tranlation table size doesn't match the enum number of elements");

    if (value >= SEVERITY_FIRST && value <= SEVERITY_LAST) {
        return TOKENS[value - SEVERITY_FIRST];
    } else {
        return nullptr;
    }
}

SeverityType GetSeverityType(const char* token) {
    for (std::size_t i = 0, n = SEVERITY_COUNT; i < n; ++i) {
        const SeverityType value = static_cast<SeverityType>(i);
        if (std::strcmp(::GetToken(value), token) == 0) {
            return value;
        }
    }

    return SEVERITY_INFORMATION;
}

QMessageBox::Icon GetIcon(SeverityType type) {
    static const QMessageBox::Icon TABLE[] = {
        QMessageBox::Information,  // SEVERITY_INFORMATION
        QMessageBox::Warning,      // SEVERITY_WARNING
        QMessageBox::Critical,     // SEVERITY_CRITICAL
        QMessageBox::Question      // SEVERITY_QUESTION
    };
    static_assert(std::size(TABLE) == SEVERITY_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[type - SEVERITY_FIRST];
}
