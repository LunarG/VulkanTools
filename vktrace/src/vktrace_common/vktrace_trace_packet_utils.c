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
#include "vktrace_trace_packet_utils.h"
#include "vktrace_interconnect.h"
#include "vktrace_filelike.h"
#ifdef WIN32
#include <rpc.h>
#pragma comment (lib, "Rpcrt4.lib")
#endif

#if defined(PLATFORM_LINUX)
#include <fcntl.h>
#include <time.h>
#endif

static uint64_t g_packet_index = 0;

void vktrace_gen_uuid(uint32_t* pUuid)
{
    uint32_t buf[] = { 0xABCDEF, 0x12345678, 0xFFFECABC, 0xABCDDEF0 };
    vktrace_platform_rand_s(buf, sizeof(buf)/sizeof(uint32_t));
    
    pUuid[0] = buf[0];
    pUuid[1] = buf[1];
    pUuid[2] = buf[2];
    pUuid[3] = buf[3];
}

#if defined(PLATFORM_LINUX)
uint64_t vktrace_get_time()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return ((uint64_t)time.tv_sec * 1000000000) + time.tv_nsec;
}
#elif defined(PLATFORM_WINDOWS)
uint64_t vktrace_get_time()
{
    // Should really avoid using RDTSC here since for RDTSC to be
    // accurate, the process needs to stay on the same CPU and the CPU
    // needs to stay at the same clock rate, which isn't always the case
    // with today's power managed CPUs.
    // But if all that is OK, the following one-liner could be used instead
    // of the rest of this function.
    //
    // return __rdtsc();
    //
    LARGE_INTEGER count;
    static LARGE_INTEGER start, freq;
    if (0 == start.QuadPart) {
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);
    }
    QueryPerformanceCounter(&count);
    // Using a relative (from start) count here postpones overflow as we convert to ns.
    return (uint64_t)(((count.QuadPart - start.QuadPart) * 1000000000) / freq.QuadPart);
}
#else
uint64_t vktrace_get_time()
{
    return 0;
}
#endif

//=============================================================================
// trace file header

vktrace_trace_file_header* vktrace_create_trace_file_header()
{
    vktrace_trace_file_header* pHeader = VKTRACE_NEW(vktrace_trace_file_header);
    memset(pHeader, 0, sizeof(vktrace_trace_file_header));
    pHeader->trace_file_version = VKTRACE_TRACE_FILE_VERSION;
    vktrace_gen_uuid(pHeader->uuid);
    pHeader->trace_start_time = vktrace_get_time();

    return pHeader;
}

void vktrace_delete_trace_file_header(vktrace_trace_file_header** ppHeader)
{
    vktrace_free(*ppHeader);
    *ppHeader = NULL;
}

//=============================================================================
// Methods for creating, populating, and writing trace packets

vktrace_trace_packet_header* vktrace_create_trace_packet(uint8_t tracer_id, uint16_t packet_id, uint64_t packet_size, uint64_t additional_buffers_size)
{
    // Always allocate at least enough space for the packet header
    uint64_t total_packet_size = sizeof(vktrace_trace_packet_header) + packet_size + additional_buffers_size;
    void* pMemory = vktrace_malloc((size_t)total_packet_size);
    memset(pMemory, 0, (size_t)total_packet_size);

    vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)pMemory;
    pHeader->size = total_packet_size;
    pHeader->global_packet_index = g_packet_index++;
    pHeader->tracer_id = tracer_id;
    pHeader->thread_id = vktrace_platform_get_thread_id();
    pHeader->packet_id = packet_id;
    if (pHeader->vktrace_begin_time == 0)
        pHeader->vktrace_begin_time = vktrace_get_time();
    pHeader->entrypoint_begin_time = pHeader->vktrace_begin_time;
    pHeader->entrypoint_end_time = 0;
    pHeader->vktrace_end_time = 0;
    pHeader->next_buffers_offset = sizeof(vktrace_trace_packet_header) + packet_size; // initial offset is from start of header to after the packet body
    if (total_packet_size > sizeof(vktrace_trace_packet_header))
    {
        pHeader->pBody = (uintptr_t)(((char*)pMemory) + sizeof(vktrace_trace_packet_header));
    }
    return pHeader;
}

void vktrace_delete_trace_packet(vktrace_trace_packet_header** ppHeader)
{
    if (ppHeader == NULL)
        return;
    if (*ppHeader == NULL)
        return;

    VKTRACE_DELETE(*ppHeader);
    *ppHeader = NULL;
}

