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
#pragma once
#include "vktraceviewer_trace_file_utils.h"
#include "vktraceviewer_view.h"
#include "vktrace_settings.h"

#include <QObject>

class vktraceviewer_QController : public QObject {
  public:
    vktraceviewer_QController() {}
    virtual ~vktraceviewer_QController() {}

    virtual const char* GetPacketIdString(uint16_t packetId) = 0;
    virtual vktrace_SettingGroup* GetSettings() = 0;
    virtual void UpdateFromSettings(vktrace_SettingGroup* pSettingGroups, unsigned int numSettingGroups) = 0;
    virtual vktrace_trace_packet_header* InterpretTracePacket(vktrace_trace_packet_header* pHeader) = 0;
    virtual bool LoadTraceFile(vktraceviewer_trace_file_info* pTraceFileInfo, vktraceviewer_view* pView) = 0;
    virtual void UnloadTraceFile(void) = 0;

  public slots:

  signals:
    void OutputMessage(VktraceLogLevel level, const QString& message);
    void OutputMessage(VktraceLogLevel level, uint64_t packetIndex, const QString& message);
};

extern "C" {
VKTRACER_EXPORT vktraceviewer_QController* VKTRACER_CDECL vtvCreateQController(void);
VKTRACER_EXPORT void VKTRACER_CDECL vtvDeleteQController(vktraceviewer_QController** ppController);

// entrypoints that must be exposed by each controller library
typedef vktraceviewer_QController*(VKTRACER_CDECL* funcptr_vktraceviewer_CreateQController)(void);
typedef void(VKTRACER_CDECL* funcptr_vktraceviewer_DeleteQController)(vktraceviewer_QController* pController);
}
