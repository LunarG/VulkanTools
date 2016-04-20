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

static vkreplayer_settings s_defaultVkReplaySettings = { NULL, 1, -1, -1, NULL, NULL };

vktrace_SettingInfo g_vk_settings_info[] =
{
    { "t", "TraceFile", VKTRACE_SETTING_STRING, &g_vkReplaySettings.pTraceFilePath, &s_defaultVkReplaySettings.pTraceFilePath, TRUE, "The trace file to replay." },
    { "l", "NumLoops", VKTRACE_SETTING_UINT, &g_vkReplaySettings.numLoops, &s_defaultVkReplaySettings.numLoops, TRUE, "The number of times to replay the trace file or loop range." },
    { "lsf", "LoopStartFrame", VKTRACE_SETTING_INT, &g_vkReplaySettings.loopStartFrame, &s_defaultVkReplaySettings.loopStartFrame, TRUE, "The start frame number of the loop range." },
    { "lef", "LoopEndFrame", VKTRACE_SETTING_INT, &g_vkReplaySettings.loopEndFrame, &s_defaultVkReplaySettings.loopEndFrame, TRUE, "The end frame number of the loop range." },
    { "s", "Screenshot", VKTRACE_SETTING_STRING, &g_vkReplaySettings.screenshotList, &s_defaultVkReplaySettings.screenshotList, TRUE, "Comma separated list of frames to take a take snapshots of" },
};

vktrace_SettingGroup g_vkReplaySettingGroup =
{
    "vkreplay_vk",
    sizeof(g_vk_settings_info) / sizeof(g_vk_settings_info[0]),
    &g_vk_settings_info[0]
};

void apply_layerSettings_overrides()
{
#if 0
    setLayerOptionEnum("DrawStateReportFlags", g_vkReplaySettings.drawStateReportFlags);
    setLayerOptionEnum("DrawStateDebugAction", g_vkReplaySettings.drawStateDebugAction);
    setLayerOptionEnum("MemTrackerReportFlags", g_vkReplaySettings.memTrackerReportFlags);
    setLayerOptionEnum("MemTrackerDebugAction", g_vkReplaySettings.memTrackerDebugAction);
    setLayerOptionEnum("ObjectTrackerReportFlags", g_vkReplaySettings.objectTrackerReportFlags);
    setLayerOptionEnum("ObjectTrackerDebugAction", g_vkReplaySettings.objectTrackerDebugAction);
#endif
}

char** get_enableLayers_list(unsigned int *pNumLayers)
{
    char** pList = NULL;
    //size_t len = strlen(g_vkReplaySettings.enableLayers);
    //assert(pNumLayers != NULL);
    //*pNumLayers = 0;

    //if (g_vkReplaySettings.enableLayers != NULL && len > 0)
    //{
    //    // The string contains 1 layer + another layer for each comma
    //    *pNumLayers = 1;
    //    size_t c;
    //    int i;

    //    // count number of commas to determine number of layers
    //    for (c = 0; c < len; c++)
    //    {
    //        if (g_vkReplaySettings.enableLayers[c] == ',')
    //        {
    //            (*pNumLayers)++;
    //        }
    //    }

    //    // allocate an array to contain pointers to the layer names
    //    pList = VKTRACE_NEW_ARRAY(char*, (*pNumLayers));

    //    // copy the entire string to the first element in the list to keep
    //    // the layer names localized in memory.
    //    pList[0] = (char*)vktrace_allocate_and_copy(g_vkReplaySettings.enableLayers);

    //    // now walk the string and replace commas with NULL and record
    //    // the pointers in the pList array.
    //    i = 1;
    //    for (c = 0; c < len; c++)
    //    {
    //        if (pList[0][c] == ',')
    //        {
    //            pList[0][c] = '\0';
    //            pList[i] = &pList[0][c+1];
    //            i++;
    //        }
    //    }
    //}

    return pList;
}

void release_enableLayer_list(char** pList)
{
    if (pList != NULL)
    {
        if (pList[0] != NULL)
        {
            VKTRACE_DELETE(pList[0]);
        }

        VKTRACE_DELETE(pList);
    }
}
