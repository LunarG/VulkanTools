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
#pragma once

extern "C" {
#include "vktrace_filelike.h"
#include "vktrace_trace_packet_identifiers.h"
#include "vktrace_trace_packet_utils.h"
}
#include <memory>

/* Class to handle fetching and sequencing packets from a tracefile.
 * Contains no knowledge of type of tracer needed to process packet.
 * Requires low level file/stream reading/seeking support. */
namespace vktrace_replay {

template <class T>
class RingBuffer {
   public:
    explicit RingBuffer(uint64_t size) : m_buffer(std::unique_ptr<T[]>(new T[size])), m_bufferSize(size) {}

    bool enqueue(T item) {
        if (full()) {
            return false;
        }
        m_buffer[m_write] = item;
        m_write = (m_write + 1) % m_bufferSize;
        return true;
    };

    bool dequeue_get(T &item) {
        if (empty()) {
            return false;
        }
        // Read data.
        // Advance the tail will be done in dequeue_release()
        item = m_buffer[m_read];
        return true;
    };

    void dequeue_release() {
        if (empty()) {
            return;
        }
        m_read = (m_read + 1) % m_bufferSize;
    };

    bool full() const { return (m_read == ((m_write + 1) % m_bufferSize)); };
    bool empty() const { return (m_read == m_write); };

   private:
    std::unique_ptr<T[]> m_buffer;
    const uint64_t m_bufferSize;
    uint64_t m_write = 0;
    uint64_t m_read = 0;
};

struct seqBookmark {
    uint64_t file_offset;
};

// replay Sequencer interface
class AbstractSequencer {
   public:
    virtual ~AbstractSequencer() {}
    virtual vktrace_trace_packet_header *get_next_packet() = 0;
    virtual void get_bookmark(seqBookmark &bookmark) = 0;
    virtual void set_bookmark(const seqBookmark &bookmark) = 0;
};

class Sequencer : public AbstractSequencer {
   public:
    Sequencer(FileLike *pFile) : m_lastPacket(NULL), m_pFile(pFile), m_pRingBuffer(NULL), m_fileLoadCompleted(false) {}
    ~Sequencer() { this->clean_up(); }

    void clean_up() {
        if (m_lastPacket) {
            free(m_lastPacket);
            m_lastPacket = NULL;
        }
        if (m_pRingBuffer) {
            m_pRingBuffer->dequeue_release();
            while (m_pRingBuffer->dequeue_get(m_lastPacket)) {
                vktrace_delete_trace_packet_no_lock(&m_lastPacket);
                m_pRingBuffer->dequeue_release();
            }
            delete (m_pRingBuffer);
            m_pRingBuffer = NULL;
            m_fileLoadCompleted = false;
        }
    }

    vktrace_trace_packet_header *get_next_packet();
    void get_bookmark(seqBookmark &bookmark);
    void set_bookmark(const seqBookmark &bookmark);
    void record_bookmark();
    void preload_trace_file(uint64_t buffered_packet_count);
    bool ready_to_replay() { return (m_pRingBuffer && (m_pRingBuffer->full() || m_fileLoadCompleted)); };
    void stop_preload() { m_fileLoadCompleted = true; };

   private:
    vktrace_trace_packet_header *m_lastPacket;
    seqBookmark m_bookmark;
    FileLike *m_pFile;
    RingBuffer<vktrace_trace_packet_header *> *m_pRingBuffer;
    bool m_fileLoadCompleted;
};

} /* namespace vktrace_replay */
