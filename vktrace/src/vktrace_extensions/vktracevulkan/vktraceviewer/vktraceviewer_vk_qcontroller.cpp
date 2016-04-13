/**************************************************************************
*
* Copyright 2014-2016 Valve Corporation
* Copyright (C) 2014-2016 LunarG, Inc.
* All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* Author: Peter Lohrmann <peterl@valvesoftware.com> <plohrmann@gmail.com>
**************************************************************************/
#include "vktraceviewer_vk_settings.h"
#include "vktraceviewer_vk_qcontroller.h"

extern "C" {
#include "vktrace_trace_packet_utils.h"
#include "vktrace_vk_packet_id.h"
}

#include <assert.h>
#include <QFileInfo>
#include <QWidget>
#include <QToolButton>
#include <QCoreApplication>
#include <QProcess>

#include "vktraceviewer_view.h"
#include "vkreplay_seq.h"

static vktraceviewer_vk_QController* s_pController;

void controllerLoggingCallback(VktraceLogLevel level, const char* pMessage)
{
    if (s_pController != NULL)
    {
        s_pController->OnOutputMessage(level, pMessage);
    }
}

vktraceviewer_vk_QController::vktraceviewer_vk_QController()
    : m_pView(NULL),
      m_pTraceFileInfo(NULL),
      m_pDrawStateDiagram(NULL),
      m_pCommandBuffersDiagram(NULL),
      m_pReplayWidget(NULL),
      m_pTraceFileModel(NULL)
{
    s_pController = this;
    vktrace_LogSetCallback(controllerLoggingCallback);
    vktrace_LogSetLevel(VKTRACE_LOG_ERROR);
    initialize_default_settings();
    vktrace_SettingGroup_reset_defaults(&g_vkTraceViewerSettingGroup);
}

vktraceviewer_vk_QController::~vktraceviewer_vk_QController()
{
}

vktrace_trace_packet_header* vktraceviewer_vk_QController::InterpretTracePacket(vktrace_trace_packet_header* pHeader)
{
    // Attempt to interpret the packet as a Vulkan packet
    vktrace_trace_packet_header* pInterpretedHeader = interpret_trace_packet_vk(pHeader);
    if (pInterpretedHeader == NULL)
    {
        vktrace_LogError("Unrecognized Vulkan packet id: %u.", pHeader->packet_id);
    }
    else if (pInterpretedHeader->packet_id == VKTRACE_TPI_VK_vkApiVersion)
    {
        packet_vkApiVersion* pPacket = (packet_vkApiVersion*)pInterpretedHeader->pBody;
        if (pPacket->version != VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION))
        {
            vktrace_LogError("Trace file is from Vulkan version 0x%x (%u.%u.%u), but the VkTraceViewer plugin only supports version 0x%x (%u.%u.%u).", pPacket->version, VK_VERSION_MAJOR(pPacket->version), VK_VERSION_MINOR(pPacket->version), VK_VERSION_PATCH(pPacket->version), VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), 1, 0, VK_HEADER_VERSION);
            pInterpretedHeader = NULL;
        }
    }

    return pInterpretedHeader;
}

bool vktraceviewer_vk_QController::LoadTraceFile(vktraceviewer_trace_file_info* pTraceFileInfo, vktraceviewer_view* pView)
{
    assert(pTraceFileInfo != NULL);
    assert(pView != NULL);
    setView(pView);
    m_pTraceFileInfo = pTraceFileInfo;

    assert(m_pReplayWidget == NULL);
    m_pReplayWidget = new vktraceviewer_QReplayWidget(&m_replayWorker);
    if (m_pReplayWidget != NULL)
    {
        // load available replayers
        if (!m_replayWorker.load_replayers(pTraceFileInfo, m_pReplayWidget->GetReplayWindow(),
            g_vkTraceViewerSettings.replay_window_width,
            g_vkTraceViewerSettings.replay_window_height,
            g_vkTraceViewerSettings.separate_replay_window))
        {
            emit OutputMessage(VKTRACE_LOG_ERROR, "Failed to load necessary replayers.");
            delete m_pReplayWidget;
            m_pReplayWidget = NULL;
        }
        else
        {
            m_pView->add_custom_state_viewer(m_pReplayWidget, "Replayer", true);
            m_pReplayWidget->setEnabled(true);
            connect(m_pReplayWidget, SIGNAL(ReplayStarted()), this, SLOT(onReplayStarted()));
            connect(m_pReplayWidget, SIGNAL(ReplayPaused(uint64_t)), this, SLOT(onReplayPaused(uint64_t)));
            connect(m_pReplayWidget, SIGNAL(ReplayContinued()), this, SLOT(onReplayContinued()));
            connect(m_pReplayWidget, SIGNAL(ReplayStopped(uint64_t)), this, SLOT(onReplayStopped(uint64_t)));
            connect(m_pReplayWidget, SIGNAL(ReplayFinished(uint64_t)), this, SLOT(onReplayFinished(uint64_t)));
            connect(m_pReplayWidget, SIGNAL(ReplayProgressUpdate(uint64_t)), this, SLOT(onReplayProgressUpdate(uint64_t)));

            connect(m_pReplayWidget, SIGNAL(OutputMessage(VktraceLogLevel, const QString&)), this, SIGNAL(OutputMessage(VktraceLogLevel, const QString&)));
            connect(m_pReplayWidget, SIGNAL(OutputMessage(VktraceLogLevel, uint64_t, const QString&)), this, SIGNAL(OutputMessage(VktraceLogLevel, uint64_t, const QString&)));
        }
    }

    assert(m_pTraceFileModel == NULL);
    m_pTraceFileModel = new vktraceviewer_vk_QFileModel(NULL, pTraceFileInfo);
    updateCallTreeBasedOnSettings();

    deleteStateDumps();

    return true;
}

