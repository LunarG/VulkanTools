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
#ifndef _VKTRACEVIEWER_QSETTINGSDIALOG_H_
#define _VKTRACEVIEWER_QSETTINGSDIALOG_H_

#include <QDialog>
#include <QTabWidget>
#include <QTableWidgetItem>
#include "vktraceviewer_settings.h"

Q_DECLARE_METATYPE(vktrace_SettingGroup)

class vktraceviewer_QSettingsDialog : public QDialog {
    Q_OBJECT

  public:
    vktraceviewer_QSettingsDialog(QWidget* parent = 0);
    ~vktraceviewer_QSettingsDialog();

    void setGroups(vktrace_SettingGroup* pSettingGroups, unsigned int numGroups);

    void save();

  private:
    vktrace_SettingGroup* m_pSettingGroups;
    unsigned int m_numSettingGroups;

    QTabWidget* m_pTabWidget;
    void add_tab(vktrace_SettingGroup* pGroup);
    virtual void resizeEvent(QResizeEvent* pEvent);
    void removeTabs();

  signals:
    void SaveSettings(vktrace_SettingGroup* pUpdatedSettingGroups, unsigned int numGroups);
    void Resized(unsigned int width, unsigned int height);

  private slots:
    void acceptCB();
    void cancelCB();

    void settingEdited(QTableWidgetItem* pItem);
};

#endif  // _VKTRACEVIEWER_QSETTINGSDIALOG_H_
