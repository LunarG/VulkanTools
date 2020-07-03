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
 * Authors:
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
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
#include <QLineEdit>
#include <QDesktopServices>

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
#include "dlgcustompaths.h"
#include "configurator.h"
#include "preferences.h"

// This is what happens when programmers can touch type....
bool bBeenWarnedAboutOldLoader = false;

#define EDITOR_CAPTION_EMPTY "Configuration Layer Settings"

static const int LAUNCH_COLUMN0_SIZE = 240;
static const int LAUNCH_COLUMN2_SIZE = 32;
static const int LAUNCH_SPACING_SIZE = 2;
#ifdef __APPLE__
static const int LAUNCH_ROW_HEIGHT = 24;
#else
static const int LAUNCH_ROW_HEIGHT = 28;
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->launchTree->installEventFilter(this);
    ui->profileTree->installEventFilter(this);

    selected_configuration_item_ = nullptr;
    pVKVia = nullptr;
    pVulkanInfo = nullptr;
    pDlgHelp = nullptr;
    launch_application_ = nullptr;
    log_file_ = nullptr;
    pLaunchAppsCombo = nullptr;
    pLaunchArguments = nullptr;

    ///////////////////////////////////////////////
    Configurator &configurator = Configurator::Get();

    // We need to resetup the new profile for consistency sake.
    QSettings settings;
    QString last_configuration = settings.value(VKCONFIG_KEY_ACTIVEPROFILE, QString("Validation - Standard")).toString();
    Configuration *current_configuration = configurator.FindConfiguration(last_configuration);
    if (configurator.override_active) ChangeActiveConfiguration(current_configuration);

    LoadConfigurationList();
    SetupLaunchTree();

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutVkConfig(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));
    connect(ui->actionHelp, SIGNAL(triggered(bool)), this, SLOT(helpShowHelp(bool)));

    connect(ui->actionCustom_Layer_Paths, SIGNAL(triggered(bool)), this, SLOT(toolsSetCustomPaths(bool)));

    connect(ui->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui->actionRestore_Default_Configurations, SIGNAL(triggered(bool)), this, SLOT(toolsResetToDefault(bool)));

    connect(ui->profileTree, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(profileItemChanged(QTreeWidgetItem *, int)));
    connect(ui->profileTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(profileTreeChanged(QTreeWidgetItem *, QTreeWidgetItem *)));

    connect(ui->layerSettingsTree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(editorExpanded(QTreeWidgetItem *)));

    connect(ui->launchTree, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(launchItemCollapsed(QTreeWidgetItem *)));
    connect(ui->launchTree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(launchItemExpanded(QTreeWidgetItem *)));

    ui->pushButtonAppList->setEnabled(configurator.override_application_list_only);
    ui->checkBoxApplyList->setChecked(configurator.override_application_list_only);
    ui->checkBoxPersistent->setChecked(configurator.override_permanent);

    if (configurator.override_active) {
        ui->radioOverride->setChecked(true);
        ui->groupBoxProfiles->setEnabled(true);
        ui->groupBoxEditor->setEnabled(true);
    } else {
        ui->radioFully->setChecked(true);
        ui->checkBoxApplyList->setEnabled(false);
        ui->checkBoxPersistent->setEnabled(false);
        ui->pushButtonAppList->setEnabled(false);
        ui->groupBoxProfiles->setEnabled(false);
        ui->groupBoxEditor->setEnabled(false);
    }

    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

    // All else is done, highlight and activeate the current profile on startup
    Configuration *pActive = configurator.GetActiveConfiguration();
    if (pActive != nullptr) {
        for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
            ContigurationListItem *pItem = dynamic_cast<ContigurationListItem *>(ui->profileTree->topLevelItem(i));
            if (pItem != nullptr)
                if (pItem->configuration == pActive) {  // Ding ding ding... we have a winner
                    ui->profileTree->setCurrentItem(pItem);
                }
        }
    }

    ui->logBrowser->append("Vulkan Development Status:");
    ui->logBrowser->append(configurator.CheckVulkanSetup());
}

MainWindow::~MainWindow() { delete ui; }

///////////////////////////////////////////////////////////////////////////////
// Load or refresh the list of profiles. Any profile that uses a layer that
// is not detected on the system is disabled.
void MainWindow::LoadConfigurationList(void) {
    // There are lots of ways into this, and in none of them
    // can we have an active editor running.
    settings_tree_manager_.CleanupGUI();
    ui->profileTree->blockSignals(true);  // No signals firing off while we do this
    ui->profileTree->clear();

    // Who is the currently active profile?
    QSettings settings;
    QString active_configuration_name = settings.value(VKCONFIG_KEY_ACTIVEPROFILE).toString();

    Configurator &configurator = Configurator::Get();

    for (int i = 0; i < configurator.available_configurations.size(); i++) {
        // Add to list
        ContigurationListItem *pItem = new ContigurationListItem();
        pItem->configuration = configurator.available_configurations[i];
        ui->profileTree->addTopLevelItem(pItem);
        pItem->setText(1, configurator.available_configurations[i]->name);
        pItem->setToolTip(1, configurator.available_configurations[i]->description);
        pItem->radio_button = new QRadioButton();
        pItem->radio_button->setText("");

        if (!configurator.available_configurations[i]->IsValid()) {
            pItem->setFlags(pItem->flags() & ~Qt::ItemIsEnabled);
            pItem->radio_button->setEnabled(false);
            pItem->setToolTip(1, "Missing Vulkan Layer to use this configuration, try to add Custom Path to locate the layers");
        }
        if (active_configuration_name == configurator.available_configurations[i]->name) {
            pItem->radio_button->setChecked(true);
        }

        pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);
        ui->profileTree->setItemWidget(pItem, 0, pItem->radio_button);
        // ui->profileTree->setColumnWidth(0, 50);
        connect(pItem->radio_button, SIGNAL(clicked(bool)), this, SLOT(profileItemClicked(bool)));
    }

    ui->profileTree->blockSignals(false);
    ChangeActiveConfiguration(configurator.GetActiveConfiguration());
    ui->profileTree->setColumnWidth(0, 24);
    ui->profileTree->resizeColumnToContents(1);
}

