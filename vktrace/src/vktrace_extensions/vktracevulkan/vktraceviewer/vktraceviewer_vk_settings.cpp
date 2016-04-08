/**************************************************************************
*
* Copyright 2014-2016 Valve Corporation
* Copyright (C) 2015-2016 LunarG, Inc.
* All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* Author: Peter Lohrmann <peterl@valvesoftware.com> <plohrmann@gmail.com>
**************************************************************************/

#include "vktraceviewer_vk_settings.h"

// declared as extern in header
vktraceviewer_vk_settings g_vkTraceViewerSettings;
static vktraceviewer_vk_settings s_defaultVkSettings;

vktrace_SettingInfo g_settings_info[] =
{
    { "ri", "PrintReplayInfoMsgs", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.printReplayInfoMsgs, &s_defaultVkSettings.printReplayInfoMsgs, TRUE, "Print info messages reported when replaying trace file."},
    { "rw", "PrintReplayWarningMsgs", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.printReplayWarningMsgs, &s_defaultVkSettings.printReplayWarningMsgs, TRUE, "Print warning messages reported when replaying trace file."},
    { "re", "PrintReplayErrorMsgs", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.printReplayErrorMsgs, &s_defaultVkSettings.printReplayErrorMsgs, TRUE, "Print error messages reported when replaying trace file."},
    { "pi", "PauseOnReplayInfo", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.pauseOnReplayInfo, &s_defaultVkSettings.pauseOnReplayInfo, TRUE, "Pause replay if an info message is reported."},
    { "pw", "PauseOnReplayWarning", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.pauseOnReplayWarning, &s_defaultVkSettings.pauseOnReplayWarning, TRUE, "Pause replay if a warning message is reported."},
    { "pe", "PauseOnReplayError", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.pauseOnReplayError, &s_defaultVkSettings.pauseOnReplayError, TRUE, "Pause replay if an error message is reported."},
    { "gf", "GroupByFrame", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.groupByFrame, &s_defaultVkSettings.groupByFrame, TRUE, "Group API calls by frame."},
    { "gt", "GroupByThread", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.groupByThread, &s_defaultVkSettings.groupByThread, TRUE, "Group API calls by the CPU thread Id on which they executed."},
    { "rw", "ReplayWindowWidth", VKTRACE_SETTING_INT, &g_vkTraceViewerSettings.replay_window_width, &s_defaultVkSettings.replay_window_width, TRUE, "Width of replay window on startup."},
    { "rh", "ReplayWindowHeight", VKTRACE_SETTING_INT, &g_vkTraceViewerSettings.replay_window_height, &s_defaultVkSettings.replay_window_height, TRUE, "Height of replay window on startup."},
    { "sr", "SeparateReplayWindow", VKTRACE_SETTING_BOOL, &g_vkTraceViewerSettings.separate_replay_window, &s_defaultVkSettings.separate_replay_window, TRUE, "Use a separate replay window."},
};

vktrace_SettingGroup g_vkTraceViewerSettingGroup =
{
    "vktraceviewer_vk",
    sizeof(g_settings_info) / sizeof(g_settings_info[0]),
    &g_settings_info[0]
};

void initialize_default_settings()
{
    s_defaultVkSettings.printReplayInfoMsgs = FALSE;
    s_defaultVkSettings.printReplayWarningMsgs = TRUE;
    s_defaultVkSettings.printReplayErrorMsgs = TRUE;
    s_defaultVkSettings.pauseOnReplayInfo = FALSE;
    s_defaultVkSettings.pauseOnReplayWarning = FALSE;
    s_defaultVkSettings.pauseOnReplayError = TRUE;
    s_defaultVkSettings.groupByFrame = FALSE;
    s_defaultVkSettings.groupByThread = FALSE;
    s_defaultVkSettings.replay_window_width = 1024;
    s_defaultVkSettings.replay_window_height = 768;
    s_defaultVkSettings.separate_replay_window = FALSE;
};
