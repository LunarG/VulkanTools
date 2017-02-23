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
#ifndef _VKTRACEVIEWER_QREPLAYWIDGET_H_
#define _VKTRACEVIEWER_QREPLAYWIDGET_H_

#include <QWidget>
#include <QThread>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QCheckBox>

#include "vktraceviewer_QReplayWorker.h"

class vktraceviewer_QReplayWidget : public QWidget {
    Q_OBJECT
   public:
    explicit vktraceviewer_QReplayWidget(vktraceviewer_QReplayWorker* pWorker, QWidget* parent = 0)
        : QWidget(parent), m_pWorker(pWorker) {
        QVBoxLayout* pLayout = new QVBoxLayout(this);
        setLayout(pLayout);

        m_pToolBar = new QToolBar("ReplayToolbar", this);
        pLayout->addWidget(m_pToolBar);

        m_pPlayButton = new QToolButton(m_pToolBar);
        m_pPlayButton->setText("Play");
        m_pPlayButton->setEnabled(ENABLE_REPLAY);
        m_pToolBar->addWidget(m_pPlayButton);
        connect(m_pPlayButton, SIGNAL(clicked()), this, SLOT(onPlayButtonClicked()));

        m_pStepButton = new QToolButton(m_pToolBar);
        m_pStepButton->setText("Step");
        m_pStepButton->setEnabled(ENABLE_REPLAY);
        m_pToolBar->addWidget(m_pStepButton);
        connect(m_pStepButton, SIGNAL(clicked()), this, SLOT(onStepButtonClicked()));

        m_pPauseButton = new QToolButton(m_pToolBar);
        m_pPauseButton->setText("Pause");
        m_pPauseButton->setEnabled(false);
        m_pToolBar->addWidget(m_pPauseButton);
        connect(m_pPauseButton, SIGNAL(clicked()), this, SLOT(onPauseButtonClicked()));

        m_pContinueButton = new QToolButton(m_pToolBar);
        m_pContinueButton->setText("Continue");
        m_pContinueButton->setEnabled(false);
        m_pToolBar->addWidget(m_pContinueButton);
        connect(m_pContinueButton, SIGNAL(clicked()), this, SLOT(onContinueButtonClicked()));

        m_pStopButton = new QToolButton(m_pToolBar);
        m_pStopButton->setText("Stop");
        m_pStopButton->setEnabled(false);
        m_pToolBar->addWidget(m_pStopButton);
        connect(m_pStopButton, SIGNAL(clicked()), this, SLOT(onStopButtonClicked()));

        m_pDetachCheckBox = new QCheckBox(m_pToolBar);
        m_pDetachCheckBox->setText("Detach");
        m_pDetachCheckBox->setEnabled(ENABLE_REPLAY);
        m_pToolBar->addWidget(m_pDetachCheckBox);
        connect(m_pDetachCheckBox, SIGNAL(clicked(bool)), this, SLOT(onDetachCheckBoxClicked(bool)));

        m_pReplayWindow = new QWidget(this);
        pLayout->addWidget(m_pReplayWindow);

        // connect worker signals to widget actions
        qRegisterMetaType<uint64_t>("uint64_t");
        m_replayThread.setObjectName("ReplayThread");
        m_pWorker->moveToThread(&m_replayThread);
        m_replayThread.start();

        // Clicking the Pause and Stop buttons are direct connections so that they happen more immediately than a queued connection.
        // Queued connections are used here whenever the replay will be advanced from a stopped state,
        // and for all the signals FROM the worker object since it is on a different thread.
        connect(this, SIGNAL(PlayButtonClicked()), m_pWorker, SLOT(StartReplay()), Qt::QueuedConnection);
        connect(this, SIGNAL(StepButtonClicked()), m_pWorker, SLOT(StepReplay()), Qt::QueuedConnection);
        connect(this, SIGNAL(PauseButtonClicked()), m_pWorker, SLOT(PauseReplay()), Qt::DirectConnection);
        connect(this, SIGNAL(ContinueButtonClicked()), m_pWorker, SLOT(ContinueReplay()), Qt::QueuedConnection);
        connect(this, SIGNAL(StopButtonClicked()), m_pWorker, SLOT(StopReplay()), Qt::DirectConnection);
        connect(this, SIGNAL(DetachCheckBoxClicked(bool)), m_pWorker, SLOT(DetachReplay(bool)), Qt::QueuedConnection);

        connect(m_pWorker, SIGNAL(ReplayStarted()), this, SLOT(slotReplayStarted()), Qt::QueuedConnection);
        connect(m_pWorker, SIGNAL(ReplayPaused(uint64_t)), this, SLOT(slotReplayPaused(uint64_t)), Qt::QueuedConnection);
        connect(m_pWorker, SIGNAL(ReplayContinued()), this, SLOT(slotReplayContinued()), Qt::QueuedConnection);
        connect(m_pWorker, SIGNAL(ReplayStopped(uint64_t)), this, SLOT(slotReplayStopped(uint64_t)), Qt::QueuedConnection);
        connect(m_pWorker, SIGNAL(ReplayFinished(uint64_t)), this, SLOT(slotReplayFinished(uint64_t)), Qt::QueuedConnection);
        connect(m_pWorker, SIGNAL(ReplayProgressUpdate(uint64_t)), this, SIGNAL(ReplayProgressUpdate(uint64_t)),
                Qt::QueuedConnection);

        connect(m_pWorker, SIGNAL(OutputMessage(VktraceLogLevel, uint64_t, const QString&)), this,
                SLOT(OnOutputMessage(VktraceLogLevel, uint64_t, const QString&)), Qt::QueuedConnection);
    }

