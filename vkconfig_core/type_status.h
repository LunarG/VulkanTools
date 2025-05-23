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

#pragma once

enum StatusType {
    STATUS_STABLE = 0,
    STATUS_BETA,
    STATUS_ALPHA,
    STATUS_DEPRECATED,

    STATUS_FIRST = STATUS_STABLE,
    STATUS_LAST = STATUS_DEPRECATED
};

enum { STATUS_COUNT = STATUS_LAST - STATUS_FIRST + 1 };

const char* GetToken(StatusType type);

StatusType GetStatusType(const char* token);