const char* vktraceviewer_vk_QController::GetPacketIdString(uint16_t packetId)
{
    return vktrace_vk_packet_id_name((VKTRACE_TRACE_PACKET_ID_VK)packetId);
}

void vktraceviewer_vk_QController::updateCallTreeBasedOnSettings()
{
    if (m_pTraceFileModel == NULL)
    {
        return;
    }

    if (g_vkTraceViewerSettings.groupByFrame)
    {
        if (m_groupByFramesProxy.sourceModel() != m_pTraceFileModel)
        {
            m_groupByFramesProxy.setSourceModel(m_pTraceFileModel);
        }
        m_pView->set_calltree_model(m_pTraceFileModel, &m_groupByFramesProxy);
    }
    else if (g_vkTraceViewerSettings.groupByThread)
    {
        if (m_groupByThreadsProxy.sourceModel() != m_pTraceFileModel)
        {
            m_groupByThreadsProxy.setSourceModel(m_pTraceFileModel);
        }
        m_pView->set_calltree_model(m_pTraceFileModel, &m_groupByThreadsProxy);
    }
    else
    {
        m_pView->set_calltree_model(m_pTraceFileModel, NULL);
    }
}

void vktraceviewer_vk_QController::deleteStateDumps() const
{
    QFile::remove("pipeline_dump.dot");
    QFile::remove("pipeline_dump.svg");
    QFile::remove("cb_dump.dot");
    QFile::remove("cb_dump.svg");
}

void vktraceviewer_vk_QController::setStateWidgetsEnabled(bool bEnabled)
{
    if(m_pDrawStateDiagram != NULL)
    {
        m_pView->enable_custom_state_viewer(m_pDrawStateDiagram, bEnabled);
    }

    if(m_pCommandBuffersDiagram != NULL)
    {
        m_pView->enable_custom_state_viewer(m_pCommandBuffersDiagram, bEnabled);
    }
}

void vktraceviewer_vk_QController::onReplayStarted()
{
    emit OutputMessage(VKTRACE_LOG_VERBOSE, "Replay Started");
    deleteStateDumps();
    setStateWidgetsEnabled(false);
    m_pView->on_replay_state_changed(true);
}

void vktraceviewer_vk_QController::onReplayPaused(uint64_t packetIndex)
{
    emit OutputMessage(VKTRACE_LOG_VERBOSE, packetIndex, "Replay Paused");
    m_pView->on_replay_state_changed(false);

    // When paused, the replay will 'continue' from the last packet,
    // so select that call to indicate to the user where the pause occured.
    m_pView->select_call_at_packet_index(packetIndex);

    // Dump state data from the replayer
    vktrace_replay::vktrace_trace_packet_replay_library* pVkReplayer = m_replayWorker.getReplayer(VKTRACE_TID_VULKAN);
    if (pVkReplayer != NULL)
    {
        int err;
        err = pVkReplayer->Dump();
        if (err)
        {
            emit OutputMessage(VKTRACE_LOG_WARNING, packetIndex, "Replayer couldn't output state data.");
        }
    }

    // Now try to load known state data.

    // Convert dot files to svg format
#if defined(PLATFORM_LINUX)
    if (QFile::exists("/usr/bin/dot"))
    {
        QProcess process;
        process.start("/usr/bin/dot pipeline_dump.dot -Tsvg -o pipeline_dump.svg");
        process.waitForFinished(-1);
        process.start("/usr/bin/dot cb_dump.dot -Tsvg -o cb_dump.svg");
        process.waitForFinished(-1);
    }
    else
    {
        emit OutputMessage(VKTRACE_LOG_ERROR, packetIndex, "DOT not found, unable to generate state diagrams.");
    }
#else
    emit OutputMessage(VKTRACE_LOG_ERROR, packetIndex, "DOT not found, unable to generate state diagrams.");
#endif

    if (QFile::exists("pipeline_dump.svg"))
    {
        if (m_pDrawStateDiagram == NULL)
        {
            m_pDrawStateDiagram = new vktraceviewer_qsvgviewer();
            m_pView->add_custom_state_viewer(m_pDrawStateDiagram, tr("Draw State"), false);
            m_pView->enable_custom_state_viewer(m_pDrawStateDiagram, false);
        }

        if (m_pDrawStateDiagram != NULL && m_pDrawStateDiagram->load(tr("pipeline_dump.svg")))
        {
            m_pView->enable_custom_state_viewer(m_pDrawStateDiagram, true);
        }

    }

    if (QFile::exists("cb_dump.svg"))
    {
        if (m_pCommandBuffersDiagram == NULL)
        {
            m_pCommandBuffersDiagram = new vktraceviewer_qsvgviewer();
            m_pView->add_custom_state_viewer(m_pCommandBuffersDiagram, tr("Command Buffers"), false);
            m_pView->enable_custom_state_viewer(m_pCommandBuffersDiagram, false);
        }

        if (m_pCommandBuffersDiagram != NULL && m_pCommandBuffersDiagram->load(tr("cb_dump.svg")))
        {
            m_pView->enable_custom_state_viewer(m_pCommandBuffersDiagram, true);
        }
    }
}

