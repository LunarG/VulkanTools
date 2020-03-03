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

#include "dlgcreateprofile.h"
#include "ui_dlgcreateprofile.h"


dlgcreateprofile::dlgcreateprofile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgcreateprofile)
{
    ui->setupUi(this);

    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();

    for(int i = 1; i < pVulkanConfig->profileList.size(); i++)
        ui->comboBoxProfiles->addItem(pVulkanConfig->profileList[i]->profileName);

}

dlgcreateprofile::~dlgcreateprofile()
{
    delete ui;
}


void dlgcreateprofile::on_pushButtonBrowse_clicked()         // Pick the test application
    {
    QString fileWildcard = ("Applications (*)");    // Linux default

#ifdef TARGET_OS_OSX
    fileWildcard = QString("Applications (*.app, *");
#endif

#ifdef TARGET_OS_WIN32
    fileWildcard = QString("Applications (*.exe)");
#endif

    QString appWithPath =  QFileDialog::getOpenFileName(this,
        tr("Select executable to pair with "), "/", fileWildcard);



    }
