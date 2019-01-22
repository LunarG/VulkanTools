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

#include "vktrace_common.h"
#include "vktrace_filelike.h"

#if defined(ANDROID)
#include <sys/un.h>
#endif
#if !defined(WIN32)
#include <errno.h>
#endif
const size_t kSendBufferSize = 1024 * 1024;

MessageStream* gMessageStream = NULL;
static VKTRACE_CRITICAL_SECTION gSendLock;
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// private functions
BOOL vktrace_MessageStream_SetupSocket(MessageStream* pStream);
BOOL vktrace_MessageStream_SetupClientSocket(MessageStream* pStream);
BOOL vktrace_MessageStream_Handshake(MessageStream* pStream);
BOOL vktrace_MessageStream_ReallySend(MessageStream* pStream, const void* _bytes, uint64_t _size, BOOL _optional);
void vktrace_MessageStream_FlushSendBuffer(MessageStream* pStream, BOOL _optional);

// public functions
MessageStream* vktrace_MessageStream_create_port_string(BOOL _isHost, const char* _address, const char* _port) {
    MessageStream* pStream;
    // make sure the strings are shorter than the destination buffer we have to store them!
    assert(strlen(_address) + 1 <= 64);
    assert(strlen(_port) + 1 <= 8);

    pStream = VKTRACE_NEW(MessageStream);
    memcpy(pStream->mAddress, _address, strlen(_address) + 1);
    memcpy(pStream->mPort, _port, strlen(_port) + 1);

    pStream->mErrorNum = 0;
    memset(pStream->mSmallBuffer, 0, 64);
    pStream->mHost = _isHost;
    pStream->mHostAddressInfo = NULL;
    pStream->mNextPacketId = 0;
    pStream->mSocket = INVALID_SOCKET;
    pStream->mServerListenSocket = INVALID_SOCKET;
    pStream->mSendBuffer = NULL;

    if (vktrace_MessageStream_SetupSocket(pStream) == FALSE) {
        VKTRACE_DELETE(pStream);
        pStream = NULL;
    }

    return pStream;
}

MessageStream* vktrace_MessageStream_create(BOOL _isHost, const char* _address, unsigned int _port) {
    char portBuf[32];
    memset(portBuf, 0, 32 * sizeof(char));
    sprintf(portBuf, "%u", _port);
    return vktrace_MessageStream_create_port_string(_isHost, _address, portBuf);
}

void vktrace_MessageStream_destroy(MessageStream** ppStream) {
    if ((*ppStream)->mSendBuffer != NULL) {
        // Try to get our data out.
        vktrace_MessageStream_FlushSendBuffer(*ppStream, TRUE);
        vktrace_SimpleBuffer_destroy(&(*ppStream)->mSendBuffer);
    }

    if ((*ppStream)->mHostAddressInfo != NULL) {
        freeaddrinfo((*ppStream)->mHostAddressInfo);
        (*ppStream)->mHostAddressInfo = NULL;
    }

    vktrace_LogDebug("Destroyed socket connection.");
#if defined(WIN32)
    if (!(*ppStream)->mHost)
    {
        // we should only do cleanup for client side because the library
        // is shared by multiple thread on server side
        WSACleanup();
    }
#endif
    VKTRACE_DELETE(*ppStream);
    (*ppStream) = NULL;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// private function implementations
BOOL vktrace_MessageStream_SetupSocket(MessageStream* pStream) {
    BOOL result = TRUE;
#if defined(WIN32)
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
        result = FALSE;
    } else
#endif
    {
        if (pStream->mHost) {
            result = vktrace_MessageStream_SetupHostSocket(pStream);
        } else {
            result = vktrace_MessageStream_SetupClientSocket(pStream);
        }
    }
    return result;
}

