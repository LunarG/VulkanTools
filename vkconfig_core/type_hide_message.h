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

#include <vector>
#include <string>

enum HideMessageType {
    HIDE_MESSAGE_NEED_APPLICATION_RESTART = 0,
    HIDE_MESSAGE_USE_SYSTEM_TRAY,
    HIDE_MESSAGE_CRITICAL_IMPLICIT_LAYER_OVERRIDE,
    HIDE_MESSAGE_WIDGET_SETTING_FLOAT,
    HIDE_MESSAGE_WIDGET_SETTING_INT,
    HIDE_MESSAGE_WIDGET_SETTING_FRAMES,
    HIDE_MESSAGE_NOTIFICATION_EXPORT_CONFIGURATION,
    HIDE_MESSAGE_NOTIFICATION_EXPORT_LAYERS_SETTINGS,
    HIDE_MESSAGE_NOTIFICATION_LAYERS_LOADED,
    HIDE_MESSAGE_NOTIFICATION_UNORDERED_LAYER,
    HIDE_MESSAGE_NOTIFICATION_EXECUTABLE_ALL,
    HIDE_MESSAGE_NOTIFICATION_EXECUTABLE_PER,
    HIDE_MESSAGE_NOTIFICATION_PER_CONFIG_UPDATE,
    HIDE_MESSAGE_WARN_REMOVE_EXECUTABLE,
    HIDE_MESSAGE_WARN_REMOVE_EXECUTABLE_OPTIONS,
    HIDE_MESSAGE_WARN_MISSING_LAYERS_IGNORE,
    HIDE_MESSAGE_WARN_CORE_SHADER_IGNORE,
    HIDE_MESSAGE_QUESTION_REMOVING_LAYERS_PATH,
    HIDE_MESSAGE_ERROR_32BIT,

    HIDE_MESSAGE_FIRST = HIDE_MESSAGE_NEED_APPLICATION_RESTART,
    HIDE_MESSAGE_LAST = HIDE_MESSAGE_ERROR_32BIT,

    HIDE_MESSAGE_INVALID = ~0,
};
enum { HIDE_MESSAGE_COUNT = HIDE_MESSAGE_LAST - HIDE_MESSAGE_FIRST + 1 };

enum HideMessageBit {};
typedef int HideMessageFlags;

const char* GetToken(HideMessageType value);

HideMessageType GetHideMessageType(const char* token);

const char* GetToken(HideMessageBit bit);

HideMessageBit GetHideMessageBit(const char* token);

HideMessageBit GetBit(HideMessageType type);

HideMessageType GetValue(HideMessageBit bit);

HideMessageFlags GetHideMessageFlags(const std::vector<std::string>& tokens);

std::vector<std::string> GetHideMessageTokens(HideMessageFlags flags);

HideMessageFlags GetHideMessageFlags(const std::string& string);

std::string GetHideMessageString(HideMessageFlags flags);
