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

#include "vktraceviewer_QReplayWorker.h"
#include <QAction>
#include <QCoreApplication>
#include "vktraceviewer_trace_file_utils.h"

vktraceviewer_QReplayWorker* g_pWorker;
static uint64_t s_currentReplayPacket = 0;

static void dbg_msg_callback(vktrace_replay::VKTRACE_DBG_MSG_TYPE msgType, uint64_t packetIndex, const char* pMsg);

void replayWorkerLoggingCallback(VktraceLogLevel level, const char* pMessage) {
    if (g_pWorker != NULL) {
        switch (level) {
            case VKTRACE_LOG_DEBUG:
                dbg_msg_callback(vktrace_replay::VKTRACE_DBG_MSG_INFO, s_currentReplayPacket, pMessage);
                break;
            case VKTRACE_LOG_ERROR:
                dbg_msg_callback(vktrace_replay::VKTRACE_DBG_MSG_ERROR, s_currentReplayPacket, pMessage);
                break;
            case VKTRACE_LOG_WARNING:
                dbg_msg_callback(vktrace_replay::VKTRACE_DBG_MSG_WARNING, s_currentReplayPacket, pMessage);
                break;
            case VKTRACE_LOG_VERBOSE:
                dbg_msg_callback(vktrace_replay::VKTRACE_DBG_MSG_INFO, s_currentReplayPacket, pMessage);
                break;
            default:
                break;
        }
    }

#if defined(WIN32)
#if _DEBUG
    OutputDebugString(pMessage);
#endif
#endif
}

vktraceviewer_QReplayWorker::vktraceviewer_QReplayWorker()
    : QObject(NULL),
      m_bPauseReplay(false),
      m_bStopReplay(false),
      m_bReplayInProgress(false),
      m_pView(NULL),
      m_pTraceFileInfo(NULL),
      m_currentReplayPacketIndex(0),
      m_pActionRunToHere(NULL),
      m_pauseAtPacketIndex((uint64_t)-1),
      m_pReplayWindow(NULL),
      m_pReplayWindowWidth(0),
      m_pReplayWindowHeight(0),
      m_bPrintReplayInfoMessages(TRUE),
      m_bPrintReplayWarningMessages(TRUE),
      m_bPrintReplayErrorMessages(TRUE),
      m_bPauseOnReplayInfoMessages(FALSE),
      m_bPauseOnReplayWarningMessages(FALSE),
      m_bPauseOnReplayErrorMessages(FALSE) {
    memset(m_pReplayers, 0, sizeof(vktrace_replay::vktrace_trace_packet_replay_library*) * VKTRACE_MAX_TRACER_ID_ARRAY_SIZE);
    g_pWorker = this;
}

vktraceviewer_QReplayWorker::~vktraceviewer_QReplayWorker() {
    setView(NULL);
    g_pWorker = NULL;

    if (m_pActionRunToHere != NULL) {
        disconnect(m_pActionRunToHere, SIGNAL(triggered()), this, SLOT(onPlayToHere()));
        delete m_pActionRunToHere;
        m_pActionRunToHere = NULL;
    }
}

void vktraceviewer_QReplayWorker::setPrintReplayMessages(BOOL bPrintInfo, BOOL bPrintWarning, BOOL bPrintError) {
    m_bPrintReplayInfoMessages = bPrintInfo;
    m_bPrintReplayWarningMessages = bPrintWarning;
    m_bPrintReplayErrorMessages = bPrintError;
}

void vktraceviewer_QReplayWorker::setPauseOnReplayMessages(BOOL bPauseOnInfo, BOOL bPauseOnWarning, BOOL bPauseOnError) {
    m_bPauseOnReplayInfoMessages = bPauseOnInfo;
    m_bPauseOnReplayWarningMessages = bPauseOnWarning;
    m_bPauseOnReplayErrorMessages = bPauseOnError;
}

BOOL vktraceviewer_QReplayWorker::PrintReplayInfoMsgs() { return m_bPrintReplayInfoMessages; }

BOOL vktraceviewer_QReplayWorker::PrintReplayWarningMsgs() { return m_bPrintReplayWarningMessages; }

BOOL vktraceviewer_QReplayWorker::PrintReplayErrorMsgs() { return m_bPrintReplayErrorMessages; }

BOOL vktraceviewer_QReplayWorker::PauseOnReplayInfoMsg() { return m_bPauseOnReplayInfoMessages; }

