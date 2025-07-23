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

enum TabType {
    TAB_CONFIGURATIONS = 0,
    TAB_DRIVERS,
    TAB_LAYERS,
    TAB_APPLICATIONS,
    TAB_DIAGNOSTIC,
    TAB_DOCUMENTATION,
    TAB_PREFERENCES,
    TAB_ABOUT,

    TAB_FIRST = TAB_CONFIGURATIONS,
    TAB_LAST = TAB_ABOUT
};

enum { TAB_COUNT = TAB_LAST - TAB_FIRST + 1 };

const char* GetLabel(TabType type);

const char* GetToken(TabType type);

TabType GetTabType(const char* token);
