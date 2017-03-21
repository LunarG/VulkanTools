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

#include "vktrace_common.h"

#define VKTRACE_TRACE_FILE_VERSION_2 0x0002
#define VKTRACE_TRACE_FILE_VERSION_3 0x0003
#define VKTRACE_TRACE_FILE_VERSION_4 0x0004
#define VKTRACE_TRACE_FILE_VERSION_5 0x0005
#define VKTRACE_TRACE_FILE_VERSION_6 0x0006
#define VKTRACE_TRACE_FILE_VERSION VKTRACE_TRACE_FILE_VERSION_6
#define VKTRACE_TRACE_FILE_VERSION_MINIMUM_COMPATIBLE VKTRACE_TRACE_FILE_VERSION_6

#define VKTRACE_FILE_MAGIC 0xABADD068ADEAFD0C

#define VKTRACE_MAX_TRACER_ID_ARRAY_SIZE 16  // Should be multiple of 8

typedef enum VKTRACE_TRACER_ID {
    VKTRACE_TID_RESERVED = 0,
    VKTRACE_TID_GL_FPS,
    VKTRACE_TID_VULKAN
    // Max enum must be less than VKTRACE_MAX_TRACER_ID_ARRAY_SIZE
} VKTRACE_TRACER_ID;

typedef struct VKTRACE_TRACER_REPLAYER_INFO {
    VKTRACE_TRACER_ID tracerId;
    BOOL needsReplayer;
    const char* const replayerLibraryName;
    const char* const debuggerLibraryname;
} VKTRACE_TRACER_REPLAYER_INFO;

// The index here should match the value of the VKTRACE_TRACER_ID
static const VKTRACE_TRACER_REPLAYER_INFO gs_tracerReplayerInfo[VKTRACE_MAX_TRACER_ID_ARRAY_SIZE] = {
    {VKTRACE_TID_RESERVED, FALSE, "", ""},
    {VKTRACE_TID_GL_FPS, FALSE, "", ""},
    {VKTRACE_TID_VULKAN, TRUE, VKTRACE_LIBRARY_NAME(vulkan_replay), VKTRACE_LIBRARY_NAME(vktraceviewer_vk)},
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
};

typedef enum _VKTRACE_TRACE_PACKET_ID {
    VKTRACE_TPI_MESSAGE,
    VKTRACE_TPI_MARKER_CHECKPOINT,
    VKTRACE_TPI_MARKER_API_BOUNDARY,
    VKTRACE_TPI_MARKER_API_GROUP_BEGIN,
    VKTRACE_TPI_MARKER_API_GROUP_END,
    VKTRACE_TPI_MARKER_TERMINATE_PROCESS,
    VKTRACE_TPI_PORTABILITY_TABLE,
    VKTRACE_TPI_BEGIN_API_HERE  // this enum should always be the last in the list. Feel free to insert new ID above this one.
} VKTRACE_TRACE_PACKET_ID;

#define VKTRACE_BIG_ENDIAN 0
#define VKTRACE_LITTLE_ENDIAN 1

typedef struct {
    uint8_t id;
    uint8_t is_64_bit;
} vktrace_tracer_info;

// vktrace_trace_file_header and vktrace_trace_packet_header are written and read
// by both host and target CPU, which can be different architectures.
// All fields in these structs are 64-bit aligned.

#if defined(WIN32)
#define ALIGN8 __declspec(align(8))
#else
#define ALIGN8 __attribute__((aligned(8)))
#endif

typedef struct {
    ALIGN8 uint64_t gpu_id;
    ALIGN8 uint64_t gpu_drv_vers;
} struct_gpuinfo;

typedef struct {
    uint16_t trace_file_version;
    uint16_t reserved1[3];
    ALIGN8 uint64_t magic;
    uint32_t uuid[4];
    ALIGN8 uint64_t first_packet_offset;  // Size of header including gpu array below
    ALIGN8 uint64_t tracer_count;         // number of tracers referenced in this trace file
    vktrace_tracer_info
        tracer_id_array[VKTRACE_MAX_TRACER_ID_ARRAY_SIZE];  // array of tracer_ids and values which are referenced in the trace file
    ALIGN8 uint64_t trace_start_time;
    ALIGN8 uint64_t portability_table_valid;

    // Description of platform on which this trace was created
    ALIGN8 uint64_t endianess;
    ALIGN8 uint64_t ptrsize;
    ALIGN8 uint64_t arch;
    ALIGN8 uint64_t os;

    // Reserve some spaece in case more fields need to be added in the future
    ALIGN8 uint64_t reserved2[8];

    // The header ends with number of gpus and a gpu_id/drv_vers pair for each gpu
    ALIGN8 uint64_t n_gpuinfo;
    // A struct_gpuinfo array of length n_gpuinfo follows this
} vktrace_trace_file_header;

typedef struct {
    ALIGN8 uint64_t size;  // total size, including extra data, needed to get to the next packet_header
    ALIGN8 uint64_t global_packet_index;
    uint8_t tracer_id;   // TODO: need to uniquely identify tracers in a way that is known by the replayer
    uint16_t packet_id;  // VKTRACE_TRACE_PACKET_ID (or one of the api-specific IDs)
    uint32_t thread_id;
    ALIGN8 uint64_t vktrace_begin_time;  // start of measuring vktrace's overhead related to this packet
    ALIGN8 uint64_t entrypoint_begin_time;
    ALIGN8 uint64_t entrypoint_end_time;
    ALIGN8 uint64_t vktrace_end_time;     // end of measuring vktrace's overhead related to this packet
    ALIGN8 uint64_t next_buffers_offset;  // used for tracking the addition of buffers to the trace packet
    ALIGN8 uintptr_t pBody;               // points to the body of the packet
} vktrace_trace_packet_header;

typedef struct {
    vktrace_trace_packet_header* pHeader;
    VktraceLogLevel type;
    uint32_t length;
    char* message;
} vktrace_trace_packet_message;

typedef struct {
    vktrace_trace_packet_header* pHeader;
    unsigned int length;
    char* label;
} vktrace_trace_packet_marker_checkpoint;

typedef vktrace_trace_packet_marker_checkpoint vktrace_trace_packet_marker_api_boundary;
typedef vktrace_trace_packet_marker_checkpoint vktrace_trace_packet_marker_api_group_begin;
typedef vktrace_trace_packet_marker_checkpoint vktrace_trace_packet_marker_api_group_end;

typedef VKTRACE_TRACER_ID(VKTRACER_CDECL* funcptr_VKTRACE_GetTracerId)();
