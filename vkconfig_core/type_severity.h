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

#pragma once

#include <QMessageBox>

enum SeverityType {
    SEVERITY_INFORMATION = 0,
    SEVERITY_WARNING,
    SEVERITY_CRITICAL,
    SEVERITY_QUESTION,

    SEVERITY_FIRST = SEVERITY_INFORMATION,
    SEVERITY_LAST = SEVERITY_QUESTION,
};

enum { SEVERITY_COUNT = SEVERITY_LAST - SEVERITY_FIRST + 1 };

const char* GetToken(SeverityType type);

SeverityType GetSeverityType(const char* token);

QMessageBox::Icon GetIcon(SeverityType type);
