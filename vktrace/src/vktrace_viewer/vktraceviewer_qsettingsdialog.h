/**************************************************************************
*
* Copyright 2015-2016 Valve Corporation
* Copyright (C) 2015-2016 LunarG, Inc.
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
#ifndef _VKTRACEVIEWER_QSETTINGSDIALOG_H_
#define _VKTRACEVIEWER_QSETTINGSDIALOG_H_

#include <QDialog>
#include <QTabWidget>
#include <QTableWidgetItem>
#include "vktraceviewer_settings.h"

Q_DECLARE_METATYPE(vktrace_SettingGroup)

class vktraceviewer_QSettingsDialog : public QDialog
{
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
    virtual void resizeEvent(QResizeEvent *pEvent);
    void removeTabs();

signals:
    void SaveSettings(vktrace_SettingGroup* pUpdatedSettingGroups, unsigned int numGroups);
    void Resized(unsigned int width, unsigned int height);

private
slots:
    void acceptCB();
    void cancelCB();

    void settingEdited(QTableWidgetItem *pItem);

};

#endif // _VKTRACEVIEWER_QSETTINGSDIALOG_H_
