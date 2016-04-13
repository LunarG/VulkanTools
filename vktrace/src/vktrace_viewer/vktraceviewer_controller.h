/**************************************************************************
*
* Copyright 2014-2016 Valve Corporation
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
* Author: Peter Lohrmann <peterl@valvesoftware.com> <plohrmann@gmail.com>
**************************************************************************/
#pragma once
#include "vktraceviewer_trace_file_utils.h"
#include "vktraceviewer_view.h"
#include "vktrace_settings.h"

#include <QObject>

class vktraceviewer_QController : public QObject
{
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

extern "C"
{
VKTRACER_EXPORT vktraceviewer_QController* VKTRACER_CDECL vtvCreateQController(void);
VKTRACER_EXPORT void VKTRACER_CDECL vtvDeleteQController(vktraceviewer_QController* pController);

// entrypoints that must be exposed by each controller library
typedef vktraceviewer_QController* (VKTRACER_CDECL *funcptr_vktraceviewer_CreateQController)(void);
typedef void (VKTRACER_CDECL *funcptr_vktraceviewer_DeleteQController)(vktraceviewer_QController* pController);
}
