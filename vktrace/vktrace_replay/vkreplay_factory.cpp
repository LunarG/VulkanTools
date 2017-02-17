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

#include "vkreplay_factory.h"
#include "vktrace_trace_packet_identifiers.h"
#include "vkreplay.h"

namespace vktrace_replay {

vktrace_trace_packet_replay_library* ReplayFactory::Create(uint8_t tracerId) {
    vktrace_trace_packet_replay_library* pReplayer = NULL;
    // void* pLibrary = NULL;

    const VKTRACE_TRACER_REPLAYER_INFO* pReplayerInfo = &(gs_tracerReplayerInfo[tracerId]);

    if (pReplayerInfo->tracerId != tracerId) {
        vktrace_LogError("Replayer info for TracerId (%d) failed consistency check.", tracerId);
        assert(!"TracerId in VKTRACE_TRACER_REPLAYER_INFO does not match the requested tracerId. The array needs to be corrected.");
    }

    // Vulkan library is built into replayer executable
    if (tracerId == VKTRACE_TID_VULKAN) {
        pReplayer = VKTRACE_NEW(vktrace_trace_packet_replay_library);
        if (pReplayer == NULL) {
            vktrace_LogError("Failed to allocate replayer library.");
        } else {
            pReplayer->pLibrary = NULL;

            pReplayer->SetLogCallback = VkReplaySetLogCallback;
            pReplayer->SetLogLevel = VkReplaySetLogLevel;

            pReplayer->RegisterDbgMsgCallback = VkReplayRegisterDbgMsgCallback;
            pReplayer->GetSettings = VkReplayGetSettings;
            pReplayer->UpdateFromSettings = VkReplayUpdateFromSettings;
            pReplayer->Initialize = VkReplayInitialize;
            pReplayer->Deinitialize = VkReplayDeinitialize;
            pReplayer->Interpret = VkReplayInterpret;
            pReplayer->Replay = VkReplayReplay;
            pReplayer->Dump = VkReplayDump;
            pReplayer->GetFrameNumber = VkReplayGetFrameNumber;
            pReplayer->ResetFrameNumber = VkReplayResetFrameNumber;
        }
    }

    //    if (pReplayerInfo->needsReplayer == TRUE)
    //    {
    //        pLibrary = vktrace_platform_open_library(pReplayerInfo->replayerLibraryName);
    //        if (pLibrary == NULL)
    //        {
    //            vktrace_LogError("Failed to load replayer '%s.", pReplayerInfo->replayerLibraryName);
    //#if defined(PLATFORM_LINUX)
    //            char* error = dlerror();
    //            vktrace_LogError(error);
    //#endif
    //        }
    //    }
    //    else
    //    {
    //        vktrace_LogError("A replayer was requested for TracerId (%d), but it does not require a replayer.", tracerId);
    //        assert(!"Invalid TracerId supplied to ReplayFactory");
    //    }
    //
    //    if (pLibrary != NULL)
    //    {
    //        pReplayer = VKTRACE_NEW(vktrace_trace_packet_replay_library);
    //        if (pReplayer == NULL)
    //        {
    //            vktrace_LogError("Failed to allocate replayer library.");
    //            vktrace_platform_close_library(pLibrary);
    //        }
    //        else
    //        {
    //            pReplayer->pLibrary = pLibrary;
    //
    //            pReplayer->SetLogCallback = (funcptr_vkreplayer_setlogcallback)vktrace_platform_get_library_entrypoint(pLibrary,
    //            "SetLogCallback");
    //            pReplayer->SetLogLevel = (funcptr_vkreplayer_setloglevel)vktrace_platform_get_library_entrypoint(pLibrary,
    //            "SetLogLevel");
    //
    //            pReplayer->RegisterDbgMsgCallback =
    //            (funcptr_vkreplayer_registerdbgmsgcallback)vktrace_platform_get_library_entrypoint(pLibrary,
    //            "RegisterDbgMsgCallback");
    //            pReplayer->GetSettings = (funcptr_vkreplayer_getSettings)vktrace_platform_get_library_entrypoint(pLibrary,
    //            "GetSettings");
    //            pReplayer->UpdateFromSettings =
    //            (funcptr_vkreplayer_updatefromsettings)vktrace_platform_get_library_entrypoint(pLibrary, "UpdateFromSettings");
    //            pReplayer->Initialize = (funcptr_vkreplayer_initialize)vktrace_platform_get_library_entrypoint(pLibrary,
    //            "Initialize");
    //            pReplayer->Deinitialize = (funcptr_vkreplayer_deinitialize)vktrace_platform_get_library_entrypoint(pLibrary,
    //            "Deinitialize");
    //            pReplayer->Interpret = (funcptr_vkreplayer_interpret)vktrace_platform_get_library_entrypoint(pLibrary,
    //            "Interpret");
    //            pReplayer->Replay = (funcptr_vkreplayer_replay)vktrace_platform_get_library_entrypoint(pLibrary, "Replay");
    //            pReplayer->Dump = (funcptr_vkreplayer_dump)vktrace_platform_get_library_entrypoint(pLibrary, "Dump");
    //
    //            if (pReplayer->SetLogCallback == NULL ||
    //                pReplayer->SetLogLevel == NULL ||
    //                pReplayer->RegisterDbgMsgCallback == NULL ||
    //                pReplayer->GetSettings == NULL ||
    //                pReplayer->UpdateFromSettings == NULL ||
    //                pReplayer->Initialize == NULL ||
    //                pReplayer->Deinitialize == NULL ||
    //                pReplayer->Interpret == NULL ||
    //                pReplayer->Replay == NULL ||
    //                pReplayer->Dump == NULL)
    //            {
    //                VKTRACE_DELETE(pReplayer);
    //                vktrace_platform_close_library(pLibrary);
    //                pReplayer = NULL;
    //            }
    //        }
    //    }

    return pReplayer;
}

void ReplayFactory::Destroy(vktrace_trace_packet_replay_library** ppReplayer) {
    assert(ppReplayer != NULL);
    assert(*ppReplayer != NULL);
    if ((*ppReplayer)->pLibrary != NULL) {
        vktrace_platform_close_library((*ppReplayer)->pLibrary);
    }
    VKTRACE_DELETE(*ppReplayer);
    *ppReplayer = NULL;
}

}  // namespace vktrace_replay