//////////////////////////////////////////////////////////
/// \brief MainWindow::on_radioFully_clicked
// No override at all, fully controlled by the application
void MainWindow::on_radioFully_clicked(void) {
    ui->checkBoxApplyList->setEnabled(false);
    ui->checkBoxPersistent->setEnabled(false);

    Configurator &configurator = Configurator::Get();

    configurator.override_active = false;
    ui->groupBoxProfiles->setEnabled(false);
    ui->groupBoxEditor->setEnabled(false);

    ui->pushButtonAppList->setEnabled(false);

    configurator.SaveSettings();
    ChangeActiveConfiguration(nullptr);
}

//////////////////////////////////////////////////////////
/// \brief MainWindow::GetCheckedItem
/// \return
/// Okay, because we are using custom controls, some of
/// the signaling is not happening as expected. So, we cannot
/// always get an accurate answer to the currently selected
/// item, but we do often need to know what has been checked
/// when an event occurs. This unambigously answers that question.
ContigurationListItem *MainWindow::GetCheckedItem(void) {
    // Just go through all the top level items
    for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        ContigurationListItem *pItem = dynamic_cast<ContigurationListItem *>(ui->profileTree->topLevelItem(i));

        if (pItem != nullptr)
            if (pItem->radio_button->isChecked()) return pItem;
    }

    return nullptr;
}

//////////////////////////////////////////////////////////
/// \brief MainWindow::on_radioOverride_clicked
/// Use the active profile as the override
void MainWindow::on_radioOverride_clicked(void) {
    Configurator &configurator = Configurator::Get();

    bool bUse = (!configurator.bHasOldLoader || !bBeenWarnedAboutOldLoader);
    ui->checkBoxApplyList->setEnabled(bUse);
    ui->pushButtonAppList->setEnabled(bUse && configurator.override_application_list_only);

    ui->checkBoxPersistent->setEnabled(true);
    configurator.override_active = true;
    ui->groupBoxProfiles->setEnabled(true);
    ui->groupBoxEditor->setEnabled(true);
    configurator.SaveSettings();

    // This just doesn't work. Make a function to look for the radio button checked.
    ContigurationListItem *pProfileItem = GetCheckedItem();
    if (pProfileItem == nullptr)
        ChangeActiveConfiguration(nullptr);
    else
        ChangeActiveConfiguration(pProfileItem->configuration);
}

