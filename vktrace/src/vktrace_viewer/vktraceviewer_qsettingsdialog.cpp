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
#include "vktraceviewer_qsettingsdialog.h"
#include "vktraceviewer_settings.h"

#include <QDialogButtonBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QTableWidget>
#include <QVBoxLayout>

Q_DECLARE_METATYPE(vktrace_SettingInfo*);

vktraceviewer_QSettingsDialog::vktraceviewer_QSettingsDialog(QWidget *parent)
    : QDialog(parent),
      m_pSettingGroups(NULL),
      m_numSettingGroups(0)
{
    this->setWindowTitle("Settings");

    QVBoxLayout* pLayout = new QVBoxLayout(this);
    this->setLayout(pLayout);

    m_pTabWidget = new QTabWidget(this);
    pLayout->addWidget(m_pTabWidget);

    QDialogButtonBox* pButtonBox = new QDialogButtonBox(/*QDialogButtonBox::Save | QDialogButtonBox::Cancel*/);
    pButtonBox->addButton("OK", QDialogButtonBox::RejectRole);
    pButtonBox->addButton("Save && Apply", QDialogButtonBox::AcceptRole);
    pLayout->addWidget(pButtonBox);
    connect(pButtonBox, SIGNAL(accepted()), this, SLOT(acceptCB()));
    connect(pButtonBox, SIGNAL(rejected()), this, SLOT(cancelCB()));
}

vktraceviewer_QSettingsDialog::~vktraceviewer_QSettingsDialog()
{
    removeTabs();
}

void vktraceviewer_QSettingsDialog::removeTabs()
{
    if (m_pTabWidget == NULL)
    {
        return;
    }

    while (m_pTabWidget->count() > 0)
    {
        m_pTabWidget->removeTab(0);
    }
}

void vktraceviewer_QSettingsDialog::setGroups(vktrace_SettingGroup* pSettingGroups, unsigned int numGroups)
{
    removeTabs();

    m_pSettingGroups = pSettingGroups;
    m_numSettingGroups = numGroups;

    // add tabs to display other groups of settings
    for (unsigned int i = 0; i < m_numSettingGroups; i++)
    {
        this->add_tab(&m_pSettingGroups[i]);
    }
}

void vktraceviewer_QSettingsDialog::acceptCB()
{
    save();
}

void vktraceviewer_QSettingsDialog::cancelCB()
{
    reject();
}

void vktraceviewer_QSettingsDialog::resizeEvent(QResizeEvent *pEvent)
{
    emit Resized(pEvent->size().width(), pEvent->size().height());
}

void vktraceviewer_QSettingsDialog::save()
{
    // save vktraceviewer settings

    emit SaveSettings(m_pSettingGroups, m_numSettingGroups);
    accept();
}

void vktraceviewer_QSettingsDialog::add_tab(vktrace_SettingGroup* pGroup)
{
    QWidget* pTab = new QWidget(m_pTabWidget);
    m_pTabWidget->addTab(pTab, pGroup->pName);
    QHBoxLayout* pLayout = new QHBoxLayout(pTab);
    pTab->setLayout(pLayout);

    QTableWidget* pTable = new QTableWidget(pGroup->numSettings, 2, pTab);

    pLayout->addWidget(pTable, 1);

    QStringList headers;
    headers << "Name" << "Value";
    pTable->setHorizontalHeaderLabels(headers);
    pTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    pTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    connect(pTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(settingEdited(QTableWidgetItem *)));
    int row = 0;
    for (unsigned int i = 0; i < pGroup->numSettings; i++)
    {
        QTableWidgetItem *nameItem = new QTableWidgetItem(pGroup->pSettings[i].pLongName);
        nameItem->setData(Qt::UserRole, QVariant::fromValue(&pGroup->pSettings[i]));
        pTable->setItem(row, 0, nameItem);

        char* pLeakedMem = vktrace_SettingInfo_stringify_value(&pGroup->pSettings[i]);
        QTableWidgetItem *valueItem = new QTableWidgetItem(pLeakedMem);
        valueItem->setData(Qt::UserRole, QVariant::fromValue(&pGroup->pSettings[i]));
        pTable->setItem(row, 1, valueItem);

        ++row;
    }
}

void vktraceviewer_QSettingsDialog::settingEdited(QTableWidgetItem *pItem)
{
    vktrace_SettingInfo* pSetting = pItem->data(Qt::UserRole).value<vktrace_SettingInfo*>();

    if (pSetting != NULL)
    {
        if (pItem->column() == 0)
        {
            vktrace_free((void*)pSetting->pLongName);
            pSetting->pLongName = vktrace_allocate_and_copy(pItem->text().toStdString().c_str());
        }
        else if (pItem->column() == 1)
        {
            vktrace_SettingInfo_parse_value(pSetting, pItem->text().toStdString().c_str());
        }
        else
        {
            // invalid column
        }
    }
}
