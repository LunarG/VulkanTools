/*
 * Copyright (c) 2013, NVIDIA CORPORATION. All rights reserved.
 * Copyright (c) 2014-2016 Valve Corporation. All rights reserved.
 * Copyright (C) 2014-2016 LunarG, Inc.
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
 */

#pragma once

#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif

#include "vktrace_common.h"
#include "vktrace_interconnect.h"

typedef struct MessageStream MessageStream;

struct FileLike;
typedef struct FileLike FileLike;
typedef struct FileLike {
    enum { File, Socket } mMode;
    FILE* mFile;
    size_t mMemLen;
    MessageStream* mMessageStream;
} FileLike;

// For creating checkpoints (consistency checks) in the various streams we're interacting with.
typedef struct Checkpoint {
    const char* mToken;
    size_t mTokenLength;
} Checkpoint;

#ifdef __cplusplus
extern "C" {
#endif

Checkpoint* vktrace_Checkpoint_create(const char* _str);
void vktrace_Checkpoint_write(Checkpoint* pCheckpoint, FileLike* _out);
BOOL vktrace_Checkpoint_read(Checkpoint* pCheckpoint, FileLike* _in);

// An interface for interacting with sockets, files, and memory streams with a file-like interface.
// This is a simple file-like interface--it doesn't support rewinding or anything fancy, just fifo
// reads and writes.

// create a filelike interface for file streaming
FileLike* vktrace_FileLike_create_file(FILE* fp);

// create a filelike interface for network streaming
FileLike* vktrace_FileLike_create_msg(MessageStream* _msgStream);

// read a size and then a buffer of that size
size_t vktrace_FileLike_Read(FileLike* pFileLike, void* _bytes, size_t _len);

// Normally, Read expects the size to live in the stream prefixing the data to be read.
// With ReadRaw, no size is expected first, and the bytes are directly read.
BOOL vktrace_FileLike_ReadRaw(FileLike* pFileLike, void* _bytes, size_t _len);

// write _len and then the buffer of size _len
void vktrace_FileLike_Write(FileLike* pFileLike, const void* _bytes, size_t _len);

// Normally, Write outputs the _len to the stream first--with WriteRaw the bytes are simply written,
// no size parameter first.
BOOL vktrace_FileLike_WriteRaw(FileLike* pFile, const void* _bytes, size_t _len);

// Get the starting position for the next vktrace_FileLike_ReadRaw
size_t vktrace_FileLike_GetCurrentPosition(FileLike* pFile);

// Set the starting position for the next vktrace_FileLike_ReadRaw
BOOL vktrace_FileLike_SetCurrentPosition(FileLike* pFile, size_t offset);

#ifdef __cplusplus
}
#endif
