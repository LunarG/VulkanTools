/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation, Inc.
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
 * Author: Peter Lohrmann <peterl@valvesoftware.com>
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
 **************************************************************************/
#include "vulkan/vk_layer.h"

#include "vkreplay_settings.h"
// declared as extern in header
vkreplayer_settings g_vkReplaySettings;

static vkreplayer_settings s_defaultVkReplaySettings = {NULL, 1, -1, -1, NULL, NULL, NULL, false};

vktrace_SettingInfo g_vk_settings_info[] = {
    {"o",
     "Open",
     VKTRACE_SETTING_STRING,
     {&g_vkReplaySettings.pTraceFilePath},
     {&s_defaultVkReplaySettings.pTraceFilePath},
     TRUE,
     "The trace file to open and replay."},
    {"t",
     "TraceFile",
     VKTRACE_SETTING_STRING,
     {&g_vkReplaySettings.pTraceFilePath},
     {&s_defaultVkReplaySettings.pTraceFilePath},
     TRUE,
     "(Deprecated, use -o or --Open instead) The trace file to replay."},
    {"l",
     "NumLoops",
     VKTRACE_SETTING_UINT,
     {&g_vkReplaySettings.numLoops},
     {&s_defaultVkReplaySettings.numLoops},
     TRUE,
     "The number of times to replay the trace file or loop range."},
    {"lsf",
     "LoopStartFrame",
     VKTRACE_SETTING_INT,
     {&g_vkReplaySettings.loopStartFrame},
     {&s_defaultVkReplaySettings.loopStartFrame},
     TRUE,
     "The start frame number of the loop range."},
    {"lef",
     "LoopEndFrame",
     VKTRACE_SETTING_INT,
     {&g_vkReplaySettings.loopEndFrame},
     {&s_defaultVkReplaySettings.loopEndFrame},
     TRUE,
     "The end frame number of the loop range."},
    {"s",
     "Screenshot",
     VKTRACE_SETTING_STRING,
     {&g_vkReplaySettings.screenshotList},
     {&s_defaultVkReplaySettings.screenshotList},
     TRUE,
     "Comma separated list of frames to take a take snapshots of"},
    {"sf",
     "ScreenshotFormat",
     VKTRACE_SETTING_STRING,
     {&g_vkReplaySettings.screenshotColorFormat},
     {&s_defaultVkReplaySettings.screenshotColorFormat},
     TRUE,
     "Color Space format of screenshot files. Formats are UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB"},
    {"r",
     "ReplayMode",
     VKTRACE_SETTING_BOOL,
     {&g_vkReplaySettings.replayMode},
     {&s_defaultVkReplaySettings.replayMode},
     TRUE,
     "Replay mode. False for standard functionality (play through, loop) (replay_trace). \
                    True for Viewer functionality (play, pause, stop, step) (VkTraceViewer)."},
};

vktrace_SettingGroup g_vkReplaySettingGroup = {"vkreplay_vk", sizeof(g_vk_settings_info) / sizeof(g_vk_settings_info[0]),
                                               &g_vk_settings_info[0]};

void apply_layerSettings_overrides() {
#if 0
    setLayerOptionEnum("DrawStateReportFlags", g_vkReplaySettings.drawStateReportFlags);
    setLayerOptionEnum("DrawStateDebugAction", g_vkReplaySettings.drawStateDebugAction);
    setLayerOptionEnum("MemTrackerReportFlags", g_vkReplaySettings.memTrackerReportFlags);
    setLayerOptionEnum("MemTrackerDebugAction", g_vkReplaySettings.memTrackerDebugAction);
    setLayerOptionEnum("ObjectTrackerReportFlags", g_vkReplaySettings.objectTrackerReportFlags);
    setLayerOptionEnum("ObjectTrackerDebugAction", g_vkReplaySettings.objectTrackerDebugAction);
#endif
}
