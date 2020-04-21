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
 * Author: Peter Lohrmann <peterl@valvesoftware.com> <plohrmann@gmail.com>
 **************************************************************************/
#ifndef VKTRACEVIEWER_QTRACEFILELOADER_H
#define VKTRACEVIEWER_QTRACEFILELOADER_H

#include <QObject>
#include "vktraceviewer_controller_factory.h"
#include "vktraceviewer_controller.h"

#define USE_STATIC_CONTROLLER_LIBRARY 1
class vktraceviewer_QTraceFileLoader : public QObject {
    Q_OBJECT
  public:
    vktraceviewer_QTraceFileLoader();
    virtual ~vktraceviewer_QTraceFileLoader();

  public slots:
    void loadTraceFile(const QString& filename);

  signals:
    void OutputMessage(VktraceLogLevel level, uint64_t packetIndex, const QString& message);
    void OutputMessage(VktraceLogLevel level, const QString& message);

    void TraceFileLoaded(bool bSuccess, const vktraceviewer_trace_file_info& fileInfo, const QString& controllerFilename);

    void Finished();

  private:
    vktraceviewer_trace_file_info m_traceFileInfo;
    vktraceviewer_controller_factory m_controllerFactory;
    vktraceviewer_QController* m_pController;
    QString m_controllerFilename;

    bool load_controllers(vktraceviewer_trace_file_info* pTraceFileInfo);

    bool populate_trace_file_info(vktraceviewer_trace_file_info* pTraceFileInfo);
};

#endif  // VKTRACEVIEWER_QTRACEFILELOADER_H
