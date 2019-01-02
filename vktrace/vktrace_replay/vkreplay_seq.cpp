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
 * Author: Jon Ashburn <jon@lunarg.com>
 **************************************************************************/
#include "vkreplay_seq.h"

extern "C" {
#include "vktrace_trace_packet_utils.h"
}

namespace vktrace_replay {

void Sequencer::preload_trace_file(uint64_t bufferedPacketCount) {
    if (m_pFile) {
        m_pRingBuffer = new RingBuffer<vktrace_trace_packet_header *>(bufferedPacketCount);
        vktrace_trace_packet_header *pPacket = NULL;
        do {
            pPacket = vktrace_read_trace_packet(m_pFile);
            if (pPacket != NULL) {
                while (!m_fileLoadCompleted) {
                    if (m_pRingBuffer->enqueue(pPacket)) {
                        break;
                    } else {
                        // vktrace_LogDebug("Ring buffer is FULL!");
                        usleep(10);
                    }
                }
            }
        } while (pPacket != NULL && !m_fileLoadCompleted);
        m_fileLoadCompleted = true;
    }
}

vktrace_trace_packet_header *Sequencer::get_next_packet() {
    if (!m_pFile) return (NULL);
    if (!m_pRingBuffer) {
        vktrace_delete_trace_packet_no_lock(&m_lastPacket);
        m_lastPacket = vktrace_read_trace_packet(m_pFile);
    } else {
        if (m_lastPacket) {
            vktrace_delete_trace_packet_no_lock(&m_lastPacket);
            m_pRingBuffer->dequeue_release();
        }

        while (1) {
            if (m_pRingBuffer->dequeue_get(m_lastPacket) || m_fileLoadCompleted) {
                break;
            } else {
                vktrace_LogWarning("Ring buffer is EMPTY! Performance impacted in preload!");
                usleep(10);
            }
        }
    }
    return (m_lastPacket);
}

void Sequencer::get_bookmark(seqBookmark &bookmark) { bookmark.file_offset = m_bookmark.file_offset; }

void Sequencer::set_bookmark(const seqBookmark &bookmark) { vktrace_FileLike_SetCurrentPosition(m_pFile, m_bookmark.file_offset); }

void Sequencer::record_bookmark() { m_bookmark.file_offset = vktrace_FileLike_GetCurrentPosition(m_pFile); }

} /* namespace vktrace_replay */
