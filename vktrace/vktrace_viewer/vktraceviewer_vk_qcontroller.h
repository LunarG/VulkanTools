/**************************************************************************
 *
 * Copyright 2014 Valve Software. All Rights Reserved.
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
 *************************************************************************/
#ifndef VKTRACEVIEWER_VK_QCONTROLLER_H
#define VKTRACEVIEWER_VK_QCONTROLLER_H

#include "vktrace_trace_packet_identifiers.h"
#include "vktraceviewer_vk_qgroupframesproxymodel.h"
#include "vktraceviewer_qgroupthreadsproxymodel.h"
#include "vktraceviewer_qsvgviewer.h"
#include "vktraceviewer_QReplayWidget.h"
#include "vktraceviewer_QReplayWorker.h"
#include "vktraceviewer_vk_qfile_model.h"
#include "vktraceviewer_controller.h"
#include <QLabel>
#include <QScrollArea>

class vktraceviewer_vk_QController : public vktraceviewer_QController {
    Q_OBJECT
  public:
    vktraceviewer_vk_QController();
    virtual ~vktraceviewer_vk_QController();

    virtual vktrace_trace_packet_header* InterpretTracePacket(vktrace_trace_packet_header* pHeader);
    virtual bool LoadTraceFile(vktraceviewer_trace_file_info* pTraceFileInfo, vktraceviewer_view* pView);
    virtual void UnloadTraceFile(void);

    void setView(vktraceviewer_view* pView) {
        m_pView = pView;
        m_replayWorker.setView(pView);
    }

    virtual vktrace_SettingGroup* GetSettings();
    virtual void UpdateFromSettings(vktrace_SettingGroup* pGroups, unsigned int numGroups);

    virtual const char* GetPacketIdString(uint16_t packetId);

  public slots:
    void OnOutputMessage(VktraceLogLevel level, const QString& msg);

  signals:
    // Inherited from glvdebug_QController
    void OutputMessage(VktraceLogLevel level, const QString& message);
    void OutputMessage(VktraceLogLevel level, uint64_t packetIndex, const QString& message);

  protected slots:
    void onReplayStarted();
    void onReplayPaused(uint64_t packetIndex);
    void onReplayContinued();
    void onReplayStopped(uint64_t packetIndex);
    void onReplayFinished(uint64_t packetIndex);
    void onReplayProgressUpdate(uint64_t packetArrayIndex);

  private:
    vktraceviewer_view* m_pView;
    vktraceviewer_trace_file_info* m_pTraceFileInfo;
    vktraceviewer_QReplayWorker m_replayWorker;
    vktraceviewer_qsvgviewer* m_pDrawStateDiagram;
    vktraceviewer_qsvgviewer* m_pCommandBuffersDiagram;
    vktraceviewer_QReplayWidget* m_pReplayWidget;
    vktraceviewer_vk_QFileModel* m_pTraceFileModel;
    vktraceviewer_vk_QGroupFramesProxyModel m_groupByFramesProxy;
    vktraceviewer_QGroupThreadsProxyModel m_groupByThreadsProxy;

    void setStateWidgetsEnabled(bool bEnabled);
    void updateCallTreeBasedOnSettings();
    void deleteStateDumps() const;
};

#endif  // VKTRACEVIEWER_VK_QCONTROLLER_H
