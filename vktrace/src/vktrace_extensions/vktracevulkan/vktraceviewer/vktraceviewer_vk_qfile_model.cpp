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

#include "vktraceviewer_vk_qfile_model.h"
extern "C" {
#include "vktrace_trace_packet_utils.h"
#include "vktrace_vk_packet_id.h"
}

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