///////////////////////////////////////////////////////////////////////
// We want to apply to just the app list... hang on there. Doe we have
// the new loader?
void MainWindow::on_checkBoxApplyList_clicked(void) {
    Configurator &configurator = Configurator::Get();

    if (configurator.bHasOldLoader && !bBeenWarnedAboutOldLoader) {
        uint32_t version = configurator.vulkan_instance_version;
        QString message;
        message = QString().asprintf(
            "The detected Vulkan Loader version is %d.%d.%d but version 1.2.141 or newer is required in order to apply layers "
            "override to only a selected list of Vulkan applications.\n\n<br><br>"
            "Get the latest Vulkan Runtime from <a href='https://vulkan.lunarg.com/sdk/home'>HERE.</a> to use this feature.",
            VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
        QMessageBox alert(this);
        alert.setTextFormat(Qt::RichText);
        alert.setText(message);
        alert.setIcon(QMessageBox::Warning);
        alert.setWindowTitle(tr("Layers override of a selected list of Vulkan Applications is not available"));
        alert.exec();

        ui->pushButtonAppList->setEnabled(false);
        ui->checkBoxApplyList->setEnabled(false);
        ui->checkBoxApplyList->setChecked(false);
        QString messageToolTip;
        messageToolTip =
            QString().asprintf("The detected Vulkan loader version is %d.%d.%d but version 1.2.141 or newer is required",
                               VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
        ui->checkBoxApplyList->setToolTip(messageToolTip);
        ui->pushButtonAppList->setToolTip(messageToolTip);
        bBeenWarnedAboutOldLoader = true;
    }

    configurator.override_application_list_only = ui->checkBoxApplyList->isChecked();
    configurator.SaveSettings();
    ui->pushButtonAppList->setEnabled(configurator.override_application_list_only);
}

//////////////////////////////////////////////////////////
void MainWindow::on_checkBoxPersistent_clicked(void) {
    Configurator &configurator = Configurator::Get();

    configurator.override_permanent = ui->checkBoxPersistent->isChecked();
    configurator.SaveSettings();
}

//////////////////////////////////////////////////////////
void MainWindow::toolsResetToDefault(bool bChecked) {
    (void)bChecked;

    // Let make sure...
    QMessageBox msg;
    msg.setIcon(QMessageBox::Warning);
    msg.setWindowTitle(tr("Restoring and Resetting all Layers Configurations to default"));
    msg.setText(
        tr("You are about to delete all the user-defined configurations and resetting all default configurations to their default "
           "state.\n\n"
           "Are you sure you want to continue?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    if (msg.exec() == QMessageBox::No) return;

    Configurator &configurator = Configurator::Get();

    // Clear the current profile as we may be about to remove it.
    configurator.SetActiveConfiguration(nullptr);

    // Delete all the *.json files in the storage folder
    QDir dir(configurator.configuration_path_);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << "*.json");
    QFileInfoList profileFiles = dir.entryInfoList();

    // Loop through all the profiles found and remove them
    for (int iProfile = 0; iProfile < profileFiles.size(); iProfile++) {
        QFileInfo info = profileFiles.at(iProfile);
        if (info.absoluteFilePath().contains("applist.json")) continue;
        remove(info.filePath().toUtf8().constData());
    }

    // Reset to recopy from resource file
    QSettings settings;
    settings.setValue(VKCONFIG_KEY_FIRST_RUN, true);

    // Now we need to kind of restart everything
    settings_tree_manager_.CleanupGUI();
    configurator.LoadAllConfigurations();

    // Find the Standard Validation and make it current if we are active
    Configuration *pNewActiveProfile = configurator.FindConfiguration(QString("Validation - Standard"));
    if (configurator.override_active) ChangeActiveConfiguration(pNewActiveProfile);

    LoadConfigurationList();

    // Active or not, set it in the tree so we can see the settings.
    for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        ContigurationListItem *pItem = dynamic_cast<ContigurationListItem *>(ui->profileTree->topLevelItem(i));
        if (pItem != nullptr)
            if (pItem->configuration == pNewActiveProfile) ui->profileTree->setCurrentItem(pItem);
    }

    configurator.FindVkCube();
    ResetLaunchOptions();

    ui->logBrowser->clear();
    ui->logBrowser->append("Vulkan Development Status:");
    ui->logBrowser->append(configurator.CheckVulkanSetup());

    if (configurator.override_active) {
        ui->radioOverride->setChecked(true);
        ui->checkBoxApplyList->setEnabled(true);
        ui->checkBoxPersistent->setEnabled(true);
        ui->groupBoxProfiles->setEnabled(true);
        ui->groupBoxEditor->setEnabled(true);
    } else {
        ui->radioFully->setChecked(true);
        ui->checkBoxApplyList->setEnabled(false);
        ui->checkBoxPersistent->setEnabled(false);
        ui->pushButtonAppList->setEnabled(false);
        ui->groupBoxProfiles->setEnabled(false);
        ui->groupBoxEditor->setEnabled(false);
    }
}

////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::profileItemClicked
/// \param bChecked
/// Thist signal actually comes from the radio button
void MainWindow::profileItemClicked(bool bChecked) {
    (void)bChecked;
    // Someone just got checked, they are now the current profile
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ContigurationListItem *pProfileItem = GetCheckedItem();
    if (pProfileItem == nullptr) return;

    Configurator &configurator = Configurator::Get();

    // Do we go ahead and activate it?
    if (configurator.override_active) {
        ChangeActiveConfiguration(pProfileItem->configuration);
        configurator.CheckApplicationRestart();
    }
}

/////////////////////////////////////////////////////////////////////////////
/// An item has been changed. Check for edit of the items name (profile name)
void MainWindow::profileItemChanged(QTreeWidgetItem *pItem, int nCol) {
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ContigurationListItem *pProfileItem = dynamic_cast<ContigurationListItem *>(pItem);
    if (pProfileItem == nullptr) return;

    if (nCol == 1) {  // Profile name
        Configurator &configurator = Configurator::Get();

        // We are renaming the file. Just delete the old one and save this
        QString completePath = configurator.GetConfigurationPath();
        completePath += "/";
        completePath += pProfileItem->configuration->file;
        remove(completePath.toUtf8().constData());

        pProfileItem->configuration->name = pProfileItem->text(1);
        pProfileItem->configuration->file = pProfileItem->text(1) + QString(".json");
        configurator.SaveConfiguration(pProfileItem->configuration);
        configurator.CheckApplicationRestart();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::profileTreeChanged
/// \param pCurrent
/// \param pPrevious
/// This gets called with keyboard selections and clicks that do not necessarily
/// result in a radio button change (but it may). So we need to do two checks here, one
/// for the radio button, and one to change the editor/information at lower right.
void MainWindow::profileTreeChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious) {
    (void)pPrevious;
    settings_tree_manager_.CleanupGUI();
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ContigurationListItem *pProfileItem = dynamic_cast<ContigurationListItem *>(pCurrent);
    if (pProfileItem == nullptr) return;

    if (!Preferences::Get().use_separated_select_and_activate) {
        pProfileItem->radio_button->setChecked(true);
        ChangeActiveConfiguration(pProfileItem->configuration);
        Configurator::Get().CheckApplicationRestart();
    }

    settings_tree_manager_.CreateGUI(ui->layerSettingsTree, pProfileItem->configuration);
    QString title = pProfileItem->configuration->name;
    title += " Settings";
    ui->groupBoxEditor->setTitle(title);
    ui->layerSettingsTree->resizeColumnToContents(0);
}

////////////////////////////////////////////////////
// Unused flag, just display the about Qt dialog
void MainWindow::aboutVkConfig(bool bChecked) {
    (void)bChecked;
    dlgAbout dlg(this);
    dlg.exec();
}

//////////////////////////////////////////////////////////
/// \brief MainWindow::toolsVulkanInfo
/// \param bChecked
/// Create the VulkanInfo dialog if it doesn't already
/// exits & show it.
void MainWindow::toolsVulkanInfo(bool bChecked) {
    (void)bChecked;

    if (pVulkanInfo == nullptr) pVulkanInfo = new dlgVulkanInfo(this);

    pVulkanInfo->RunTool();
}

//////////////////////////////////////////////////////////
/// \brief MainWindow::toolsVulkanInstallation
/// \param bChecked
/// Create the VulkanTools dialog if it doesn't already
/// exist & show it.
void MainWindow::toolsVulkanInstallation(bool bChecked) {
    (void)bChecked;
    if (pVKVia == nullptr) pVKVia = new dlgVulkanAnalysis(this);

    pVKVia->RunTool();
}

////////////////////////////////////////////////////////////////
/// \brief MainWindow::helpShowHelp
/// \param bChecked
/// Show help, which is just a rich text file
void MainWindow::helpShowHelp(bool bChecked) {
    (void)bChecked;
    if (pDlgHelp == nullptr) pDlgHelp = new dlgHelp(nullptr);

    pDlgHelp->show();
}

////////////////////////////////////////////////////////////////
/// \brief MainWindow::closeEvent
/// \param event
/// The only thing we need to do here is clear the profile if
/// the user does not want it active.
void MainWindow::closeEvent(QCloseEvent *event) {
    Configurator &configurator = Configurator::Get();
    if (!configurator.override_permanent) configurator.SetActiveConfiguration(nullptr);

    configurator.SaveOverriddenApplicationList();
    configurator.SaveSettings();

    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

////////////////////////////////////////////////////////////////
/// \brief MainWindow::resizeEvent
/// \param pEvent
/// Resizing needs a little help. Yes please, there has to be
/// a better way of doing this.
void MainWindow::resizeEvent(QResizeEvent *pEvent) {
    if (pEvent != nullptr) pEvent->accept();
}

/////////////////////////////////////////////////////////////
void MainWindow::showEvent(QShowEvent *event) {
    //  resizeEvent(nullptr); // Fake to get controls to do the right thing
    event->accept();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonAppList_clicked
/// Edit the list of apps that can be filtered.
void MainWindow::on_pushButtonAppList_clicked(void) {
    dlgCreateAssociation dlg(this);
    dlg.exec();

    Configurator &configurator = Configurator::Get();

    if (Preferences::Get().use_last_selected_application_in_launcher) {
        configurator.SelectLaunchApplication(dlg.GetSelectedLaunchApplicationIndex());
    }

    configurator.SaveOverriddenApplicationList();
    ResetLaunchOptions();

    // Also, we may have changed exclusion flags, so reset override
    Configuration *pCurr = configurator.GetActiveConfiguration();
    if (pCurr != nullptr) configurator.SetActiveConfiguration(pCurr);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonEditProfile_clicked
/// Just resave the list anytime we go into the editor
void MainWindow::on_pushButtonEditProfile_clicked() {
    // Who is selected?
    ContigurationListItem *pItem = dynamic_cast<ContigurationListItem *>(ui->profileTree->currentItem());
    if (pItem == nullptr) return;

    // Save current state before we go in
    settings_tree_manager_.CleanupGUI();

    dlgProfileEditor dlg(this, pItem->configuration);
    dlg.exec();
    // pItem will be invalid after LoadProfileList, but I still
    // need the pointer to the profile
    QString editedProfileName = pItem->configuration->name;

    Configurator::Get().LoadAllConfigurations();
    LoadConfigurationList();

    // Reset the current item
    for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        pItem = dynamic_cast<ContigurationListItem *>(ui->profileTree->topLevelItem(i));
        if (pItem != nullptr)
            if (pItem->configuration->name == editedProfileName) {
                ui->profileTree->setCurrentItem(pItem);
                //                settingsTreeManager.CreateGUI(ui->layerSettingsTree, pProfile);
                break;
            }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Create a new blank profile
void MainWindow::NewClicked() {
    Configurator &configurator = Configurator::Get();

    Configuration *pNewProfile = configurator.CreateEmptyConfiguration();
    dlgProfileEditor dlg(this, pNewProfile);
    if (QDialog::Accepted == dlg.exec()) {
        configurator.LoadAllConfigurations();
        LoadConfigurationList();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::addCustomPaths
/// Allow addition or removal of custom layer paths. Afterwards reset the list
/// of loaded layers, but only if something was changed.
void MainWindow::addCustomPaths() {
    // Get the tree state and clear it.
    settings_tree_manager_.CleanupGUI();

    dlgCustomPaths dlg(this);
    dlg.exec();
    LoadConfigurationList();  // Force a reload
}

//////////////////////////////////////////////////////////////////////////////
/// Remove the currently selected user defined profile.
void MainWindow::RemoveClicked(ContigurationListItem *pItem) {
    // Let make sure...
    QMessageBox msg;
    msg.setInformativeText(pItem->configuration->name);
    msg.setText(tr("Are you sure you want to remove this configuration?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    if (msg.exec() == QMessageBox::No) return;

    // What if this is the active profile? We will go boom boom soon...
    Configurator &configurator = Configurator::Get();
    if (configurator.GetActiveConfiguration() == pItem->configuration) configurator.SetActiveConfiguration(nullptr);

    // Delete the file
    QString completePath = configurator.GetConfigurationPath();
    completePath += "/";
    completePath += pItem->configuration->file;
    remove(completePath.toUtf8().constData());

    // Reload profiles
    configurator.LoadAllConfigurations();
    LoadConfigurationList();
}

/////////////////////////////////////////////////////////////////////////////
void MainWindow::RenameClicked(ContigurationListItem *pItem) { ui->profileTree->editItem(pItem, 1); }

/////////////////////////////////////////////////////////////////////////////
// Copy the current configuration
void MainWindow::DuplicateClicked(ContigurationListItem *pItem) {
    QString qsNewName = pItem->configuration->name;
    qsNewName += "2";
    settings_tree_manager_.CleanupGUI();
    pItem->configuration->name = qsNewName;

    Configurator &configurator = Configurator::Get();
    configurator.SaveConfiguration(pItem->configuration);
    configurator.LoadAllConfigurations();
    LoadConfigurationList();

    // Good enough? Nope, I want to select it and edit the name.
    // Find it.
    for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        ContigurationListItem *pItem = dynamic_cast<ContigurationListItem *>(ui->profileTree->topLevelItem(i));
        if (pItem->configuration->name == qsNewName) {
            ui->profileTree->editItem(pItem, 1);
            return;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Import a configuration file. File copy followed by a reload.
void MainWindow::ImportClicked(ContigurationListItem *pItem) {
    (void)pItem;  // We don't need this
    QString qsGetIt = QFileDialog::getOpenFileName(this, "Import Layers Configuration File", QDir::homePath(), "*.json");
    if (qsGetIt.isEmpty()) return;
    settings_tree_manager_.CleanupGUI();
    Configurator::Get().ImportConfiguration(qsGetIt);
    LoadConfigurationList();
}

/////////////////////////////////////////////////////////////////////////////
// Export a configuration file. Basically just a file copy
void MainWindow::ExportClicked(ContigurationListItem *pItem) {
    // Where to put it and what to call it
    QString qsSaveIt = QFileDialog::getSaveFileName(this, "Export Layers Configuration File", QDir::homePath(), "*.json");
    if (qsSaveIt.isEmpty()) return;

    // Copy away
    Configurator &configurator = Configurator::Get();
    QString fullSourceName = configurator.configuration_path_ + "/";
    fullSourceName += pItem->configuration->file;
    configurator.ExportConfiguration(fullSourceName, qsSaveIt);
}

/////////////////////////////////////////////////////////////////////////////
// Export a configuration file. Basically just a file copy
void MainWindow::EditCustomPathsClicked(ContigurationListItem *pItem) {
    (void)pItem;
    addCustomPaths();
    LoadConfigurationList();  // Force a reload
}

void MainWindow::toolsSetCustomPaths(bool bChecked) {
    (void)bChecked;
    addCustomPaths();
    LoadConfigurationList();  // Force a reload
}

/////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::UpdateActiveDecorations
/// Update "decorations": window caption, (Active) status in list
void MainWindow::ChangeActiveConfiguration(Configuration *pNewProfile) {
    Configurator &configurator = Configurator::Get();

    if (pNewProfile == nullptr || !configurator.override_active) {
        configurator.SetActiveConfiguration(nullptr);

        setWindowTitle("Vulkan Configurator <VULKAN APPLICATION CONTROLLED>");
    } else {
        QString newCaption = pNewProfile->name;
        if (!pNewProfile->IsValid()) newCaption += " (DISABLED)";
        newCaption += " - Vulkan Configurator ";
        configurator.SetActiveConfiguration(pNewProfile);
        newCaption += "<VULKAN APPLICATIONS OVERRIDDEN>";

        setWindowTitle(newCaption);
    }
}

void MainWindow::editorExpanded(QTreeWidgetItem *pItem) {
    (void)pItem;
    ui->layerSettingsTree->resizeColumnToContents(0);
}

void MainWindow::profileItemExpanded(QTreeWidgetItem *pItem) {
    (void)pItem;
    ui->layerSettingsTree->resizeColumnToContents(0);
    ui->layerSettingsTree->resizeColumnToContents(1);
}

///////////////////////////////////////////////////////////////////
/// \brief MainWindow::ResetLaunchOptions
/// Reload controls for launch control
void MainWindow::ResetLaunchOptions(void) {
    Configurator &configurator = Configurator::Get();
    ui->pushButtonLaunch->setEnabled(!configurator.overridden_application_list.empty());

    // Reload launch apps selections
    pLaunchAppsCombo->blockSignals(true);
    pLaunchAppsCombo->clear();

    for (int i = 0; i < configurator.overridden_application_list.size(); i++) {
        pLaunchAppsCombo->addItem(configurator.overridden_application_list[i]->executable_path);
    }

    if (configurator.overridden_application_list.size() == 0) {
        pLaunchArguments->setText("");
        pLaunchWorkingFolder->setText("");
        pLaunchLogFileEdit->setText("");
        return;
    }

    int launch_application_index = configurator.GetLaunchApplicationIndex();
    assert(launch_application_index >= 0);

    pLaunchAppsCombo->setCurrentIndex(launch_application_index);

    // Reset working folder and command line choices
    pLaunchArguments->setText(configurator.overridden_application_list[launch_application_index]->arguments);
    pLaunchWorkingFolder->setText(configurator.overridden_application_list[launch_application_index]->working_folder);
    pLaunchLogFileEdit->setText(configurator.overridden_application_list[launch_application_index]->log_file);
    pLaunchAppsCombo->blockSignals(false);
}

///////////////////////////////////////////////////////////////////
/// \brief MainWindow::SetupLaunchTree
/// Launch and log area
void MainWindow::SetupLaunchTree(void) {
    /////////////////////////////////////////////////////////////////
    // Executable
    QTreeWidgetItem *pLauncherParent = new QTreeWidgetItem();
    pLauncherParent->setText(0, "Executable Path");
    ui->launchTree->addTopLevelItem(pLauncherParent);

    pLaunchAppsCombo = new QComboBox();
    pLaunchAppsCombo->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    pLaunchAppsCombo->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(pLauncherParent, 1, pLaunchAppsCombo);

    pLuanchAppBrowseButton = new QPushButton();
    pLuanchAppBrowseButton->setText("...");
    pLuanchAppBrowseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pLuanchAppBrowseButton->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    pLuanchAppBrowseButton->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    pLuanchAppBrowseButton->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(pLauncherParent, 2, pLuanchAppBrowseButton);
    connect(pLaunchAppsCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(launchItemChanged(int)));
    connect(pLuanchAppBrowseButton, SIGNAL(clicked()), this, SLOT(on_pushButtonAppList_clicked()));

    //////////////////////////////////////////////////////////////////
    // Working folder
    QTreeWidgetItem *pLauncherFolder = new QTreeWidgetItem();
    pLauncherFolder->setText(0, "Working Directory");
    pLauncherParent->addChild(pLauncherFolder);

    pLaunchWorkingFolder = new QLineEdit();
    pLaunchWorkingFolder->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    pLaunchWorkingFolder->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(pLauncherFolder, 1, pLaunchWorkingFolder);
    pLaunchWorkingFolder->setReadOnly(false);

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
    pLaunchArguments->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    pLaunchArguments->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(pLauncherCMD, 1, pLaunchArguments);
    connect(pLaunchArguments, SIGNAL(textEdited(const QString &)), this, SLOT(launchArgsEdited(const QString &)));

    // Comming soon
    //    pButton = new QPushButton();
    //    pButton->setText("...");
    //    ui->launchTree->setItemWidget(pLauncherCMD, 2, pButton);

    //////////////////////////////////////////////////////////////////
    // LOG FILE
    QTreeWidgetItem *pLauncherLogFile = new QTreeWidgetItem();
    pLauncherLogFile->setText(0, "Output Log");
    pLauncherParent->addChild(pLauncherLogFile);

    pLaunchLogFileEdit = new QLineEdit();
    pLaunchLogFileEdit->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    pLaunchLogFileEdit->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(pLauncherLogFile, 1, pLaunchLogFileEdit);

    pLaunchLogFilebutton = new QPushButton();
    pLaunchLogFilebutton->setText("...");
    pLaunchLogFilebutton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pLaunchLogFilebutton->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    ui->launchTree->setItemWidget(pLauncherLogFile, 2, pLaunchLogFilebutton);
    connect(pLaunchLogFilebutton, SIGNAL(clicked()), this, SLOT(launchSetLogFile()));

    //////////////////////////////////////////////////////////////////
    ui->launchTree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    ui->launchTree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);

    ui->launchTree->setColumnWidth(0, LAUNCH_COLUMN0_SIZE);
    ui->launchTree->setColumnWidth(
        1, ui->launchTree->rect().width() - LAUNCH_COLUMN0_SIZE - LAUNCH_COLUMN2_SIZE - LAUNCH_SPACING_SIZE);
    ui->launchTree->setColumnWidth(2, LAUNCH_COLUMN2_SIZE);

    ui->launchTree->expandItem(pLauncherParent);
    ui->launchTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->launchTree->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ResetLaunchOptions();
}

////////////////////////////////////////////////////////////////////
// Expanding the tree also grows the tree to match
void MainWindow::launchItemExpanded(QTreeWidgetItem *pItem) {
    (void)pItem;
    ui->launchTree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    ui->launchTree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
}

////////////////////////////////////////////////////////////////////
// Collapsing the tree also shrinks the tree to match and show only
// the first line
void MainWindow::launchItemCollapsed(QTreeWidgetItem *pItem) {
    QRect rect = ui->launchTree->visualItemRect(pItem);
    ui->launchTree->setMinimumHeight(rect.height());
    ui->launchTree->setMaximumHeight(rect.height());
}

////////////////////////////////////////////////////////////////////
void MainWindow::launchSetLogFile(void) {
    int nLaunchIndex = pLaunchAppsCombo->currentIndex();
    Q_ASSERT(nLaunchIndex >= 0);

    QString logFile = QFileDialog::getSaveFileName(this, tr("Set Log File To..."), ".", tr("Log text(*.txt)"));

    logFile = QDir::toNativeSeparators(logFile);

    Configurator &configurator = Configurator::Get();
    configurator.overridden_application_list[nLaunchIndex]->log_file = logFile;

    if (logFile.isEmpty())
        pLaunchLogFileEdit->setText("");
    else
        pLaunchLogFileEdit->setText(logFile);

    configurator.SaveOverriddenApplicationList();
}

////////////////////////////////////////////////////////////////////
/// Launch app change
void MainWindow::launchItemChanged(int application_index) {
    Configurator &configurator = Configurator::Get();

    if (application_index >= configurator.overridden_application_list.size()) return;

    if (application_index < 0) return;

    pLaunchArguments->setText(configurator.overridden_application_list[application_index]->arguments);
    pLaunchWorkingFolder->setText(configurator.overridden_application_list[application_index]->working_folder);
    pLaunchLogFileEdit->setText(configurator.overridden_application_list[application_index]->log_file);

    configurator.SelectLaunchApplication(application_index);
    configurator.SaveSettings();
}

/////////////////////////////////////////////////////////////////////
/// \brief MainWindow::launchArgsEdited
/// \param newText
/// New command line arguments. Update them.
void MainWindow::launchArgsEdited(const QString &arguments) {
    int application_index = pLaunchAppsCombo->currentIndex();
    if (application_index < 0) return;

    Configurator &configurator = Configurator::Get();
    configurator.overridden_application_list[application_index]->arguments = arguments;
    configurator.SaveOverriddenApplicationList();
}

//////////////////////////////////////////////////////////////////////
// Clear the browser window
void MainWindow::on_pushButtonClearLog_clicked() {
    ui->logBrowser->clear();
    ui->logBrowser->update();
    ui->pushButtonClearLog->setEnabled(false);
}

//////////////////////////////////////////////////////////////////////
bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    // Launch tree does some fancy resizing and since it's down in
    // layouts and splitters, we can't just relay on the resize method
    // of this window.
    if (target == ui->launchTree) {
        if (event->type() == QEvent::Resize) {
            QRect rect = ui->launchTree->rect();
            ui->launchTree->setColumnWidth(0, LAUNCH_COLUMN0_SIZE);
            ui->launchTree->setColumnWidth(1, rect.width() - LAUNCH_COLUMN0_SIZE - LAUNCH_COLUMN2_SIZE - LAUNCH_SPACING_SIZE);
            ui->launchTree->setColumnWidth(2, LAUNCH_COLUMN2_SIZE);
            return false;
        }
    }

    // Context menus for layer configuration files
    if (target == ui->profileTree) {
        QContextMenuEvent *pRightClick = dynamic_cast<QContextMenuEvent *>(event);
        if (pRightClick && event->type() == QEvent::ContextMenu) {
            // Which item were we over?
            QTreeWidgetItem *pProfileItem = ui->profileTree->itemAt(pRightClick->pos());
            ContigurationListItem *pItem = dynamic_cast<ContigurationListItem *>(pProfileItem);

            // Create context menu here
            QMenu menu(ui->profileTree);

            QAction *pNewAction = new QAction("New Layers Configuration...");
            pNewAction->setEnabled(true);
            menu.addAction(pNewAction);

            QAction *pDuplicateAction = new QAction("Duplicate the Layers Configuration");
            pDuplicateAction->setEnabled(pItem != nullptr);
            menu.addAction(pDuplicateAction);

            QAction *pRemoveAction = new QAction("Remove the Layers Configuration");
            pRemoveAction->setEnabled(pItem != nullptr);
            menu.addAction(pRemoveAction);

            QAction *pRenameAction = new QAction("Rename the Layers Configuration");
            pRenameAction->setEnabled(pItem != nullptr);
            menu.addAction(pRenameAction);

            menu.addSeparator();

            QAction *pImportAction = new QAction("Import a Layers Configuration...");
            pImportAction->setEnabled(true);
            menu.addAction(pImportAction);

            QAction *pExportAction = new QAction("Export the Layers Configuration...");
            pExportAction->setEnabled(pItem != nullptr);
            menu.addAction(pExportAction);

            menu.addSeparator();

            QAction *pCustomPathAction = new QAction("Edit Layers Custom Path...");
            pCustomPathAction->setEnabled(true);
            menu.addAction(pCustomPathAction);

            QPoint point(pRightClick->globalX(), pRightClick->globalY());
            QAction *pAction = menu.exec(point);

            // Pointer compares made me throw up in my mouth at least a little
            // less than doing a full string compare. Setting up signal/slot for
            // all of these just seemed ridiculous. Every problem is not a nail,
            // put the hammer away....
            // New Profile...
            if (pAction == pNewAction) {
                settings_tree_manager_.CleanupGUI();
                NewClicked();
                ui->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Duplicate
            if (pAction == pDuplicateAction) {
                settings_tree_manager_.CleanupGUI();
                DuplicateClicked(pItem);
                settings_tree_manager_.CleanupGUI();
                ui->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Remove this profile....
            if (pAction == pRemoveAction) {
                settings_tree_manager_.CleanupGUI();
                RemoveClicked(pItem);
                ui->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Rename this profile...
            if (pAction == pRenameAction) {
                RenameClicked(pItem);
                settings_tree_manager_.CleanupGUI();
                ui->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Export this profile (copy the .json)
            if (pAction == pExportAction) {
                settings_tree_manager_.CleanupGUI();
                ExportClicked(pItem);
                ui->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Import a profile (copy a json)
            if (pAction == pImportAction) {
                settings_tree_manager_.CleanupGUI();
                ImportClicked(pItem);
                ui->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Edit Layer custom paths
            if (pAction == pCustomPathAction) {
                settings_tree_manager_.CleanupGUI();
                EditCustomPathsClicked(pItem);
                ui->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
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
void MainWindow::on_pushButtonLaunch_clicked(void) {
    // Are we already monitoring a running app? If so, terminate it
    if (launch_application_ != nullptr) {
        launch_application_->terminate();
        launch_application_->deleteLater();
        launch_application_ = nullptr;
        ui->pushButtonLaunch->setText(tr("Launch"));
        return;
    }

    // Is there an app selected?
    int nIndex = pLaunchAppsCombo->currentIndex();

    // Launch the test application
    launch_application_ = new QProcess(this);
    connect(launch_application_, SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));

    connect(launch_application_, SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));

    connect(launch_application_, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processClosed(int, QProcess::ExitStatus)));

    Configurator &configurator = Configurator::Get();
    launch_application_->setProgram(configurator.overridden_application_list[nIndex]->executable_path);
    launch_application_->setWorkingDirectory(configurator.overridden_application_list[nIndex]->working_folder);

    if (!configurator.overridden_application_list[nIndex]->arguments.isEmpty()) {
        QStringList args = configurator.overridden_application_list[nIndex]->arguments.split(" ");
        launch_application_->setArguments(args);
    }

    // Some of these may have changed
    configurator.SaveSettings();

    launch_application_->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    launch_application_->setProcessChannelMode(QProcess::MergedChannels);
    launch_application_->closeWriteChannel();

    // We are logging, let's add that we've launched a new application
    QString launchLog = "Launching Vulkan Application:\n";

    if (configurator.GetActiveConfiguration() == nullptr) {
        launchLog += QString().asprintf("- Layers fully controlled by the application.\n");
    } else if (!configurator.GetActiveConfiguration()->IsValid()) {
        launchLog += QString().asprintf("- No layers override. The active \"%s\" configuration is missing a layer.\n",
                                        configurator.GetActiveConfiguration()->name.toUtf8().constData());
    } else if (configurator.override_active) {
        launchLog += QString().asprintf("- Layers overridden by \"%s\" configuration.\n",
                                        configurator.GetActiveConfiguration()->name.toUtf8().constData());
    }

    launchLog += QString().asprintf("- Executable Path: %s\n",
                                    configurator.overridden_application_list[nIndex]->executable_path.toUtf8().constData());
    launchLog += QString().asprintf("- Working Directory: %s\n",
                                    configurator.overridden_application_list[nIndex]->working_folder.toUtf8().constData());
    launchLog += QString().asprintf("- Command-line Arguments: %s\n",
                                    configurator.overridden_application_list[nIndex]->arguments.toUtf8().constData());

    if (!configurator.overridden_application_list[nIndex]->log_file.isEmpty()) {
        // This should never happen... but things that should never happen do in
        // fact happen... so just a sanity check.
        if (log_file_ != nullptr) {
            log_file_->close();
            log_file_ = nullptr;
        }

        // Start logging
        log_file_ = new QFile(configurator.overridden_application_list[nIndex]->log_file);

        // Open and append, or open and truncate?
        QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
        if (!ui->checkBoxClearOnLaunch->isChecked()) mode |= QIODevice::Append;

        if (!configurator.overridden_application_list[nIndex]->log_file.isEmpty()) {
            if (!log_file_->open(mode)) {
                QMessageBox err;
                err.setText(tr("Cannot open log file"));
                err.setIcon(QMessageBox::Warning);
                err.exec();
                delete log_file_;
                log_file_ = nullptr;
            }
        }

        if (log_file_) {
            log_file_->write((launchLog + "\n").toUtf8().constData(), launchLog.length());
        }
    }

    if (ui->checkBoxClearOnLaunch->isChecked()) ui->logBrowser->clear();
    ui->logBrowser->append(launchLog);
    ui->pushButtonClearLog->setEnabled(true);

    // Wait... did we start? Give it 4 seconds, more than enough time
    if (!launch_application_->waitForStarted(4000)) {
        launch_application_->waitForStarted();
        launch_application_->deleteLater();
        launch_application_ = nullptr;

        QString outFailed = QString().asprintf(
            "Failed to launch %s!\n", configurator.overridden_application_list[nIndex]->executable_path.toUtf8().constData());

        ui->logBrowser->append(outFailed);
        if (log_file_) log_file_->write(outFailed.toUtf8().constData(), outFailed.length());

        return;
    }

    // We are off to the races....
    ui->pushButtonLaunch->setText(tr("Terminate"));
}

/////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::processClosed
/// \param exitCode
/// \param status
/// The process we are following is closed. We don't actually care about the
/// exit status/code, we just need to know to destroy the QProcess object
/// and set it back to nullptr so that we know we can launch a new app.
/// Also, if we are logging, it's time to close the log file.
void MainWindow::processClosed(int exitCode, QProcess::ExitStatus status) {
    (void)exitCode;
    (void)status;

    // Not likely, but better to be sure...
    if (launch_application_ == nullptr) return;

    disconnect(launch_application_, SIGNAL(finished(int, QProcess::ExitStatus)), this,
               SLOT(processClosed(int, QProcess::ExitStatus)));

    disconnect(launch_application_, SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));

    disconnect(launch_application_, SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));

    ui->pushButtonLaunch->setText(tr("Launch"));

    if (log_file_) {
        log_file_->close();
        delete log_file_;
        log_file_ = nullptr;
    }

    delete launch_application_;
    launch_application_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::standardOutputAvailable
/// This signal get's raised whenever the spawned Vulkan appliction writes
/// to stdout and there is data to be read. The layers flush after all stdout
/// writes, so we should see layer output here in realtime, as we just read
/// the string and append it to the text browser.
/// If a log file is open, we also write the output to the log.
void MainWindow::standardOutputAvailable(void) {
    if (launch_application_ == nullptr) return;

    QString inFromApp = launch_application_->readAllStandardOutput();
    ui->logBrowser->append(inFromApp);
    ui->pushButtonClearLog->setEnabled(true);

    // Are we logging?
    if (log_file_) log_file_->write(inFromApp.toUtf8().constData(), inFromApp.length());
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::errorOutputAvailable(void) {
    if (launch_application_ == nullptr) return;

    QString inFromApp = launch_application_->readAllStandardError();
    ui->logBrowser->append(inFromApp);
    ui->pushButtonClearLog->setEnabled(true);

    // Are we logging?
    if (log_file_) log_file_->write(inFromApp.toUtf8().constData(), inFromApp.length());
}
