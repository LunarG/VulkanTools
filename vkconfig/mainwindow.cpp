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


/*
static const char *szWelcomeText = "<font color='red'>\nWelcome to LunarG Vulkan Configurator. This tool allows configuring Vulkan "
                                   "Layers.\n\n"
                                   "<br><br>To start, click on \"Edit List...\" and add the applications you want to configure "
                                   "layers for.</br></br>";

static const char *szStartText = "- Select a \"Configuration\" and \"Activate\" to start "
                                 "applying Vulkan Layers to the selected Vulkan application list.\n\n"
                                 "-Activated Configurations will remain active after closing the Vulkan Configurator.\n\n"
                                 "-Vulkan Configurator effects are fully discarded using the \"Deactivate\" button.\n\n"
                                 "-An activated layer configuration and its settings will be taken into account only when restarting the application.";

// Yes, for some reason when I use the font, I have to manually add the breaks.
// Probably because it doesn't normally use the CSS, but once used, you have to use it.
static const char *szStartTextRed = "<font color='red'>\n- Select a \"Configuration\" and \"Activate\" to start "
                                 "applying Vulkan Layers to the selected Vulkan application list.\n\n"
                                 "<br><br>-Activated Configurations will remain active after closing the Vulkan Configurator.\n\n</br></br>"
                                 "<br><br>-Vulkan Configurator effects are fully discarded using the \"Deactivate\" button.\n\n</br></br>"
                                 "<br><br>-An activated layer configuration and its settings will be taken into account only when restarting the application.</br></br>";
*/
#define         ACTIVATE_TEXT   "Activate"
#define         DEACTIVATE_TEXT "Deactivate"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    {
    ui->setupUi(this);
    ui->launchTree->installEventFilter(this);

    CANNED_PROFILE_COUNT = 0;
    pLastSelectedProfileItem = nullptr;
    pVKVia = nullptr;
    pVulkanInfo = nullptr;
    pTestEnv = nullptr;
    pDlgHelp = nullptr;


    // This loads all the layer information and current settings.
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();

    ///////////////////////////////////////////////
    CheckAppListState();

    // We need to resetup the new profile for consistency sake.
    QSettings settings;
    QString lastProfile = settings.value(VKCONFIG_KEY_ACTIVEPROFILE).toString();
    CProfileDef *pCurrentProfile = pVulkanConfig->FindProfile(lastProfile);
    if(pVulkanConfig->bOverrideActive)
        ChangeActiveProfile(pCurrentProfile);

    LoadProfileList();
    SetupLaunchTree();

