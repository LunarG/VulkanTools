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
#pragma once

#include <QColor>
#include <QFont>
#include <QSize>
#include <qabstractitemmodel.h>
#include "vktraceviewer_trace_file_utils.h"

class vktraceviewer_QTraceFileModel : public QAbstractItemModel {
    Q_OBJECT
   public:
    vktraceviewer_QTraceFileModel(QObject* parent, vktraceviewer_trace_file_info* pTraceFileInfo) : QAbstractItemModel(parent) {
        m_pTraceFileInfo = pTraceFileInfo;
    }

    virtual ~vktraceviewer_QTraceFileModel() {}

    virtual bool isDrawCall(const VKTRACE_TRACE_PACKET_ID_VK packetId) const { return false; }

    virtual QString get_packet_string(const vktrace_trace_packet_header* pHeader) const {
        switch (pHeader->packet_id) {
            case VKTRACE_TPI_MESSAGE: {
                vktrace_trace_packet_message* pPacket = (vktrace_trace_packet_message*)pHeader->pBody;
                return QString(pPacket->message);
            }
            case VKTRACE_TPI_MARKER_CHECKPOINT:
            case VKTRACE_TPI_MARKER_API_BOUNDARY:
            case VKTRACE_TPI_MARKER_API_GROUP_BEGIN:
            case VKTRACE_TPI_MARKER_API_GROUP_END:
            case VKTRACE_TPI_MARKER_TERMINATE_PROCESS:
            case VKTRACE_TPI_PORTABILITY_TABLE:
            default: { return QString("%1").arg(pHeader->packet_id); }
        }
    }

    virtual QString get_packet_string_multiline(const vktrace_trace_packet_header* pHeader) const {
        // Default implemention is naive.
        return get_packet_string(pHeader);
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const {
        if (parent.column() > 0) {
            return 0;
        }

        int rowCount = 0;
        if (m_pTraceFileInfo != NULL) {
            rowCount = m_pTraceFileInfo->packetCount;
        }

        if (parent.isValid()) {
            // there is a valid parent, so this is a child node, which has no rows
            rowCount = 0;
        }

        return rowCount;
    }

    enum Columns {
        Column_EntrypointName,
        Column_TracerId,
        Column_PacketIndex,
        Column_ThreadId,
        Column_BeginTime,
        Column_EndTime,
        Column_PacketSize,
        Column_CpuDuration,
        cNumColumns
    };

    int columnCount(const QModelIndex& parent = QModelIndex()) const { return cNumColumns; }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const {
        if (m_pTraceFileInfo == NULL) {
            return QVariant();
        }

        if (role == Qt::FontRole) {
            vktrace_trace_packet_header* pHeader =
                (vktrace_trace_packet_header*)this->index(index.row(), Column_EntrypointName, index.parent()).internalPointer();
            if (isDrawCall((VKTRACE_TRACE_PACKET_ID_VK)pHeader->packet_id)) {
                QFont font;
                font.setBold(true);
                return font;
            }
        }

        if (role == Qt::SizeHintRole) {
            return QSize(20, 20);
        }

        if (role == Qt::BackgroundRole && !m_searchString.isEmpty()) {
            QVariant cellData = data(index, Qt::DisplayRole);
            QString string = cellData.toString();
            if (string.contains(m_searchString, Qt::CaseInsensitive)) {
                return QColor(Qt::yellow);
            }
        }

        if (role == Qt::DisplayRole) {
            switch (index.column()) {
                case Column_EntrypointName: {
                    vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)index.internalPointer();
                    QString apiStr = this->get_packet_string(pHeader);
                    return apiStr;
                }
                case Column_TracerId:
                    return QVariant(*(uint8_t*)index.internalPointer());
                case Column_PacketIndex:
                case Column_ThreadId:
                    return QVariant(*(uint32_t*)index.internalPointer());
                case Column_BeginTime:
                case Column_EndTime:
                case Column_PacketSize:
                    return QVariant(*(unsigned long long*)index.internalPointer());
                case Column_CpuDuration: {
                    vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)index.internalPointer();
                    uint64_t duration = pHeader->entrypoint_end_time - pHeader->entrypoint_begin_time;
                    return QVariant((unsigned int)duration);
                }
            }
        }

