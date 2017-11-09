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

#include "vktraceviewer_qtracefileloader.h"
#include "vktraceviewer_controller_factory.h"

extern "C" {
#include "vktrace_trace_packet_utils.h"
}

vktraceviewer_QTraceFileLoader::vktraceviewer_QTraceFileLoader() : QObject(NULL) {
    qRegisterMetaType<vktraceviewer_trace_file_info>("vktraceviewer_trace_file_info");
}

vktraceviewer_QTraceFileLoader::~vktraceviewer_QTraceFileLoader() {}

//-----------------------------------------------------------------------------
void vktraceviewer_QTraceFileLoader::loadTraceFile(const QString& filename) {
    // open trace file and read in header
    memset(&m_traceFileInfo, 0, sizeof(vktraceviewer_trace_file_info));
    m_traceFileInfo.pFile = fopen(filename.toStdString().c_str(), "rb");

    bool bOpened = (m_traceFileInfo.pFile != NULL);
    if (!bOpened) {
        emit OutputMessage(VKTRACE_LOG_ERROR, "Unable to open file.");
    } else {
        m_traceFileInfo.filename = vktrace_allocate_and_copy(filename.toStdString().c_str());
        if (populate_trace_file_info(&m_traceFileInfo) == FALSE) {
            emit OutputMessage(VKTRACE_LOG_ERROR, "Unable to populate trace file info from file.");
            bOpened = false;
        } else {
            // Make sure trace file version is supported
            if (m_traceFileInfo.pHeader->trace_file_version < VKTRACE_TRACE_FILE_VERSION_MINIMUM_COMPATIBLE ||
                m_traceFileInfo.pHeader->trace_file_version > VKTRACE_TRACE_FILE_VERSION) {
                emit OutputMessage(VKTRACE_LOG_ERROR,
                                   QString("Trace file version %1 is not compatible with this viewer (%2).\nYou'll need to "
                                           "the appropriate viewer.")
                                       .arg(m_traceFileInfo.pHeader->trace_file_version)
                                       .arg(VKTRACE_TRACE_FILE_VERSION_MINIMUM_COMPATIBLE));
                bOpened = false;
            }

#ifdef USE_STATIC_CONTROLLER_LIBRARY
            m_pController = vtvCreateQController();
            if (bOpened)
#else
            if (!load_controllers(&m_traceFileInfo)) {
                emit OutputMessage(VKTRACE_LOG_ERROR, "Failed to load necessary debug controllers.");
                bOpened = false;
            } else if (bOpened)
#endif
            {
                connect(m_pController, SIGNAL(OutputMessage(VktraceLogLevel, const QString&)), this,
                        SIGNAL(OutputMessage(VktraceLogLevel, const QString&)));
                connect(m_pController, SIGNAL(OutputMessage(VktraceLogLevel, uint64_t, const QString&)), this,
                        SIGNAL(OutputMessage(VktraceLogLevel, uint64_t, const QString&)));

                // interpret the trace file packets
                for (uint64_t i = 0; i < m_traceFileInfo.packetCount; i++) {
                    vktraceviewer_trace_file_packet_offsets* pOffsets = &m_traceFileInfo.pPacketOffsets[i];
                    switch (pOffsets->pHeader->packet_id) {
                        case VKTRACE_TPI_MESSAGE:
                            m_traceFileInfo.pPacketOffsets[i].pHeader =
                                vktrace_interpret_body_as_trace_packet_message(pOffsets->pHeader)->pHeader;
                            break;
                        case VKTRACE_TPI_MARKER_CHECKPOINT:
                            break;
                        case VKTRACE_TPI_MARKER_API_BOUNDARY:
                            break;
                        case VKTRACE_TPI_MARKER_API_GROUP_BEGIN:
                            break;
                        case VKTRACE_TPI_MARKER_API_GROUP_END:
                            break;
                        case VKTRACE_TPI_MARKER_TERMINATE_PROCESS:
                            break;
                        case VKTRACE_TPI_PORTABILITY_TABLE:
                            break;
                        // TODO processing code for all the above cases
                        default: {
                            vktrace_trace_packet_header* pHeader = m_pController->InterpretTracePacket(pOffsets->pHeader);
                            if (pHeader == NULL) {
                                bOpened = false;
                                emit OutputMessage(VKTRACE_LOG_ERROR,
                                                   QString("Unrecognized packet type: %1").arg(pOffsets->pHeader->packet_id));
                                m_traceFileInfo.pPacketOffsets[i].pHeader = NULL;
                                break;
                            }
                            m_traceFileInfo.pPacketOffsets[i].pHeader = pHeader;
                        }
                    }

                    // break from loop if there is an error
                    if (bOpened == false) {
                        break;
                    }
                }

#ifdef USE_STATIC_CONTROLLER_LIBRARY
                vtvDeleteQController(&m_pController);
#else
                m_controllerFactory.Unload(&m_pController);
#endif
            }
        }

        // TODO: We don't really want to close the trace file yet.
        // I think we want to keep it open so that we can dynamically read from it.
        // BUT we definitely don't want it to get locked open, so we need a smart
        // way to open / close from it when reading.
        fclose(m_traceFileInfo.pFile);
        m_traceFileInfo.pFile = NULL;
    }

    // populate the UI based on trace file info
    emit TraceFileLoaded(bOpened, m_traceFileInfo, m_controllerFilename);

    emit Finished();
}