//    connect(ui->listWidgetProfiles, SIGNAL(itemSelectionChanged()), this, SLOT(selectedProfileChanged()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(fileExit(bool)));
    connect(ui->actionHistory, SIGNAL(triggered(bool)), this, SLOT(fileHistory(bool)));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutVkConfig(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));
    connect(ui->actionHelp, SIGNAL(triggered(bool)), this, SLOT(helpShowHelp(bool)));

    connect(ui->actionCustom_Layer_Paths, SIGNAL(triggered(bool)), this, SLOT(toolsSetCustomPaths(bool)));

    connect(ui->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui->actionSet_Test_Application, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanTestApp(bool)));
    connect(ui->actionLog_API_Dump, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanAPIDump(bool)));

    connect(ui->profileTree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(profileItemChanged(QTreeWidgetItem*, int)));
    connect(ui->profileTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(profileItemHighlighted(QTreeWidgetItem *, int)));
    connect(ui->profileTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(profileItemHighlighted(QTreeWidgetItem *, int)));
    connect(ui->profileTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(profileTreeChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

    if(pVulkanConfig->bOverrideActive) {
        ui->radioOverride->setChecked(true);
        ui->checkBoxApplyList->setEnabled(true);
        ui->checkBoxPersistent->setEnabled(true);
        }
    else {
        ui->radioFully->setChecked(true);
        ui->checkBoxApplyList->setEnabled(false);
        ui->checkBoxPersistent->setEnabled(false);
        }

    ui->checkBoxApplyList->setChecked(pVulkanConfig->bApplyOnlyToList);
    ui->checkBoxPersistent->setChecked(pVulkanConfig->bKeepActiveOnExit);
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

    // Add canned profiles first
    QTreeWidgetItem *pCannedProfiles = new QTreeWidgetItem();
    pCannedProfiles->setText(0, tr("Built-In Layer Configurations"));
    ui->profileTree->addTopLevelItem(pCannedProfiles);

    for(int i = 0; i < pVulkanConfig->profileList.size(); i++) {
        if(!pVulkanConfig->profileList[i]->bFixedProfile)
            continue;

        // Add to list
        CProfileListItem *pItem = new CProfileListItem();
        pItem->pProfilePointer = pVulkanConfig->profileList[i];
        pCannedProfiles->addChild(pItem);
        pItem->setText(0, "");
        pItem->setToolTip(0, pVulkanConfig->profileList[i]->qsDescription);
        pItem->pRadioButton = new QRadioButton();
        pItem->pRadioButton->setText(pVulkanConfig->profileList[i]->qsProfileName);
        if(activeProfileName == pVulkanConfig->profileList[i]->qsProfileName)
            pItem->pRadioButton->setChecked(true);

        if(!bKhronosAvailable)
           pItem->setFlags(pItem->flags() & ~Qt::ItemIsEnabled);

        ui->profileTree->setItemWidget(pItem, 0, pItem->pRadioButton);
        connect(pItem->pRadioButton, SIGNAL(clicked(bool)), this, SLOT(profileItemClicked(bool)));
        CANNED_PROFILE_COUNT++;
        }

    // Add the user defined
    QTreeWidgetItem *pUserDefinedProfiles = new QTreeWidgetItem();
    pUserDefinedProfiles->setText(0, tr("User-Defined Layer Configurations"));
    ui->profileTree->addTopLevelItem(pUserDefinedProfiles);


    for(int i = 0; i < pVulkanConfig->profileList.size(); i++) {
        if(pVulkanConfig->profileList[i]->bFixedProfile)
            continue;

        CProfileListItem *pItem = new CProfileListItem();
        pItem->pProfilePointer = pVulkanConfig->profileList[i];
        pUserDefinedProfiles->addChild(pItem);
        pItem->setText(0, "");
        pItem->setToolTip(0, pVulkanConfig->profileList[i]->qsDescription);
        pItem->pRadioButton = new QRadioButton();
        pItem->pRadioButton->setText(pVulkanConfig->profileList[i]->qsProfileName);

        if(activeProfileName == pVulkanConfig->profileList[i]->qsProfileName)
            pItem->pRadioButton->setChecked(true);

        ui->profileTree->setItemWidget(pItem, 0, pItem->pRadioButton);
        connect(pItem->pRadioButton, SIGNAL(clicked(bool)), this, SLOT(profileItemClicked(bool)));
        }

    ui->profileTree->expandItem(pCannedProfiles);
    ui->profileTree->expandItem(pUserDefinedProfiles);

    ui->profileTree->blockSignals(false);

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
    // We know there are only two top level items, but
    // just in case we add more later
    for(int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        QTreeWidgetItem *pParent = ui->profileTree->topLevelItem(i);

        for(int c = 0; c < pParent->childCount(); c++) {
            CProfileListItem *pItem = dynamic_cast<CProfileListItem *>(pParent->child(c));
            if(pItem != nullptr)
                if(pItem->pRadioButton->isChecked())
                    return pItem;
            }
        }

    return nullptr;
    }


//////////////////////////////////////////////////////////
/// \brief MainWindow::on_radioOverride_clicked
/// Use the active profile as the override
void MainWindow::on_radioOverride_clicked(void)
    {
    ui->checkBoxApplyList->setEnabled(true);
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
    }

//////////////////////////////////////////////////////////
void MainWindow::on_checkBoxPersistent_clicked(void)
    {
    pVulkanConfig->bKeepActiveOnExit = ui->checkBoxPersistent->isChecked();
    pVulkanConfig->SaveAppSettings();
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


//////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::profileItemChanged
/// \param pItem
/// \param nCol
/// Something was selected, as in a radio button that changes which profile
/// is now active.
void MainWindow::profileItemChanged(QTreeWidgetItem *pItem, int nCol)
    {
    (void)nCol;
    printf("profileItemChanged\n");

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
    (void)pPrevious;
    printf("profileTreechanged\n");

    // This pointer will only be valid if it's one of the elements with
    // the radio button
    CProfileListItem *pProfileItem = GetCheckedItem();
    if(pProfileItem == nullptr)
        return;

    // Do we go ahead and activate it?
    if(pVulkanConfig->bOverrideActive)
        pVulkanConfig->SetCurrentActiveProfile(pProfileItem->pProfilePointer);
    }


/////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::setGetStartedText
/// \param szText
/// Set hint text
void MainWindow::updateGetStartedStatus(QString qsText)
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
    pLabelGetStarted->setText(qsText);
    }


void MainWindow::CheckAppListState(void)
    {
//    // Final check - if there are no apps, disable the profiles list
//    if(pVulkanConfig->appList.length() == 0) {
//        this->setWindowTitle("Vulkan Configurator (Warning, no app list specified)");
//        ui->pushButtonAppList->setStyleSheet("QPushButton { color: red;}");
//        ui->groupBoxEditor->setTitle(tr("Getting Started"));
//        ui->groupBoxProfiles->setEnabled(false);
//        settingsEditor.CleanupGUI();    // Just in case
//        updateGetStartedStatus(szWelcomeText);
//        }
//    else { // There are apps
//        this->setWindowTitle(VKCONFIG_NAME);
//        ui->pushButtonAppList->setStyleSheet("QPushButton { color: black;}");

//        // But was one of them selected?
//        int nSelected = ui->listWidgetProfiles->currentRow();
//        if(nSelected == -1) {
//            ui->groupBoxProfiles->setEnabled(false);
//            ui->groupBoxEditor->setTitle(tr("Using Vulkan Configurations"));
//            ui->groupBoxProfiles->setEnabled(true);

//            if(pVulkanConfig->GetCurrentActiveProfile() == nullptr) {
//                updateGetStartedStatus(szStartTextRed);
//                }
//            else
//                updateGetStartedStatus(szStartText);
//            }
//        else {
//            // An item was selected and active
//            ui->groupBoxProfiles->setEnabled(true);
//            }
//        }
    }

/////////////////////////////////////////////////////
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

void MainWindow::on_pushButtonLaunch_clicked(void)
    {
    toolsVulkanTestApp(false);
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

    // If we come back and there are no apps in the app list, don't leave any holes
    if(pVulkanConfig->appList.size() == 0 && pVulkanConfig->GetCurrentActiveProfile() != nullptr) {
        pVulkanConfig->SetCurrentActiveProfile(nullptr);
        LoadProfileList();
        }

    CheckAppListState();
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonEditProfile_clicked
/// Just resave the list anytime we go into the editor
void MainWindow::on_pushButtonEdit_clicked(void)
    {
//    int nSelection = ui->listWidgetProfiles->currentRow();
//    Q_ASSERT(nSelection >= 0);

//    // Which profile is selected?
//    QListWidgetItem* pItem = ui->listWidgetProfiles->item(nSelection);
//    if(pItem != nullptr) {
//        CProfileListItem *pProfileItem = dynamic_cast<CProfileListItem*>(pItem);
//        if(pProfileItem != nullptr) {
//            // Save current state before we go in
//            if(settingsEditor.CollectSettings())
//                pVulkanConfig->SaveProfile(pProfileItem->pProfilePointer);
//            dlgProfileEditor dlg(this, pProfileItem->pProfilePointer);
//            dlg.exec();
//            pVulkanConfig->LoadAllProfiles(); // Reset
//            LoadProfileList();  // Force a reload
//            if(pVulkanConfig->GetCurrentActiveProfile() == nullptr)
//                ui->pushButtonActivate->setEnabled(false);
//            }
//        }
    CheckAppListState();
    }

////////////////////////////////////////////////////////////////////////////////
// Create a new blank profile
void MainWindow::on_pushButtonNewProfile_clicked(void)
    {
//    dlgProfileEditor dlg(this, nullptr);
//    dlg.exec();
//    LoadProfileList();  // force a reload
//    CheckAppListState();
//    if(pVulkanConfig->GetCurrentActiveProfile() == nullptr)
//        ui->pushButtonActivate->setEnabled(false);
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
/// \brief MainWindow::on_pushButtonRemoved_clicked
/// Remove the currently selected user defined profile.
void MainWindow::on_pushButtonRemove_clicked()
    {
//    int nSelection = ui->listWidgetProfiles->currentRow();
//    Q_ASSERT(nSelection != -1);

//    // Which profile is selected?
//    QListWidgetItem* pItem = ui->listWidgetProfiles->item(nSelection);
//    CProfileListItem *pProfileItem = dynamic_cast<CProfileListItem*>(pItem);
//    Q_ASSERT(pProfileItem != nullptr);

//    QMessageBox warning;
//    warning.setInformativeText(tr("Are you sure you want to delete this configuration?"));
//    warning.setText(pProfileItem->pProfilePointer->qsProfileName);
//    warning.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//    warning.setDefaultButton(QMessageBox::No);
//    if(QMessageBox::No == warning.exec())
//        return; // No harm, no foul

//    // Delete the file
//    QString completePath = pVulkanConfig->GetProfilePath();
//    completePath += "/";
//    completePath += pProfileItem->pProfilePointer->qsFileName;
//    remove(completePath.toUtf8().constData());

//    // Reload profiles
//    pVulkanConfig->LoadAllProfiles();
//    LoadProfileList();
//    CheckAppListState();

//    if(pVulkanConfig->GetCurrentActiveProfile() == nullptr) {
//        ui->pushButtonActivate->setText(tr("Activate"));
//        ui->pushButtonActivate->setEnabled(false);
//        }
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


//////////////////////////////////////////////////////////////
// Should be easy enough
void MainWindow::updateActivateButtonState(void)
    {
//    // Capture the original style sheet so disabled doesn't look weird
//    static QString originalStyle = ui->pushButtonActivate->styleSheet();

//    // Nothing is active. Text says Activate
//    // Might be disabled, might not.
//    if(pVulkanConfig->GetCurrentActiveProfile() == nullptr) {
//        ui->pushButtonActivate->setText(tr(ACTIVATE_TEXT));
//        ui->pushButtonActivate->setStyleSheet(originalStyle);

//        if(ui->listWidgetProfiles->currentRow() == -1)
//            ui->pushButtonActivate->setEnabled(false);
//        else
//            ui->pushButtonActivate->setEnabled(true);
//        }
//    else {
//        // This is always enabled, so we are okay with an abbreviated style sheet
//        ui->pushButtonActivate->setStyleSheet("QPushButton { color: red;}");
//        ui->pushButtonActivate->setText(tr(DEACTIVATE_TEXT));
//        }
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

    QComboBox *pDropDown = new QComboBox();
    ui->launchTree->setItemWidget(pLauncherParent, 1, pDropDown);
    //pDropDown->setStyleSheet("QComboBox { background-color: white; }");
    for(int i = 0; i < pVulkanConfig->appList.size(); i++)
        pDropDown->addItem(pVulkanConfig->appList[i]->qsAppNameWithPath);

    QPushButton *pButton = new QPushButton();
    pButton->setText("...");
    pButton->setMinimumWidth(32);
    ui->launchTree->setItemWidget(pLauncherParent, 2, pButton);

    //////////////////////////////////////////////////////////////////
    // Working folder
    QTreeWidgetItem *pLauncherFolder = new QTreeWidgetItem();
    pLauncherFolder->setText(0, "Working directory");
    pLauncherParent->addChild(pLauncherFolder);

    QLineEdit *pLineEdit = new QLineEdit();
    ui->launchTree->setItemWidget(pLauncherFolder, 1, pLineEdit);


    pButton = new QPushButton();
    pButton->setText("...");
    pButton->setMinimumWidth(32);
    ui->launchTree->setItemWidget(pLauncherFolder, 2, pButton);

    //////////////////////////////////////////////////////////////////
    // Command line arguments
    QTreeWidgetItem *pLauncherCMD = new QTreeWidgetItem();
    pLauncherCMD->setText(0, "Command-line Arguments");
    pLauncherParent->addChild(pLauncherCMD);

    pLineEdit = new QLineEdit();
    ui->launchTree->setItemWidget(pLauncherCMD, 1, pLineEdit);

    pButton = new QPushButton();
    pButton->setText("...");
    pButton->setMinimumWidth(32);
    ui->launchTree->setItemWidget(pLauncherCMD, 2, pButton);

    //////////////////////////////////////////////////////////////////
    // LOG FILE
    QTreeWidgetItem *pLauncherLogFile = new QTreeWidgetItem();
    pLauncherLogFile->setText(0, "Log File");
    pLauncherParent->addChild(pLauncherLogFile);

    pLineEdit = new QLineEdit();
    ui->launchTree->setItemWidget(pLauncherLogFile, 1, pLineEdit);

    pButton = new QPushButton();
    pButton->setText("...");
    pButton->setMinimumWidth(32);
    ui->launchTree->setItemWidget(pLauncherLogFile, 2, pButton);

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
    ui->launchTree->resizeColumnToContents(0);
    ui->launchTree->resizeColumnToContents(1);
    }

//////////////////////////////////////////////////////////////////////
bool MainWindow::eventFilter(QObject *target, QEvent *event)
    {
    if(target == ui->launchTree) {
        if(event->type() == QEvent::Resize) {
            QRect rect = ui->launchTree->rect();
            ui->launchTree->setColumnWidth(0, 210);
            ui->launchTree->setColumnWidth(1, rect.width() - 210 - 25-16);
            ui->launchTree->setColumnWidth(2, 25);
            }
        }

    return false;
    }
