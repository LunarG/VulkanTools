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
extern "C" {
#include "vktrace_trace_packet_utils.h"
#include "vktrace_vk_packet_id.h"
}

#include "vktraceviewer_vk_qfile_model.h"

vktraceviewer_vk_QFileModel::vktraceviewer_vk_QFileModel(QObject* parent, vktraceviewer_trace_file_info* pTraceFileInfo)
        : vktraceviewer_QTraceFileModel(parent, pTraceFileInfo)
{
}

vktraceviewer_vk_QFileModel::~vktraceviewer_vk_QFileModel()
{
}

QString vktraceviewer_vk_QFileModel::get_packet_string(const vktrace_trace_packet_header* pHeader) const
{
    if (pHeader->packet_id < VKTRACE_TPI_BEGIN_API_HERE)
    {
        return vktraceviewer_QTraceFileModel::get_packet_string(pHeader);
    }
    else
    {
        QString packetString = vktrace_stringify_vk_packet_id((const enum VKTRACE_TRACE_PACKET_ID_VK) pHeader->packet_id, pHeader);
        return packetString;
    }
}

QString vktraceviewer_vk_QFileModel::get_packet_string_multiline(const vktrace_trace_packet_header* pHeader) const
{
    if (pHeader->packet_id < VKTRACE_TPI_BEGIN_API_HERE)
    {
        return vktraceviewer_QTraceFileModel::get_packet_string_multiline(pHeader);
    }
    else
    {
        QString packetString = vktrace_stringify_vk_packet_id((const enum VKTRACE_TRACE_PACKET_ID_VK) pHeader->packet_id, pHeader);
        return packetString;
    }
}

bool vktraceviewer_vk_QFileModel::isDrawCall(const VKTRACE_TRACE_PACKET_ID packetId) const
{
    // TODO : Update this based on latest API updates
    bool isDraw = false;
    switch((VKTRACE_TRACE_PACKET_ID_VK)packetId)
    {
        case VKTRACE_TPI_VK_vkCmdDraw:
        case VKTRACE_TPI_VK_vkCmdDrawIndexed:
        case VKTRACE_TPI_VK_vkCmdDrawIndirect:
        case VKTRACE_TPI_VK_vkCmdDrawIndexedIndirect:
        case VKTRACE_TPI_VK_vkCmdDispatch:
        case VKTRACE_TPI_VK_vkCmdDispatchIndirect:
        case VKTRACE_TPI_VK_vkCmdCopyBuffer:
        case VKTRACE_TPI_VK_vkCmdCopyImage:
        case VKTRACE_TPI_VK_vkCmdCopyBufferToImage:
        case VKTRACE_TPI_VK_vkCmdCopyImageToBuffer:
        case VKTRACE_TPI_VK_vkCmdUpdateBuffer:
        case VKTRACE_TPI_VK_vkCmdFillBuffer:
        case VKTRACE_TPI_VK_vkCmdClearColorImage:
        case VKTRACE_TPI_VK_vkCmdClearDepthStencilImage:
        case VKTRACE_TPI_VK_vkCmdClearAttachments:
        case VKTRACE_TPI_VK_vkCmdResolveImage:
        {
            isDraw = true;
            break;
        }
        default:
        {
            isDraw = false;
        }
    }
    return isDraw;
}
