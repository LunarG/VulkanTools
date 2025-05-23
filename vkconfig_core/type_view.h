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

enum SettingView {
    SETTING_VIEW_STANDARD = 0,
    SETTING_VIEW_ADVANCED,  // Deprecated in favor of SETTING_VIEW_DEBUG
    SETTING_VIEW_DEBUG,
    SETTING_VIEW_HIDDEN,

    SETTING_VIEW_FIRST = SETTING_VIEW_STANDARD,
    SETTING_VIEW_LAST = SETTING_VIEW_HIDDEN
};

enum { SETTING_VIEW_COUNT = SETTING_VIEW_LAST - SETTING_VIEW_FIRST + 1 };

SettingView GetSettingView(const char* token);
const char* GetToken(SettingView state);
