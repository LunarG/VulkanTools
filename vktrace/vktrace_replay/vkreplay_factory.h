/**************************************************************************
 *
 * Copyright 2015-2016 Valve Corporation
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
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Peter Lohrmann <peterl@valvesoftware.com>
 **************************************************************************/
#pragma once

extern "C" {
#include "vktrace_common.h"
#include "vktrace_settings.h"
#include "vktrace_trace_packet_identifiers.h"
}
#include "vkreplay_window.h"
#include "vkreplay_main.h"

namespace vktrace_replay {

enum VKTRACE_REPLAY_RESULT {
    VKTRACE_REPLAY_SUCCESS = 0,
    VKTRACE_REPLAY_ERROR,            // internal error unrelated to the specific packet
    VKTRACE_REPLAY_INVALID_ID,       // packet_id invalid
    VKTRACE_REPLAY_BAD_RETURN,       // replay return value != trace return value
    VKTRACE_REPLAY_CALL_ERROR,       // replaying call caused an error
    VKTRACE_REPLAY_INVALID_PARAMS,   // trace file parameters are invalid
    VKTRACE_REPLAY_VALIDATION_ERROR  // callback Msg error from validation layer
};

enum VKTRACE_DBG_MSG_TYPE { VKTRACE_DBG_MSG_INFO = 0, VKTRACE_DBG_MSG_WARNING, VKTRACE_DBG_MSG_ERROR };

// callback signature
typedef void (*VKTRACE_DBG_MSG_CALLBACK_FUNCTION)(VKTRACE_DBG_MSG_TYPE msgType, const char *pMsg);

// entrypoints that must be exposed by each replayer library
extern "C" {
// entrypoints

typedef void(VKTRACER_CDECL *funcptr_vkreplayer_setloglevel)(VktraceLogLevel level);
typedef void(VKTRACER_CDECL *funcptr_vkreplayer_setlogcallback)(VKTRACE_REPORT_CALLBACK_FUNCTION pCallback);

typedef void(VKTRACER_CDECL *funcptr_vkreplayer_registerdbgmsgcallback)(VKTRACE_DBG_MSG_CALLBACK_FUNCTION pCallback);
typedef vktrace_SettingGroup *(VKTRACER_CDECL *funcptr_vkreplayer_getSettings)();
typedef void(VKTRACER_CDECL *funcptr_vkreplayer_updatefromsettings)(vktrace_SettingGroup *pSettingGroups,
                                                                    unsigned int numSettingGroups);
typedef int(VKTRACER_CDECL *funcptr_vkreplayer_initialize)(vktrace_replay::ReplayDisplay *pDisplay,
                                                           vkreplayer_settings *pReplaySettings,
                                                           vktrace_trace_file_header *pFileheader);
typedef void(VKTRACER_CDECL *funcptr_vkreplayer_deinitialize)();
typedef vktrace_trace_packet_header *(VKTRACER_CDECL *funcptr_vkreplayer_interpret)(vktrace_trace_packet_header *pPacket);
typedef vktrace_replay::VKTRACE_REPLAY_RESULT(VKTRACER_CDECL *funcptr_vkreplayer_replay)(vktrace_trace_packet_header *pPacket);
typedef int(VKTRACER_CDECL *funcptr_vkreplayer_dump)();
typedef int(VKTRACER_CDECL *funcptr_vkreplayer_getframenumber)();
typedef void(VKTRACER_CDECL *funcptr_vkreplayer_resetframenumber)(int frameNumber);
}

struct vktrace_trace_packet_replay_library {
    void *pLibrary;
    funcptr_vkreplayer_setloglevel SetLogLevel;
    funcptr_vkreplayer_setlogcallback SetLogCallback;

    funcptr_vkreplayer_registerdbgmsgcallback RegisterDbgMsgCallback;
    funcptr_vkreplayer_getSettings GetSettings;
    funcptr_vkreplayer_updatefromsettings UpdateFromSettings;
    funcptr_vkreplayer_initialize Initialize;
    funcptr_vkreplayer_deinitialize Deinitialize;
    funcptr_vkreplayer_interpret Interpret;
    funcptr_vkreplayer_replay Replay;
    funcptr_vkreplayer_dump Dump;
    funcptr_vkreplayer_getframenumber GetFrameNumber;
    funcptr_vkreplayer_resetframenumber ResetFrameNumber;
};

class ReplayFactory {
   public:
    vktrace_trace_packet_replay_library *Create(uint8_t tracerId);
    void Destroy(vktrace_trace_packet_replay_library **ppReplayer);
};
}  // namespace vktrace_replay
