/**************************************************************************
 *
 * Copyright 2014 Lunarg, Inc.
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
 **************************************************************************/
#ifndef VKTRACEVIEWER_VK_QFILE_MODEL_H_
#define VKTRACEVIEWER_VK_QFILE_MODEL_H_

#include "vktrace_trace_packet_identifiers.h"
#include "vktraceviewer_QTraceFileModel.h"
#include <QObject>

class vktraceviewer_vk_QFileModel : public vktraceviewer_QTraceFileModel
{
    Q_OBJECT
public:
    vktraceviewer_vk_QFileModel(QObject * parent, vktraceviewer_trace_file_info *);
    virtual ~vktraceviewer_vk_QFileModel();

    virtual QString get_packet_string(const vktrace_trace_packet_header* pHeader) const;
    virtual QString get_packet_string_multiline(const vktrace_trace_packet_header* pHeader) const;

    virtual bool isDrawCall(const VKTRACE_TRACE_PACKET_ID packetId) const;

};

#endif //VKTRACEVIEWER_VK_QFILE_MODEL_H_
