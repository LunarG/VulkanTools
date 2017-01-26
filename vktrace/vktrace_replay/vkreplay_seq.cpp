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

vktrace_trace_packet_header * Sequencer::get_next_packet()
{
    vktrace_free(m_lastPacket);
    if (!m_pFile)
        return (NULL);
    m_lastPacket = vktrace_read_trace_packet(m_pFile);
    return(m_lastPacket);
}

void Sequencer::get_bookmark(seqBookmark &bookmark) {
    bookmark.file_offset = m_bookmark.file_offset;
}


void Sequencer::set_bookmark(const seqBookmark &bookmark) {
    fseek(m_pFile->mFile, m_bookmark.file_offset, SEEK_SET);
}

void Sequencer::record_bookmark()
{
    m_bookmark.file_offset = ftell(m_pFile->mFile);
}

} /* namespace vktrace_replay */
