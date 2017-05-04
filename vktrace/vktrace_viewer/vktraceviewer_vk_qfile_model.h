/**************************************************************************
 *
 * Copyright 2014 Lunarg, Inc.
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
 **************************************************************************/
#ifndef VKTRACEVIEWER_VK_QFILE_MODEL_H_
#define VKTRACEVIEWER_VK_QFILE_MODEL_H_

#include "vktrace_trace_packet_identifiers.h"
#include "vktraceviewer_QTraceFileModel.h"
#include <QObject>

class vktraceviewer_vk_QFileModel : public vktraceviewer_QTraceFileModel {
    Q_OBJECT
   public:
    vktraceviewer_vk_QFileModel(QObject* parent, vktraceviewer_trace_file_info*);
    virtual ~vktraceviewer_vk_QFileModel();

    virtual QString get_packet_string(const vktrace_trace_packet_header* pHeader) const;
    virtual QString get_packet_string_multiline(const vktrace_trace_packet_header* pHeader) const;

    virtual bool isDrawCall(const VKTRACE_TRACE_PACKET_ID_VK packetId) const;
};

#endif  // VKTRACEVIEWER_VK_QFILE_MODEL_H_
