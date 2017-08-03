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
 **************************************************************************/
#pragma once
#include "vkreplay_window.h"
#include "vkreplay_factory.h"
#include "vkreplay_settings.h"

extern void VkReplaySetLogCallback(VKTRACE_REPORT_CALLBACK_FUNCTION pCallback);
extern void VkReplaySetLogLevel(VktraceLogLevel level);
extern void VkReplayRegisterDbgMsgCallback(vktrace_replay::VKTRACE_DBG_MSG_CALLBACK_FUNCTION pCallback);
extern vktrace_SettingGroup* VKTRACER_CDECL VkReplayGetSettings();
extern void VKTRACER_CDECL VkReplayUpdateFromSettings(vktrace_SettingGroup* pSettingGroups, unsigned int numSettingGroups);
extern int VKTRACER_CDECL VkReplayInitialize(vktrace_replay::ReplayDisplay* pDisplay, vkreplayer_settings* pReplaySettings,
                                             vktrace_trace_file_header* pFileheader);
extern void VKTRACER_CDECL VkReplayDeinitialize();
extern vktrace_trace_packet_header* VKTRACER_CDECL VkReplayInterpret(vktrace_trace_packet_header* pPacket);
extern vktrace_replay::VKTRACE_REPLAY_RESULT VKTRACER_CDECL VkReplayReplay(vktrace_trace_packet_header* pPacket);
extern int VKTRACER_CDECL VkReplayDump();
extern int VKTRACER_CDECL VkReplayGetFrameNumber();
extern void VKTRACER_CDECL VkReplayResetFrameNumber(int frameNumber);

extern PFN_vkDebugReportCallbackEXT g_fpDbgMsgCallback;