        if (role == Qt::ToolTipRole && index.column() == Column_EntrypointName) {
            vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)index.internalPointer();
            QString tip;
            tip += "<html><table>";
#if defined(_DEBUG)
            tip += "<tr><td><b>Packet header (pHeader->):</b></td><td/></tr>";
            tip += QString("<tr><td>size</td><td>= %1 bytes</td></tr>").arg(pHeader->size);
            tip += QString("<tr><td>global_packet_index</td><td>= %1</td></tr>").arg(pHeader->global_packet_index);
            tip += QString("<tr><td>tracer_id</td><td>= %1</td></tr>").arg(pHeader->tracer_id);
            tip += QString("<tr><td>packet_id</td><td>= %1</td></tr>").arg(pHeader->packet_id);
            tip += QString("<tr><td>thread_id</td><td>= %1</td></tr>").arg(pHeader->thread_id);
            tip += QString("<tr><td>vktrace_begin_time</td><td>= %1</td></tr>").arg(pHeader->vktrace_begin_time);
            tip += QString("<tr><td>entrypoint_begin_time</td><td>= %1</td></tr>").arg(pHeader->entrypoint_begin_time);
            tip += QString("<tr><td>entrypoint_end_time</td><td>= %1 (%2)</td></tr>")
                       .arg(pHeader->entrypoint_end_time)
                       .arg(pHeader->entrypoint_end_time - pHeader->entrypoint_begin_time);
            tip += QString("<tr><td>vktrace_end_time</td><td>= %1 (%2)</td></tr>")
                       .arg(pHeader->vktrace_end_time)
                       .arg(pHeader->vktrace_end_time - pHeader->vktrace_begin_time);
            tip += QString("<tr><td>next_buffers_offset</td><td>= %1</td></tr>").arg(pHeader->next_buffers_offset);
            tip += QString("<tr><td>pBody</td><td>= %1</td></tr>").arg(pHeader->pBody);
            tip += "<br>";
#endif
            tip += "<tr><td><b>";
            QString multiline = this->get_packet_string_multiline(pHeader);
            // only replaces the first '('
            multiline.replace(multiline.indexOf("("), 1, "</b>(</td><td/></tr><tr><td>");
            multiline.replace(", ", ", </td></tr><tr><td>");
            multiline.replace(" = ", "</td><td>= ");

            // only replaces the final ')'
            multiline.replace(multiline.lastIndexOf(")"), 1, "</td></tr><tr><td>)</td><td>");
            tip += multiline;
            tip += "</td></tr></table>";
            tip += "</html>";
            return tip;
        }

        return QVariant();
    }

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const {
        if (m_pTraceFileInfo == NULL || m_pTraceFileInfo->packetCount == 0) {
            return createIndex(row, column);
        }

        if ((uint64_t)row >= m_pTraceFileInfo->packetCount) {
            return QModelIndex();
        }

        vktrace_trace_packet_header* pHeader = m_pTraceFileInfo->pPacketOffsets[row].pHeader;
        void* pData = NULL;
        switch (column) {
            case Column_EntrypointName:
                pData = pHeader;
                break;
            case Column_TracerId:
                pData = &pHeader->tracer_id;
                break;
            case Column_PacketIndex:
                pData = &pHeader->global_packet_index;
                break;
            case Column_ThreadId:
                pData = &pHeader->thread_id;
                break;
            case Column_BeginTime:
                pData = &pHeader->entrypoint_begin_time;
                break;
            case Column_EndTime:
                pData = &pHeader->entrypoint_end_time;
                break;
            case Column_PacketSize:
                pData = &pHeader->size;
                break;
            case Column_CpuDuration:
                pData = pHeader;
                break;
        }

        return createIndex(row, column, pData);
    }

    QModelIndex parent(const QModelIndex& index) const { return QModelIndex(); }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const {
        if (role == Qt::DisplayRole) {
            if (orientation == Qt::Horizontal) {
                switch (section) {
                    case Column_EntrypointName:
                        return QString("API Call");
                    case Column_TracerId:
                        return QString("Tracer ID");
                    case Column_PacketIndex:
                        return QString("Index");
                    case Column_ThreadId:
                        return QString("Thread ID");
                    case Column_BeginTime:
                        return QString("Start Time");
                    case Column_EndTime:
                        return QString("End Time");
                    case Column_PacketSize:
                        return QString("Size (bytes)");
                    case Column_CpuDuration:
                        return QString("Duration");
                }
            }
        }
        return QVariant();
    }

    void set_highlight_search_string(const QString searchString) { m_searchString = searchString; }

   private:
    vktraceviewer_trace_file_info* m_pTraceFileInfo;
    QString m_searchString;
};
