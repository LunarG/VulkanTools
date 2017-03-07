/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation
 * Copyright (C) 2014-2016 LunarG, Inc.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Peter Lohrmann <peterl@valvesoftware.com> <plohrmann@gmail.com>
 **************************************************************************/
#ifndef VKTRACEVIEWER_VK_SETTINGS_H
#define VKTRACEVIEWER_VK_SETTINGS_H

extern "C" {
#include "vktrace_settings.h"
}

typedef struct vktraceviewer_vk_settings {
    BOOL printReplayInfoMsgs;
    BOOL printReplayWarningMsgs;
    BOOL printReplayErrorMsgs;
    BOOL pauseOnReplayInfo;
    BOOL pauseOnReplayWarning;
    BOOL pauseOnReplayError;
    BOOL groupByFrame;
    BOOL groupByThread;
    int replay_window_width;
    int replay_window_height;
    BOOL separate_replay_window;
} vktraceviewer_vk_settings;

extern vktraceviewer_vk_settings g_vkTraceViewerSettings;
extern vktrace_SettingGroup g_vkTraceViewerSettingGroup;

void initialize_default_settings();

#endif  // VKTRACEVIEWER_VK_SETTINGS_H