BOOL vktrace_MessageStream_SetupHostSocket(MessageStream* pStream) {
    int hr = 0;
#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    int yes = 1;
#endif
    struct addrinfo hostAddrInfo = {0};
    SOCKET listenSocket;
    if (pStream->mServerListenSocket == INVALID_SOCKET) {
        vktrace_create_critical_section(&gSendLock);
        hostAddrInfo.ai_family = AF_INET;
        hostAddrInfo.ai_socktype = SOCK_STREAM;
        hostAddrInfo.ai_protocol = IPPROTO_TCP;
        hostAddrInfo.ai_flags = AI_PASSIVE;

        hr = getaddrinfo(NULL, pStream->mPort, &hostAddrInfo, &pStream->mHostAddressInfo);
        if (hr != 0) {
            vktrace_LogError("Host: Failed getaddrinfo.");
            return FALSE;
        }

        listenSocket = socket(pStream->mHostAddressInfo->ai_family, pStream->mHostAddressInfo->ai_socktype,
                              pStream->mHostAddressInfo->ai_protocol);
        if (listenSocket == INVALID_SOCKET) {
            // TODO: Figure out errors
            vktrace_LogError("Host: Failed creating a listen socket.");
            freeaddrinfo(pStream->mHostAddressInfo);
            pStream->mHostAddressInfo = NULL;
            return FALSE;
        } else {
            pStream->mServerListenSocket = listenSocket;
        }

#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
        setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
#endif
        hr = bind(listenSocket, pStream->mHostAddressInfo->ai_addr, (int)pStream->mHostAddressInfo->ai_addrlen);
        if (hr == SOCKET_ERROR) {
            vktrace_LogError("Host: Failed binding socket err=%d.", VKTRACE_WSAGetLastError());
            freeaddrinfo(pStream->mHostAddressInfo);
            pStream->mHostAddressInfo = NULL;
            closesocket(listenSocket);
            return FALSE;
        }

        // Done with this.
        freeaddrinfo(pStream->mHostAddressInfo);
        pStream->mHostAddressInfo = NULL;

        hr = listen(listenSocket, 5);
        if (hr == SOCKET_ERROR) {
            vktrace_LogError("Host: Failed listening on socket err=%d.", VKTRACE_WSAGetLastError());
            closesocket(listenSocket);
            return FALSE;
        }
    } else {
        listenSocket = pStream->mServerListenSocket;
    }

    // Fo reals.
    vktrace_LogVerbose("Listening for connections on port %s.", pStream->mPort);

    pStream->mSocket = INVALID_SOCKET;
    // We need to put the accept call in while loop because none-blocking
    // mode is used during receiving trace file data.
    int error_code;
    while (pStream->mSocket == INVALID_SOCKET) {
        pStream->mSocket = accept(listenSocket, NULL, NULL);
        if (pStream->mSocket == INVALID_SOCKET) {
#if defined(WIN32)
            error_code = WSAGetLastError();
            if ((error_code == WSAECONNRESET) || (error_code == WSAENETDOWN)) {
#else
            error_code = errno;
            if ((error_code == ECONNABORTED) || (error_code == ENETDOWN)) {
#endif
                vktrace_LogError("Host: Failed network connection.");
                return FALSE;
            }
        }
    }

    vktrace_LogVerbose("Connected on port %s.", pStream->mPort);
    if (vktrace_MessageStream_Handshake(pStream)) {
        // TODO: The SendBuffer can cause big delays in sending messages back to the client.
        // We haven't verified if this improves performance in real applications,
        // so disable it for now.
        // pStream->mSendBuffer = vktrace_SimpleBuffer_create(kSendBufferSize);
        pStream->mSendBuffer = NULL;
    } else {
        vktrace_LogError("vktrace_MessageStream_SetupHostSocket failed handshake.");
    }
    return TRUE;
}

// ------------------------------------------------------------------------------------------------
BOOL vktrace_MessageStream_SetupClientSocket(MessageStream* pStream) {
    int hr = 0;
    unsigned int attempt = 0;
    BOOL bConnected = FALSE;
    struct addrinfo hostAddrInfo = {0}, *currentAttempt = NULL;
    vktrace_create_critical_section(&gSendLock);

#if defined(ANDROID)

    struct sockaddr_un addr;
    socklen_t namelen;

    // Copy the string such that a null character precedes it, i.e. "0\vktrace"
    memset(&addr, 0, sizeof(addr));
    strcpy(addr.sun_path + 1, pStream->mPort);
    addr.sun_family = AF_UNIX;
    namelen = sizeof(addr.sun_family) + strlen(pStream->mPort) + 1;

    pStream->mSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    hr = connect(pStream->mSocket, (struct sockaddr*)&addr, namelen);

    if (hr == SOCKET_ERROR) {
        vktrace_LogError("Client: Failed connect to abstract socket.");
        closesocket(pStream->mSocket);
        pStream->mSocket = INVALID_SOCKET;
    }

#else

    hostAddrInfo.ai_family = AF_UNSPEC;
    hostAddrInfo.ai_socktype = SOCK_STREAM;
    hostAddrInfo.ai_protocol = IPPROTO_TCP;

    hr = getaddrinfo(pStream->mAddress, pStream->mPort, &hostAddrInfo, &pStream->mHostAddressInfo);
    if (hr != 0) {
        vktrace_LogError("Client: Failed getaddrinfo result=%d.", hr);
        return FALSE;
    }

    // make several attempts to connect before bailing out
    for (attempt = 0; attempt < 10 && !bConnected; attempt++) {
        for (currentAttempt = pStream->mHostAddressInfo; currentAttempt != NULL; currentAttempt = currentAttempt->ai_next) {
            pStream->mSocket = socket(currentAttempt->ai_family, currentAttempt->ai_socktype, currentAttempt->ai_protocol);

            hr = connect(pStream->mSocket, currentAttempt->ai_addr, (int)currentAttempt->ai_addrlen);
            if (hr == SOCKET_ERROR) {
                vktrace_LogVerbose("Client: Failed connect. Possibly non-fatal.");
                closesocket(pStream->mSocket);
                pStream->mSocket = INVALID_SOCKET;
                continue;
            }

            bConnected = TRUE;
            break;
        }

        if (!bConnected) {
            Sleep(1);
            vktrace_LogVerbose("Client: Connect attempt %u on port %s failed, trying again.", attempt, pStream->mPort);
        } else {
            vktrace_LogVerbose("Client: Connected to port %s successfully.", pStream->mPort);
        }
    }

    freeaddrinfo(pStream->mHostAddressInfo);
    pStream->mHostAddressInfo = NULL;

#endif

    if (pStream->mSocket == INVALID_SOCKET) {
        vktrace_LogError("Client: Couldn't find any connections.");
        return FALSE;
    }

    if (!vktrace_MessageStream_Handshake(pStream)) {
        vktrace_LogError("Client: Failed handshake with host.");
        return FALSE;
    }
    return TRUE;
}

// ------------------------------------------------------------------------------------------------
BOOL vktrace_MessageStream_Handshake(MessageStream* pStream) {
    BOOL result = TRUE;
    FileLike* fileLike = vktrace_FileLike_create_msg(pStream);
    Checkpoint* syn = vktrace_Checkpoint_create("It's a trap!");
    Checkpoint* ack = vktrace_Checkpoint_create(" - Admiral Ackbar");

    if (pStream->mHost) {
        vktrace_Checkpoint_write(syn, fileLike);
        result = vktrace_Checkpoint_read(ack, fileLike);
    } else {
        if (vktrace_Checkpoint_read(syn, fileLike)) {
            vktrace_Checkpoint_write(ack, fileLike);
        } else {
            result = FALSE;
        }
    }

    // Turn on non-blocking modes for sockets now.
    if (result) {
#if defined(WIN32)
        u_long asyncMode = 1;
        ioctlsocket(pStream->mSocket, FIONBIO, &asyncMode);
#else
        fcntl(pStream->mSocket, F_SETFL, O_NONBLOCK);
#endif
    }

    VKTRACE_DELETE(syn);
    VKTRACE_DELETE(ack);
    VKTRACE_DELETE(fileLike);

    return result;
}

// ------------------------------------------------------------------------------------------------
void vktrace_MessageStream_FlushSendBuffer(MessageStream* pStream, BOOL _optional) {
    uint64_t bufferedByteSize = 0;
    const void* bufferBytes = vktrace_SimpleBuffer_GetBytes(pStream->mSendBuffer, &bufferedByteSize);
    if (bufferedByteSize > 0) {
        // TODO use return value from ReallySend
        vktrace_MessageStream_ReallySend(pStream, bufferBytes, bufferedByteSize, _optional);
        vktrace_SimpleBuffer_EmptyBuffer(pStream->mSendBuffer);
    }
}

// ------------------------------------------------------------------------------------------------
BOOL vktrace_MessageStream_BufferedSend(MessageStream* pStream, const void* _bytes, uint64_t _size, BOOL _optional) {
    BOOL result = TRUE;
    if (pStream->mSendBuffer == NULL) {
        result = vktrace_MessageStream_ReallySend(pStream, _bytes, _size, _optional);
    } else {
        if (!vktrace_SimpleBuffer_WouldOverflow(pStream->mSendBuffer, _size)) {
            result = vktrace_SimpleBuffer_AddBytes(pStream->mSendBuffer, _bytes, _size);
        } else {
            // Time to flush the cache.
            vktrace_MessageStream_FlushSendBuffer(pStream, FALSE);

            // Check to see if the packet is larger than the send buffer
            if (vktrace_SimpleBuffer_WouldOverflow(pStream->mSendBuffer, _size)) {
                result = vktrace_MessageStream_ReallySend(pStream, _bytes, _size, _optional);
            } else {
                result = vktrace_SimpleBuffer_AddBytes(pStream->mSendBuffer, _bytes, _size);
            }
        }
    }
    return result;
}

// ------------------------------------------------------------------------------------------------
BOOL vktrace_MessageStream_Send(MessageStream* pStream, const void* _bytes, uint64_t _len) {
    return vktrace_MessageStream_BufferedSend(pStream, _bytes, _len, FALSE);
}

// ------------------------------------------------------------------------------------------------
BOOL vktrace_MessageStream_ReallySend(MessageStream* pStream, const void* _bytes, uint64_t _size, BOOL _optional) {
    size_t bytesSent = 0;
    assert(_size > 0);

    vktrace_enter_critical_section(&gSendLock);
    do {
        int sentThisTime = send(pStream->mSocket, (const char*)_bytes + bytesSent, (int)_size - (int)bytesSent, 0);
        if (sentThisTime == SOCKET_ERROR) {
            int socketError = VKTRACE_WSAGetLastError();
            if (socketError == WSAEWOULDBLOCK) {
                // Try again. Don't sleep, because that nukes performance from orbit.
                continue;
            }

            if (!_optional) {
                vktrace_leave_critical_section(&gSendLock);
                return FALSE;
            }
        }
        if (sentThisTime == 0) {
            if (!_optional) {
                vktrace_leave_critical_section(&gSendLock);
                return FALSE;
            }
            vktrace_LogDebug("Send on socket 0 bytes, totalbytes sent so far %u.", bytesSent);
            break;
        }

        bytesSent += sentThisTime;

    } while (bytesSent < _size);
    vktrace_leave_critical_section(&gSendLock);
    return TRUE;
}

// ------------------------------------------------------------------------------------------------
BOOL vktrace_MessageStream_Recv(MessageStream* pStream, void* _out, uint64_t _len) {
    unsigned int totalDataRead = 0;
    unsigned int attempts = 0;
    do {
        attempts++;
        int dataRead = recv(pStream->mSocket, ((char*)_out) + totalDataRead, (int)_len - totalDataRead, 0);
        if (dataRead == SOCKET_ERROR) {
            pStream->mErrorNum = VKTRACE_WSAGetLastError();
            if (pStream->mErrorNum == WSAEWOULDBLOCK || pStream->mErrorNum == EAGAIN) {
                if (totalDataRead == 0) {
                    return FALSE;
                } else {
                    // I don't do partial reads--once I start receiving I wait for everything.
                    vktrace_LogDebug("Sleep on partial socket recv (%u bytes / %u), error num %d.", totalDataRead, _len,
                                     pStream->mErrorNum);
                    Sleep(1);
                }
                // I've split these into two blocks because one of them is expected and the other isn't.
            } else if (pStream->mErrorNum == WSAECONNRESET) {
                // The remote client disconnected, probably not an issue.
                vktrace_LogDebug("Connection was reset by client.");
                return FALSE;
            } else {
                // Some other wonky network error--place a breakpoint here.
                vktrace_LogError("Unexpected error (%d) while receiving message stream.", pStream->mErrorNum);
                return FALSE;
            }
        } else if (dataRead == 0) {
            // From recv(2): "When  a  stream  socket  peer  has  performed  an orderly shutdown, the return value will be 0"
            // Try 100 times before sleeping for a while. When we have tried 200 times, give up.
            if (attempts == 100) {
                vktrace_LogDebug("Sleep on socket recv of 0 (%u bytes / %u).", totalDataRead, _len);
                Sleep(1);
            }
            if (attempts == 200) {
                // Give up
                pStream->mErrorNum = WSAECONNRESET;
                vktrace_LogDebug("Connection was reset by client.");
                return FALSE;
            }
        } else {
            totalDataRead += dataRead;
        }
    } while (totalDataRead < _len);

    return TRUE;
}

// ------------------------------------------------------------------------------------------------
BOOL vktrace_MessageStream_BlockingRecv(MessageStream* pStream, void* _outBuffer, uint64_t _len) {
    while (!vktrace_MessageStream_Recv(pStream, _outBuffer, _len)) {
        if (pStream->mErrorNum == WSAECONNRESET) {
            return FALSE;
        }
        Sleep(1);
    }
    return TRUE;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
SimpleBuffer* vktrace_SimpleBuffer_create(size_t _bufferSize) {
    SimpleBuffer* pBuffer = VKTRACE_NEW(SimpleBuffer);
    pBuffer->mBuffer = (unsigned char*)vktrace_malloc(_bufferSize);
    if (pBuffer->mBuffer == NULL) {
        VKTRACE_DELETE(pBuffer);
        return NULL;
    }

    pBuffer->mEnd = 0;
    pBuffer->mSize = _bufferSize;

    return pBuffer;
}

void vktrace_SimpleBuffer_destroy(SimpleBuffer** ppBuffer) {
    vktrace_free((*ppBuffer)->mBuffer);
    VKTRACE_DELETE(*ppBuffer);
}

BOOL vktrace_SimpleBuffer_AddBytes(SimpleBuffer* pBuffer, const void* _bytes, uint64_t _size) {
    if (vktrace_SimpleBuffer_WouldOverflow(pBuffer, _size)) {
        return FALSE;
    }

    memcpy((unsigned char*)pBuffer->mBuffer + pBuffer->mEnd, _bytes, (size_t)_size);
    pBuffer->mEnd += (size_t)_size;

    return TRUE;
}

void vktrace_SimpleBuffer_EmptyBuffer(SimpleBuffer* pBuffer) { pBuffer->mEnd = 0; }

BOOL vktrace_SimpleBuffer_WouldOverflow(SimpleBuffer* pBuffer, uint64_t _requestedSize) {
    return pBuffer->mEnd + _requestedSize > pBuffer->mSize;
}

const void* vktrace_SimpleBuffer_GetBytes(SimpleBuffer* pBuffer, uint64_t* _outByteCount) {
    (*_outByteCount) = pBuffer->mEnd;
    return pBuffer->mBuffer;
}

//// ------------------------------------------------------------------------------------------------
// void RemoteCommand::Read(FileLike* _fileLike)
//{
//    unsigned int myCommand = 0;
//    _fileLike->Read(&myCommand);
//    mRemoteCommandType = (EnumRemoteCommand)myCommand;
//}
//
//// ------------------------------------------------------------------------------------------------
// void RemoteCommand::Write(FileLike* _fileLike) const
//{
//    _fileLike->Write((unsigned int)mRemoteCommandType);
//}
