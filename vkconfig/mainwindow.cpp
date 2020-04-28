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
#include "dlgprofileeditor.h"
#include "dlgcreateassociation.h"
#include "dlghistory.h"
#include "dlgcustompaths.h"
#include "profiledef.h"


static const char *szWelcomeText = "\nWelcome to LunarG Vulkan Configurator. This tool allows configuring the Vulkan "
                                   "Layers to do Vulkan Applicaton Validation, helping to detect application issues.\n\n"
                                   "To start, click on \"Edit List...\" and add the applications you want to configure "
                                   "layers for.";

static const char *szStartText = "\n- Select a \"Configuration\" and \"Activate\" to start "
                                 "applying Vulkan Layers to the selected Vulkan applications.\n\n"
                                 "- All layer configurations can be disabled using the \"Deactivate\" button.";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    {
    ui->setupUi(this);
    CANNED_PROFILE_COUNT = 0;
    pLastSelectedProfileItem = nullptr;

    // This loads all the layer information and current settings.
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();

    ///////////////////////////////////////////////
    checkAppListState();
    if(pVulkanConfig->appList.size() == 0)
        pVulkanConfig->SetCurrentActiveProfile(nullptr); // Just to be sure

    LoadProfileList();

    connect(ui->listWidgetProfiles, SIGNAL(itemSelectionChanged()), this, SLOT(selectedProfileChanged()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(fileExit(bool)));
    connect(ui->actionHistory, SIGNAL(triggered(bool)), this, SLOT(fileHistory(bool)));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutVkConfig(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));

    connect(ui->actionCustom_Layer_Paths, SIGNAL(triggered(bool)), this, SLOT(toolsSetCustomPaths(bool)));

    connect(ui->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui->actionSet_Test_Application, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanTestApp(bool)));
//    connect(ui->layerTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(layerTree_itemClicked(QTreeWidgetItem*, int)));

    // Activate should always be off because nothing is selected on startup ever
    ui->pushButtonActivate->setEnabled(false);
    }

MainWindow::~MainWindow()
    {
    delete ui;
    }


