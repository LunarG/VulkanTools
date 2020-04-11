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

#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QFrame>
#include <QVariant>
#ifndef _WIN32
#include <unistd.h>
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgabout.h"
#include "dlglayeroutput.h"
#include "dlgvulkananalysis.h"
#include "dlgvulkaninfo.h"
#include "dlgcreateprofile.h"
#include "dlgcreateassociation.h"
#include "dlghistory.h"
#include "dlgprofileeditor.h"
#include "dlgcustompaths.h"
#include "profiledef.h"

class CProfileListItem : public QListWidgetItem
{
public:
    CProfileDef* pProfilePointer;

};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    {
    ui->setupUi(this);
    CANNED_PROFILE_COUNT = 0;

    // This loads all the layer information and current settings.
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();

    LoadProfileList();

    // Why is this loaded now?
    pSettingsEditor = new CSettingsEditor();

 //   ui->listWidgetProfiles->setStyleSheet( "QListWidget::item[separator=""true""] { border-bottom: 1px solid black; }" );


    //    ui->listWidgetProfiles->setProperty("separator", true);

   // ui->listWidgetProfiles->item(0)->setCheckState(Qt::Checked);

    connect(ui->listWidgetProfiles, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(profileItemChanged(QListWidgetItem*)));
    connect(ui->listWidgetProfiles, SIGNAL(currentRowChanged(int)), this, SLOT(currentProfileRowChanged(int)));
    connect(ui->listWidgetProfiles, SIGNAL(itemSelectionChanged()), this, SLOT(selectedProfileChanged()));

    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(fileExit(bool)));
    connect(ui->actionHistory, SIGNAL(triggered(bool)), this, SLOT(fileHistory(bool)));

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutVkConfig(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));

    connect(ui->actionCustom_Layer_Paths, SIGNAL(triggered(bool)), this, SLOT(toolsSetCustomPaths(bool)));

    connect(ui->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui->actionSet_Test_Application, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanTestApp(bool)));
//    connect(ui->layerTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(layerTree_itemClicked(QTreeWidgetItem*, int)));


    ///////////////////////////////////////////////
    // Final check - if there are no apps, disable the profiles list
    if(pVulkanConfig->appList.length() == 0) {
        this->setWindowTitle("Vulkan Configurator (Warning, no app list specified)");
        ui->pushButtonAppList->setStyleSheet("QPushButton { color: red;}");
        }
    else {
        this->setWindowTitle("Vulkan Configurator");
        ui->pushButtonAppList->setStyleSheet("QPushButton { color: black;}");
        }
    }

MainWindow::~MainWindow()
    {
    delete ui;
    }


///////////////////////////////////////////////////////////////////////////////
// Load or refresh the list of profiles
void MainWindow::LoadProfileList(void)
    {
    ui->listWidgetProfiles->clear();
    ui->listWidgetProfiles->blockSignals(true);

    // Add canned profiles first
    int nItemCount = 0;
    for(int i = 0; i < pVulkanConfig->profileList.size(); i++) {
        if(!pVulkanConfig->profileList[i]->bContainsReadOnlyFields)
            continue;

        // Add to list
        CProfileListItem *pItem = new CProfileListItem();
        pItem->pProfilePointer = pVulkanConfig->profileList[i];
        pItem->setText(pVulkanConfig->profileList[i]->qsProfileName);
        pItem->setCheckState(Qt::Unchecked);
        ui->listWidgetProfiles->addItem(pItem);
        CANNED_PROFILE_COUNT++;
        }

    // Add the seperator
    QFrame *line;
    line = new QFrame(ui->listWidgetProfiles);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setFrameRect(QRect(90, 8, 10, 20));

    CProfileListItem *pItem = new CProfileListItem();
    pItem->setText("User Defined");
            //, ui->listWidgetProfiles);
    pItem->pProfilePointer = nullptr;
    pItem->setFlags(pItem->flags() & ~Qt::ItemIsEnabled);
    ui->listWidgetProfiles->addItem(pItem);
    ui->listWidgetProfiles->setItemWidget(pItem, line);
    nItemCount++;


    for(int i = 0; i < pVulkanConfig->profileList.size(); i++) {
        if(pVulkanConfig->profileList[i]->bContainsReadOnlyFields)
            continue;

        CProfileListItem *pItem = new CProfileListItem();
        pItem->pProfilePointer = pVulkanConfig->profileList[i];
        pItem->setText(pVulkanConfig->profileList[i]->qsProfileName);
        pItem->setCheckState(Qt::Unchecked);
        ui->listWidgetProfiles->addItem(pItem);
        }

    ui->listWidgetProfiles->blockSignals(false);
    }


void MainWindow::checkAppListState(void)
    {
    // Final check - if there are no apps, disable the profiles list
    if(pVulkanConfig->appList.length() == 0) {
        this->setWindowTitle("Vulkan Configurator (Warning, no app list specified)");
        ui->pushButtonAppList->setStyleSheet("QPushButton { color: red;}");
        }
    else {
        this->setWindowTitle("Vulkan Configurator");
        ui->pushButtonAppList->setStyleSheet("QPushButton { color: black;}");
        }
    }

/////////////////////////////////////////////////////
// Unused flag, just display the about Qt dialog
void MainWindow::aboutVkConfig(bool bChecked)
{
    (void)bChecked;
    dlgAbout dlg(this);
    dlg.exec();

}


void MainWindow::toolsVulkanInfo(bool bChecked)
{
    (void)bChecked;
    dlgVulkanInfo dlg(this);
    dlg.exec();
}
void MainWindow::toolsVulkanInstallation(bool bChecked)
{
    (void)bChecked;
    dlgVulkanAnalysis dlg(this);
    dlg.exec();
}


