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

enum DriverMode {
    DRIVER_MODE_SINGLE = 0,
    DRIVER_MODE_SORTED,

    DRIVER_MODE_FIRST = DRIVER_MODE_SINGLE,
    DRIVER_MODE_LAST = DRIVER_MODE_SORTED
};

enum { DRIVER_MODE_COUNT = DRIVER_MODE_LAST - DRIVER_MODE_FIRST + 1 };

const char* GetToken(DriverMode mode);

DriverMode GetDriverMode(const char* token);
