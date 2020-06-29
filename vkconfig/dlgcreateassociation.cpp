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
#include <QCheckBox>

#include "dlgcreateassociation.h"
#include "ui_dlgcreateassociation.h"

//////////////////////////////////////////////////////////////////////////////
dlgCreateAssociation::dlgCreateAssociation(QWidget *parent) : QDialog(parent), ui(new Ui::dlgCreateAssociation) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    nLastSelectedApp = -1;

    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();
    bool enabledOnlyToList = !pVulkanConfig->bHasOldLoader && pVulkanConfig->bApplyOnlyToList;

    if (!enabledOnlyToList) setWindowTitle(tr("Applications Launcher Shortcuts"));

    // Show the current list
    for (int i = 0; i < pVulkanConfig->appList.size(); i++) {
        QTreeWidgetItem *pItem = new QTreeWidgetItem();
        pItem->setText(0, pVulkanConfig->appList[i]->qsAppNameWithPath);
        ui->treeWidget->addTopLevelItem(pItem);

        if (enabledOnlyToList) {
            QCheckBox *pCheckBox = new QCheckBox();
            pCheckBox->setChecked(pVulkanConfig->appList[i]->bExcludeFromGlobalList);
            pCheckBox->setToolTip(tr("Exclude from Layers Override"));
            ui->treeWidget->setItemWidget(pItem, 1, pCheckBox);
            connect(pCheckBox, SIGNAL(clicked(bool)), this, SLOT(itemClicked(bool)));
        }
    }

    QTreeWidgetItem *pHeader = ui->treeWidget->headerItem();
    pHeader->setText(0, tr("Application Executables"));
    pHeader->setText(1, tr("Exclude from Layers Override"));
    ui->treeWidget->installEventFilter(this);

    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(selectedPathChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(itemChanged(QTreeWidgetItem *, int)));
    connect(ui->lineEditCmdArgs, SIGNAL(textEdited(const QString &)), this, SLOT(editCommandLine(const QString &)));
    connect(ui->lineEditWorkingFolder, SIGNAL(textEdited(const QString &)), this, SLOT(editWorkingFolder(const QString &)));
    connect(ui->lineEditLogFile, SIGNAL(textEdited(const QString &)), this, SLOT(editLogFile(const QString &)));
}

///////////////////////////////////////////////////////////////////////////////
dlgCreateAssociation::~dlgCreateAssociation() { delete ui; }