//-----------------------------------------------------------------------------
bool vktraceviewer_QTraceFileLoader::load_controllers(vktraceviewer_trace_file_info* pTraceFileInfo) {
    if (pTraceFileInfo->pHeader->tracer_count == 0) {
        emit OutputMessage(VKTRACE_LOG_ERROR, "No API specified in tracefile for replaying.");
        return false;
    }

    for (uint64_t i = 0; i < pTraceFileInfo->pHeader->tracer_count; i++) {
        uint8_t tracerId = pTraceFileInfo->pHeader->tracer_id_array[i].id;

        const VKTRACE_TRACER_REPLAYER_INFO* pReplayerInfo = &(gs_tracerReplayerInfo[tracerId]);

        if (pReplayerInfo->tracerId != tracerId) {
            emit OutputMessage(VKTRACE_LOG_ERROR,
                               QString("Replayer info for TracerId (%1) failed consistency check.").arg(tracerId));
            assert(!"TracerId in VKTRACE_TRACER_REPLAYER_INFO does not match the requested tracerId. The array needs to be corrected.");
        } else if (strlen(pReplayerInfo->debuggerLibraryname) != 0) {
            // Have our factory create the necessary controller
            emit OutputMessage(VKTRACE_LOG_VERBOSE, QString("Loading controller: %1").arg(pReplayerInfo->debuggerLibraryname));

            m_pController = m_controllerFactory.Load(pReplayerInfo->debuggerLibraryname);

            if (m_pController != NULL) {
                m_controllerFilename = QString(pReplayerInfo->debuggerLibraryname);
                // Only one controller needs to be loaded, so break from loop
                break;
            } else {
                // controller failed to be created
                emit OutputMessage(VKTRACE_LOG_ERROR, QString("Unable to load controller for TracerId %1.").arg(tracerId));
            }
        }
    }

    return m_pController != NULL;
}

