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

#include <QIcon>

enum Icon {
    ICON_CLEAR = 0,
    ICON_DRAG,
    ICON_NEXT,
    ICON_PREV,
    ICON_MODE,
    ICON_EXIT,
    ICON_SEARCH_CASE,
    ICON_SEARCH_WHOLE,
    ICON_SEARCH_REGEX,
    ICON_FILE_APPEND,
    ICON_FILE_EXPORT,
    ICON_FILE_REMOVE,
    ICON_FILE_SEARCH,
    ICON_FOLDER_APPEND,
    ICON_FOLDER_EXPORT,
    ICON_FOLDER_RELOAD,
    ICON_FOLDER_REMOVE,
    ICON_FOLDER_SEARCH,
    ICON_OPTIONS_COPY,
    ICON_OPTIONS_REMOVE,
    ICON_RELOAD,
    ICON_RESET,
    ICON_ADVANCED,
    ICON_BASIC,
    ICON_SYSTEM_INVALID,
    ICON_SYSTEM_OFF,
    ICON_SYSTEM_ON,

    ICON_FIRST = ICON_CLEAR,
    ICON_LAST = ICON_SYSTEM_ON,
};

enum { ICON_COUNT = ICON_LAST - ICON_FIRST + 1 };

QIcon Get(Icon icon);

bool IsDarkMode();
