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
#ifndef VKTRACEVIEWER_TRACE_FILE_UTILS_H_
#define VKTRACEVIEWER_TRACE_FILE_UTILS_H_

//#include <string>
#include <QString>

extern "C" {
#include "vktrace_trace_packet_identifiers.h"
}
#include "vktraceviewer_output.h"

struct vktraceviewer_trace_file_packet_offsets {
    // the file offset to this particular packet
    unsigned int fileOffset;

    // Pointer to the packet header if it's been read from disk
    vktrace_trace_packet_header* pHeader;
};

struct vktraceviewer_trace_file_info {
    // the trace file name & path
    char* filename;

    // the trace file
    FILE* pFile;

    // trace file header
    vktrace_trace_file_header* pHeader;
    struct_gpuinfo* pGpuinfo;

    // number of packets in file which should also be number of elements in pPacketOffsets array
    uint64_t packetCount;

    // array of packet offsets
    vktraceviewer_trace_file_packet_offsets* pPacketOffsets;
};

BOOL vktraceviewer_populate_trace_file_info(vktraceviewer_trace_file_info* pTraceFileInfo);

#endif  // VKTRACEVIEWER_TRACE_FILE_UTILS_H_
