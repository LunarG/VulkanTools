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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    {
    ui->setupUi(this);
    CANNED_PROFILE_COUNT = 0;
    pLastSelectedProfileItem = nullptr;

    // This loads all the layer information and current settings.
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();

    LoadProfileList();

    connect(ui->listWidgetProfiles, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(profileItemChanged(QListWidgetItem*)));
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
    ui->listWidgetProfiles->blockSignals(true);
    ui->listWidgetProfiles->clear();
    ui->pushButtonRemove->setEnabled(false); // Nothing is selected
    ui->pushButtonEditProfile->setEnabled(false);

    // Default profiles need the VK_LAYER_KHRONOS_validation layer.
    // If it's not found, we need to disable it.
    bool bSDKAvailable = (nullptr != pVulkanConfig->findLayerNamed("VK_LAYER_KHRONOS_validation"));


    // Add canned profiles first
    int nItemCount = 0;
    for(int i = 0; i < pVulkanConfig->profileList.size(); i++) {
        if(!pVulkanConfig->profileList[i]->bContainsReadOnlyFields)
            continue;

        // Add to list
        CProfileListItem *pItem = new CProfileListItem();
        pItem->pProfilePointer = pVulkanConfig->profileList[i];
        pItem->setText(pVulkanConfig->profileList[i]->qsProfileName);
        pItem->setToolTip(pVulkanConfig->profileList[i]->qsDescription);

        if(pVulkanConfig->GetCurrentActiveProfile() == pItem->pProfilePointer)
            pItem->setCheckState(Qt::Checked);
        else
            pItem->setCheckState(Qt::Unchecked);

        if(!bSDKAvailable)
           pItem->setFlags(pItem->flags() & ~Qt::ItemIsEnabled);

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
        pItem->setToolTip((pVulkanConfig->profileList[i]->qsDescription));

        if(pVulkanConfig->GetCurrentActiveProfile() == pItem->pProfilePointer)
            pItem->setCheckState(Qt::Checked);
        else
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
            LoadProfileList();  // Force a reload
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
// Create a new blank profile
void MainWindow::on_pushButtonNewProfile_clicked(void)
    {
    dlgProfileEditor dlg(this, nullptr);
    dlg.exec();
    LoadProfileList();  // force a reload
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


//////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonRemoved_clicked
/// Remove the currently selected user defined profile.
void MainWindow::on_pushButtonRemove_clicked()
    {
    int nSelection = ui->listWidgetProfiles->currentRow();
    Q_ASSERT(nSelection != -1);

    // Which profile is selected?
    QListWidgetItem* pItem = ui->listWidgetProfiles->item(nSelection);
    CProfileListItem *pProfileItem = dynamic_cast<CProfileListItem*>(pItem);
    Q_ASSERT(pProfileItem != nullptr);

    QMessageBox warning;
    warning.setInformativeText(tr("Are you sure you want to delete this profile?"));
    warning.setText(pProfileItem->pProfilePointer->qsProfileName);
    warning.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    warning.setDefaultButton(QMessageBox::No);
    if(QMessageBox::No == warning.exec())
        return; // No harm, no foul

    // Delete the file
    QString completePath = pVulkanConfig->getProfilePath();
    completePath += "/";
    completePath += pProfileItem->pProfilePointer->qsFileName;
    remove(completePath.toUtf8().constData());

    // Reload profiles
    pVulkanConfig->loadAllProfiles();
    this->LoadProfileList();
    }

void MainWindow::toolsSetCustomPaths(bool bChecked)
    {
    (void)bChecked;
    on_pushButtonCustomPaths_clicked();
    }


///////////////////////////////////////////////////////////////////////////////
// Check for activating a new profile. A new one may be checked or the
// current one may be unchecked.
void MainWindow::profileItemChanged(QListWidgetItem *item)
    {
    // Get the currently selected item
    int nRow = ui->listWidgetProfiles->currentRow();
    CProfileListItem *pSelectedItem = dynamic_cast<CProfileListItem *>(ui->listWidgetProfiles->item(nRow));

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
        if(pProfileItem != nullptr) // Better to ignore than crash
            pVulkanConfig->SetCurrentActiveProfile(pProfileItem->pProfilePointer);

        // If the currently selected item was also just checked... we need to
        // disable edting
        if(pSelectedItem != nullptr)
            if(pSelectedItem->pProfilePointer == pProfileItem->pProfilePointer) {
                ui->tabWidget->setEnabled(false);

                if(!pProfileItem->pProfilePointer->bContainsReadOnlyFields)
                    ui->pushButtonEditProfile->setEnabled(false);
                }

        return;
        }

    ///////////////////////////////////////////////////////////////////////////////////////
    // We enforce above that only one item can be checked, but what if we uncheck
    // the only selected item? If so, we end up here and we need to remove the currently
    // active profile, and re-enable the editor.
    pVulkanConfig->SetCurrentActiveProfile(nullptr);
    ui->tabWidget->setEnabled(true);
    ui->pushButtonEditProfile->setEnabled(true);
    }


//////////////////////////////////////////////////////////////////////////////
// The primary purpose here is to enable/disable/rename the edit/clone
// button for the highlighted profile
void MainWindow::selectedProfileChanged(void)
    {
    // We have changed settings? Were there any edits to the last one?
    if(settingsEditor.CollectSettings() == true)
        if(pLastSelectedProfileItem != nullptr) {
            // Write changes to the profile
            pVulkanConfig->SaveProfile(pLastSelectedProfileItem->pProfilePointer);

            // Oh yeah... if we have changed the current profile, we need to also update
            // the overrid settings.
            if(pVulkanConfig->GetCurrentActiveProfile() == pLastSelectedProfileItem->pProfilePointer)
                pVulkanConfig->SetCurrentActiveProfile(pLastSelectedProfileItem->pProfilePointer);
            }

    // We need the list item that was selected
    int nRow = ui->listWidgetProfiles->currentRow();
    CProfileListItem *pSelectedItem = dynamic_cast<CProfileListItem *>(ui->listWidgetProfiles->item(nRow));
    pLastSelectedProfileItem = pSelectedItem;
    if(pSelectedItem == nullptr) {
        ui->listWidgetProfiles->setCurrentRow(-1);
        ui->pushButtonEditProfile->setEnabled(false);
        ui->pushButtonRemove->setEnabled(false);    // Only the ones you can edit can be deleted
        return; // This should never happen, but if they do, nothing is selected
        }

    // Wait... if this is the currently active profile, we do not allow editing,
    // But still show the settings
    if(pSelectedItem->pProfilePointer == pVulkanConfig->GetCurrentActiveProfile()) {
        if(!pSelectedItem->pProfilePointer->bContainsReadOnlyFields)
            ui->pushButtonEditProfile->setEnabled(false);

        ui->tabWidget->setEnabled(false);
        settingsEditor.CleanupGUI();
        settingsEditor.CreateGUI(ui->scrollArea, pSelectedItem->pProfilePointer->layers[0]->layerSettings);
        return;
        }


    // Something is selected, so we need to enable the button
    ui->pushButtonEditProfile->setEnabled(true);
    ui->tabWidget->setEnabled(true);

    // Label the button appropriately, but if a canned profile, we do need to
    // setup the GUI
    if(pSelectedItem->pProfilePointer->bContainsReadOnlyFields) {
        ui->pushButtonEditProfile->setText("Clone Profile");
        ui->pushButtonRemove->setEnabled(false);
        settingsEditor.CleanupGUI();
        settingsEditor.CreateGUI(ui->scrollArea, pSelectedItem->pProfilePointer->layers[0]->layerSettings);
        }
    else {
        ui->pushButtonEditProfile->setText("Edit Profile");
        ui->pushButtonRemove->setEnabled(true);    // Only the ones you can edit can be deleted
        settingsEditor.CleanupGUI();
        }
    }


