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
#include <QTextStream>

#include "dlgcreateassociation.h"
#include "ui_dlgcreateassociation.h"




///////////////////////////////////////////////////////////////////////////////
/// \brief dlgCreateAssociation::GetExecutableFromAppBundle
/// \param csPath
/// This is only used on macOS to extract the executable from the bundle.
/// You have to look at the plist.info file, you can't just assume whatever
/// you find in the /MacOS folder is the executable.
/// The initial path is the folder where info.plist resides, and the
/// path is completed to the executable upon completion.
void GetExecutableFromAppBundle(QString& csPath)
    {
    csPath += "/Contents/";
    QString pListFile = csPath + "Info.plist";
    QFile file(pListFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);

    // Read a line at a time looking for the executable tag
    QString lineBuffer;
    while(!stream.atEnd()) {
        lineBuffer = stream.readLine();
        if(lineBuffer.contains("<key>CFBundleExecutable</key>")) {  // Exe follows this
            lineBuffer = stream.readLine();  // <string>Qt Creator</string>
            char *cExeName = new char[lineBuffer.length()]; // Prevent buffer overrun

            const char *pStart = strstr(lineBuffer.toUtf8().constData(), "<string>");
            if(pStart == nullptr)
                return;

            // We found it, now extract it out
            pStart += 8;
            int iIndex = 0;
            while(*pStart != '<') {
                cExeName[iIndex++] = *pStart++;
                }
            cExeName[iIndex] = '\0';

            // Complete the partial path
            csPath += QString("MacOS/");
            csPath += QString(cExeName);
            delete [] cExeName;
            break;
            }
        }

    file.close();
    }

//////////////////////////////////////////////////////////////////////////////
dlgCreateAssociation::dlgCreateAssociation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgCreateAssociation)
    {
    ui->setupUi(this);

    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();

    for(int i = 0; i < pVulkanConfig->appList.size(); i++)
        ui->listWidget->addItem(pVulkanConfig->appList[i]);

    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(selectedPathChanged()));
    }

///////////////////////////////////////////////////////////////////////////////
dlgCreateAssociation::~dlgCreateAssociation()
    {
    delete ui;
    }



///////////////////////////////////////////////////////////////////////////////
/// \brief dlgCreateAssociation::on_pushButtonAdd_clicked
/// Browse for and select an executable file to add to the list.
void dlgCreateAssociation::on_pushButtonAdd_clicked()         // Pick the test application
    {
    QString fileWildcard = ("Applications (*)");    // Linux default

#ifdef __APPLE__
    fileWildcard = QString("Applications (*.app, *");
#endif

#ifdef _WIN32
    fileWildcard = QString("Applications (*.exe)");
#endif

    // Go get it.
    QString appWithPath =  QFileDialog::getOpenFileName(this,
        tr("Select executable to pair with "), "/", fileWildcard);

    // If they have selected something!
    if(!appWithPath.isEmpty()) {
        // On macOS, they may have selected a binary, or they may have selected an app bundle.
        // If the later, we need to drill down to the actuall applicaiton
        if(appWithPath.right(4) == QString(".app")) {
            // Start by drilling down
            GetExecutableFromAppBundle(appWithPath);
            }

        pVulkanConfig->appList << appWithPath;
        ui->listWidget->addItem(appWithPath);
        pVulkanConfig->SaveAppList();
        pVulkanConfig->RefreshProfile();
        }
    }

///////////////////////////////////////////////////////////////////////////////
/// \brief dlgCreateAssociation::on_pushButtonRemove_clicked
/// Easy enough, just remove the selected program from the list
void dlgCreateAssociation::on_pushButtonRemove_clicked(void)
    {
    int nItem = ui->listWidget->currentRow();
    if(nItem < 0)
        return;

    pVulkanConfig->appList.removeAt(nItem);
    QListWidgetItem *pItem = ui->listWidget->takeItem(nItem);
    delete pItem;

    if(ui->listWidget->currentRow() == -1)
        ui->pushButtonRemove->setEnabled(false);

    pVulkanConfig->SaveAppList();
    pVulkanConfig->RefreshProfile();
    }

///////////////////////////////////////////////////////////////////////////////
/// \brief dlgCreateAssociation::selectedPathChanged
/// The remove button is disabled until/unless something is selected that can
/// be removed.
void dlgCreateAssociation::selectedPathChanged(void)
    {
    ui->pushButtonRemove->setEnabled(true);
    }