void* vktrace_trace_packet_get_new_buffer_address(vktrace_trace_packet_header* pHeader, uint64_t byteCount)
{
    void* pBufferStart;
    assert(byteCount > 0);
    assert(pHeader->size >= pHeader->next_buffers_offset + byteCount);
    if (pHeader->size < pHeader->next_buffers_offset + byteCount || byteCount == 0)
    {
        // not enough memory left in packet to hold buffer
        // or request is for 0 bytes
        return NULL;
    }

    pBufferStart = (void*)((char*)pHeader + pHeader->next_buffers_offset);
    pHeader->next_buffers_offset += byteCount;
    return pBufferStart;
}

void vktrace_add_buffer_to_trace_packet(vktrace_trace_packet_header* pHeader, void** ptr_address, uint64_t size, const void* pBuffer)
{
    assert(ptr_address != NULL);
    if (pBuffer == NULL || size == 0)
    {
        *ptr_address = NULL;
    }
    else
    {
        // set ptr to the location of the added buffer
        *ptr_address = vktrace_trace_packet_get_new_buffer_address(pHeader, size);

        // copy buffer to the location
        memcpy(*ptr_address, pBuffer, (size_t)size);
    }
}

void vktrace_finalize_buffer_address(vktrace_trace_packet_header* pHeader, void** ptr_address)
{
    assert(ptr_address != NULL);

    if (*ptr_address != NULL)
    {
        // turn ptr into an offset from the packet body
        uint64_t offset = (uint64_t)*ptr_address - (uint64_t) (pHeader->pBody);
        *ptr_address = (void*)offset;
    }
}

void vktrace_set_packet_entrypoint_end_time(vktrace_trace_packet_header* pHeader)
{
    pHeader->entrypoint_end_time = vktrace_get_time();
}

void vktrace_finalize_trace_packet(vktrace_trace_packet_header* pHeader)
{
    if (pHeader->entrypoint_end_time == 0)
    {
        vktrace_set_packet_entrypoint_end_time(pHeader);
    }
    pHeader->vktrace_end_time = vktrace_get_time();
}

void vktrace_write_trace_packet(const vktrace_trace_packet_header* pHeader, FileLike* pFile)
{
    static int errorCount = 0;
    BOOL res = vktrace_FileLike_WriteRaw(pFile, pHeader, (size_t)pHeader->size);
    if (!res && pHeader->packet_id != VKTRACE_TPI_MARKER_TERMINATE_PROCESS && errorCount < 10)
    {
        errorCount++;
        vktrace_LogError("Failed to send trace packet index %u packetId %u size %u.", pHeader->global_packet_index, pHeader->packet_id, pHeader->size);
    }
}

//=============================================================================
// Methods for Reading and interpretting trace packets

vktrace_trace_packet_header* vktrace_read_trace_packet(FileLike* pFile)
{
    // read size
    // allocate space
    // offset to after size
    // read the rest of the packet
    uint64_t total_packet_size = 0;
    void* pMemory;
    vktrace_trace_packet_header* pHeader;

    if (vktrace_FileLike_ReadRaw(pFile, &total_packet_size, sizeof(uint64_t)) == FALSE)
    {
        return NULL;
    }

    // allocate space
    pMemory = vktrace_malloc((size_t)total_packet_size);
    pHeader = (vktrace_trace_packet_header*)pMemory;

    if (pHeader != NULL)
    {
        pHeader->size = total_packet_size;
        if (vktrace_FileLike_ReadRaw(pFile, (char*)pHeader + sizeof(uint64_t), (size_t)total_packet_size - sizeof(uint64_t)) == FALSE)
        {
            vktrace_LogError("Failed to read trace packet with size of %u.", total_packet_size);
            return NULL;
        }

        pHeader->pBody = (uintptr_t)pHeader + sizeof(vktrace_trace_packet_header);
    }
    else {
        vktrace_LogError("Malloc failed in vktrace_read_trace_packet of size %u.", total_packet_size);
    }

    return pHeader;
}

void* vktrace_trace_packet_interpret_buffer_pointer(vktrace_trace_packet_header* pHeader, intptr_t ptr_variable)
{
    // the pointer variable actually contains a byte offset from the packet body to the start of the buffer.
    uint64_t offset = ptr_variable;
    void* buffer_location;

    // if the offset is 0, then we know the pointer to the buffer was NULL, so no buffer exists and we return NULL.
    if (offset == 0)
        return NULL;

    buffer_location = (char*)(pHeader->pBody) + offset;
    return buffer_location;
}
