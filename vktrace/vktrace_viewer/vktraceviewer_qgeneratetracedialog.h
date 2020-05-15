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
#ifndef VKTRACEVIEWER_QGENERATETRACEDIALOG_H
#define VKTRACEVIEWER_QGENERATETRACEDIALOG_H

extern "C" {
#include "vktrace_platform.h"
#include "vktrace_tracelog.h"
}

#include <QDialog>
#include <QProcessEnvironment>

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>

class vktraceviewer_QGenerateTraceDialog : public QDialog {
    Q_OBJECT
  public:
    explicit vktraceviewer_QGenerateTraceDialog(QWidget *parent = 0);
    virtual ~vktraceviewer_QGenerateTraceDialog();

    virtual int exec();

    enum DialogCode { Cancelled, Succeeded, Failed };

    QString get_trace_file_path();

  signals:
    void OutputMessage(VktraceLogLevel level, const QString &message);

  public slots:

  private slots:
    void on_applicationLineEdit_textChanged(const QString &text);
    void on_traceFileLineEdit_textChanged(const QString &text);
    void on_vkLayerPathLineEdit_textChanged(const QString &text);
    void on_findApplicationButton_clicked();
    void on_vkLayerPathButton_clicked();
    void on_findTraceFileButton_clicked();

    void on_readStandardOutput();
    void on_readStandardError();

  private:
    bool launch_application_to_generate_trace();

    void check_inputs();

    QProcess *m_pGenerateTraceProcess;
    QGridLayout *m_pGridLayout;
    QLabel *m_pApplicationLabel;
    QLineEdit *m_pApplicationLineEdit;
    QPushButton *m_pFindApplicationButton;
    QLabel *m_pArgumentsLabel;
    QLineEdit *m_pArgumentsLineEdit;
    QLabel *m_pWorkingDirLabel;
    QLineEdit *m_pWorkingDirLineEdit;
    QLabel *m_pVkLayerPathLabel;
    QLineEdit *m_pVkLayerPathLineEdit;
    QPushButton *m_pVkLayerPathButton;
    QLabel *m_pTracefileLabel;
    QLineEdit *m_pTraceFileLineEdit;
    QPushButton *m_pFindTraceFileButton;
    QFrame *m_pButtonFrame;
    QHBoxLayout *m_pButtonHorizontalLayout;
    QSpacerItem *m_pButtonHSpacer;
    QPushButton *m_pCancelButton;
    QPushButton *m_pOkButton;
    QSpacerItem *m_pButtonHSpacer2;
};

#endif  // VKTRACEVIEWER_QGENERATETRACEDIALOG_H
