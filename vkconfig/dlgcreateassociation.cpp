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
#include <QCloseEvent>

#include "dlgcreateassociation.h"
#include "ui_dlgcreateassociation.h"





//////////////////////////////////////////////////////////////////////////////
dlgCreateAssociation::dlgCreateAssociation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgCreateAssociation)
    {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();

    // Show the current list
    for(int i = 0; i < pVulkanConfig->appList.size(); i++)
        ui->listWidget->addItem(pVulkanConfig->appList[i]->qsAppNameWithPath);

    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(selectedPathChanged()));
    connect(ui->lineEditCmdArgs, SIGNAL(textEdited(const QString&)), this, SLOT(editCommandLine(const QString&)));
    connect(ui->lineEditWorkingFolder, SIGNAL(textEdited(const QString&)), this, SLOT(editWorkingFolder(const QString&)));
    }

///////////////////////////////////////////////////////////////////////////////
dlgCreateAssociation::~dlgCreateAssociation()
    {
    delete ui;
    }


void dlgCreateAssociation::closeEvent(QCloseEvent *pEvent)
    {
    pVulkanConfig->SaveAppList();
    pEvent->accept();
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

        TAppListEntry *pNewApp = new TAppListEntry;
        pNewApp->qsAppNameWithPath = appWithPath;
        pNewApp->qsWorkingFolder = QFileInfo(appWithPath).path();
        pVulkanConfig->appList.push_back(pNewApp);
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

    QListWidgetItem *pItem = ui->listWidget->takeItem(nItem);
    delete pItem;
    pVulkanConfig->appList.removeAt(nItem);

    if(ui->listWidget->currentRow() == -1) {
        ui->pushButtonRemove->setEnabled(false);
        ui->lineEditCmdArgs->setText("");
        ui->lineEditCmdArgs->setEnabled(false);
        ui->lineEditWorkingFolder->setText("");
        ui->lineEditWorkingFolder->setEnabled(false);
        }

    pVulkanConfig->SaveAppList();
    pVulkanConfig->RefreshProfile();
    }

///////////////////////////////////////////////////////////////////////////////
/// \brief dlgCreateAssociation::selectedPathChanged
/// The remove button is disabled until/unless something is selected that can
/// be removed. Also the working folder and command line arguments are updated
void dlgCreateAssociation::selectedPathChanged(void)
    {
    int iRow = ui->listWidget->currentRow();
    if(iRow < 0) {
        ui->groupLaunchInfo->setEnabled(false);
        ui->pushButtonRemove->setEnabled(false);
        ui->lineEditCmdArgs->setText("");
        ui->lineEditWorkingFolder->setText("");
        return;
        }

    ui->groupLaunchInfo->setEnabled(true);
    ui->pushButtonRemove->setEnabled(true);
    ui->lineEditWorkingFolder->setText(pVulkanConfig->appList[iRow]->qsWorkingFolder);
    ui->lineEditCmdArgs->setText(pVulkanConfig->appList[iRow]->qsArguments);
    }


void dlgCreateAssociation::editCommandLine(const QString& cmdLine)
    {
    int iRow = ui->listWidget->currentRow();
    if(iRow < 0)
        return;

    pVulkanConfig->appList[iRow]->qsArguments = cmdLine;
    }

void dlgCreateAssociation::editWorkingFolder(const QString& workingFolder)
    {
    int iRow = ui->listWidget->currentRow();
    if(iRow < 0)
        return;

    pVulkanConfig->appList[iRow]->qsWorkingFolder = workingFolder;

    }


///////////////////////////////////////////////////////////////////////////////
/// \brief dlgCreateAssociation::GetExecutableFromAppBundle
/// \param csPath
/// This is only used on macOS to extract the executable from the bundle.
/// You have to look at the plist.info file, you can't just assume whatever
/// you find in the /MacOS folder is the executable.
/// The initial path is the folder where info.plist resides, and the
/// path is completed to the executable upon completion.
void dlgCreateAssociation::GetExecutableFromAppBundle(QString& csPath)
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
