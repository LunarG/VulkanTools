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

#include <errno.h>
#include "vktrace_common.h"

#if defined(PLATFORM_POSIX)
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#define SOCKET int
#define INVALID_SOCKET 0
#define SOCKET_ERROR -1
#define closesocket close
#define VKTRACE_WSAGetLastError() errno
#define WSAEWOULDBLOCK EWOULDBLOCK
#define WSAEAGAIN EAGAIN
#define WSAECONNRESET ECONNRESET
#elif defined(WIN32)
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define VKTRACE_WSAGetLastError() WSAGetLastError()
#endif

static const unsigned int VKTRACE_BASE_PORT = 34199;
struct SSerializeDataPacket;

struct SimpleBuffer;

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
typedef struct MessageStream {
    SOCKET mServerListenSocket;
    SOCKET mSocket;
    struct addrinfo* mHostAddressInfo;
    size_t mNextPacketId;
    struct SimpleBuffer* mSendBuffer;

    // Used if someone asks for a receive of a small string.
    char mSmallBuffer[64];

    char mAddress[64];

    char mPort[8];

    BOOL mHost;
    int mErrorNum;
} MessageStream;

#if defined(__cplusplus)
extern "C" {
#endif
MessageStream* vktrace_MessageStream_create_port_string(BOOL _isHost, const char* _address, const char* _port);
MessageStream* vktrace_MessageStream_create(BOOL _isHost, const char* _address, unsigned int _port);
BOOL vktrace_MessageStream_SetupHostSocket(MessageStream* pStream);
void vktrace_MessageStream_destroy(MessageStream** ppStream);
BOOL vktrace_MessageStream_BufferedSend(MessageStream* pStream, const void* _bytes, uint64_t _size, BOOL _optional);
BOOL vktrace_MessageStream_Send(MessageStream* pStream, const void* _bytes, uint64_t _len);

BOOL vktrace_MessageStream_Recv(MessageStream* pStream, void* _out, uint64_t _len);
BOOL vktrace_MessageStream_BlockingRecv(MessageStream* pStream, void* _outBuffer, uint64_t _len);

extern MessageStream* gMessageStream;
#if defined(__cplusplus)
}
#endif
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
typedef struct SimpleBuffer {
    void* mBuffer;
    size_t mEnd;
    size_t mSize;
} SimpleBuffer;

SimpleBuffer* vktrace_SimpleBuffer_create(size_t _bufferSize);
void vktrace_SimpleBuffer_destroy(SimpleBuffer** ppBuffer);
BOOL vktrace_SimpleBuffer_AddBytes(SimpleBuffer* pBuffer, const void* _bytes, uint64_t _size);
void vktrace_SimpleBuffer_EmptyBuffer(SimpleBuffer* pBuffer);
BOOL vktrace_SimpleBuffer_WouldOverflow(SimpleBuffer* pBuffer, uint64_t _requestedSize);
const void* vktrace_SimpleBuffer_GetBytes(SimpleBuffer* pBuffer, uint64_t* _outByteCount);
