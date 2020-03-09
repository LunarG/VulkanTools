/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */
#include <QFileDialog>

#include "dlgcreateassociation.h"
#include "ui_dlgcreateassociation.h"

dlgCreateAssociation::dlgCreateAssociation(QWidget *parent, int nProfileIndex) :
    QDialog(parent),
    ui(new Ui::dlgCreateAssociation)
    {
    ui->setupUi(this);

    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();
    nCurrentProfile = nProfileIndex;

    if(nProfileIndex >= 0) {
        QString title = tr("Application List for ");
        title += pVulkanConfig->profileList[nProfileIndex]->profileName;
        ui->groupBox->setTitle(title);

        for(int i = 0; i < pVulkanConfig->profileList[nProfileIndex]->appList.size(); i++)
            ui->listWidget->addItem(pVulkanConfig->profileList[nProfileIndex]->appList[i]);
        }
    }

dlgCreateAssociation::~dlgCreateAssociation()
    {
    delete ui;
    }


void dlgCreateAssociation::on_pushButtonAdd_clicked()         // Pick the test application
    {
    QString fileWildcard = ("Applications (*)");    // Linux default

#ifdef __APPLE__
    fileWildcard = QString("Applications (*.app, *");
#endif

#ifdef _WIN32
    fileWildcard = QString("Applications (*.exe)");
#endif

    QString appWithPath =  QFileDialog::getOpenFileName(this,
        tr("Select executable to pair with "), "/", fileWildcard);

    if(!appWithPath.isEmpty()) {
        pVulkanConfig->profileList[nCurrentProfile]->appList << appWithPath;
        ui->listWidget->addItem(appWithPath);
        }
    }

void dlgCreateAssociation::on_pushButtonRemove_clicked(void)
    {
    int nItem = ui->listWidget->currentRow();
    if(nItem < 0)
        return;

    pVulkanConfig->profileList[nCurrentProfile]->appList.removeAt(nItem);
    QListWidgetItem *pItem = ui->listWidget->takeItem(nItem);
    delete pItem;
    }
