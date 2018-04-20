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
#include <inttypes.h>
#include "vkreplay.h"
#include "vkreplay_vkreplay.h"
#include "vktrace_vk_packet_id.h"
#include "vktrace_tracelog.h"

static vkreplayer_settings s_defaultVkReplaySettings = {NULL, 1, UINT_MAX, UINT_MAX, NULL, NULL, NULL};

vkReplay* g_pReplayer = NULL;
VKTRACE_CRITICAL_SECTION g_handlerLock;
PFN_vkDebugReportCallbackEXT g_fpDbgMsgCallback;
vktrace_replay::VKTRACE_DBG_MSG_CALLBACK_FUNCTION g_fpVktraceCallback = NULL;

static VKAPI_ATTR VkBool32 VKAPI_CALL vkErrorHandler(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType, uint64_t srcObjectHandle,
                                                     size_t location, int32_t msgCode, const char* pLayerPrefix, const char* pMsg,
                                                     void* pUserData) {
    VkBool32 bail = false;

    vktrace_enter_critical_section(&g_handlerLock);
    if ((msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) == VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        vktrace_LogError("MsgFlags %d with object %#" PRIxLEAST64 ", location %u returned msgCode %d and msg %s", msgFlags,
                         srcObjectHandle, location, msgCode, (char*)pMsg);
        g_pReplayer->push_validation_msg(msgFlags, objType, srcObjectHandle, location, msgCode, pLayerPrefix, pMsg, pUserData);
        if (g_fpVktraceCallback != NULL) {
            g_fpVktraceCallback(vktrace_replay::VKTRACE_DBG_MSG_ERROR, pMsg);
        }
        /* TODO: bailing out of the call chain due to this error should allow
         * the app to continue in some fashion.
         * Is that needed here?
         */
        bail = true;
    } else if ((msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) == VK_DEBUG_REPORT_WARNING_BIT_EXT ||
               (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) == VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        if (g_fpVktraceCallback != NULL) {
            g_fpVktraceCallback(vktrace_replay::VKTRACE_DBG_MSG_WARNING, pMsg);
        }
    } else {
        if (g_fpVktraceCallback != NULL) {
            g_fpVktraceCallback(vktrace_replay::VKTRACE_DBG_MSG_INFO, pMsg);
        }
    }
    vktrace_leave_critical_section(&g_handlerLock);

    return bail;
}

void VkReplaySetLogCallback(VKTRACE_REPORT_CALLBACK_FUNCTION pCallback) {}

void VkReplaySetLogLevel(VktraceLogLevel level) {}

void VkReplayRegisterDbgMsgCallback(vktrace_replay::VKTRACE_DBG_MSG_CALLBACK_FUNCTION pCallback) {
    g_fpVktraceCallback = pCallback;
}

vktrace_SettingGroup* VKTRACER_CDECL VkReplayGetSettings() {
    static BOOL bFirstTime = TRUE;
    if (bFirstTime == TRUE) {
        vktrace_SettingGroup_reset_defaults(&g_vkReplaySettingGroup);
        bFirstTime = FALSE;
    }

    return &g_vkReplaySettingGroup;
}

void VKTRACER_CDECL VkReplayUpdateFromSettings(vktrace_SettingGroup* pSettingGroups, unsigned int numSettingGroups) {
    vktrace_SettingGroup_Apply_Overrides(&g_vkReplaySettingGroup, pSettingGroups, numSettingGroups);
}

int VKTRACER_CDECL VkReplayInitialize(vktrace_replay::ReplayDisplay* pDisplay, vkreplayer_settings* pReplaySettings,
                                      vktrace_trace_file_header* pFileHeader) {
    try {
        if (pReplaySettings == NULL) {
            g_pReplayer = new vkReplay(&s_defaultVkReplaySettings, pFileHeader);
        } else {
            g_pReplayer = new vkReplay(pReplaySettings, pFileHeader);
        }
    } catch (int e) {
        vktrace_LogError("Failed to create vkReplay, probably out of memory. Error %d", e);
        return -1;
    }

    vktrace_create_critical_section(&g_handlerLock);
    g_fpDbgMsgCallback = vkErrorHandler;
    int result = g_pReplayer->init(*pDisplay);
    return result;
}

void VKTRACER_CDECL VkReplayDeinitialize() {
    if (g_pReplayer != NULL) {
        delete g_pReplayer;
        g_pReplayer = NULL;
    }
    vktrace_delete_critical_section(&g_handlerLock);
}

vktrace_trace_packet_header* VKTRACER_CDECL VkReplayInterpret(vktrace_trace_packet_header* pPacket) {
    // Attempt to interpret the packet as a Vulkan packet
    vktrace_trace_packet_header* pInterpretedHeader = interpret_trace_packet_vk(pPacket);
    if (pInterpretedHeader == NULL) {
        vktrace_LogError("Unrecognized Vulkan packet_id: %u", pPacket->packet_id);
    }

    return pInterpretedHeader;
}

vktrace_replay::VKTRACE_REPLAY_RESULT VKTRACER_CDECL VkReplayReplay(vktrace_trace_packet_header* pPacket) {
    vktrace_replay::VKTRACE_REPLAY_RESULT result = vktrace_replay::VKTRACE_REPLAY_ERROR;
    if (g_pReplayer != NULL) {
        result = g_pReplayer->replay(pPacket);

        if (result == vktrace_replay::VKTRACE_REPLAY_SUCCESS) result = g_pReplayer->pop_validation_msgs();
    }
    return result;
}

int VKTRACER_CDECL VkReplayDump() {
    if (g_pReplayer != NULL) {
        g_pReplayer->dump_validation_data();
        return 0;
    }
    return -1;
}

int VKTRACER_CDECL VkReplayGetFrameNumber() {
    if (g_pReplayer != NULL) {
        return g_pReplayer->get_frame_number();
    }
    return -1;
}

void VKTRACER_CDECL VkReplayResetFrameNumber(int frameNumber) {
    if (g_pReplayer != NULL) {
        g_pReplayer->reset_frame_number(frameNumber);
    }
}