    virtual ~vktraceviewer_QReplayWidget() {
        m_replayThread.quit();
        m_replayThread.wait();
    }

    virtual QPaintEngine* paintEngine() const { return NULL; }

    QWidget* GetReplayWindow() const { return m_pReplayWindow; }

   signals:
    void PlayButtonClicked();
    void StepButtonClicked();
    void PauseButtonClicked();
    void ContinueButtonClicked();
    void StopButtonClicked();
    void DetachCheckBoxClicked(bool checked);

    void ReplayStarted();
    void ReplayPaused(uint64_t packetIndex);
    void ReplayContinued();
    void ReplayStopped(uint64_t packetIndex);
    void ReplayFinished(uint64_t packetIndex);
    void ReplayProgressUpdate(uint64_t packetIndex);

    void OutputMessage(VktraceLogLevel level, const QString& msg);
    void OutputMessage(VktraceLogLevel level, uint64_t packetIndex, const QString& msg);

   private slots:

    void slotReplayStarted() {
        m_pPlayButton->setEnabled(false);
        m_pStepButton->setEnabled(false);
        m_pPauseButton->setEnabled(true);
        m_pContinueButton->setEnabled(false);
        m_pStopButton->setEnabled(true);
        m_pDetachCheckBox->setEnabled(false);

        emit ReplayStarted();
    }

    void slotReplayPaused(uint64_t packetIndex) {
        m_pPlayButton->setEnabled(false);
        m_pStepButton->setEnabled(true);
        m_pPauseButton->setEnabled(false);
        m_pContinueButton->setEnabled(true);
        m_pStopButton->setEnabled(true);
        m_pDetachCheckBox->setEnabled(false);

        emit ReplayPaused(packetIndex);
    }

    void slotReplayContinued() {
        m_pPlayButton->setEnabled(false);
        m_pStepButton->setEnabled(false);
        m_pPauseButton->setEnabled(true);
        m_pContinueButton->setEnabled(false);
        m_pStopButton->setEnabled(true);
        m_pDetachCheckBox->setEnabled(false);

        emit ReplayContinued();
    }

    void slotReplayStopped(uint64_t packetIndex) {
        m_pPlayButton->setEnabled(true);
        m_pStepButton->setEnabled(true);
        m_pPauseButton->setEnabled(false);
        m_pContinueButton->setEnabled(false);
        m_pStopButton->setEnabled(false);
        m_pDetachCheckBox->setEnabled(true);

        emit ReplayStopped(packetIndex);
    }

    void slotReplayFinished(uint64_t packetIndex) {
        m_pPlayButton->setEnabled(true);
        m_pStepButton->setEnabled(true);
        m_pPauseButton->setEnabled(false);
        m_pContinueButton->setEnabled(false);
        m_pStopButton->setEnabled(false);
        m_pDetachCheckBox->setEnabled(true);

        emit ReplayFinished(packetIndex);
    }

    void OnOutputMessage(VktraceLogLevel level, uint64_t packetIndex, const QString& msg) {
        emit OutputMessage(level, packetIndex, msg);
    }

   public slots:
    void onPlayButtonClicked() { emit PlayButtonClicked(); }

    void onStepButtonClicked() { emit StepButtonClicked(); }

    void onPauseButtonClicked() {
        m_pPlayButton->setEnabled(false);
        m_pPauseButton->setEnabled(false);
        m_pContinueButton->setEnabled(false);
        m_pStopButton->setEnabled(false);

        emit PauseButtonClicked();
    }

    void onContinueButtonClicked() { emit ContinueButtonClicked(); }

    void onStopButtonClicked() { emit StopButtonClicked(); }

    void onDetachCheckBoxClicked(bool checked) { emit DetachCheckBoxClicked(checked); }

   private:
    vktraceviewer_QReplayWorker* m_pWorker;
    QWidget* m_pReplayWindow;
    QToolBar* m_pToolBar;
    QToolButton* m_pPlayButton;
    QToolButton* m_pStepButton;
    QToolButton* m_pPauseButton;
    QToolButton* m_pContinueButton;
    QToolButton* m_pStopButton;
    QCheckBox* m_pDetachCheckBox;
    QThread m_replayThread;
};

#endif  //_VKTRACEVIEWER_QREPLAYWIDGET_H_