BOOL vktraceviewer_QReplayWorker::PauseOnReplayWarningMsg() { return m_bPauseOnReplayWarningMessages; }

BOOL vktraceviewer_QReplayWorker::PauseOnReplayErrorMsg() { return m_bPauseOnReplayErrorMessages; }

void vktraceviewer_QReplayWorker::setView(vktraceviewer_view* pView) { m_pView = pView; }

bool vktraceviewer_QReplayWorker::load_replayers(vktraceviewer_trace_file_info* pTraceFileInfo, QWidget* pReplayWindow,
                                                 int const replayWindowWidth, int const replayWindowHeight,
                                                 bool const separateReplayWindow) {
    // Get window handle of the widget to replay into.
    assert(pReplayWindow != NULL);
    assert(replayWindowWidth > 0);
    assert(replayWindowHeight > 0);

    m_pReplayWindow = pReplayWindow;
    m_pReplayWindowWidth = replayWindowWidth;
    m_pReplayWindowHeight = replayWindowHeight;

    m_pTraceFileInfo = pTraceFileInfo;

    // TODO: Get the width and height from the replayer. We can't do this yet
    // because the replayer doesn't know the render target's size.

    WId hWindow = pReplayWindow->winId();

    // load any API specific driver libraries and init replayer objects
    uint8_t tidApi = VKTRACE_TID_RESERVED;
    bool bReplayerLoaded = false;

    vktrace_replay::ReplayDisplay disp;
    if (separateReplayWindow) {
        disp = vktrace_replay::ReplayDisplay(replayWindowWidth, replayWindowHeight, 0, false);
    } else {
        disp = vktrace_replay::ReplayDisplay((vktrace_window_handle)hWindow, replayWindowWidth, replayWindowHeight);
    }

    for (int i = 0; i < VKTRACE_MAX_TRACER_ID_ARRAY_SIZE; i++) {
        m_pReplayers[i] = NULL;
    }

    for (int i = 0; i < pTraceFileInfo->pHeader->tracer_count; i++) {
        uint8_t tracerId = pTraceFileInfo->pHeader->tracer_id_array[i].id;
        tidApi = tracerId;

        const VKTRACE_TRACER_REPLAYER_INFO* pReplayerInfo = &(gs_tracerReplayerInfo[tracerId]);

        if (pReplayerInfo->tracerId != tracerId) {
            emit OutputMessage(VKTRACE_LOG_ERROR,
                               QString("Replayer info for TracerId (%1) failed consistency check.").arg(tracerId));
            assert(!"TracerId in VKTRACE_TRACER_REPLAYER_INFO does not match the requested tracerId. The array needs to be corrected.");
        } else if (pReplayerInfo->needsReplayer == TRUE) {
            // Have our factory create the necessary replayer
            m_pReplayers[tracerId] = m_replayerFactory.Create(tracerId);

            if (m_pReplayers[tracerId] == NULL) {
                // replayer failed to be created
                emit OutputMessage(VKTRACE_LOG_ERROR, QString("Couldn't create replayer for TracerId %1.").arg(tracerId));
                bReplayerLoaded = false;
            } else {
                m_pReplayers[tracerId]->SetLogCallback(replayWorkerLoggingCallback);
                m_pReplayers[tracerId]->SetLogLevel(VKTRACE_LOG_ERROR);
                m_pReplayers[tracerId]->RegisterDbgMsgCallback(
                    (vktrace_replay::VKTRACE_DBG_MSG_CALLBACK_FUNCTION)&dbg_msg_callback);

                // get settings from the replayer
                m_pView->add_setting_group(m_pReplayers[tracerId]->GetSettings());

                // update replayer with updated state
                vktrace_SettingGroup* pGlobalSettings = NULL;
                unsigned int numGlobalSettings = m_pView->get_global_settings(&pGlobalSettings);
                m_pReplayers[tracerId]->UpdateFromSettings(pGlobalSettings, numGlobalSettings);

                // Initialize the replayer
                int err = m_pReplayers[tracerId]->Initialize(&disp, NULL, pTraceFileInfo->pHeader);
                if (err) {
                    emit OutputMessage(VKTRACE_LOG_ERROR, QString("Couldn't Initialize replayer for TracerId %1.").arg(tracerId));
                    return false;
                }

                bReplayerLoaded = true;
            }
        }
    }

    if (tidApi == VKTRACE_TID_RESERVED) {
        emit OutputMessage(VKTRACE_LOG_ERROR, QString("No API specified in tracefile for replaying."));
        return false;
    }

    if (ENABLE_REPLAY && bReplayerLoaded) {
        m_pActionRunToHere = new QAction("Play to here", NULL);
        connect(m_pActionRunToHere, SIGNAL(triggered()), this, SLOT(onPlayToHere()));
        m_pView->add_calltree_contextmenu_item(m_pActionRunToHere);
    }

    return bReplayerLoaded;
}

