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

#if defined(__cplusplus)
extern "C" {
#endif

// pUuid is expected to be an array of 4 unsigned ints
void vktrace_gen_uuid(uint32_t* pUuid);

uint64_t vktrace_get_time();

void vktrace_initialize_trace_packet_utils();
void vktrace_deinitialize_trace_packet_utils();

uint64_t get_endianess();
uint64_t get_arch();
uint64_t get_os();

static FILE* vktrace_open_trace_file(vktrace_process_capture_trace_thread_info* trace_thread_info) {
    FILE* tracefp = NULL;
    assert(trace_thread_info != NULL);

    // For any title captured by vktrace, there is a base trace file
    // name which is a mermber of vktrace_process_info: "traceFilename".
    // it's specified by user or by default.
    //
    // One title may have multiple processes running and thus generate
    // multiple trace files. The first one of these trace files will
    // have same name with the base trace file name and the name of
    // others will be a concatenated string which is base name + "-n".
    // for example, the second one name will be base name+"-1",
    // the third one name will be base name + "-2".....
    //
    // open trace file for the trace thread to record into it.
    if (trace_thread_info->traceFileIndex != 0) {
        char index[16];
        char* process_trace_file_name =
            vktrace_allocate_and_copy_n(trace_thread_info->pProcessInfo->traceFilename,
                strlen(trace_thread_info->pProcessInfo->traceFilename) + 1 + sizeof(index));
        assert(process_trace_file_name != NULL);
        sprintf(index, "-%d", trace_thread_info->traceFileIndex);
        strcat(process_trace_file_name, index);
        tracefp = fopen(process_trace_file_name, "w+b");
        vktrace_free(process_trace_file_name);
    } else {
        tracefp = fopen(trace_thread_info->pProcessInfo->traceFilename, "w+b");
    }

    if (tracefp == NULL) {
        vktrace_LogError("Cannot open trace file for writing %s.", trace_thread_info->pProcessInfo->traceFilename);
        return tracefp;
    } else {
        vktrace_LogDebug("Creating trace file: '%s'", trace_thread_info->pProcessInfo->traceFilename);
    }

    // create critical section
    if (trace_thread_info->pProcessInfo->traceFileCriticalSectionCreated != TRUE) {
        vktrace_create_critical_section(&trace_thread_info->pProcessInfo->traceFileCriticalSection);
        trace_thread_info->pProcessInfo->traceFileCriticalSectionCreated = TRUE;
    }

    return tracefp;
};

//=============================================================================
// trace packets
// There is a trace_packet_header before every trace_packet_body.
// Additional buffers will come after the trace_packet_body.

//=============================================================================
// Methods for creating, populating, and writing trace packets

// \param packet_size should include the total bytes for the specific type of packet, and any additional buffers needed by the
// packet.
//        The size of the header will be added automatically within the function.
vktrace_trace_packet_header* vktrace_create_trace_packet(uint8_t tracer_id, uint16_t packet_id, uint64_t packet_size,
                                                         uint64_t additional_buffers_size);

// deletes a trace packet and sets pointer to NULL, this function should be used on a packet created to write to trace file
void vktrace_delete_trace_packet(vktrace_trace_packet_header** ppHeader);

// gets the next address available to write a buffer into the packet
void* vktrace_trace_packet_get_new_buffer_address(vktrace_trace_packet_header* pHeader, uint64_t byteCount);

// copies buffer data into a trace packet at the specified offset (from the end of the header).
// it is up to the caller to ensure that buffers do not overlap.
void vktrace_add_buffer_to_trace_packet(vktrace_trace_packet_header* pHeader, void** ptr_address, uint64_t size,
                                        const void* pBuffer);

// adds pNext structures to a trace packet
void vktrace_add_pnext_structs_to_trace_packet(vktrace_trace_packet_header* pHeader, void* pOut, const void* pIn);

// converts buffer pointers into byte offset so that pointer can be interpretted after being read into memory
void vktrace_finalize_buffer_address(vktrace_trace_packet_header* pHeader, void** ptr_address);

// sets entrypoint end time
void vktrace_set_packet_entrypoint_end_time(vktrace_trace_packet_header* pHeader);

// void initialize_trace_packet_header(vktrace_trace_packet_header* pHeader, uint8_t tracer_id, uint16_t packet_id, uint64_t
// total_packet_size);
void vktrace_finalize_trace_packet(vktrace_trace_packet_header* pHeader);

// Write the trace packet to the filelike thing.
// This has no knowledge of the details of the packet other than its size.
void vktrace_write_trace_packet(const vktrace_trace_packet_header* pHeader, FileLike* pFile);

//=============================================================================
// Methods for Reading and interpretting trace packets

// Reads in the trace packet header, the body of the packet, and additional buffers
vktrace_trace_packet_header* vktrace_read_trace_packet(FileLike* pFile);

// deletes a trace packet and sets pointer to NULL, this function should be used on a packet read from trace file
void vktrace_delete_trace_packet_no_lock(vktrace_trace_packet_header** ppHeader);

// converts a pointer variable that is currently byte offset into a pointer to the actual offset location
void* vktrace_trace_packet_interpret_buffer_pointer(vktrace_trace_packet_header* pHeader, intptr_t ptr_variable);

// Adding to packets TODO: Move to codegen
void add_VkApplicationInfo_to_packet(vktrace_trace_packet_header* pHeader, VkApplicationInfo** ppStruct,
                                     const VkApplicationInfo* pInStruct);
void add_VkInstanceCreateInfo_to_packet(vktrace_trace_packet_header* pHeader, VkInstanceCreateInfo** ppStruct,
                                        VkInstanceCreateInfo* pInStruct);
void add_VkDeviceCreateInfo_to_packet(vktrace_trace_packet_header* pHeader, VkDeviceCreateInfo** ppStruct,
                                      const VkDeviceCreateInfo* pInStruct);

// Interpreting packets TODO: Move to codegen
VkInstanceCreateInfo* interpret_VkInstanceCreateInfo(vktrace_trace_packet_header* pHeader, intptr_t ptr_variable);
VkDeviceCreateInfo* interpret_VkDeviceCreateInfo(vktrace_trace_packet_header* pHeader, intptr_t ptr_variable);
void interpret_VkPipelineShaderStageCreateInfo(vktrace_trace_packet_header* pHeader, VkPipelineShaderStageCreateInfo* pShader);
VkDeviceGroupDeviceCreateInfo* interpret_VkDeviceGroupDeviceCreateInfoKHX(vktrace_trace_packet_header* pHeader,
                                                                          intptr_t ptr_variable);
// converts the Vulkan struct pnext chain that is currently byte offsets into pointers
void vktrace_interpret_pnext_pointers(vktrace_trace_packet_header* pHeader, void* struct_ptr);

//=============================================================================
// trace packet message
// Interpretting a trace_packet_message should be done only when:
// 1) a trace_packet is first created and most of the contents are empty.
// 2) immediately after the packet was read from memory
// All other conversions of the trace packet body from the header should
// be performed using a C-style cast.
static vktrace_trace_packet_message* vktrace_interpret_body_as_trace_packet_message(vktrace_trace_packet_header* pHeader) {
    vktrace_trace_packet_message* pPacket = (vktrace_trace_packet_message*)pHeader->pBody;
    // update pointers
    pPacket->pHeader = pHeader;
    pPacket->message = (char*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->message);
    return pPacket;
}

#if defined(__cplusplus)
}
#endif