//////////////////////////////////////////////////////////////////////////////
bool dlgCreateAssociation::eventFilter(QObject *target, QEvent *event) {
    // Launch tree does some fancy resizing and since it's down in
    // layouts and splitters, we can't just relay on the resize method
    // of this window.
    if (target == ui->treeWidget) {
        if (event->type() == QEvent::Resize) {
            ui->treeWidget->resizeColumnToContents(1);
            int nLastColumnWidth = ui->treeWidget->columnWidth(1);
            QRect rect = ui->treeWidget->geometry();
            ui->treeWidget->setColumnWidth(0, rect.width() - nLastColumnWidth);
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
/// Make sure any changes are saved
void dlgCreateAssociation::closeEvent(QCloseEvent *pEvent) {
    pVulkanConfig->SaveAppList();
    pEvent->accept();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief dlgCreateAssociation::on_pushButtonAdd_clicked
/// Browse for and select an executable file to add to the list.
void dlgCreateAssociation::on_pushButtonAdd_clicked()  // Pick the test application
{
    QString fileWildcard = ("Applications (*)");  // Linux default

#ifdef __APPLE__
    fileWildcard = QString("Applications (*.app, *");
#endif

#ifdef _WIN32
    fileWildcard = QString("Applications (*.exe)");
#endif

    // Go get it.
    QString appWithPath = QFileDialog::getOpenFileName(this, tr("Select a Vulkan Executable"), "/", fileWildcard);

    // If they have selected something!
    if (!appWithPath.isEmpty()) {
        // On macOS, they may have selected a binary, or they may have selected an app bundle.
        // If the later, we need to drill down to the actuall applicaiton
        if (appWithPath.right(4) == QString(".app")) {
            // Start by drilling down
            GetExecutableFromAppBundle(appWithPath);
        }

        appWithPath = QDir::toNativeSeparators(appWithPath);
        TAppListEntry *pNewApp = new TAppListEntry;
        pNewApp->qsAppNameWithPath = appWithPath;
        pNewApp->qsWorkingFolder = QDir::toNativeSeparators(QFileInfo(appWithPath).path());
        pNewApp->bExcludeFromGlobalList = false;
        pNewApp->qsLogFile = QDir::toNativeSeparators(ui->lineEditLogFile->text());
        pVulkanConfig->appList.push_back(pNewApp);
        QTreeWidgetItem *pItem = new QTreeWidgetItem();
        pItem->setText(0, appWithPath);
        QCheckBox *pCheck = new QCheckBox();
        ui->treeWidget->addTopLevelItem(pItem);
        ui->treeWidget->setItemWidget(pItem, 1, pCheck);
        pVulkanConfig->SaveAppList();
        pVulkanConfig->RefreshProfile();
        ui->treeWidget->setCurrentItem(pItem);
        nLastSelectedApp = ui->treeWidget->indexOfTopLevelItem(pItem);
        connect(pCheck, SIGNAL(clicked(bool)), this, SLOT(itemClicked(bool)));
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief dlgCreateAssociation::on_pushButtonRemove_clicked
/// Easy enough, just remove the selected program from the list
void dlgCreateAssociation::on_pushButtonRemove_clicked(void) {
    QTreeWidgetItem *pCurrent = ui->treeWidget->currentItem();
    nLastSelectedApp = ui->treeWidget->indexOfTopLevelItem(pCurrent);
    if (nLastSelectedApp < 0) return;

    ui->treeWidget->takeTopLevelItem(nLastSelectedApp);
    ui->treeWidget->setCurrentItem(nullptr);
    pVulkanConfig->appList.removeAt(nLastSelectedApp);

    ui->groupLaunchInfo->setEnabled(false);
    ui->pushButtonRemove->setEnabled(false);
    ui->lineEditCmdArgs->setText("");
    ui->lineEditWorkingFolder->setText("");
    ui->lineEditWorkingFolder->setText("");

    pVulkanConfig->SaveAppList();
    pVulkanConfig->RefreshProfile();
    nLastSelectedApp = -1;
}

///////////////////////////////////////////////////////////////////////////////
/// The remove button is disabled until/unless something is selected that can
/// be removed. Also the working folder and command line arguments are updated
void dlgCreateAssociation::selectedPathChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious) {
    (void)pPrevious;
    nLastSelectedApp = ui->treeWidget->indexOfTopLevelItem(pCurrent);
    if (nLastSelectedApp < 0) {
        ui->groupLaunchInfo->setEnabled(false);
        ui->pushButtonRemove->setEnabled(false);
        ui->lineEditCmdArgs->setText("");
        ui->lineEditWorkingFolder->setText("");
        ui->lineEditWorkingFolder->setText("");
        return;
    }

    ui->groupLaunchInfo->setEnabled(true);
    ui->pushButtonRemove->setEnabled(true);
    ui->lineEditWorkingFolder->setText(pVulkanConfig->appList[nLastSelectedApp]->qsWorkingFolder);
    ui->lineEditCmdArgs->setText(pVulkanConfig->appList[nLastSelectedApp]->qsArguments);
    ui->lineEditLogFile->setText(pVulkanConfig->appList[nLastSelectedApp]->qsLogFile);
}

///////////////////////////////////////////////////////////////////////////////
void dlgCreateAssociation::itemChanged(QTreeWidgetItem *pItem, int nColumn) {
    nLastSelectedApp = ui->treeWidget->indexOfTopLevelItem(pItem);
    QCheckBox *pCheckBox = dynamic_cast<QCheckBox *>(ui->treeWidget->itemWidget(pItem, nColumn));
    if (pCheckBox != nullptr) pVulkanConfig->appList[nLastSelectedApp]->bExcludeFromGlobalList = pCheckBox->isChecked();
}

///////////////////////////////////////////////////////////////////////////////
/// Something was clicked. We don't know what, and short of setting up a new
/// signal/slot for each button, this seemed a reasonable approach. Just poll
/// all of them. There aren't that many, so KISS (keep it simple stupid)
void dlgCreateAssociation::itemClicked(bool bClicked) {
    (void)bClicked;

    // Loop through the whole list and reset the checkboxes
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
        QTreeWidgetItem *pItem = ui->treeWidget->topLevelItem(i);
        QCheckBox *pCheckBox = dynamic_cast<QCheckBox *>(ui->treeWidget->itemWidget(pItem, 1));
        Q_ASSERT(pCheckBox != nullptr);
        pVulkanConfig->appList[i]->bExcludeFromGlobalList = pCheckBox->isChecked();
    }
}

///////////////////////////////////////////////////////////////////////////////
void dlgCreateAssociation::editCommandLine(const QString &cmdLine) {
    QTreeWidgetItem *pCurrent = ui->treeWidget->currentItem();
    nLastSelectedApp = ui->treeWidget->indexOfTopLevelItem(pCurrent);
    if (nLastSelectedApp < 0) return;

    pVulkanConfig->appList[nLastSelectedApp]->qsArguments = cmdLine;
}

//////////////////////////////////////////////////////////////////////////////
void dlgCreateAssociation::editWorkingFolder(const QString &workingFolder) {
    QTreeWidgetItem *pCurrent = ui->treeWidget->currentItem();
    nLastSelectedApp = ui->treeWidget->indexOfTopLevelItem(pCurrent);
    if (nLastSelectedApp < 0) return;

    pVulkanConfig->appList[nLastSelectedApp]->qsWorkingFolder = workingFolder;
}


void dlgCreateAssociation::editLogFile(const QString &logFile) {
    QTreeWidgetItem *pCurrent = ui->treeWidget->currentItem();
    nLastSelectedApp = ui->treeWidget->indexOfTopLevelItem(pCurrent);
    if(nLastSelectedApp < 0) return;

    pVulkanConfig->appList[nLastSelectedApp]->qsLogFile = logFile;
}


///////////////////////////////////////////////////////////////////////////////
/// \brief dlgCreateAssociation::GetExecutableFromAppBundle
/// \param csPath
/// This is only used on macOS to extract the executable from the bundle.
/// You have to look at the plist.info file, you can't just assume whatever
/// you find in the /MacOS folder is the executable.
/// The initial path is the folder where info.plist resides, and the
/// path is completed to the executable upon completion.
void dlgCreateAssociation::GetExecutableFromAppBundle(QString &csPath) {
    csPath += "/Contents/";
    QString pListFile = csPath + "Info.plist";
    QFile file(pListFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream stream(&file);

    // Read a line at a time looking for the executable tag
    QString lineBuffer;
    while (!stream.atEnd()) {
        lineBuffer = stream.readLine();
        if (lineBuffer.contains("<key>CFBundleExecutable</key>")) {  // Exe follows this
            lineBuffer = stream.readLine();                          // <string>Qt Creator</string>
            char *cExeName = new char[lineBuffer.length()];          // Prevent buffer overrun

            const char *pStart = strstr(lineBuffer.toUtf8().constData(), "<string>");
            if (pStart == nullptr) return;

            // We found it, now extract it out
            pStart += 8;
            int iIndex = 0;
            while (*pStart != '<') {
                cExeName[iIndex++] = *pStart++;
            }
            cExeName[iIndex] = '\0';

            // Complete the partial path
            csPath += QString("MacOS/");
            csPath += QString(cExeName);
            delete[] cExeName;
            break;
        }
    }

    file.close();
}
