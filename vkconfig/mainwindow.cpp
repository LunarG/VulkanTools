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
#include <QComboBox>
#include <QVariant>
#include <QContextMenuEvent>
#include <QFileDialog>

#ifndef _WIN32
#include <unistd.h>
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgabout.h"
#include "dlgvulkananalysis.h"
#include "dlgvulkaninfo.h"
#include "dlgprofileeditor.h"
#include "dlgcreateassociation.h"
#include "dlghistory.h"
#include "dlgcustompaths.h"
#include "profiledef.h"



#define         ACTIVATE_TEXT   "Activate"
#define         DEACTIVATE_TEXT "Deactivate"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    {
    ui->setupUi(this);
    ui->launchTree->installEventFilter(this);
    ui->profileTree->installEventFilter(this);

    pLastSelectedProfileItem = nullptr;
    pVKVia = nullptr;
    pVulkanInfo = nullptr;
    pTestEnv = nullptr;
    pDlgHelp = nullptr;
    pVulkanApp = nullptr;
    pLogFile = nullptr;
    pLaunchAppsCombo = nullptr;
    pLaunchArguments = nullptr;

    // This loads all the layer information and current settings.
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();

    ///////////////////////////////////////////////

    // We need to resetup the new profile for consistency sake.
    QSettings settings;
    QString lastProfile = settings.value(VKCONFIG_KEY_ACTIVEPROFILE, QString("Standard Validation")).toString();
    CProfileDef *pCurrentProfile = pVulkanConfig->FindProfile(lastProfile);
    if(pVulkanConfig->bOverrideActive)
        ChangeActiveProfile(pCurrentProfile);

    LoadProfileList();
    SetupLaunchTree();

    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(fileExit(bool)));
    connect(ui->actionHistory, SIGNAL(triggered(bool)), this, SLOT(fileHistory(bool)));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutVkConfig(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));
    connect(ui->actionHelp, SIGNAL(triggered(bool)), this, SLOT(helpShowHelp(bool)));

    connect(ui->actionCustom_Layer_Paths, SIGNAL(triggered(bool)), this, SLOT(toolsSetCustomPaths(bool)));

    connect(ui->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui->actionSet_Test_Application, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanTestApp(bool)));
    connect(ui->actionLog_API_Dump, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanAPIDump(bool)));
    connect(ui->actionRestore_Default_Configurations, SIGNAL(triggered(bool)), this, SLOT(toolsResetDefaultProfiles(bool)));

    connect(ui->profileTree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(profileItemChanged(QTreeWidgetItem*, int)));
    connect(ui->profileTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(profileItemHighlighted(QTreeWidgetItem *, int)));
    connect(ui->profileTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(profileItemHighlighted(QTreeWidgetItem *, int)));
    connect(ui->profileTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(profileTreeChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

    connect(ui->layerSettingsTree, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(editorExpanded(QTreeWidgetItem*)));

    connect(ui->launchTree, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(launchItemCollapsed(QTreeWidgetItem*)));
    connect(ui->launchTree, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(launchItemExpanded(QTreeWidgetItem* )));

    // Always off if old loader, only on if selected
    if(pVulkanConfig->bHasOldLoader)
        ui->pushButtonAppList->setEnabled(false);
    else
        ui->pushButtonAppList->setEnabled(pVulkanConfig->bApplyOnlyToList);

    if(pVulkanConfig->bOverrideActive) {
        ui->radioOverride->setChecked(true);
        ui->checkBoxApplyList->setEnabled(!pVulkanConfig->bHasOldLoader);
        ui->checkBoxPersistent->setEnabled(true);
        }
    else {
        ui->radioFully->setChecked(true);
        ui->checkBoxApplyList->setEnabled(false);
        ui->checkBoxPersistent->setEnabled(false);
        ui->pushButtonAppList->setEnabled(false);
        }

    ui->checkBoxApplyList->setChecked(pVulkanConfig->bApplyOnlyToList);
    ui->checkBoxPersistent->setChecked(pVulkanConfig->bKeepActiveOnExit);

//    if(pVulkanConfig->qsLogFileWPath.isEmpty())
//        ui->labelLogFile->setText(tr("Log file not set."));
//    else
//        ui->labelLogFile->setText(pVulkanConfig->qsLogFileWPath);

    }

MainWindow::~MainWindow()
    {
    delete pTestEnv;
    delete ui;
    }


///////////////////////////////////////////////////////////////////////////////
// Load or refresh the list of profiles
void MainWindow::LoadProfileList(void)
    {
    settingsEditor.CleanupGUI();            // Cleanup from last time, just in case
    ui->profileTree->blockSignals(true);    // No signals firing off while we do this
    ui->profileTree->clear();

    // Default profiles need the VK_LAYER_KHRONOS_validation layer.
    // If it's not found, we need to disable it.
    bool bKhronosAvailable = (nullptr != pVulkanConfig->FindLayerNamed("VK_LAYER_KHRONOS_validation"));

    // Who is the currently active profile?
    QSettings settings;
    QString activeProfileName = settings.value(VKCONFIG_KEY_ACTIVEPROFILE).toString();

    for(int i = 0; i < pVulkanConfig->profileList.size(); i++) {
        // Add to list
        CProfileListItem *pItem = new CProfileListItem();
        pItem->pProfilePointer = pVulkanConfig->profileList[i];
        ui->profileTree->addTopLevelItem(pItem);
        pItem->setText(1, pVulkanConfig->profileList[i]->qsProfileName);
        pItem->setToolTip(1, pVulkanConfig->profileList[i]->qsDescription);
        pItem->pRadioButton = new QRadioButton();
        pItem->pRadioButton->setText("");
        if(activeProfileName == pVulkanConfig->profileList[i]->qsProfileName)
            pItem->pRadioButton->setChecked(true);

        if(!bKhronosAvailable)
           pItem->setFlags(pItem->flags() & ~Qt::ItemIsEnabled);

        pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);
        ui->profileTree->setItemWidget(pItem, 0, pItem->pRadioButton);
        connect(pItem->pRadioButton, SIGNAL(clicked(bool)), this, SLOT(profileItemClicked(bool)));
        }

    ui->profileTree->blockSignals(false);
    ui->profileTree->resizeColumnToContents(0);

    ChangeActiveProfile(pVulkanConfig->GetCurrentActiveProfile());
    }

