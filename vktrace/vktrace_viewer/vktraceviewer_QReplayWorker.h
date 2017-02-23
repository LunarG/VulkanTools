/**************************************************************************
 *
 * Copyright 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
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
#ifndef VKTRACEVIEWER_QREPLAYWORKER_H
#define VKTRACEVIEWER_QREPLAYWORKER_H

#include <QObject>
#include "vktraceviewer_view.h"
#include "vkreplay_factory.h"

// Replay from vktraceviewer doesn't work yet. Disable it for now.
#define ENABLE_REPLAY false

class vktraceviewer_QReplayWorker : public QObject {
    Q_OBJECT
   public:
    vktraceviewer_QReplayWorker();
    virtual ~vktraceviewer_QReplayWorker();

    void setPrintReplayMessages(BOOL bPrintInfo, BOOL bPrintWarning, BOOL bPrintError);
    void setPauseOnReplayMessages(BOOL bPauseOnInfo, BOOL bPauseOnWarning, BOOL bPauseOnError);

    BOOL PrintReplayInfoMsgs();
    BOOL PrintReplayWarningMsgs();
    BOOL PrintReplayErrorMsgs();

    BOOL PauseOnReplayInfoMsg();
    BOOL PauseOnReplayWarningMsg();
    BOOL PauseOnReplayErrorMsg();

    void setView(vktraceviewer_view* pView);

    bool load_replayers(vktraceviewer_trace_file_info* pTraceFileInfo, QWidget* pReplayWindow, int const replayWindowWidth,
                        int const replayWindowHeight, bool const separateReplayWindow);

    void unloadReplayers();

   protected slots:
    virtual void playCurrentTraceFile(uint64_t startPacketIndex);
    virtual void onPlayToHere();

   public slots:
    void StartReplay();
    void StepReplay();
    void PauseReplay();
    void ContinueReplay();
    void StopReplay();

    void onSettingsUpdated(vktrace_SettingGroup* pGroups, unsigned int numGroups);

    vktrace_replay::vktrace_trace_packet_replay_library* getReplayer(VKTRACE_TRACER_ID tracerId);

    void DetachReplay(bool detach);

   signals:
    void ReplayStarted();
    void ReplayPaused(uint64_t packetIndex);
    void ReplayContinued();
    void ReplayStopped(uint64_t packetIndex);
    void ReplayFinished(uint64_t packetIndex);

    void ReplayProgressUpdate(uint64_t packetIndex);

    void OutputMessage(VktraceLogLevel level, const QString& msg);
    void OutputMessage(VktraceLogLevel level, uint64_t packetIndex, const QString& msg);

   private:
    volatile bool m_bPauseReplay;
    volatile bool m_bStopReplay;
    volatile bool m_bReplayInProgress;
    vktraceviewer_view* m_pView;
    vktraceviewer_trace_file_info* m_pTraceFileInfo;
    uint64_t m_currentReplayPacketIndex;
    QAction* m_pActionRunToHere;
    uint64_t m_pauseAtPacketIndex;

    QWidget* m_pReplayWindow;
    int m_pReplayWindowWidth;
    int m_pReplayWindowHeight;

    BOOL m_bPrintReplayInfoMessages;
    BOOL m_bPrintReplayWarningMessages;
    BOOL m_bPrintReplayErrorMessages;

    BOOL m_bPauseOnReplayInfoMessages;
    BOOL m_bPauseOnReplayWarningMessages;
    BOOL m_bPauseOnReplayErrorMessages;

    vktrace_replay::ReplayFactory m_replayerFactory;
    vktrace_replay::vktrace_trace_packet_replay_library* m_pReplayers[VKTRACE_MAX_TRACER_ID_ARRAY_SIZE];

    void doReplayPaused(uint64_t packetIndex);
    void doReplayStopped(uint64_t packetIndex);
    void doReplayFinished(uint64_t packetIndex);
};

#endif  // VKTRACEVIEWER_QREPLAYWORKER_H