void MainWindow::toolsVulkanTestApp(bool bChecked)
{
    (void)bChecked;
    dlgLayerOutput dlg(this);
    dlg.exec();
}

void MainWindow::on_pushButtonLaunch_clicked()
    {
    dlgLayerOutput dlg(this);
    dlg.bAutoLaunch = true;
    dlg.exec();
    }


void MainWindow::fileExit(bool bChecked)
{
    (void)bChecked;
    close();
}


void MainWindow::fileHistory(bool bChecked)
{
    (void)bChecked;
    dlgHistory dlg(this);
    dlg.exec();
}


///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonAppList_clicked
/// Edit the list of apps that can be filtered.
void MainWindow::on_pushButtonAppList_clicked(void)
    {
    dlgCreateAssociation dlg(this);
    dlg.exec();
    pVulkanConfig->saveAppList();
    checkAppListState();
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonEditProfile_clicked
/// Just resave the list anytime we go into the editor
void MainWindow::on_pushButtonEditProfile_clicked(void)
    {
    int nSelection = ui->listWidgetProfiles->currentRow();
    if(nSelection == -1) {
        QMessageBox msg;
        msg.setInformativeText(tr("Select a specific profile to clone."));
        msg.setText(tr("Edit profile"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
        }


    // Which profile is selected?
    QListWidgetItem* pItem = ui->listWidgetProfiles->item(nSelection);
    if(pItem != nullptr) {
        CProfileListItem *pProfileItem = dynamic_cast<CProfileListItem*>(pItem);
        if(pProfileItem != nullptr) {
            dlgProfileEditor dlg(this, pProfileItem->pProfilePointer);
            dlg.exec();
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
// Create a new blank profile
void MainWindow::on_pushButtonNewProfile_clicked(void)
    {
    dlgProfileEditor dlg(this, nullptr);
    dlg.exec();
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonCustomPaths_clicked
/// Allow addition or removal of custom layer paths. Afterwards reset the list
/// of loaded layers, but only if something was changed.
void MainWindow::on_pushButtonCustomPaths_clicked()
    {
    dlgCustomPaths dlg(this);
    dlg.exec();
    }

void MainWindow::toolsSetCustomPaths(bool bChecked)
    {
    (void)bChecked;
    on_pushButtonCustomPaths_clicked();
    }


///////////////////////////////////////////////////////////////////////////////
// Check for activating a new profile
void MainWindow::profileItemChanged(QListWidgetItem *item)
    {
    // If an item is checked, well, a few things need to happen.
    if(item->checkState() == Qt::Checked)
        {
        // First, can we check things?
        if(pVulkanConfig->appList.length() == 0) { // Big nope
            item->setCheckState(Qt::Unchecked);
            QMessageBox warning;
            warning.setInformativeText(tr("You cannot activate a profile with an empty applicaton list."));
            warning.setText(tr("App list is empty"));
            warning.setStandardButtons(QMessageBox::Ok);
            warning.exec();
            return;
            }

        // Okay, checks are mutually exclusive (but rado buttons are ugly, so I went this way)
        // Only one profile can actually be checked
        for(int i = 0; i < ui->listWidgetProfiles->count(); i++)
            if(item != ui->listWidgetProfiles->item(i) && i != CANNED_PROFILE_COUNT)
                ui->listWidgetProfiles->item(i)->setCheckState(Qt::Unchecked);


        CProfileListItem *pProfileItem = dynamic_cast<CProfileListItem*>(item);
        if(pProfileItem != nullptr) { // Better to ignore than crash
            pVulkanConfig->pActiveProfile = pProfileItem->pProfilePointer;
 //           if(pVulkanConfig->pActiveProfile != nullptr) // Ditto...
 //               pVulkanConfig->pActiveProfile->MakeMeTheCurrentProfile();
            }

        return;
        }

    // No, I don't actually.
    //printf("I need to do something here\n");
    }


//////////////////////////////////////////////////////////////////////////////
// The primary purpose here is to enable/disable/rename the edit/clone
// button for the highlighted profile
void MainWindow::selectedProfileChanged(void)
    {
    // We need the list item that was selected
    int nRow = ui->listWidgetProfiles->currentRow();
    CProfileListItem *pSelectedItem = dynamic_cast<CProfileListItem *>(ui->listWidgetProfiles->item(nRow));
    if(pSelectedItem == nullptr) {
        ui->listWidgetProfiles->setCurrentRow(-1);
        ui->pushButtonEditProfile->setEnabled(false);
        return; // This should never happen, but if they do, nothing is selected
        }

    ui->pushButtonEditProfile->setEnabled(true);


    if(pSelectedItem->pProfilePointer->bContainsReadOnlyFields)
        ui->pushButtonEditProfile->setText("Clone");
    else
        ui->pushButtonEditProfile->setText("Edit");
    }

///////////////////////////////////////////////////////////////////////////////
// A row has been selected. If there are editable items for the profile
// display them in the lower panel.
void MainWindow::currentProfileRowChanged(int row)
    {
    // We need the list item that was selected
    int nRow = ui->listWidgetProfiles->currentRow();
    CProfileListItem *pSelectedItem = dynamic_cast<CProfileListItem *>(ui->listWidgetProfiles->item(nRow));

    // Only the canned profiles have read only settings
    if(pSelectedItem->pProfilePointer->bContainsReadOnlyFields) {



        }

    printf("Row %d selected\n", row);
    // Display editor for profile
   //   pSettingsEditor->CreateGUI(ui->tabWidget, pVulkanConfig->explicitLayers[row]->layerSettings);
 //   ui->tabWidget->update();

    }