//////////////////////////////////////////////////////////
/// \brief MainWindow::on_radioFully_clicked
// No override at all, fully controlled by the application
void MainWindow::on_radioFully_clicked(void)
    {
    ui->checkBoxApplyList->setEnabled(false);
    ui->checkBoxPersistent->setEnabled(false);
    pVulkanConfig->bOverrideActive = false;

    ui->pushButtonAppList->setEnabled(false);

    pVulkanConfig->SaveAppSettings();
    ChangeActiveProfile(nullptr);
    }


//////////////////////////////////////////////////////////
/// \brief MainWindow::GetCheckedItem
/// \return
/// Okay, because we are using custom controls, some of
/// the signaling is not happening as expected. So, we cannot
/// always get an accurate answer to the currently selected
/// item, but we do often need to know what has been checked
/// when an event occurs. This unambigously answers that question.
CProfileListItem* MainWindow::GetCheckedItem(void)
    {
    // Just go through all the top level items
    for(int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        CProfileListItem *pItem = dynamic_cast<CProfileListItem *>(ui->profileTree->topLevelItem(i));

        if(pItem != nullptr)
            if(pItem->pRadioButton->isChecked())
                return pItem;
        }

    return nullptr;
    }


//////////////////////////////////////////////////////////
/// \brief MainWindow::on_radioOverride_clicked
/// Use the active profile as the override
void MainWindow::on_radioOverride_clicked(void)
    {
    ui->checkBoxApplyList->setEnabled(!pVulkanConfig->bHasOldLoader);
    ui->pushButtonAppList->setEnabled(!pVulkanConfig->bHasOldLoader && pVulkanConfig->bApplyOnlyToList);
    ui->checkBoxPersistent->setEnabled(true);
    pVulkanConfig->bOverrideActive = true;
    pVulkanConfig->SaveAppSettings();

    // This just doesn't work. Make a function to look for the radio button checked.
    CProfileListItem *pProfileItem = GetCheckedItem();
    if(pProfileItem == nullptr)
        ChangeActiveProfile(nullptr);
    else
        ChangeActiveProfile(pProfileItem->pProfilePointer);
    }


//////////////////////////////////////////////////////////
void MainWindow::on_checkBoxApplyList_clicked(void)
    {
    pVulkanConfig->bApplyOnlyToList = ui->checkBoxApplyList->isChecked();
    pVulkanConfig->SaveAppSettings();    
    ui->pushButtonAppList->setEnabled(pVulkanConfig->bApplyOnlyToList);
    }

//////////////////////////////////////////////////////////
void MainWindow::on_checkBoxPersistent_clicked(void)
    {
    pVulkanConfig->bKeepActiveOnExit = ui->checkBoxPersistent->isChecked();
    pVulkanConfig->SaveAppSettings();
    }