void vktraceviewer_QReplayWorker::unloadReplayers() {
    m_pTraceFileInfo = NULL;

    // Clean up replayers
    if (m_pReplayers != NULL) {
        for (int i = 0; i < VKTRACE_MAX_TRACER_ID_ARRAY_SIZE; i++) {
            if (m_pReplayers[i] != NULL) {
                m_pReplayers[i]->Deinitialize();
                m_replayerFactory.Destroy(&m_pReplayers[i]);
            }
        }
    }
}

void vktraceviewer_QReplayWorker::playCurrentTraceFile(uint64_t startPacketIndex) {
    vktraceviewer_trace_file_info* pTraceFileInfo = m_pTraceFileInfo;
    vktraceviewer_trace_file_packet_offsets* pCurPacket = NULL;
    unsigned int res = vktrace_replay::VKTRACE_REPLAY_ERROR;
    vktrace_replay::vktrace_trace_packet_replay_library* replayer;

    m_bReplayInProgress = true;

    for (uint64_t i = startPacketIndex; i < pTraceFileInfo->packetCount; i++) {
        m_currentReplayPacketIndex = i;
        emit ReplayProgressUpdate(m_currentReplayPacketIndex);

        pCurPacket = &pTraceFileInfo->pPacketOffsets[i];
        s_currentReplayPacket = pCurPacket->pHeader->global_packet_index;
        switch (pCurPacket->pHeader->packet_id) {
            case VKTRACE_TPI_MESSAGE: {
                vktrace_trace_packet_message* msgPacket;
                msgPacket = (vktrace_trace_packet_message*)pCurPacket->pHeader;
                replayWorkerLoggingCallback(msgPacket->type, msgPacket->message);
                break;
            }
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
                if (pCurPacket->pHeader->tracer_id >= VKTRACE_MAX_TRACER_ID_ARRAY_SIZE ||
                    pCurPacket->pHeader->tracer_id == VKTRACE_TID_RESERVED) {
                    replayWorkerLoggingCallback(VKTRACE_LOG_WARNING, QString("Tracer_id from packet num packet %1 invalid.")
                                                                         .arg(pCurPacket->pHeader->packet_id)
                                                                         .toStdString()
                                                                         .c_str());
                    continue;
                }
                replayer = m_pReplayers[pCurPacket->pHeader->tracer_id];
                if (replayer == NULL) {
                    replayWorkerLoggingCallback(
                        VKTRACE_LOG_WARNING,
                        QString("Tracer_id %1 has no valid replayer.").arg(pCurPacket->pHeader->tracer_id).toStdString().c_str());
                    continue;
                }
                if (pCurPacket->pHeader->packet_id >= VKTRACE_TPI_VK_vkApiVersion) {
                    // replay the API packet
                    try {
                        res = replayer->Replay(pCurPacket->pHeader);
                    } catch (std::exception& e) {
                        replayWorkerLoggingCallback(VKTRACE_LOG_ERROR,
                                                    QString("Caught std::exception while replaying packet %1: %2")
                                                        .arg(pCurPacket->pHeader->global_packet_index)
                                                        .arg(e.what())
                                                        .toStdString()
                                                        .c_str());
                    } catch (int i) {
                        replayWorkerLoggingCallback(VKTRACE_LOG_ERROR,
                                                    QString("Caught int exception: %1").arg(i).toStdString().c_str());
                    } catch (...) {
                        replayWorkerLoggingCallback(VKTRACE_LOG_ERROR, "Caught unknown exception.");
                    }

                    if (res == vktrace_replay::VKTRACE_REPLAY_ERROR || res == vktrace_replay::VKTRACE_REPLAY_INVALID_ID ||
                        res == vktrace_replay::VKTRACE_REPLAY_CALL_ERROR) {
                        replayWorkerLoggingCallback(VKTRACE_LOG_ERROR, QString("Failed to replay packet %1.")
                                                                           .arg(pCurPacket->pHeader->global_packet_index)
                                                                           .toStdString()
                                                                           .c_str());
                    } else if (res == vktrace_replay::VKTRACE_REPLAY_BAD_RETURN) {
                        replayWorkerLoggingCallback(
                            VKTRACE_LOG_WARNING,
                            QString("Replay of packet %1 has diverged from trace due to a different return value.")
                                .arg(pCurPacket->pHeader->global_packet_index)
                                .toStdString()
                                .c_str());
                    } else if (res == vktrace_replay::VKTRACE_REPLAY_INVALID_PARAMS ||
                               res == vktrace_replay::VKTRACE_REPLAY_VALIDATION_ERROR) {
                        // validation layer should have reported these if the user wanted them, so don't print any additional
                        // warnings here.
                    } else if (res != vktrace_replay::VKTRACE_REPLAY_SUCCESS) {
                        replayWorkerLoggingCallback(VKTRACE_LOG_ERROR, QString("Unknown error caused by packet %1.")
                                                                           .arg(pCurPacket->pHeader->global_packet_index)
                                                                           .toStdString()
                                                                           .c_str());
                    }
                } else {
                    replayWorkerLoggingCallback(VKTRACE_LOG_ERROR, QString("Bad packet type id=%1, index=%2.")
                                                                       .arg(pCurPacket->pHeader->packet_id)
                                                                       .arg(pCurPacket->pHeader->global_packet_index)
                                                                       .toStdString()
                                                                       .c_str());
                }
            }
        }

        // Process events and pause or stop if needed
        if (m_bPauseReplay || m_pauseAtPacketIndex == pCurPacket->pHeader->global_packet_index) {
            if (m_pauseAtPacketIndex == pCurPacket->pHeader->global_packet_index) {
                // reset
                m_pauseAtPacketIndex = -1;
            }

            m_bReplayInProgress = false;
            doReplayPaused(pCurPacket->pHeader->global_packet_index);
            return;
        }

        if (m_bStopReplay) {
            m_bReplayInProgress = false;
            doReplayStopped(pCurPacket->pHeader->global_packet_index);
            return;
        }
    }

    m_bReplayInProgress = false;
    doReplayFinished(pCurPacket->pHeader->global_packet_index);
}

