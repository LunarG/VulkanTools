/**************************************************************************
 *
 * Copyright 2016 Valve Corporation
 * Copyright (C) 2016 LunarG, Inc.
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
#include "vktraceviewer_vk_qgroupframesproxymodel.h"

extern "C" {
#include "vktrace_vk_packet_id.h"
}

void vktraceviewer_vk_QGroupFramesProxyModel::buildGroups() {
    m_mapSourceRowToProxyGroupRow.clear();
    m_frameList.clear();
    m_curFrameCount = 0;

    if (sourceModel() != NULL) {
        FrameInfo* pCurFrame = addNewFrame();
        m_mapSourceRowToProxyGroupRow.reserve(sourceModel()->rowCount());
        for (int srcRow = 0; srcRow < sourceModel()->rowCount(); srcRow++) {
            int proxyRow = pCurFrame->mapChildRowToSourceRow.count();

            // map source row to it's corresponding row in the proxy group.
            m_mapSourceRowToProxyGroupRow.append(proxyRow);

            // add this src row to the current proxy group.
            pCurFrame->mapChildRowToSourceRow.append(srcRow);

            // Should a new frame be started based on the API call in the previous row?
            // If source data is a frame boundary make a new frame
            QModelIndex tmpIndex = sourceModel()->index(srcRow, 0);
            assert(tmpIndex.isValid());
            vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)tmpIndex.internalPointer();
            if (pHeader != NULL && pHeader->tracer_id == VKTRACE_TID_VULKAN &&
                pHeader->packet_id == VKTRACE_TPI_VK_vkQueuePresentKHR) {
                pCurFrame = addNewFrame();
            }
        }  // end for each source row
    }
}