//////////////////////////////////////////////////////////
void MainWindow::toolsResetDefaultProfiles(bool bChecked)
    {
    (void)bChecked;

    // Let make sure...
    QMessageBox msg;
    msg.setText(tr("This will reset/restore the 5 default layer configurations to their default state. "
                   "If you've made changes to these configurations they will also be lost. Are you sure you want to continue?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    if(msg.exec() == QMessageBox::No)
        return;

    QSettings settings;
    settings.setValue(VKCONFIG_KEY_FIRST_RUN, 1);

    // Now we need to kind of restart everything
    pVulkanConfig->LoadAllProfiles();
    LoadProfileList();
    }


////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::profileItemClicked
/// \param bChecked
/// Thist signal actually comes from the radio button
void MainWindow::profileItemClicked(bool bChecked)
    {
    printf("profileItemClicked\n");
    if(!bChecked) {
        // Anything important going on here?

        }

    // Someone just got checked, they are now the current profile
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    CProfileListItem *pProfileItem = GetCheckedItem();
    if(pProfileItem == nullptr)
        return;

    // Do we go ahead and activate it?
    if(pVulkanConfig->bOverrideActive)
        ChangeActiveProfile(pProfileItem->pProfilePointer);
    }


/////////////////////////////////////////////////////////////////////////////
/// An item has been changed. Check for edit of the items name (profile name)
void MainWindow::profileItemChanged(QTreeWidgetItem *pItem, int nCol)
    {
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    CProfileListItem *pProfileItem = dynamic_cast<CProfileListItem *>(pItem);
    if(pProfileItem == nullptr)
        return;

    if(nCol == 1) { // Profile name
        // We are renaming the file. Just delete the old one and save this
        QString completePath = pVulkanConfig->GetProfilePath();
        completePath += "/";
        completePath += pProfileItem->pProfilePointer->qsFileName;
        remove(completePath.toUtf8().constData());

        pProfileItem->pProfilePointer->qsProfileName = pProfileItem->text(1);
        pProfileItem->pProfilePointer->qsFileName = pProfileItem->text(1) + QString(".json");
        pVulkanConfig->SaveProfile(pProfileItem->pProfilePointer);
        }
    }


//////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::profileItemHighlighted
/// \param pItem
/// \param nCol
/// Just scrolling and selecting different profiles. This needs to activate
/// the editor appropriately.
void MainWindow::profileItemHighlighted(QTreeWidgetItem *pItem, int nCol)
    {
    (void)nCol;
    printf("profileItemHighlighted\n");

    // This pointer will only be valid if it's one of the elements with
    // the radio button
    CProfileListItem *pProfileItem = dynamic_cast<CProfileListItem *>(pItem);
    if(pProfileItem == nullptr)
        return;

//    // We don't care what was unchecked
//    if(!pProfileItem->pRadioButton->isChecked())
//        return;

//    // Save the name of the current profile
//    QSettings settings;
//    settings.setValue(VKCONFIG_KEY_ACTIVEPROFILE, pProfileItem->pProfilePointer->qsProfileName);

//    // Do we go ahead and activate it?
//    if(pVulkanConfig->bOverrideActive)
//        pVulkanConfig->SetCurrentActiveProfile(pProfileItem->pProfilePointer);



    }

//////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::profileTreeChanged
/// \param pCurrent
/// \param pPrevious
/// This gets called with keyboard selections and clicks that do not necessarily
/// result in a radio button change (but it may). So we need to do two checks here, one
/// for the radio button, and one to change the editor/information at lower right.
void MainWindow::profileTreeChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious)
    {
    // If we have made edits to the last profile, we need to save them
    CProfileListItem *pLastItem = dynamic_cast<CProfileListItem *>(pPrevious);
    if(pLastItem)
        pVulkanConfig->SaveProfile(pLastItem->pProfilePointer);

    // This pointer will only be valid if it's one of the elements with
    // the radio button
    CProfileListItem *pProfileItem = dynamic_cast<CProfileListItem*>(pCurrent);
    if(pProfileItem == nullptr)
        return;

    settingsTreeManager.CreateGUI(ui->layerSettingsTree, pProfileItem->pProfilePointer);
    }



////////////////////////////////////////////////////
// Unused flag, just display the about Qt dialog
void MainWindow::aboutVkConfig(bool bChecked)
{
    (void)bChecked;
    dlgAbout dlg(this);
    dlg.exec();

}

//////////////////////////////////////////////////////////
/// \brief MainWindow::toolsVulkanInfo
/// \param bChecked
/// Create the VulkanInfo dialog if it doesn't already
/// exits & show it.
void MainWindow::toolsVulkanInfo(bool bChecked)
    {
    (void)bChecked;

    if(pVulkanInfo == nullptr)
        pVulkanInfo = new dlgVulkanInfo(this);

    pVulkanInfo->RunTool();
    }

//////////////////////////////////////////////////////////
/// \brief MainWindow::toolsVulkanInstallation
/// \param bChecked
/// Create the VulkanTools dialog if it doesn't already
/// exist & show it.
void MainWindow::toolsVulkanInstallation(bool bChecked)
    {
    (void)bChecked;
    if(pVKVia == nullptr)
        pVKVia = new dlgVulkanAnalysis(this);

    pVKVia->RunTool();
    }

/////////////////////////////////////////////////////////////
void MainWindow::toolsVulkanTestApp(bool bChecked)
    {
    (void)bChecked;
    if(pTestEnv == nullptr)
        pTestEnv = new dlgLayerOutput(nullptr);

    pTestEnv->show();
    pTestEnv->raise();
    pTestEnv->setFocus();
    }



//////////////////////////////////////////////////////////////
void MainWindow::toolsVulkanAPIDump(bool bChecked)
    {
    (void)bChecked;

    dlgLayerOutput apiDump(this);

    CProfileDef *pAPIDump = pVulkanConfig->LoadProfile(":/resourcefiles/API dump.json");
    pVulkanConfig->pushProfile(pAPIDump);
    apiDump.bAPIDump = true;

    apiDump.exec();
    pVulkanConfig->popProfile();
    }


//////////////////////////////////////////////////////////////
void MainWindow::fileExit(bool bChecked)
    {
    (void)bChecked;

//    int nRow = ui->listWidgetProfiles->currentRow();
//    if(nRow != -1) {
//        CProfileListItem *pSelectedItem = dynamic_cast<CProfileListItem *>(ui->listWidgetProfiles->item(nRow));

//        // Make sure we capture any changes.
//        if(settingsEditor.CollectSettings())
//            pVulkanConfig->SaveProfile(pSelectedItem->pProfilePointer);
//        }

    close();
    }

////////////////////////////////////////////////////////////////
/// \brief MainWindow::helpShowHelp
/// \param bChecked
/// Show help, which is just a rich text file
void MainWindow::helpShowHelp(bool bChecked)
    {
    (void)bChecked;
    if(pDlgHelp == nullptr)
        pDlgHelp = new dlgHelp(nullptr);

    pDlgHelp->show();
    }

////////////////////////////////////////////////////////////////
/// \brief MainWindow::closeEvent
/// \param event
/// The only thing we need to do here is clear the profile if
/// the user does not want it active.
void MainWindow::closeEvent(QCloseEvent *event)
    {
    if(!pVulkanConfig->bKeepActiveOnExit)
        pVulkanConfig->SetCurrentActiveProfile(nullptr);

    event->accept();
    }

////////////////////////////////////////////////////////////////
/// \brief MainWindow::resizeEvent
/// \param pEvent
/// Resizing needs a little help. Yes please, there has to be
/// a better way of doing this.
void MainWindow::resizeEvent(QResizeEvent *pEvent)
    {
    if(pEvent != nullptr)
        pEvent->accept();
    }

/////////////////////////////////////////////////////////////
void MainWindow::showEvent(QShowEvent *event)
    {
  //  resizeEvent(nullptr); // Fake to get controls to do the right thing

    event->accept();
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
    pVulkanConfig->SaveAppList();
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonEditProfile_clicked
/// Just resave the list anytime we go into the editor
void MainWindow::EditClicked(CProfileListItem *pItem)
    {
    if(pItem == nullptr)
        return;

    // Save current state before we go in
    dlgProfileEditor dlg(this, pItem->pProfilePointer);
    dlg.exec();
    pVulkanConfig->LoadAllProfiles(); // Reset
    LoadProfileList();  // Force a reload
    }

////////////////////////////////////////////////////////////////////////////////
// Create a new blank profile
void MainWindow::on_pushButtonNewProfile_clicked()
    {
    dlgProfileEditor dlg(this, nullptr);
    dlg.exec();
    LoadProfileList();  // force a reload
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::addCustomPaths
/// Allow addition or removal of custom layer paths. Afterwards reset the list
/// of loaded layers, but only if something was changed.
void MainWindow::addCustomPaths()
    {
    dlgCustomPaths dlg(this);
    dlg.exec();
    LoadProfileList();  // Force a reload
    }


//////////////////////////////////////////////////////////////////////////////
/// Remove the currently selected user defined profile.
void MainWindow::RemoveClicked(CProfileListItem *pItem)
    {
    // Let make sure...
    QMessageBox msg;
    msg.setInformativeText(pItem->pProfilePointer->qsProfileName);
    msg.setText(tr("Are you sure you want to remove this configuration?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    if(msg.exec() == QMessageBox::No)
        return;

    // What if this is the active profile? We will go boom boom soon...
    if(pVulkanConfig->GetCurrentActiveProfile() == pItem->pProfilePointer)
        pVulkanConfig->SetCurrentActiveProfile(nullptr);

    // Delete the file
    QString completePath = pVulkanConfig->GetProfilePath();
    completePath += "/";
    completePath += pItem->pProfilePointer->qsFileName;
    remove(completePath.toUtf8().constData());

    // Reload profiles
    pVulkanConfig->LoadAllProfiles();
    LoadProfileList();
    }

/////////////////////////////////////////////////////////////////////////////
void MainWindow::RenameClicked(CProfileListItem *pItem)
    {
    ui->profileTree->editItem(pItem, 1);
    }

/////////////////////////////////////////////////////////////////////////////
// Import a configuration file. File copy followed by a reload.
void MainWindow::ImportClicked(CProfileListItem *pItem)
    {
    (void)pItem; // We don't need this
    QString qsGetIt = QFileDialog::getOpenFileName(this, "Import Configuration File", QDir::homePath(), "*.json");
    if(qsGetIt.isEmpty())
        return;

    QFile input(qsGetIt);
    QString qsFullDestName = pVulkanConfig->qsProfileFilesPath + "/";
    qsFullDestName += QFileInfo(qsGetIt).fileName();
    input.copy(qsFullDestName);
    pVulkanConfig->LoadAllProfiles();
    this->LoadProfileList();
    }

/////////////////////////////////////////////////////////////////////////////
// Export a configuration file. Basically just a file copy
void MainWindow::ExportClicked(CProfileListItem *pItem)
    {
    // Where to put it and what to call it
    QString qsSaveIt = QFileDialog::getSaveFileName(this, "Export Configuration File", QDir::homePath(), "*.json");
    if(qsSaveIt.isEmpty())
        return;

    // Copy away
    QString fullSourceName = pVulkanConfig->qsProfileFilesPath + "/";
    fullSourceName += pItem->pProfilePointer->qsFileName;
    QFile file(fullSourceName);
    file.copy(qsSaveIt);
    }


void MainWindow::toolsSetCustomPaths(bool bChecked)
    {
    (void)bChecked;
    addCustomPaths();
    LoadProfileList();  // Force a reload
    }



/////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::UpdateActiveDecorations
/// Update "decorations": window caption, (Active) status in list
void MainWindow::ChangeActiveProfile(CProfileDef *pNewProfile)
    {
    if(pNewProfile == nullptr || !pVulkanConfig->bOverrideActive) {
        pVulkanConfig->SetCurrentActiveProfile(nullptr);
        setWindowTitle("Vulkan Configurator");
        return;
        }

    QString newCaption = pNewProfile->qsProfileName;
    newCaption += " - Vulkan Configurator ";
    pVulkanConfig->SetCurrentActiveProfile(pNewProfile);
    newCaption += "<VULKAN APPLICATIONS OVERRIDDEN>";

    this->setWindowTitle(newCaption);
    }

void MainWindow::editorExpanded(QTreeWidgetItem *pItem)
    {
    (void)pItem;
    ui->layerSettingsTree->resizeColumnToContents(0);
    }


//////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonActivate_clicked
/// Activate the currently selected profile
void MainWindow::on_pushButtonActivate_clicked()
    {
//    // Get the current profile and activate it
//    int nRow = ui->listWidgetProfiles->currentRow();
//    CProfileListItem *pSelectedItem = dynamic_cast<CProfileListItem *>(ui->listWidgetProfiles->item(nRow));

//    // Make sure we capture any changes.
//    if(settingsEditor.CollectSettings())
//        pVulkanConfig->SaveProfile(pSelectedItem->pProfilePointer);

//    // If something is active, we are deactivating it
//    if(pVulkanConfig->GetCurrentActiveProfile() != nullptr) {
//        pVulkanConfig->SetCurrentActiveProfile(nullptr);

//        // Keep the current row selection if one was set
//        LoadProfileList(); // Reload to clear activate status
//        if(nRow == -1) {
//            updateGetStartedStatus(szStartTextRed);
//            }
//        else {
//            ui->listWidgetProfiles->setCurrentRow(nRow);
//            selectedProfileChanged();
//            }
//        }
//    else
//        { // We are activating a profile
//        Q_ASSERT(pSelectedItem->pProfilePointer != nullptr);
//        ChangeActiveProfile(pSelectedItem->pProfilePointer);
//        }

//    updateActivateButtonState();
//    if(pVulkanConfig->GetCurrentActiveProfile() == nullptr)
//        return;

//    QSettings settings;
//    if(settings.value("VKCONFIG_SETTING_RESTART_WARNING_IGNORE").toBool() == false) {
//        QMessageBox alert(this);
//        alert.setText("Any running Vulkan applications must be restarted before the new layer overrides can take effect.");
//        QCheckBox *pDoNotShow = new QCheckBox();
//        pDoNotShow->setText("Do not show again");
//        alert.setCheckBox(pDoNotShow);
//        alert.exec();
//        if(pDoNotShow->isChecked())
//            settings.setValue("VKCONFIG_SETTING_RESTART_WARNING_IGNORE", true);
//        }

    }

//////////////////////////////////////////////////////////////////////////////
// The primary purpose here is to enable/disable/rename the edit/clone
// button for the highlighted profile
void MainWindow::selectedProfileChanged(void)
    {
////    updateActivateButtonState();
////    ui->pushButtonEdit->setEnabled(true);   // All profiles can be edited

////    // We have changed settings? Were there any edits to the last one?
////    if(settingsEditor.CollectSettings() == true)
////        if(pLastSelectedProfileItem != nullptr) {
////            // Write changes to the profile
////            pVulkanConfig->SaveProfile(pLastSelectedProfileItem->pProfilePointer);

////            // Oh yeah... if we have changed the current profile, we need to also update
////            // the override settings.
////            if(pVulkanConfig->GetCurrentActiveProfile() == pLastSelectedProfileItem->pProfilePointer)
////                pVulkanConfig->SetCurrentActiveProfile(pLastSelectedProfileItem->pProfilePointer);
////            }

////    // We need the list item that was selected
////    int nRow = ui->listWidgetProfiles->currentRow();
////    Q_ASSERT(-1);
////    CProfileListItem *pSelectedItem = dynamic_cast<CProfileListItem *>(ui->listWidgetProfiles->item(nRow));
////    pLastSelectedProfileItem = pSelectedItem;
////    if(pSelectedItem == nullptr) {
////        ui->listWidgetProfiles->setCurrentRow(-1);
////        ui->pushButtonEdit->setEnabled(true);
////        ui->pushButtonRemove->setEnabled(false);    // Only the ones you can edit can be deleted
////        ui->groupBoxEditor->setTitle(tr("Khronos Ouput Settings"));
////        return; // This should never happen, but if they do, nothing is selected
////        }


////    // We might need the Khronos layer
////    CLayerFile* pKhronos = pSelectedItem->pProfilePointer->GetKhronosLayer();

////    // This is the currently active profile
////    if(pSelectedItem->pProfilePointer == pVulkanConfig->GetCurrentActiveProfile()) {
////        ui->pushButtonRemove->setEnabled(!pSelectedItem->pProfilePointer->bFixedProfile);

////        QString title = tr("Khronos Ouput Settings");
////        title += " - ";
////        title += pSelectedItem->pProfilePointer->qsProfileName;
////        ui->groupBoxEditor->setTitle(title);

////        settingsEditor.CleanupGUI();
////        Q_ASSERT(pKhronos != nullptr);
////        if(pKhronos != nullptr)
////            settingsEditor.CreateGUI(ui->scrollArea, pKhronos->layerSettings, EDITOR_TYPE_KHRONOS,
////                        pSelectedItem->pProfilePointer->qsDescription);

//        return;
//        }

//    // Something is selected, so we need to enable the button
//    ui->pushButtonRemove->setEnabled(true);

//    QString title = tr("Khronos Ouput Settings");
//    title += " - ";
//    title += pSelectedItem->pProfilePointer->qsProfileName;
//    ui->groupBoxEditor->setTitle(title);


//    if(pSelectedItem->pProfilePointer->bFixedProfile)
//        ui->pushButtonRemove->setEnabled(false);

//    // Label the button appropriately, but if a canned profile, we do need to
//    // setup the GUI
//    if(pKhronos != nullptr) {
//        settingsEditor.CleanupGUI();
//        Q_ASSERT(pKhronos != nullptr);
//        settingsEditor.CreateGUI(ui->scrollArea, pKhronos->layerSettings, EDITOR_TYPE_KHRONOS,
//                pSelectedItem->pProfilePointer->qsDescription);
//        }
//    else {
//        ui->pushButtonRemove->setEnabled(true);    // Only the ones you can edit can be deleted
//        settingsEditor.CleanupGUI();
//        QVector <TLayerSettings *> dummy;
//        settingsEditor.CreateGUI(ui->scrollArea, dummy, EDITOR_TYPE_GENERAL, pSelectedItem->pProfilePointer->qsDescription);
//        }
    }




///////////////////////////////////////////////////////////////////
/// \brief MainWindow::ResetLaunchOptions
/// Reload controls for launch control
void MainWindow::ResetLaunchOptions(void)
    {
    // Reload launch apps selections
    int nFoundLast = -1;
    pLaunchAppsCombo->blockSignals(true);
    pLaunchAppsCombo->clear();
    for(int i = 0; i < pVulkanConfig->appList.size(); i++) {
        pLaunchAppsCombo->addItem(pVulkanConfig->appList[i]->qsAppNameWithPath);
        if(pVulkanConfig->appList[i]->qsAppNameWithPath == pVulkanConfig->qsLaunchApplicationWPath)
            nFoundLast = i;
        }

    if(nFoundLast < 0)
        nFoundLast = 0;

    pLaunchAppsCombo->setCurrentIndex(nFoundLast);

    // Reset working folder and command line choices
    pLaunchArguments->setText(pVulkanConfig->appList[nFoundLast]->qsArguments);
    pLaunchWorkingFolder->setText(pVulkanConfig->appList[nFoundLast]->qsWorkingFolder);
    pLaunchAppsCombo->blockSignals(false);
    }


///////////////////////////////////////////////////////////////////
/// \brief MainWindow::SetupLaunchTree
/// Launch and log area
void MainWindow::SetupLaunchTree(void)
    {
    /////////////////////////////////////////////////////////////////
    // Executable
    QTreeWidgetItem *pLauncherParent = new QTreeWidgetItem();
    pLauncherParent->setText(0, "Executable Path");
    ui->launchTree->addTopLevelItem(pLauncherParent);

    pLaunchAppsCombo = new QComboBox();
    ui->launchTree->setItemWidget(pLauncherParent, 1, pLaunchAppsCombo);

    pLuanchAppBrowseButton = new QPushButton();
    pLuanchAppBrowseButton->setText("...");
    pLuanchAppBrowseButton->setMinimumWidth(32);
    ui->launchTree->setItemWidget(pLauncherParent, 2, pLuanchAppBrowseButton);
    connect(pLaunchAppsCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(launchItemChanged(int)));
    connect(pLuanchAppBrowseButton, SIGNAL(clicked()), this, SLOT(launchAddProgram()));


    //////////////////////////////////////////////////////////////////
    // Working folder
    QTreeWidgetItem *pLauncherFolder = new QTreeWidgetItem();
    pLauncherFolder->setText(0, "Working directory");
    pLauncherParent->addChild(pLauncherFolder);

    pLaunchWorkingFolder = new QLineEdit();
    ui->launchTree->setItemWidget(pLauncherFolder, 1, pLaunchWorkingFolder);
    pLaunchWorkingFolder->setReadOnly(true);

    // Comming soon
//    pLaunchWorkingFolderButton = new QPushButton();
//    pLaunchWorkingFolderButton->setText("...");
//    pLaunchWorkingFolderButton->setMinimumWidth(32);
//    ui->launchTree->setItemWidget(pLauncherFolder, 2, pLaunchWorkingFolderButton);

    //////////////////////////////////////////////////////////////////
    // Command line arguments
    QTreeWidgetItem *pLauncherCMD = new QTreeWidgetItem();
    pLauncherCMD->setText(0, "Command-line Arguments");
    pLauncherParent->addChild(pLauncherCMD);

    pLaunchArguments = new QLineEdit();
    ui->launchTree->setItemWidget(pLauncherCMD, 1, pLaunchArguments);

    // Comming soon
//    pButton = new QPushButton();
//    pButton->setText("...");
//    pButton->setMinimumWidth(32);
//    ui->launchTree->setItemWidget(pLauncherCMD, 2, pButton);

    //////////////////////////////////////////////////////////////////
    // LOG FILE
    QTreeWidgetItem *pLauncherLogFile = new QTreeWidgetItem();
    pLauncherLogFile->setText(0, "Log File");
    pLauncherParent->addChild(pLauncherLogFile);

    pLaunchLogFile = new QLineEdit();
    ui->launchTree->setItemWidget(pLauncherLogFile, 1, pLaunchLogFile);
    pLaunchLogFile->setText(pVulkanConfig->qsLogFileWPath);

    pLaunchLogFilebutton = new QPushButton();
    pLaunchLogFilebutton->setText("...");
    pLaunchLogFilebutton->setMinimumWidth(32);
    ui->launchTree->setItemWidget(pLauncherLogFile, 2, pLaunchLogFilebutton);
    connect(pLaunchLogFilebutton, SIGNAL(clicked()), this, SLOT(launchSetLogFile()));

    //////////////////////////////////////////////////////////////////
    QRect rect = ui->launchTree->visualItemRect(pLauncherParent);
    ui->launchTree->setMinimumHeight((rect.height() * 4)+6);


    rect = ui->launchTree->rect();
    ui->launchTree->setColumnWidth(0, 210);
    ui->launchTree->setColumnWidth(1, rect.width() - 210 - 25-16);
    ui->launchTree->setColumnWidth(2, 25);
    ui->launchTree->expandItem(pLauncherParent);
    ui->launchTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->launchTree->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->launchTree->resizeColumnToContents(1);
    ResetLaunchOptions();
    }

////////////////////////////////////////////////////////////////////
// Expanding the tree also grows the tree to match
void MainWindow::launchItemExpanded(QTreeWidgetItem* pItem)
    {
    QRect rect = ui->launchTree->visualItemRect(pItem);
    ui->launchTree->setMinimumHeight((rect.height() * 4)+6);
    ui->launchTree->setMaximumHeight((rect.height() * 4)+6);
    }


////////////////////////////////////////////////////////////////////
// Collapsing the tree also shrinks the tree to match and show only
// the first line
void MainWindow::launchItemCollapsed(QTreeWidgetItem* pItem)
    {
    QRect rect = ui->launchTree->visualItemRect(pItem);
    ui->launchTree->setMinimumHeight(rect.height()+6);
    ui->launchTree->setMaximumHeight(rect.height()+6);
    }

/////////////////////////////////////////////////////////////////////
void MainWindow::launchAddProgram(void)
    {
    dlgCreateAssociation dlg(this);
    dlg.exec();
    pVulkanConfig->SaveAppList();

    ResetLaunchOptions();
    }

////////////////////////////////////////////////////////////////////
void MainWindow::launchSetLogFile(void)
    {
    pVulkanConfig->qsLogFileWPath = QFileDialog::getSaveFileName(this,
        tr("Set Log File To..."),
        ".", tr("Log text(*.txt)"));

    if(pVulkanConfig->qsLogFileWPath.isEmpty())
        pLaunchLogFile->setText("");
    else
        pLaunchLogFile->setText(pVulkanConfig->qsLogFileWPath);

    pVulkanConfig->SaveAppSettings();
    }

////////////////////////////////////////////////////////////////////
/// Launch app change
void MainWindow::launchItemChanged(int nIndex)
    {
    if(nIndex >= pVulkanConfig->appList.size())
        return;

    if(nIndex < 0)
        return;

    pLaunchArguments->setText(pVulkanConfig->appList[nIndex]->qsArguments);
    pLaunchWorkingFolder->setText(pVulkanConfig->appList[nIndex]->qsWorkingFolder);

    // Update last used settings too. I'm saving this way instead of an index, because
    // in the future there will be more than one set of working directories and argument lists
    pVulkanConfig->qsLaunchApplicationWPath = pVulkanConfig->appList[nIndex]->qsAppNameWithPath;
    pVulkanConfig->qsLaunchApplicationArgs = pVulkanConfig->appList[nIndex]->qsArguments;
    pVulkanConfig->qsLaunchApplicationWorkingDir = pVulkanConfig->appList[nIndex]->qsWorkingFolder;
    pVulkanConfig->SaveAppSettings();
    }

//////////////////////////////////////////////////////////////////////
// Clear the browser window
void MainWindow::on_pushButtonClearLog_clicked(void)
    {
    ui->logBrowser->clear();
    }

//////////////////////////////////////////////////////////////////////
// Open an existing log file and display it's contents
void MainWindow::on_pushButtonOpenLog_clicked(void)
    {
    QFile file(pVulkanConfig->qsLogFileWPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString string = file.readAll();
    file.close();
    ui->logBrowser->setPlainText(string);
    }

//////////////////////////////////////////////////////////////////////
bool MainWindow::eventFilter(QObject *target, QEvent *event)
    {
    // Launch tree does some fancy resizing and since it's down in
    // layouts and splitters, we can't just relay on the resize method
    // of this window.
    if(target == ui->launchTree) {
        if(event->type() == QEvent::Resize) {
            QRect rect = ui->launchTree->rect();
            ui->launchTree->setColumnWidth(0, 210);
            ui->launchTree->setColumnWidth(1, rect.width() - 210 - 25-16);
            ui->launchTree->setColumnWidth(2, 25);
            }
        }


    // Context menus for layer configuration files
    if(target == ui->profileTree) {
        QContextMenuEvent* pRightClick = dynamic_cast<QContextMenuEvent*>(event);
        if(pRightClick && event->type() == QEvent::ContextMenu)
            {
            // Which item were we over?
            QTreeWidgetItem *pProfileItem = ui->profileTree->itemAt(pRightClick->pos());
            CProfileListItem *pItem = dynamic_cast<CProfileListItem *>(pProfileItem);
            if(pItem != nullptr) {
                //Create context menu here
                QMenu menu(ui->profileTree);
                QAction *pNewAction = new QAction("New");
                menu.addAction(pNewAction);

                QAction *pEditAction = new QAction("Edit");
                menu.addAction(pEditAction);

                QAction *pRemoveAction = new QAction("Remove");
                menu.addAction(pRemoveAction);
                menu.addSeparator();

                QAction *pRenameAction = new QAction("Rename");
                menu.addAction(pRenameAction);
                menu.addSeparator();

                QAction *pExportAction = new QAction("Export...");
                menu.addAction(pExportAction);

                QAction *pImportAction = new QAction("Import...");
                menu.addAction(pImportAction);

                QPoint  point(pRightClick->globalX(), pRightClick->globalY());
                QAction *pAction = menu.exec(point);

                // Pointer compares made me throw up in my mouth at least a little
                // less than doing a full string compare. Setting up signal/slot for
                // all of these just seemed ridiculous. Every problem is not a nail,
                // put the hammer away....
                // New Profile...
                if(pAction == pNewAction) {
                    on_pushButtonNewProfile_clicked();
                    return true;
                    }

                // Edit this profile....
                if(pAction == pEditAction) {
                    EditClicked(pItem);
                    return true;
                    }

                // Remove this profile....
                if(pAction == pRemoveAction) {
                    RemoveClicked(pItem);
                    return true;
                    }

                // Rename this profile...
                if(pAction == pRenameAction) {
                    RenameClicked(pItem);
                    return true;
                    }

                // Export his profile (copy the .json)
                if(pAction == pExportAction) {
                    ExportClicked(pItem);
                    return true;
                    }

                // Import a profile (copy a json)
                if(pAction == pImportAction) {
                    ImportClicked(pItem);
                    return true;
                    }
                }

            // Do not pass on
            return true;
            }
        }

    // Pass it on
    return false;
    }

///////////////////////////////////////////////////////////////////////////////////
/// Launch the app and monitor it's stdout to get layer output.
/// stdout is buffered by default, so in order to see realtime output it must
/// be flushed. Either of the following in the other app will do.
/// > fflush(stdout);    // Flush now
/// setlinebuf(stdout);  // always flush at the end of a line
///
/// The layers are automtically flushed, so they should show up as they
/// generated. One note... any other stdout generated by the monitored
/// application will also be captured.
///
/// If logging is enbabled (by setting a logging file), then the log file
/// is also opened.
void MainWindow::on_pushButtonLaunch_clicked(void)
    {
    // Are we already monitoring a running app? If so, terminate it
    if(pVulkanApp != nullptr) {
        pVulkanApp->terminate();
        return;
        }

    // Is there an app selected?
    int nIndex = pLaunchAppsCombo->currentIndex();
    if(nIndex < 0) {
        QMessageBox msg;
        msg.setInformativeText(tr("No test application has been specified."));
        msg.setText(tr("No Application Selected"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
        }

    // Display warning first
    QSettings settings;
    if(settings.value("VKCONFIG_HIDE_RESTART_WARNING").toBool())
        return;

    QMessageBox alert(this);
    alert.setText("Vulkan Layers are configured when creating a Vulkan instance which\n"
                  "typically happens at application start.\n\n"
                  "For Vulkan Layer overrides to take effect, running Vulkan applications\n"
                  "should be restarted.");
    QCheckBox *pCheckBox = new QCheckBox();
    pCheckBox->setText(DONT_SHOW_AGAIN_MESSAGE);
    alert.setWindowTitle("Runnng Vulkan Applications must be restarted");
    alert.setCheckBox(pCheckBox);
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
    if(pCheckBox->isChecked())
        settings.setValue("VKCONFIG_HIDE_RESTART_WARNING", true);


    // Launch the test application
    pVulkanApp = new QProcess(this);
    connect(pVulkanApp, SIGNAL(readyReadStandardOutput()), this,
                 SLOT(standardOutputAvailable()));

    connect(pVulkanApp, SIGNAL(readyReadStandardError()), this,
                SLOT(errorOutputAvailable()));

    connect(pVulkanApp, SIGNAL(finished(int, QProcess::ExitStatus)),
                 this, SLOT(processClosed(int, QProcess::ExitStatus)));

     pVulkanApp->setProgram(pVulkanConfig->appList[nIndex]->qsAppNameWithPath);
     pVulkanConfig->qsLaunchApplicationWorkingDir = pVulkanConfig->appList[nIndex]->qsWorkingFolder;
     pVulkanApp->setWorkingDirectory(pVulkanConfig->appList[nIndex]->qsWorkingFolder);

     if(!pVulkanConfig->appList[nIndex]->qsArguments.isEmpty()) {
         pVulkanApp->setArguments(QStringList() << pVulkanConfig->appList[nIndex]->qsArguments);
         pVulkanConfig->qsLaunchApplicationArgs = pVulkanConfig->appList[nIndex]->qsArguments;
        }

     // Some of these may have changed
     pVulkanConfig->SaveAppSettings();

     pVulkanApp->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
     pVulkanApp->setProcessChannelMode(QProcess::MergedChannels);
     pVulkanApp->closeWriteChannel();
     ui->pushButtonLaunch->setText(tr("Terminate"));

     // No log file is set, just bail
     if(pLaunchLogFile->text() != pVulkanConfig->qsLogFileWPath) {
        pVulkanConfig->qsLogFileWPath = pLaunchLogFile->text();
        pVulkanConfig->SaveAppSettings();
        }

     if(pVulkanConfig->qsLogFileWPath.isEmpty())
        return;

     // This should never happen... but things that should never happen do in
     // fact happen... so just a sanity check.
     if(pLogFile != nullptr) {
         pLogFile->close();
         pLogFile = nullptr;
         }

     if(ui->checkBoxClearOnLaunch->isChecked())
         ui->logBrowser->clear();

     // Start logging
     pLogFile = new QFile(pVulkanConfig->qsLogFileWPath);

     // Open and append, or open and truncate?
     QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
     if(!ui->checkBoxClearOnLaunch->isChecked())
         mode |= QIODevice::Append;

     if(!pVulkanConfig->qsLogFileWPath.isEmpty()) {
         if (!pLogFile->open(mode)) {
            QMessageBox err;
            err.setText(tr("Warning: Cannot open log file"));
            err.exec();
            delete pLogFile;
            pLogFile = nullptr;
            }
        }


    // We are logging, let's add that we've launched a new application
    QString out;
    out.asprintf("Starting Vulkan Application: %s\n", pVulkanConfig->qsLaunchApplicationWPath.toUtf8().constData());
    pLogFile->write(out.toUtf8().constData(), out.length());
    out.asprintf("Working folder: %s\n", pVulkanConfig->qsLaunchApplicationWorkingDir.toUtf8().constData());
    pLogFile->write(out.toUtf8().constData(), out.length());
    out.asprintf("Command line arguments: %s\n", pVulkanConfig->qsLaunchApplicationArgs.toUtf8().constData());
    pLogFile->write(out.toUtf8().constData(), out.length());
    }



/////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::processClosed
/// \param exitCode
/// \param status
/// The process we are following is closed. We don't actually care about the
/// exit status/code, we just need to know to destroy the QProcess object
/// and set it back to nullptr so that we know we can launch a new app.
/// Also, if we are logging, it's time to close the log file.
void MainWindow::processClosed(int exitCode, QProcess::ExitStatus status)
    {
    (void)exitCode;
    (void)status;

    // Not likely, but better to be sure...
    if(pVulkanApp == nullptr)
        return;

    disconnect(pVulkanApp, SIGNAL(finished(int, QProcess::ExitStatus)),
                this, SLOT(processClosed(int, QProcess::ExitStatus)));

    disconnect(pVulkanApp, SIGNAL(readyReadStandardError()), this,
                SLOT(errorOutputAvailable()));

    disconnect(pVulkanApp, SIGNAL(readyReadStandardOutput()), this,
                SLOT(standardOutputAvailable()));

    ui->pushButtonLaunch->setText(tr("Launch"));

    if(pLogFile) {
        pLogFile->close();
        delete pLogFile;
        pLogFile = nullptr;
        }

    delete pVulkanApp;
    pVulkanApp = nullptr;
    }


////////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::standardOutputAvailable
/// This signal get's raised whenever the spawned Vulkan appliction writes
/// to stdout and there is data to be read. The layers flush after all stdout
/// writes, so we should see layer output here in realtime, as we just read
/// the string and append it to the text browser.
/// If a log file is open, we also write the output to the log.
void MainWindow::standardOutputAvailable(void)
    {
    if(pVulkanApp == nullptr)
        return;

    QString inFromApp = pVulkanApp->readAllStandardOutput();
    ui->logBrowser->append(inFromApp);

    // Are we logging?
    if(pLogFile)
        pLogFile->write(inFromApp.toUtf8().constData(), inFromApp.length());
    }

///////////////////////////////////////////////////////////////////////////////
void MainWindow::errorOutputAvailable(void)
    {
    if(pVulkanApp == nullptr)
        return;

    QString inFromApp = pVulkanApp->readAllStandardError();
    ui->logBrowser->append(inFromApp);

    // Are we logging?
    if(pLogFile)
        pLogFile->write(inFromApp.toUtf8().constData(), inFromApp.length());
    }

