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
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Peter Lohrmann <peterl@valvesoftware.com>
 **************************************************************************/
#pragma once

#include "vktrace_multiplatform.h"
#include "vktrace_trace_packet_identifiers.h"
#include "vktrace_filelike.h"
#include "vktrace_memory.h"
#include "vktrace_process.h"

#ifdef __cplusplus
extern "C" {
#endif

// pUuid is expected to be an array of 4 unsigned ints
void vktrace_gen_uuid(uint32_t* pUuid);

uint64_t vktrace_get_time();

//=============================================================================
// trace file header

// there is a file header at the start of every trace file
vktrace_trace_file_header* vktrace_create_trace_file_header();

// deletes the trace file header and sets pointer to NULL
void vktrace_delete_trace_file_header(vktrace_trace_file_header** ppHeader);

static FILE* vktrace_write_trace_file_header(vktrace_process_info* pProcInfo)
{
    FILE* tracefp = NULL;
    vktrace_trace_file_header* pHeader = NULL;
    size_t items_written = 0;
    assert(pProcInfo != NULL);

    // open trace file
    tracefp = fopen(pProcInfo->traceFilename, "wb");
    if (tracefp == NULL)
    {
        vktrace_LogError("Cannot open trace file for writing %s.", pProcInfo->traceFilename);
        return tracefp;
    }
    else
    {
        vktrace_LogDebug("Creating trace file: '%s'", pProcInfo->traceFilename);
    }

    // populate header information
    pHeader = vktrace_create_trace_file_header();
    pHeader->first_packet_offset = sizeof(vktrace_trace_file_header);
    pHeader->tracer_count = 1;


    pHeader->tracer_id_array[0].id = pProcInfo->pCaptureThreads[0].tracerId;
    pHeader->tracer_id_array[0].is_64_bit = (sizeof(intptr_t) == 8) ? 1 : 0;

    // create critical section
    vktrace_create_critical_section(&pProcInfo->traceFileCriticalSection);

    // write header into file
    vktrace_enter_critical_section(&pProcInfo->traceFileCriticalSection);
    items_written = fwrite(pHeader, sizeof(vktrace_trace_file_header), 1, tracefp);
    vktrace_leave_critical_section(&pProcInfo->traceFileCriticalSection);
    if (items_written != 1)
    {
        vktrace_LogError("Failed to write trace file header.");
        vktrace_delete_critical_section(&pProcInfo->traceFileCriticalSection);
        fclose(tracefp);
        return NULL;
    }
    vktrace_delete_trace_file_header(&pHeader);
    return tracefp;
};


//=============================================================================
// trace packets
// There is a trace_packet_header before every trace_packet_body.
// Additional buffers will come after the trace_packet_body.

//=============================================================================
// Methods for creating, populating, and writing trace packets

// \param packet_size should include the total bytes for the specific type of packet, and any additional buffers needed by the packet.
//        The size of the header will be added automatically within the function.
vktrace_trace_packet_header* vktrace_create_trace_packet(uint8_t tracer_id, uint16_t packet_id, uint64_t packet_size, uint64_t additional_buffers_size);

// deletes a trace packet and sets pointer to NULL
void vktrace_delete_trace_packet(vktrace_trace_packet_header** ppHeader);

// gets the next address available to write a buffer into the packet
void* vktrace_trace_packet_get_new_buffer_address(vktrace_trace_packet_header* pHeader, uint64_t byteCount);

// copies buffer data into a trace packet at the specified offset (from the end of the header).
// it is up to the caller to ensure that buffers do not overlap.
void vktrace_add_buffer_to_trace_packet(vktrace_trace_packet_header* pHeader, void** ptr_address, uint64_t size, const void* pBuffer);

// converts buffer pointers into byte offset so that pointer can be interpretted after being read into memory
void vktrace_finalize_buffer_address(vktrace_trace_packet_header* pHeader, void** ptr_address);

// sets entrypoint end time
void vktrace_set_packet_entrypoint_end_time(vktrace_trace_packet_header* pHeader);

//void initialize_trace_packet_header(vktrace_trace_packet_header* pHeader, uint8_t tracer_id, uint16_t packet_id, uint64_t total_packet_size);
void vktrace_finalize_trace_packet(vktrace_trace_packet_header* pHeader);

// Write the trace packet to the filelike thing.
// This has no knowledge of the details of the packet other than its size.
void vktrace_write_trace_packet(const vktrace_trace_packet_header* pHeader, FileLike* pFile);

//=============================================================================
// Methods for Reading and interpretting trace packets

// Reads in the trace packet header, the body of the packet, and additional buffers
vktrace_trace_packet_header* vktrace_read_trace_packet(FileLike* pFile);

// converts a pointer variable that is currently byte offset into a pointer to the actual offset location
void* vktrace_trace_packet_interpret_buffer_pointer(vktrace_trace_packet_header* pHeader, intptr_t ptr_variable);

//=============================================================================
// trace packet message
// Interpretting a trace_packet_message should be done only when:
// 1) a trace_packet is first created and most of the contents are empty.
// 2) immediately after the packet was read from memory
// All other conversions of the trace packet body from the header should 
// be performed using a C-style cast.
static vktrace_trace_packet_message* vktrace_interpret_body_as_trace_packet_message(vktrace_trace_packet_header* pHeader)
{
    vktrace_trace_packet_message* pPacket = (vktrace_trace_packet_message*)pHeader->pBody;
    // update pointers
    pPacket->pHeader = pHeader;
    pPacket->message = (char*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->message);
    return pPacket;
}

#ifdef __cplusplus
}
#endif
