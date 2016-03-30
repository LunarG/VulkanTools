/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation, Inc.
 * Copyright (C) 2014-2016 LunarG, Inc.
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
 * Author: Peter Lohrmann <peterl@valvesoftware.com>
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
 **************************************************************************/
#ifndef VKREPLAY__VK_SETTINGS_H
#define VKREPLAY__VK_SETTINGS_H

extern "C"
{
#include "vktrace_settings.h"
#include "vkreplay_main.h"
}

#include <vulkan/vulkan.h>

static vkreplayer_settings s_defaultVkReplaySettings = { NULL, 1, -1, -1, NULL };

extern vkreplayer_settings g_vkReplaySettings;
extern vktrace_SettingGroup g_vkReplaySettingGroup;

void apply_layerSettings_overrides();
char** get_enableLayers_list(unsigned int* pNumLayers);
void release_enableLayer_list(char** pList);

#endif // VKREPLAY__VK_SETTINGS_H
