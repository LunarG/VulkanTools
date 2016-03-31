/**************************************************************************
*
* Copyright 2015-2016 Valve Corporation
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
#ifndef VKTRACEVIEWER_QTRACEFILELOADER_H
#define VKTRACEVIEWER_QTRACEFILELOADER_H

#include <QObject>
#include "vktraceviewer_controller_factory.h"
#include "vktraceviewer_controller.h"

class vktraceviewer_QTraceFileLoader : public QObject
{
    Q_OBJECT
public:
    vktraceviewer_QTraceFileLoader();
    virtual ~vktraceviewer_QTraceFileLoader();

public slots:
    void loadTraceFile(const QString& filename);

signals:
    void OutputMessage(VktraceLogLevel level, uint64_t packetIndex, const QString& message);
    void OutputMessage(VktraceLogLevel level, const QString& message);

    void TraceFileLoaded(bool bSuccess, vktraceviewer_trace_file_info fileInfo, const QString& controllerFilename);

    void Finished();

private:
    vktraceviewer_trace_file_info m_traceFileInfo;
    vktraceviewer_controller_factory m_controllerFactory;
    vktraceviewer_QController* m_pController;
    QString m_controllerFilename;

    bool load_controllers(vktraceviewer_trace_file_info* pTraceFileInfo);

    bool populate_trace_file_info(vktraceviewer_trace_file_info* pTraceFileInfo);

};

#endif // VKTRACEVIEWER_QTRACEFILELOADER_H
