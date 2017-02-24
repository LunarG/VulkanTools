/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
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

#include "vktraceviewer_vk_settings.h"

// declared as extern in header
vktraceviewer_vk_settings g_vkTraceViewerSettings;
static vktraceviewer_vk_settings s_defaultVkSettings;

vktrace_SettingInfo g_vk_settings[] = {
    {"ri", "PrintReplayInfoMsgs", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.printReplayInfoMsgs,
     &s_defaultVkSettings.printReplayInfoMsgs, TRUE, "Print info messages reported when replaying trace file."},
    {"rw", "PrintReplayWarningMsgs", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.printReplayWarningMsgs,
     &s_defaultVkSettings.printReplayWarningMsgs, TRUE, "Print warning messages reported when replaying trace file."},
    {"re", "PrintReplayErrorMsgs", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.printReplayErrorMsgs,
     &s_defaultVkSettings.printReplayErrorMsgs, TRUE, "Print error messages reported when replaying trace file."},
    {"pi", "PauseOnReplayInfo", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.pauseOnReplayInfo,
     &s_defaultVkSettings.pauseOnReplayInfo, TRUE, "Pause replay if an info message is reported."},
    {"pw", "PauseOnReplayWarning", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.pauseOnReplayWarning,
     &s_defaultVkSettings.pauseOnReplayWarning, TRUE, "Pause replay if a warning message is reported."},
    {"pe", "PauseOnReplayError", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.pauseOnReplayError,
     &s_defaultVkSettings.pauseOnReplayError, TRUE, "Pause replay if an error message is reported."},
    {"gf", "GroupByFrame", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.groupByFrame, &s_defaultVkSettings.groupByFrame, TRUE,
     "Group API calls by frame."},
    {"gt", "GroupByThread", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.groupByThread, &s_defaultVkSettings.groupByThread, TRUE,
     "Group API calls by the CPU thread Id on which they executed."},
    {"rw", "ReplayWindowWidth", VKTRACE_SETTING_INT, &g_vkTraceViewerSettings.replay_window_width,
     &s_defaultVkSettings.replay_window_width, TRUE, "Width of replay window on startup."},
    {"rh", "ReplayWindowHeight", VKTRACE_SETTING_INT, &g_vkTraceViewerSettings.replay_window_height,
     &s_defaultVkSettings.replay_window_height, TRUE, "Height of replay window on startup."},
};

vktrace_SettingGroup g_vkTraceViewerSettingGroup = {"vktraceviewer_vk", sizeof(g_vk_settings) / sizeof(g_vk_settings[0]),
                                                    &g_vk_settings[0]};

void initialize_default_settings() {
    s_defaultVkSettings.printReplayInfoMsgs = FALSE;
    s_defaultVkSettings.printReplayWarningMsgs = TRUE;
    s_defaultVkSettings.printReplayErrorMsgs = TRUE;
    s_defaultVkSettings.pauseOnReplayInfo = FALSE;
    s_defaultVkSettings.pauseOnReplayWarning = FALSE;
    s_defaultVkSettings.pauseOnReplayError = TRUE;
    s_defaultVkSettings.groupByFrame = FALSE;
    s_defaultVkSettings.groupByThread = FALSE;
    s_defaultVkSettings.replay_window_width = 100;
    s_defaultVkSettings.replay_window_height = 100;
    s_defaultVkSettings.separate_replay_window = TRUE;
};
