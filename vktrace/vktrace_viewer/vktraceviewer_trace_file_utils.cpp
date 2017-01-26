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
#include "vktraceviewer_trace_file_utils.h"
#include "vktrace_memory.h"

BOOL vktraceviewer_populate_trace_file_info(vktraceviewer_trace_file_info* pTraceFileInfo)
{
    assert(pTraceFileInfo != NULL);
    assert(pTraceFileInfo->pFile != NULL);

    // read trace file header
    if (1 != fread(&(pTraceFileInfo->header), sizeof(vktrace_trace_file_header), 1, pTraceFileInfo->pFile))
    {
        vktraceviewer_output_error("Unable to read header from file.");
        return FALSE;
    }

    // Find out how many trace packets there are.

    // Seek to first packet
    long first_offset = pTraceFileInfo->header.first_packet_offset;
    int seekResult = fseek(pTraceFileInfo->pFile, first_offset, SEEK_SET);
    if (seekResult != 0)
    {
        vktraceviewer_output_warning("Failed to seek to the first packet offset in the trace file.");
    }

    uint64_t fileOffset = pTraceFileInfo->header.first_packet_offset;
    uint64_t packetSize = 0;
    while(1 == fread(&packetSize, sizeof(uint64_t), 1, pTraceFileInfo->pFile))
    {
        // success!
        pTraceFileInfo->packetCount++;
        fileOffset += packetSize;

        fseek(pTraceFileInfo->pFile, fileOffset, SEEK_SET);
    }

    if (pTraceFileInfo->packetCount == 0)
    {
        if (ferror(pTraceFileInfo->pFile) != 0)
        {
            perror("File Read error:");
            vktraceviewer_output_warning("There was an error reading the trace file.");
            return FALSE;
        }
        else if (feof(pTraceFileInfo->pFile) != 0)
        {
            vktraceviewer_output_warning("Reached the end of the file.");
        }
        vktraceviewer_output_warning("There are no trace packets in this trace file.");
        pTraceFileInfo->pPacketOffsets = NULL;
    }
    else
    {
        pTraceFileInfo->pPacketOffsets = VKTRACE_NEW_ARRAY(vktraceviewer_trace_file_packet_offsets, pTraceFileInfo->packetCount);

        // rewind to first packet and this time, populate the packet offsets
        if (fseek(pTraceFileInfo->pFile, first_offset, SEEK_SET) != 0)
        {
            vktraceviewer_output_error("Unable to rewind trace file to gather packet offsets.");
            return FALSE;
        }

        unsigned int packetIndex = 0;
        fileOffset = first_offset;
        while(1 == fread(&packetSize, sizeof(uint64_t), 1, pTraceFileInfo->pFile))
        {
            // the fread confirms that this packet exists
            // NOTE: We do not actually read the entire packet into memory right now.
            pTraceFileInfo->pPacketOffsets[packetIndex].fileOffset = fileOffset;

            // rewind slightly
            fseek(pTraceFileInfo->pFile, -1*(long)sizeof(uint64_t), SEEK_CUR);

            // allocate space for the packet and read it in
            pTraceFileInfo->pPacketOffsets[packetIndex].pHeader = (vktrace_trace_packet_header*)vktrace_malloc(packetSize);
            if (1 != fread(pTraceFileInfo->pPacketOffsets[packetIndex].pHeader, packetSize, 1, pTraceFileInfo->pFile))
            {
                vktraceviewer_output_error("Unable to read in a trace packet.");
                return FALSE;
            }

            // adjust pointer to body of the packet
            pTraceFileInfo->pPacketOffsets[packetIndex].pHeader->pBody = (uintptr_t)pTraceFileInfo->pPacketOffsets[packetIndex].pHeader + sizeof(vktrace_trace_packet_header);

            // now seek to what should be the next packet
            fileOffset += packetSize;
            packetIndex++;
        }

        if (fseek(pTraceFileInfo->pFile, first_offset, SEEK_SET) != 0)
        {
            vktraceviewer_output_error("Unable to rewind trace file to restore position.");
            return FALSE;
        }
    }

    return TRUE;
}