///////////////////////////////////////////////////////////////////////////////
// Load or refresh the list of profiles
void MainWindow::LoadProfileList(void)
    {
    settingsEditor.CleanupGUI(); // Cleanup from last time, just in case
    ui->listWidgetProfiles->blockSignals(true);
    ui->listWidgetProfiles->clear();
    ui->pushButtonRemove->setEnabled(false); // Nothing is selected
    ui->pushButtonEdit->setEnabled(false);

    // Default profiles need the VK_LAYER_KHRONOS_validation layer.
    // If it's not found, we need to disable it.
    bool bSDKAvailable = (nullptr != pVulkanConfig->findLayerNamed("VK_LAYER_KHRONOS_validation"));


    // Add canned profiles first
    int nItemCount = 0;
    for(int i = 0; i < pVulkanConfig->profileList.size(); i++) {
        if(!pVulkanConfig->profileList[i]->bFixedProfile)
            continue;

        // Add to list
        CProfileListItem *pItem = new CProfileListItem();
        pItem->pProfilePointer = pVulkanConfig->profileList[i];
        pItem->setText(pVulkanConfig->profileList[i]->qsProfileName);
        pItem->setToolTip(pVulkanConfig->profileList[i]->qsDescription);

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
    line->setFrameRect(QRect(85, 8, 10, 20));

    CProfileListItem *pItem = new CProfileListItem();
    pItem->setText("User Defined");
            //, ui->listWidgetProfiles);
    pItem->pProfilePointer = nullptr;
    pItem->setFlags(pItem->flags() & ~Qt::ItemIsEnabled);
    ui->listWidgetProfiles->addItem(pItem);
    ui->listWidgetProfiles->setItemWidget(pItem, line);
    nItemCount++;


    for(int i = 0; i < pVulkanConfig->profileList.size(); i++) {
        if(pVulkanConfig->profileList[i]->bFixedProfile)
            continue;

        CProfileListItem *pItem = new CProfileListItem();
        pItem->pProfilePointer = pVulkanConfig->profileList[i];
        pItem->setText(pVulkanConfig->profileList[i]->qsProfileName);
        pItem->setToolTip((pVulkanConfig->profileList[i]->qsDescription));

        ui->listWidgetProfiles->addItem(pItem);
        }

    ui->listWidgetProfiles->blockSignals(false);
    ui->listWidgetProfiles->setSelectionMode(QAbstractItemView::SingleSelection);

    ChangeActiveProfile(pVulkanConfig->GetCurrentActiveProfile());
    }


/////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::setGetStartedText
/// \param szText
/// Set hint text
void MainWindow::setGetStartedText(const char *szText)
    {
    // Create the label, just so that it fills the scroll area
    // this automatically gets deleted whenever the scroll area gets reset
    QLabel*             pLabelGetStarted;
    pLabelGetStarted = new QLabel(ui->scrollArea);
    QSize size = ui->scrollArea->size();
    size.setWidth(size.width() - 10);
    size.setHeight(size.height() - 10);
    pLabelGetStarted->setAlignment(Qt::AlignTop);
    pLabelGetStarted->setWordWrap(true);
    pLabelGetStarted->setMinimumSize(size);
    pLabelGetStarted->setMargin(8);
    ui->scrollArea->setWidget(pLabelGetStarted);
    pLabelGetStarted->show();
    pLabelGetStarted->setText(szText);
    }


void MainWindow::checkAppListState(void)
    {
    // Final check - if there are no apps, disable the profiles list
    if(pVulkanConfig->appList.length() == 0) {
        this->setWindowTitle("Vulkan Control Panel (Warning, no app list specified)");
        ui->pushButtonAppList->setStyleSheet("QPushButton { color: red;}");
        ui->groupBoxEditor->setTitle(tr("Getting Started"));
        ui->groupBoxProfiles->setEnabled(false);
        settingsEditor.CleanupGUI();    // Just in case
        setGetStartedText(szWelcomeText);
        }
    else { // There are apps
        this->setWindowTitle(VKCONFIG_NAME);
        ui->pushButtonAppList->setStyleSheet("QPushButton { color: black;}");

        // But was one of them selected?
        int nSelected = ui->listWidgetProfiles->currentRow();
        if(nSelected == -1) {
            ui->groupBoxProfiles->setEnabled(false);
            ui->groupBoxEditor->setTitle(tr("Getting Started"));
            ui->groupBoxProfiles->setEnabled(true);
            setGetStartedText(szStartText);
            }
        else {
            // An item was selected and active
            ui->groupBoxProfiles->setEnabled(true);
            }
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

    // If we come back and there are no apps in the app list, don't leave any holes
    if(pVulkanConfig->appList.size() == 0 && pVulkanConfig->GetCurrentActiveProfile() != nullptr) {
        pVulkanConfig->SetCurrentActiveProfile(nullptr);
        LoadProfileList();
        }

    checkAppListState();
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonEditProfile_clicked
/// Just resave the list anytime we go into the editor
void MainWindow::on_pushButtonEdit_clicked(void)
    {
    int nSelection = ui->listWidgetProfiles->currentRow();
    Q_ASSERT(nSelection >= 0);

    // I may have made edits
    settingsEditor.CollectSettings();

    // Which profile is selected?
    QListWidgetItem* pItem = ui->listWidgetProfiles->item(nSelection);
    if(pItem != nullptr) {
        CProfileListItem *pProfileItem = dynamic_cast<CProfileListItem*>(pItem);
        if(pProfileItem != nullptr) {
            dlgProfileEditor dlg(this, pProfileItem->pProfilePointer);
            dlg.exec();
            pVulkanConfig->loadAllProfiles(); // Reset
            LoadProfileList();  // Force a reload
            }
        }
    checkAppListState();
    }

////////////////////////////////////////////////////////////////////////////////
// Create a new blank profile
void MainWindow::on_pushButtonNewProfile_clicked(void)
    {
    dlgProfileEditor dlg(this, nullptr);
    dlg.exec();
    LoadProfileList();  // force a reload
    checkAppListState();
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::addCustomPaths
/// Allow addition or removal of custom layer paths. Afterwards reset the list
/// of loaded layers, but only if something was changed.
void MainWindow::addCustomPaths()
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
    checkAppListState();
    }

void MainWindow::toolsSetCustomPaths(bool bChecked)
    {
    (void)bChecked;
    addCustomPaths();
    }



/////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::UpdateActiveDecorations
/// Update "decorations": window caption, (Active) status in list
void MainWindow::ChangeActiveProfile(CProfileDef *pNewProfile)
    {
    // Was there a last profile?
//    const CProfileDef *pLastProfile = pVulkanConfig->GetCurrentActiveProfile();

    // Loop through all the items
    for(int i = 0; i < ui->listWidgetProfiles->count(); i++) {
        CProfileListItem *pItem = dynamic_cast<CProfileListItem *>(ui->listWidgetProfiles->item(i));
        Q_ASSERT(pItem != nullptr);

        CProfileDef *pThisProfile = pItem->pProfilePointer;
        if(pThisProfile) {
            if(pThisProfile != pNewProfile) { // No longer active
                pItem->setText(pThisProfile->qsProfileName);
                continue;
                }
            else {
                QString label = pItem->pProfilePointer->qsProfileName;
                label += tr(" (Activated)");
                pItem->setText(label);
                }
            }
        }

    // A profile is active, make it so...
    if(pNewProfile != nullptr) {
        ui->pushButtonActivate->setEnabled(false);
        ui->pushButtonDisable->setEnabled(true);
        QString title = pNewProfile->qsProfileName;
        title += " - ";
        title += tr(VKCONFIG_NAME);
        setWindowTitle(title);
        }
    else { // No profile is currently active
        setWindowTitle(tr(VKCONFIG_NAME));
        ui->pushButtonDisable->setEnabled(false);

        // Only enable the activeate button if something is selected that can
        // be activated
        ui->pushButtonActivate->setEnabled(ui->listWidgetProfiles->currentItem() != nullptr);
        ui->groupBoxEditor->setEnabled(true); // Turn this back on iff it was off
        }

    // Actually set/disable the profile
    pVulkanConfig->SetCurrentActiveProfile(pNewProfile);
    }

//////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonDisable_clicked
/// Disable the currently active profile
void MainWindow::on_pushButtonDisable_clicked()
    {
    ChangeActiveProfile(nullptr);
    ui->groupBoxEditor->setEnabled(true);
    }

//////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonActivate_clicked
/// Activate the currently selected profile
void MainWindow::on_pushButtonActivate_clicked()
    {
    // Get the current profile and activate it
    int nRow = ui->listWidgetProfiles->currentRow();
    settingsEditor.CollectSettings();
    CProfileListItem *pSelectedItem = dynamic_cast<CProfileListItem *>(ui->listWidgetProfiles->item(nRow));
    Q_ASSERT(pSelectedItem != nullptr);
    ChangeActiveProfile(pSelectedItem->pProfilePointer);

    ui->groupBoxEditor->setEnabled(false); // You can't edit me...
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
            // the override settings.
            if(pVulkanConfig->GetCurrentActiveProfile() == pLastSelectedProfileItem->pProfilePointer)
                pVulkanConfig->SetCurrentActiveProfile(pLastSelectedProfileItem->pProfilePointer);
            }

    // We need the list item that was selected
    int nRow = ui->listWidgetProfiles->currentRow();
    Q_ASSERT(-1);
    CProfileListItem *pSelectedItem = dynamic_cast<CProfileListItem *>(ui->listWidgetProfiles->item(nRow));
    pLastSelectedProfileItem = pSelectedItem;
    if(pSelectedItem == nullptr) {
        ui->listWidgetProfiles->setCurrentRow(-1);
        ui->pushButtonEdit->setEnabled(true);
        ui->pushButtonRemove->setEnabled(false);    // Only the ones you can edit can be deleted
        ui->groupBoxEditor->setEnabled(false);
        ui->groupBoxEditor->setTitle(tr("Khronos Ouput Settings"));
        return; // This should never happen, but if they do, nothing is selected
        }

    // This is the currently active profile, editing is a clone, but this is transparent here
    if(pSelectedItem->pProfilePointer == pVulkanConfig->GetCurrentActiveProfile()) {
        if(pSelectedItem->pProfilePointer->bFixedProfile) {
            ui->pushButtonEdit->setEnabled(false);
            ui->pushButtonRemove->setEnabled(false);
            }

        ui->groupBoxEditor->setEnabled(true);
        QString title = tr("Khronos Ouput Settings");
        title += " - ";
        title += pSelectedItem->pProfilePointer->qsProfileName;
        ui->groupBoxEditor->setTitle(title);

        settingsEditor.CleanupGUI();
        if(pSelectedItem->pProfilePointer->layers.size() > 0)
            settingsEditor.CreateGUI(ui->scrollArea, pSelectedItem->pProfilePointer->layers[0]->layerSettings, true);
        return;
        }

    // Something is selected, so we need to enable the button
    ui->pushButtonEdit->setEnabled(true);
    ui->pushButtonRemove->setEnabled(true);
    ui->pushButtonActivate->setEnabled(true);

    ui->groupBoxEditor->setEnabled(true);
    QString title = tr("Khronos Ouput Settings");
    title += " - ";
    title += pSelectedItem->pProfilePointer->qsProfileName;
    ui->groupBoxEditor->setTitle(title);


    if(pSelectedItem->pProfilePointer->bFixedProfile)
        ui->pushButtonRemove->setEnabled(false);

    // Label the button appropriately, but if a canned profile, we do need to
    // setup the GUI
    if(pSelectedItem->pProfilePointer->bContainsKhronosOutput) {
        settingsEditor.CleanupGUI();
        settingsEditor.CreateGUI(ui->scrollArea, pSelectedItem->pProfilePointer->layers[0]->layerSettings, true);
        }
    else {
        ui->pushButtonRemove->setEnabled(true);    // Only the ones you can edit can be deleted
        settingsEditor.CleanupGUI();
        }
    }


