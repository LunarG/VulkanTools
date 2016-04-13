/**************************************************************************
 *
 * Copyright 2014 Valve Software. All Rights Reserved.
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


class vktraceviewer_vk_QController : public vktraceviewer_QController
{
    Q_OBJECT
public:
    vktraceviewer_vk_QController();
    virtual ~vktraceviewer_vk_QController();

    virtual vktrace_trace_packet_header* InterpretTracePacket(vktrace_trace_packet_header* pHeader);
    virtual bool LoadTraceFile(vktraceviewer_trace_file_info* pTraceFileInfo, vktraceviewer_view* pView);
    virtual void UnloadTraceFile(void);

    void setView(vktraceviewer_view* pView)
    {
        m_pView = pView;
        m_replayWorker.setView(pView);
    }

    virtual vktrace_SettingGroup* GetSettings();
    virtual void UpdateFromSettings(vktrace_SettingGroup *pGroups, unsigned int numGroups);

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

#endif // VKTRACEVIEWER_VK_QCONTROLLER_H