void vktraceviewer_QReplayWorker::onPlayToHere() {
    m_pauseAtPacketIndex = m_pView->get_current_packet_index();
    if (m_pauseAtPacketIndex <= m_currentReplayPacketIndex || m_currentReplayPacketIndex == 0) {
        // pause location is behind the current replay position, so restart the replay.
        StartReplay();
    } else {
        // pause location is ahead of current replay position, so continue the replay.
        ContinueReplay();
    }
}

void vktraceviewer_QReplayWorker::StartReplay() {
    // Starting the replay can happen immediately.
    emit ReplayStarted();

    // Reset some flags and play the replay from the beginning
    m_bPauseReplay = false;
    m_bStopReplay = false;
    playCurrentTraceFile(0);
}

void vktraceviewer_QReplayWorker::StepReplay() {
    // Stepping the replay can happen immediately.
    emit ReplayContinued();

    // Set the pause flag so that the replay will stop after replaying the next packet.
    m_bPauseReplay = true;
    m_bStopReplay = false;
    playCurrentTraceFile(m_currentReplayPacketIndex + 1);
}

void vktraceviewer_QReplayWorker::PauseReplay() {
    // Pausing the replay happens asyncronously.
    // So set the pause flag and the replay will
    // react to it as soon as it can. It will call
    // doReplayPaused() when it has paused.
    m_bPauseReplay = true;
}

void vktraceviewer_QReplayWorker::ContinueReplay() {
    // Continuing the replay can happen immediately.
    emit ReplayContinued();

    // clear the pause and stop flags and continue the replay from the next packet
    m_bPauseReplay = false;
    m_bStopReplay = false;
    playCurrentTraceFile(m_currentReplayPacketIndex + 1);
}