void vktraceviewer_vk_QController::onReplayContinued()
{
    emit OutputMessage(VKTRACE_LOG_VERBOSE, "Replay Continued");
    deleteStateDumps();
    setStateWidgetsEnabled(false);
    m_pView->on_replay_state_changed(true);
}

void vktraceviewer_vk_QController::onReplayStopped(uint64_t packetIndex)
{
    emit OutputMessage(VKTRACE_LOG_VERBOSE, packetIndex, "Replay Stopped");
    m_pView->on_replay_state_changed(false);
    setStateWidgetsEnabled(false);

    // Stopping the replay means that it will 'play' or 'step' from the beginning,
    // so select the first packet index to indicate to the user what stopping replay does.
    m_pView->select_call_at_packet_index(0);
}

void vktraceviewer_vk_QController::onReplayProgressUpdate(uint64_t packetArrayIndex)
{
    m_pView->highlight_timeline_item(packetArrayIndex, true, true);
}

void vktraceviewer_vk_QController::onReplayFinished(uint64_t packetIndex)
{
    emit OutputMessage(VKTRACE_LOG_VERBOSE, packetIndex, "Replay Finished");
    m_pView->on_replay_state_changed(false);
    setStateWidgetsEnabled(false);

    // The replay has completed, so highlight the final packet index.
    m_pView->select_call_at_packet_index(packetIndex);
}

void vktraceviewer_vk_QController::OnOutputMessage(VktraceLogLevel level, const QString& msg)
{
    emit OutputMessage(level, msg);
}

vktrace_SettingGroup* vktraceviewer_vk_QController::GetSettings()
{
    return &g_vkTraceViewerSettingGroup;
}

void vktraceviewer_vk_QController::UpdateFromSettings(vktrace_SettingGroup *pGroups, unsigned int numGroups)
{
    vktrace_SettingGroup_Apply_Overrides(&g_vkTraceViewerSettingGroup, pGroups, numGroups);

    m_replayWorker.setPrintReplayMessages(g_vkTraceViewerSettings.printReplayInfoMsgs,
        g_vkTraceViewerSettings.printReplayWarningMsgs,
        g_vkTraceViewerSettings.printReplayErrorMsgs);

    m_replayWorker.setPauseOnReplayMessages(g_vkTraceViewerSettings.pauseOnReplayInfo,
        g_vkTraceViewerSettings.pauseOnReplayWarning,
        g_vkTraceViewerSettings.pauseOnReplayError);

    m_replayWorker.onSettingsUpdated(pGroups, numGroups);

    updateCallTreeBasedOnSettings();
}

void vktraceviewer_vk_QController::UnloadTraceFile(void)
{
    if (m_pView != NULL)
    {
        m_pView->set_calltree_model(NULL, NULL);
        m_pView = NULL;
    }

    if (m_pTraceFileModel != NULL)
    {
        delete m_pTraceFileModel;
        m_pTraceFileModel = NULL;
    }

    if (m_pReplayWidget != NULL)
    {
        delete m_pReplayWidget;
        m_pReplayWidget = NULL;
    }

    if (m_pDrawStateDiagram != NULL)
    {
        delete m_pDrawStateDiagram;
        m_pDrawStateDiagram = NULL;
    }

    if (m_pCommandBuffersDiagram != NULL)
    {
        delete m_pCommandBuffersDiagram;
        m_pCommandBuffersDiagram = NULL;
    }

    m_replayWorker.unloadReplayers();
}