//-----------------------------------------------------------------------------
bool vktraceviewer_QTraceFileLoader::populate_trace_file_info(vktraceviewer_trace_file_info* pTraceFileInfo) {
    vktrace_trace_file_header header;
    assert(pTraceFileInfo != NULL);
    assert(pTraceFileInfo->pFile != NULL);

    // read trace file header
    if (1 != fread(&header, sizeof(vktrace_trace_file_header), 1, pTraceFileInfo->pFile)) {
        emit OutputMessage(VKTRACE_LOG_ERROR, "Unable to read header from file.");
        return false;
    }

    // Make sure there is at least one gpuinfo struct in header
    if (header.n_gpuinfo < 1) {
        emit OutputMessage(VKTRACE_LOG_ERROR, "Trace file head may be corrupt - gpu info missing.");
        return false;
    }

    // allocate trace file header that includes space for gpuinfo array
    pTraceFileInfo->pHeader =
        (vktrace_trace_file_header*)vktrace_malloc(sizeof(vktrace_trace_file_header) + header.n_gpuinfo * sizeof(struct_gpuinfo));
    if (!pTraceFileInfo->pHeader) {
        emit OutputMessage(VKTRACE_LOG_ERROR, "Unable to allocate memory for file read header.");
        return false;
    }
    *pTraceFileInfo->pHeader = header;
    pTraceFileInfo->pGpuinfo = (struct_gpuinfo*)(pTraceFileInfo->pHeader + 1);

    // read the gpuinfo array
    if (1 != fread(pTraceFileInfo->pGpuinfo, header.n_gpuinfo * sizeof(struct_gpuinfo), 1, pTraceFileInfo->pFile)) {
        vktrace_free(pTraceFileInfo->pHeader);
        emit OutputMessage(VKTRACE_LOG_ERROR, "Unable to read header from file.");
        return false;
    }

    // Set global version num
    vktrace_set_trace_version(pTraceFileInfo->pHeader->trace_file_version);

    // Find out how many trace packets there are.

    // Seek to first packet
    long first_offset = pTraceFileInfo->pHeader->first_packet_offset;
    int seekResult = Fseek(pTraceFileInfo->pFile, first_offset, SEEK_SET);
    if (seekResult != 0) {
        emit OutputMessage(VKTRACE_LOG_WARNING, "Failed to seek to the first packet offset in the trace file.");
    }

    // "Walk" through each packet based on the packet size (which is the first 64-bits of the packet header)
    uint64_t fileOffset = pTraceFileInfo->pHeader->first_packet_offset;
    uint64_t packetSize = 0;
    while (1 == fread(&packetSize, sizeof(uint64_t), 1, pTraceFileInfo->pFile)) {
        // success!
        pTraceFileInfo->packetCount++;
        fileOffset += packetSize;

        seekResult = fseek(pTraceFileInfo->pFile, packetSize - sizeof(uint64_t), SEEK_CUR);
        if (seekResult != 0) {
            emit OutputMessage(VKTRACE_LOG_ERROR, "Error while seeking through trace file.");
            break;
        }

        if (ferror(pTraceFileInfo->pFile) != 0) {
            emit OutputMessage(VKTRACE_LOG_ERROR, "Error while reading trace file.");
            break;
        }
    }

    if (pTraceFileInfo->packetCount == 0) {
        if (ferror(pTraceFileInfo->pFile) != 0) {
            perror("File Read error:");
            emit OutputMessage(VKTRACE_LOG_ERROR, "There was an error reading the trace file.");
            return false;
        } else if (feof(pTraceFileInfo->pFile) != 0) {
            emit OutputMessage(VKTRACE_LOG_WARNING, "Reached the end of the file.");
        }
        emit OutputMessage(VKTRACE_LOG_WARNING, "There are no trace packets in this trace file.");
        pTraceFileInfo->pPacketOffsets = NULL;
    } else {
        pTraceFileInfo->pPacketOffsets = VKTRACE_NEW_ARRAY(vktraceviewer_trace_file_packet_offsets, pTraceFileInfo->packetCount);

        // rewind to first packet and this time, populate the packet offsets
        if (fseek(pTraceFileInfo->pFile, first_offset, SEEK_SET) != 0) {
            vktrace_free(pTraceFileInfo->pHeader);
            emit OutputMessage(VKTRACE_LOG_ERROR, "Unable to rewind trace file to gather packet offsets.");
            return false;
        }

        unsigned int packetIndex = 0;
        fileOffset = first_offset;
        while (1 == fread(&packetSize, sizeof(uint64_t), 1, pTraceFileInfo->pFile)) {
            // the fread confirms that this packet exists
            // NOTE: We do not actually read the entire packet into memory right now.
            pTraceFileInfo->pPacketOffsets[packetIndex].fileOffset = fileOffset;

            // rewind slightly
            seekResult = fseek(pTraceFileInfo->pFile, -1 * (long)sizeof(uint64_t), SEEK_CUR);

            if (seekResult != 0) {
                emit OutputMessage(VKTRACE_LOG_ERROR, "Error while seeking between packets.");
                break;
            }

            // allocate space for the packet and read it in
            pTraceFileInfo->pPacketOffsets[packetIndex].pHeader = (vktrace_trace_packet_header*)vktrace_malloc(packetSize);
            if (1 != fread(pTraceFileInfo->pPacketOffsets[packetIndex].pHeader, packetSize, 1, pTraceFileInfo->pFile)) {
                vktrace_free(pTraceFileInfo->pHeader);
                emit OutputMessage(VKTRACE_LOG_ERROR, "Unable to read in a trace packet.");
                return false;
            }

            // adjust pointer to body of the packet
            pTraceFileInfo->pPacketOffsets[packetIndex].pHeader->pBody =
                (uintptr_t)pTraceFileInfo->pPacketOffsets[packetIndex].pHeader + sizeof(vktrace_trace_packet_header);

            // now seek to what should be the next packet
            fileOffset += packetSize;
            packetIndex++;
        }

        // If the last packet is the portability table, remove it
        if (pTraceFileInfo->pPacketOffsets[pTraceFileInfo->packetCount - 1].pHeader->packet_id == VKTRACE_TPI_PORTABILITY_TABLE) {
            vktrace_free(pTraceFileInfo->pPacketOffsets[pTraceFileInfo->packetCount - 1].pHeader);
            pTraceFileInfo->packetCount--;
        }

        if (fseek(pTraceFileInfo->pFile, first_offset, SEEK_SET) != 0) {
            vktrace_free(pTraceFileInfo->pHeader);
            emit OutputMessage(VKTRACE_LOG_ERROR, "Unable to rewind trace file to restore position.");
            return false;
        }
    }

    return true;
}