void vktraceviewer_QReplayWorker::StopReplay() {
    if (m_bReplayInProgress) {
        // If a replay is in progress, then
        // Stopping the replay happens asycnronously.
        // Set the stop flag and the replay will
        // react to it as soon as it can. It will call
        // doReplayStopped() when it has stopped.
        m_bStopReplay = true;
    } else {
        // Replay is not in progress means:
        // 1) replay wasn't started (in which case stop button should be disabled and we can't get to this point),
        // 2) replay is currently paused, so do same actions as if the replay detected that it should stop.
        uint64_t packetIndex = this->m_pTraceFileInfo->pPacketOffsets[m_currentReplayPacketIndex].pHeader->global_packet_index;
        doReplayStopped(packetIndex);
    }
}

void vktraceviewer_QReplayWorker::onSettingsUpdated(vktrace_SettingGroup* pGroups, unsigned int numGroups) {
    if (m_pReplayers != NULL) {
        for (unsigned int tracerId = 0; tracerId < VKTRACE_MAX_TRACER_ID_ARRAY_SIZE; tracerId++) {
            if (m_pReplayers[tracerId] != NULL) {
                // now update the replayer with the loaded settings
                m_pReplayers[tracerId]->UpdateFromSettings(pGroups, numGroups);
            }
        }
    }
}

vktrace_replay::vktrace_trace_packet_replay_library* vktraceviewer_QReplayWorker::getReplayer(VKTRACE_TRACER_ID tracerId) {
    if (tracerId < 0 || tracerId >= VKTRACE_MAX_TRACER_ID_ARRAY_SIZE) {
        return NULL;
    }

    return m_pReplayers[tracerId];
}

void vktraceviewer_QReplayWorker::DetachReplay(bool detach) {
    for (int i = 0; i < VKTRACE_MAX_TRACER_ID_ARRAY_SIZE; i++) {
        if (m_pReplayers[i] != NULL) {
            m_pReplayers[i]->Deinitialize();

            vktrace_replay::ReplayDisplay disp;
            if (detach) {
                disp = vktrace_replay::ReplayDisplay(m_pReplayWindowWidth, m_pReplayWindowHeight, 0, false);
            } else {
                WId hWindow = m_pReplayWindow->winId();
                disp = vktrace_replay::ReplayDisplay((vktrace_window_handle)hWindow, m_pReplayWindowWidth, m_pReplayWindowHeight);
            }

            int err = m_pReplayers[i]->Initialize(&disp, NULL, NULL);
            assert(err == 0);
        }
    }
}

void vktraceviewer_QReplayWorker::doReplayPaused(uint64_t packetIndex) { emit ReplayPaused(packetIndex); }

void vktraceviewer_QReplayWorker::doReplayStopped(uint64_t packetIndex) {
    emit ReplayStopped(packetIndex);

    // Replay will start again from the beginning, so setup for that now.
    m_currentReplayPacketIndex = 0;
}

void vktraceviewer_QReplayWorker::doReplayFinished(uint64_t packetIndex) {
    // Indicate that the replay finished at the particular packet.
    emit ReplayFinished(packetIndex);

    // Replay will start again from the beginning, so setup for that now.
    m_currentReplayPacketIndex = 0;
}

//=============================================================================
void dbg_msg_callback(vktrace_replay::VKTRACE_DBG_MSG_TYPE msgType, uint64_t packetIndex, const char* pMsg) {
    if (g_pWorker != NULL) {
        if (msgType == vktrace_replay::VKTRACE_DBG_MSG_ERROR) {
            if (g_pWorker->PrintReplayErrorMsgs()) {
                g_pWorker->OutputMessage(VKTRACE_LOG_ERROR, packetIndex, QString(pMsg));
            }
            if (g_pWorker->PauseOnReplayErrorMsg()) {
                g_pWorker->PauseReplay();
            }
        } else if (msgType == vktrace_replay::VKTRACE_DBG_MSG_WARNING) {
            if (g_pWorker->PrintReplayWarningMsgs()) {
                g_pWorker->OutputMessage(VKTRACE_LOG_WARNING, packetIndex, QString(pMsg));
            }
            if (g_pWorker->PauseOnReplayWarningMsg()) {
                g_pWorker->PauseReplay();
            }
        } else {
            if (g_pWorker->PrintReplayInfoMsgs()) {
                g_pWorker->OutputMessage(VKTRACE_LOG_VERBOSE, packetIndex, QString(pMsg));
            }
            if (g_pWorker->PauseOnReplayInfoMsg()) {
                g_pWorker->PauseReplay();
            }
        }
    }
}
