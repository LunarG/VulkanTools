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

#include "mainwindow.h"
#include "dlgabout.h"
#include "dlgvulkananalysis.h"
#include "dlgvulkaninfo.h"
#include "dlgprofileeditor.h"
#include "dlgcreateassociation.h"
#include "dlgcustompaths.h"
#include "configurator.h"
#include "preferences.h"

#include "ui_mainwindow.h"

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

// This is what happens when programmers can touch type....
bool been_warned_about_old_loader = false;

#define EDITOR_CAPTION_EMPTY "Configuration Layer Settings"

static const int LAUNCH_COLUMN0_SIZE = 220;
static const int LAUNCH_COLUMN2_SIZE = 32;
static const int LAUNCH_SPACING_SIZE = 2;
#ifdef __APPLE__
static const int LAUNCH_ROW_HEIGHT = 26;
#else
static const int LAUNCH_ROW_HEIGHT = 28;
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
    ui_->launchTree->installEventFilter(this);
    ui_->profileTree->installEventFilter(this);

    selected_configuration_item_ = nullptr;
    vk_via_ = nullptr;
    vk_info_ = nullptr;
    help_ = nullptr;
    launch_application_ = nullptr;
    log_file_ = nullptr;
    launcher_apps_combo_ = nullptr;
    launch_arguments_ = nullptr;

    ///////////////////////////////////////////////
    Configurator &configurator = Configurator::Get();

    // We need to resetup the new profile for consistency sake.
    QString last_configuration = settings_.value(VKCONFIG_KEY_ACTIVEPROFILE, QString("Validation - Standard")).toString();
    Configuration *current_configuration = configurator.FindConfiguration(last_configuration);
    if (configurator.override_active) ChangeActiveConfiguration(current_configuration);

    LoadConfigurationList();
    SetupLaunchTree();

    connect(ui_->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutVkConfig(bool)));
    connect(ui_->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));
    connect(ui_->actionHelp, SIGNAL(triggered(bool)), this, SLOT(helpShowHelp(bool)));

    connect(ui_->actionCustom_Layer_Paths, SIGNAL(triggered(bool)), this, SLOT(toolsSetCustomPaths(bool)));

    connect(ui_->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui_->actionRestore_Default_Configurations, SIGNAL(triggered(bool)), this, SLOT(toolsResetToDefault(bool)));

    connect(ui_->profileTree, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(profileItemChanged(QTreeWidgetItem *, int)));
    connect(ui_->profileTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(profileTreeChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui_->profileTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationTreeClicked(QTreeWidgetItem *, int)));

    connect(ui_->layerSettingsTree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(editorExpanded(QTreeWidgetItem *)));
    connect(ui_->layerSettingsTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationSettingsTreeClicked(QTreeWidgetItem *, int)));

    connect(ui_->launchTree, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(launchItemCollapsed(QTreeWidgetItem *)));
    connect(ui_->launchTree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(launchItemExpanded(QTreeWidgetItem *)));

    if (!configurator.HasOverriddenApplications() || configurator.overridden_application_list.empty())
        configurator.override_application_list_only = false;

    ui_->pushButtonAppList->setEnabled(configurator.override_application_list_only);
    ui_->checkBoxApplyList->setChecked(configurator.override_application_list_only);
    ui_->checkBoxPersistent->setChecked(configurator.override_permanent);

    if (configurator.override_active) {
        ui_->radioOverride->setChecked(true);
        ui_->groupBoxProfiles->setEnabled(true);
        ui_->groupBoxEditor->setEnabled(true);
    } else {
        ui_->radioFully->setChecked(true);
        ui_->checkBoxApplyList->setEnabled(false);
        ui_->checkBoxPersistent->setEnabled(false);
        ui_->pushButtonAppList->setEnabled(false);
        ui_->groupBoxProfiles->setEnabled(false);
        ui_->groupBoxEditor->setEnabled(false);
    }

    restoreGeometry(settings_.value("geometry").toByteArray());
    restoreState(settings_.value("windowState").toByteArray());
    ui_->splitter->restoreState(settings_.value("splitter1State").toByteArray());
    ui_->splitter_2->restoreState(settings_.value("splitter2State").toByteArray());
    ui_->splitter_3->restoreState(settings_.value("splitter3State").toByteArray());

    // All else is done, highlight and activeate the current profile on startup
    Configuration *pActive = configurator.GetActiveConfiguration();
    if (pActive != nullptr) {
        for (int i = 0; i < ui_->profileTree->topLevelItemCount(); i++) {
            ContigurationListItem *pItem = dynamic_cast<ContigurationListItem *>(ui_->profileTree->topLevelItem(i));
            if (pItem != nullptr)
                if (pItem->configuration == pActive) {  // Ding ding ding... we have a winner
                    ui_->profileTree->setCurrentItem(pItem);
                }
        }
    }

    ui_->logBrowser->append("Vulkan Development Status:");
    ui_->logBrowser->append(configurator.CheckVulkanSetup());
    ui_->profileTree->scrollToItem(ui_->profileTree->topLevelItem(0), QAbstractItemView::PositionAtTop);
}

MainWindow::~MainWindow() { delete ui_; }

///////////////////////////////////////////////////////////////////////////////
// Load or refresh the list of profiles. Any profile that uses a layer that
// is not detected on the system is disabled.
void MainWindow::LoadConfigurationList() {
    // There are lots of ways into this, and in none of them
    // can we have an active editor running.
    settings_tree_manager_.CleanupGUI();
    ui_->profileTree->blockSignals(true);  // No signals firing off while we do this
    ui_->profileTree->clear();

    // Who is the currently active profile?
    QString active_configuration_name = settings_.value(VKCONFIG_KEY_ACTIVEPROFILE).toString();

    Configurator &configurator = Configurator::Get();

    for (int i = 0; i < configurator.available_configurations.size(); i++) {
        // Add to list
        ContigurationListItem *item = new ContigurationListItem();
        item->configuration = configurator.available_configurations[i];
        ui_->profileTree->addTopLevelItem(item);
        item->setText(1, configurator.available_configurations[i]->name);
        item->setToolTip(1, configurator.available_configurations[i]->description);
        item->radio_button = new QRadioButton();
        item->radio_button->setText("");

        if (!configurator.available_configurations[i]->IsValid()) {
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
            item->radio_button->setEnabled(false);
            item->setToolTip(1, "Missing Vulkan Layer to use this configuration, try to add Custom Path to locate the layers");
        }
        if (active_configuration_name == configurator.available_configurations[i]->name) {
            item->radio_button->setChecked(true);
        }

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui_->profileTree->setItemWidget(item, 0, item->radio_button);
        connect(item->radio_button, SIGNAL(clicked(bool)), this, SLOT(profileItemClicked(bool)));
    }

    ui_->profileTree->blockSignals(false);
    ChangeActiveConfiguration(configurator.GetActiveConfiguration());
    ui_->profileTree->setColumnWidth(0, 24);
    ui_->profileTree->resizeColumnToContents(1);
}

//////////////////////////////////////////////////////////
// No override at all, fully controlled by the application
void MainWindow::on_radioFully_clicked() {
    ui_->checkBoxApplyList->setEnabled(false);
    ui_->checkBoxPersistent->setEnabled(false);

    Configurator &configurator = Configurator::Get();

    configurator.override_active = false;
    ui_->groupBoxProfiles->setEnabled(false);
    ui_->groupBoxEditor->setEnabled(false);

    ui_->pushButtonAppList->setEnabled(false);

    configurator.SaveSettings();
    ChangeActiveConfiguration(nullptr);
}

//////////////////////////////////////////////////////////
/// Okay, because we are using custom controls, some of
/// the signaling is not happening as expected. So, we cannot
/// always get an accurate answer to the currently selected
/// item, but we do often need to know what has been checked
/// when an event occurs. This unambigously answers that question.
ContigurationListItem *MainWindow::GetCheckedItem() {
    // Just go through all the top level items
    for (int i = 0; i < ui_->profileTree->topLevelItemCount(); i++) {
        ContigurationListItem *item = dynamic_cast<ContigurationListItem *>(ui_->profileTree->topLevelItem(i));

        if (item != nullptr)
            if (item->radio_button->isChecked()) return item;
    }

    return nullptr;
}

//////////////////////////////////////////////////////////
/// Use the active profile as the override
void MainWindow::on_radioOverride_clicked() {
    Configurator &configurator = Configurator::Get();

    bool use = (!configurator.has_old_loader || !been_warned_about_old_loader);
    ui_->checkBoxApplyList->setEnabled(use);
    ui_->pushButtonAppList->setEnabled(use && configurator.override_application_list_only);

    ui_->checkBoxPersistent->setEnabled(true);
    configurator.override_active = true;
    ui_->groupBoxProfiles->setEnabled(true);
    ui_->groupBoxEditor->setEnabled(true);
    configurator.SaveSettings();

    // This just doesn't work. Make a function to look for the radio button checked.
    ContigurationListItem *configuration_item = GetCheckedItem();
    if (configuration_item == nullptr)
        ChangeActiveConfiguration(nullptr);
    else
        ChangeActiveConfiguration(configuration_item->configuration);
}

///////////////////////////////////////////////////////////////////////
// We want to apply to just the app list... hang on there. Doe we have
// the new loader?
void MainWindow::on_checkBoxApplyList_clicked() {
    Configurator &configurator = Configurator::Get();

    if (configurator.has_old_loader && !been_warned_about_old_loader) {
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

        ui_->pushButtonAppList->setEnabled(false);
        ui_->checkBoxApplyList->setEnabled(false);
        ui_->checkBoxApplyList->setChecked(false);
        QString messageToolTip;
        messageToolTip =
            QString().asprintf("The detected Vulkan loader version is %d.%d.%d but version 1.2.141 or newer is required",
                               VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
        ui_->checkBoxApplyList->setToolTip(messageToolTip);
        ui_->pushButtonAppList->setToolTip(messageToolTip);
        been_warned_about_old_loader = true;
    }

    configurator.override_application_list_only = ui_->checkBoxApplyList->isChecked();
    configurator.SaveSettings();
    ui_->pushButtonAppList->setEnabled(configurator.override_application_list_only);

    if (configurator.override_application_list_only &&
        (configurator.overridden_application_list.empty() || !configurator.HasOverriddenApplications())) {
        on_pushButtonAppList_clicked();
    } else {
        // Checking the list, the configuration need to be updated to the system
        if (configurator.GetActiveConfiguration()) ChangeActiveConfiguration(configurator.GetActiveConfiguration());
    }
}

//////////////////////////////////////////////////////////
void MainWindow::on_checkBoxPersistent_clicked() {
    Configurator &configurator = Configurator::Get();

    configurator.override_permanent = ui_->checkBoxPersistent->isChecked();
    configurator.SaveSettings();
}

//////////////////////////////////////////////////////////
void MainWindow::toolsResetToDefault(bool checked) {
    (void)checked;

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
    QDir dir(configurator.GetPath(Configurator::ConfigurationPath));
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << "*.json");
    QFileInfoList configuration_files = dir.entryInfoList();

    // Loop through all the profiles found and remove them
    for (int i = 0; i < configuration_files.size(); i++) {
        QFileInfo info = configuration_files.at(i);
        if (info.absoluteFilePath().contains("applist.json")) continue;
        remove(info.filePath().toUtf8().constData());
    }

    // Reset to recopy from resource file
    settings_.setValue(VKCONFIG_KEY_FIRST_RUN, true);

    // Now we need to kind of restart everything
    settings_tree_manager_.CleanupGUI();
    configurator.LoadAllConfigurations();

    // Find the Standard Validation and make it current if we are active
    Configuration *active_configuration = configurator.FindConfiguration(QString("Validation - Standard"));
    if (configurator.override_active) ChangeActiveConfiguration(active_configuration);

    LoadConfigurationList();

    // Active or not, set it in the tree so we can see the settings.
    for (int i = 0; i < ui_->profileTree->topLevelItemCount(); i++) {
        ContigurationListItem *item = dynamic_cast<ContigurationListItem *>(ui_->profileTree->topLevelItem(i));
        if (item != nullptr)
            if (item->configuration == active_configuration) ui_->profileTree->setCurrentItem(item);
    }

    configurator.FindVkCube();
    ResetLaunchOptions();

    ui_->logBrowser->clear();
    ui_->logBrowser->append("Vulkan Development Status:");
    ui_->logBrowser->append(configurator.CheckVulkanSetup());

    if (configurator.override_active) {
        ui_->radioOverride->setChecked(true);
        ui_->checkBoxApplyList->setEnabled(true);
        ui_->pushButtonAppList->setEnabled(false);
        ui_->checkBoxPersistent->setEnabled(true);
        ui_->groupBoxProfiles->setEnabled(true);
        ui_->groupBoxEditor->setEnabled(true);
    } else {
        ui_->radioFully->setChecked(true);
        ui_->checkBoxApplyList->setEnabled(false);
        ui_->pushButtonAppList->setEnabled(false);
        ui_->checkBoxPersistent->setEnabled(false);
        ui_->pushButtonAppList->setEnabled(false);
        ui_->groupBoxProfiles->setEnabled(false);
        ui_->groupBoxEditor->setEnabled(false);
    }
}

////////////////////////////////////////////////////////////////////////////
/// Thist signal actually comes from the radio button
void MainWindow::profileItemClicked(bool checked) {
    (void)checked;
    // Someone just got checked, they are now the current profile
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ContigurationListItem *configuration_item = GetCheckedItem();
    if (configuration_item == nullptr) return;

    Configurator &configurator = Configurator::Get();

    // Do we go ahead and activate it?
    if (configurator.override_active) {
        ChangeActiveConfiguration(configuration_item->configuration);
    }
}

/////////////////////////////////////////////////////////////////////////////
/// An item has been changed. Check for edit of the items name (configuration name)
void MainWindow::profileItemChanged(QTreeWidgetItem *item, int column) {
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ContigurationListItem *configuration_item = dynamic_cast<ContigurationListItem *>(item);
    if (configuration_item == nullptr) return;

    if (column == 1) {  // Profile name
        Configurator &configurator = Configurator::Get();

        // We are renaming the file. Just delete the old one and save this
        const QString full_path =
            configurator.GetPath(Configurator::ConfigurationPath) + "/" + configuration_item->configuration->file;
        remove(full_path.toUtf8().constData());

        configuration_item->configuration->name = configuration_item->text(1);
        configuration_item->configuration->file = configuration_item->text(1) + QString(".json");
        configurator.SaveConfiguration(configuration_item->configuration);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
/// This gets called with keyboard selections and clicks that do not necessarily
/// result in a radio button change (but it may). So we need to do two checks here, one
/// for the radio button, and one to change the editor/information at lower right.
void MainWindow::profileTreeChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    (void)previous;
    settings_tree_manager_.CleanupGUI();
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ContigurationListItem *configuration_item = dynamic_cast<ContigurationListItem *>(current);
    if (configuration_item == nullptr) return;

    if (!Preferences::Get().use_separated_select_and_activate) {
        configuration_item->radio_button->setChecked(true);
        ChangeActiveConfiguration(configuration_item->configuration);
    }

    settings_tree_manager_.CreateGUI(ui_->layerSettingsTree, configuration_item->configuration);
    QString title = configuration_item->configuration->name;
    title += " Settings";
    ui_->groupBoxEditor->setTitle(title);
    ui_->layerSettingsTree->resizeColumnToContents(0);
}

////////////////////////////////////////////////////
// Unused flag, just display the about Qt dialog
void MainWindow::aboutVkConfig(bool checked) {
    (void)checked;
    dlgAbout dlg(this);
    dlg.exec();
}

//////////////////////////////////////////////////////////
/// Create the VulkanInfo dialog if it doesn't already
/// exits & show it.
void MainWindow::toolsVulkanInfo(bool checked) {
    (void)checked;

    if (vk_info_ == nullptr) vk_info_ = new dlgVulkanInfo(this);

    vk_info_->RunTool();
}

//////////////////////////////////////////////////////////
/// Create the VulkanTools dialog if it doesn't already
/// exist & show it.
void MainWindow::toolsVulkanInstallation(bool checked) {
    (void)checked;
    if (vk_via_ == nullptr) vk_via_ = new dlgVulkanAnalysis(this);

    vk_via_->RunTool();
}

////////////////////////////////////////////////////////////////
/// Show help, which is just a rich text file
void MainWindow::helpShowHelp(bool checked) {
    (void)checked;
    if (help_ == nullptr) help_ = new dlgHelp(nullptr);

    help_->show();
}

////////////////////////////////////////////////////////////////
/// The only thing we need to do here is clear the profile if
/// the user does not want it active.
void MainWindow::closeEvent(QCloseEvent *event) {
    Configurator &configurator = Configurator::Get();
    if (!configurator.override_permanent) configurator.SetActiveConfiguration(nullptr);

    configurator.SaveOverriddenApplicationList();
    configurator.SaveSettings();

    settings_.setValue("geometry", saveGeometry());
    settings_.setValue("windowState", saveState());
    settings_.setValue("splitter1State", ui_->splitter->saveState());
    settings_.setValue("splitter2State", ui_->splitter_2->saveState());
    settings_.setValue("splitter3State", ui_->splitter_3->saveState());

    QMainWindow::closeEvent(event);
}

////////////////////////////////////////////////////////////////
/// Resizing needs a little help. Yes please, there has to be
/// a better way of doing this.
void MainWindow::resizeEvent(QResizeEvent *event) {
    if (event != nullptr) event->accept();
}

/////////////////////////////////////////////////////////////
void MainWindow::showEvent(QShowEvent *event) {
    //  resizeEvent(nullptr); // Fake to get controls to do the right thing
    event->accept();
}

///////////////////////////////////////////////////////////////////////////////
/// Edit the list of apps that can be filtered.
void MainWindow::on_pushButtonAppList_clicked() {
    dlgCreateAssociation dlg(this);
    dlg.exec();

    Configurator &configurator = Configurator::Get();

    if (Preferences::Get().use_last_selected_application_in_launcher) {
        configurator.SelectLaunchApplication(dlg.GetSelectedLaunchApplicationIndex());
    }

    configurator.SaveOverriddenApplicationList();
    ResetLaunchOptions();

    // Also, we may have changed exclusion flags, so reset override
    Configuration *active_configuration = configurator.GetActiveConfiguration();
    if (active_configuration != nullptr) configurator.SetActiveConfiguration(active_configuration);
}

///////////////////////////////////////////////////////////////////////////////
/// Just resave the list anytime we go into the editor
void MainWindow::on_pushButtonEditProfile_clicked() {
    // Who is selected?
    ContigurationListItem *item = dynamic_cast<ContigurationListItem *>(ui_->profileTree->currentItem());
    if (item == nullptr) return;

    // Save current state before we go in
    settings_tree_manager_.CleanupGUI();

    dlgProfileEditor dlg(this, item->configuration);
    dlg.exec();

    // 'item' will be invalid after LoadAllConfigurations, but I still -  // need the pointer to the configuration
    const QString edited_configuration_name = item->configuration->name;

    Configurator::Get().LoadAllConfigurations();
    LoadConfigurationList();

    // Reset the current item
    for (int i = 0; i < ui_->profileTree->topLevelItemCount(); i++) {
        item = dynamic_cast<ContigurationListItem *>(ui_->profileTree->topLevelItem(i));
        if (item != nullptr)
            if (item->configuration->name == edited_configuration_name) {
                ui_->profileTree->setCurrentItem(item);
                break;
            }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Create a new blank profile
void MainWindow::NewClicked() {
    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.CreateEmptyConfiguration();
    dlgProfileEditor dlg(this, configuration);
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
void MainWindow::RemoveClicked(ContigurationListItem *item) {
    // Let make sure...
    QMessageBox msg;
    msg.setInformativeText(item->configuration->name);
    msg.setText(tr("Are you sure you want to remove this configuration?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    if (msg.exec() == QMessageBox::No) return;

    // What if this is the active profile? We will go boom boom soon...
    Configurator &configurator = Configurator::Get();
    if (configurator.GetActiveConfiguration() == item->configuration) configurator.SetActiveConfiguration(nullptr);

    // Delete the file
    const QString full_path = configurator.GetPath(Configurator::ConfigurationPath) + "/" + item->configuration->file;
    remove(full_path.toUtf8().constData());

    // Reload profiles
    configurator.LoadAllConfigurations();
    LoadConfigurationList();
}

/////////////////////////////////////////////////////////////////////////////
void MainWindow::RenameClicked(ContigurationListItem *item) { ui_->profileTree->editItem(item, 1); }

/////////////////////////////////////////////////////////////////////////////
// Copy the current configuration
void MainWindow::DuplicateClicked(ContigurationListItem *item) {
    QString new_name = item->configuration->name;
    new_name += "2";
    settings_tree_manager_.CleanupGUI();
    item->configuration->name = new_name;

    Configurator &configurator = Configurator::Get();
    configurator.SaveConfiguration(item->configuration);
    configurator.LoadAllConfigurations();
    LoadConfigurationList();

    // Good enough? Nope, I want to select it and edit the name.
    // Find it.
    for (int i = 0; i < ui_->profileTree->topLevelItemCount(); i++) {
        ContigurationListItem *pItem = dynamic_cast<ContigurationListItem *>(ui_->profileTree->topLevelItem(i));
        if (pItem->configuration->name == new_name) {
            ui_->profileTree->editItem(pItem, 1);
            return;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Import a configuration file. File copy followed by a reload.
void MainWindow::ImportClicked(ContigurationListItem *item) {
    (void)item;  // We don't need this

    Configurator &configurator = Configurator::Get();

    QString full_suggested_path = configurator.GetPath(Configurator::LastImportPath);
    QString full_import_path =
        QFileDialog::getOpenFileName(this, "Import Layers Configuration File", full_suggested_path, "*.json");
    if (full_import_path.isEmpty()) return;

    settings_tree_manager_.CleanupGUI();
    Configurator::Get().ImportConfiguration(full_import_path);
    LoadConfigurationList();
    configurator.SaveSettings();
}

/////////////////////////////////////////////////////////////////////////////
// Export a configuration file. Basically just a file copy
void MainWindow::ExportClicked(ContigurationListItem *item) {
    Configurator &configurator = Configurator::Get();

    // Where to put it and what to call it
    QString full_suggested_path = configurator.GetPath(Configurator::LastExportPath) + "/" + item->configuration->file;
    QString full_export_path =
        QFileDialog::getSaveFileName(this, "Export Layers Configuration File", full_suggested_path, "*.json");
    if (full_export_path.isEmpty()) return;

    configurator.ExportConfiguration(item->configuration->file, full_export_path);
    configurator.SaveSettings();
}

/////////////////////////////////////////////////////////////////////////////
// Export a configuration file. Basically just a file copy
void MainWindow::EditCustomPathsClicked(ContigurationListItem *item) {
    (void)item;
    addCustomPaths();
    LoadConfigurationList();  // Force a reload
}

void MainWindow::toolsSetCustomPaths(bool bChecked) {
    (void)bChecked;
    addCustomPaths();
    LoadConfigurationList();  // Force a reload
}

/////////////////////////////////////////////////////////////////////////////
/// Update "decorations": window caption, (Active) status in list
void MainWindow::ChangeActiveConfiguration(Configuration *configuration) {
    Configurator &configurator = Configurator::Get();

    if (configuration == nullptr || !configurator.override_active) {
        configurator.SetActiveConfiguration(nullptr);

        setWindowTitle("Vulkan Configurator <VULKAN APPLICATION CONTROLLED>");
    } else {
        QString newCaption = configuration->name;
        if (!configuration->IsValid()) newCaption += " (DISABLED)";
        newCaption += " - Vulkan Configurator ";
        configurator.SetActiveConfiguration(configuration);
        newCaption += "<VULKAN APPLICATIONS OVERRIDDEN>";

        setWindowTitle(newCaption);
    }
}

void MainWindow::editorExpanded(QTreeWidgetItem *item) {
    (void)item;
    ui_->layerSettingsTree->resizeColumnToContents(0);
}

void MainWindow::profileItemExpanded(QTreeWidgetItem *item) {
    (void)item;
    ui_->layerSettingsTree->resizeColumnToContents(0);
    ui_->layerSettingsTree->resizeColumnToContents(1);
}

void MainWindow::OnConfigurationTreeClicked(QTreeWidgetItem *item, int column) {
    (void)item;
    (void)column;
    Configurator::Get().CheckApplicationRestart();
}

void MainWindow::OnConfigurationSettingsTreeClicked(QTreeWidgetItem *item, int column) {
    (void)item;
    (void)column;

    Configurator::Get().CheckApplicationRestart();
}

///////////////////////////////////////////////////////////////////
/// Reload controls for launch control
void MainWindow::ResetLaunchOptions() {
    Configurator &configurator = Configurator::Get();
    ui_->pushButtonLaunch->setEnabled(!configurator.overridden_application_list.empty());

    // Reload launch apps selections
    launcher_apps_combo_->blockSignals(true);
    launcher_apps_combo_->clear();

    for (int i = 0; i < configurator.overridden_application_list.size(); i++) {
        launcher_apps_combo_->addItem(configurator.overridden_application_list[i]->executable_path);
    }

    if (configurator.overridden_application_list.isEmpty()) {
        launch_arguments_->setText("");
        launcher_working_->setText("");
        launcher_log_file_edit_->setText("");
        return;
    }

    int launch_application_index = configurator.GetLaunchApplicationIndex();
    Q_ASSERT(launch_application_index >= 0);

    configurator.SelectLaunchApplication(launch_application_index);
    launcher_apps_combo_->setCurrentIndex(launch_application_index);

    // Reset working folder and command line choices
    launch_arguments_->setText(configurator.overridden_application_list[launch_application_index]->arguments);
    launcher_working_->setText(configurator.overridden_application_list[launch_application_index]->working_folder);
    launcher_log_file_edit_->setText(configurator.overridden_application_list[launch_application_index]->log_file);
    launcher_apps_combo_->blockSignals(false);
}

///////////////////////////////////////////////////////////////////
/// Launch and log area
void MainWindow::SetupLaunchTree() {
    /////////////////////////////////////////////////////////////////
    // Executable
    QTreeWidgetItem *launcher_parent = new QTreeWidgetItem();
    launcher_parent->setText(0, "Executable Path");
    ui_->launchTree->addTopLevelItem(launcher_parent);

    launcher_apps_combo_ = new QComboBox();
    launcher_apps_combo_->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    launcher_apps_combo_->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui_->launchTree->setItemWidget(launcher_parent, 1, launcher_apps_combo_);

    launcher_apps_browse_button_ = new QPushButton();
    launcher_apps_browse_button_->setText("...");
    launcher_apps_browse_button_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    launcher_apps_browse_button_->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    launcher_apps_browse_button_->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    launcher_apps_browse_button_->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui_->launchTree->setItemWidget(launcher_parent, 2, launcher_apps_browse_button_);
    connect(launcher_apps_combo_, SIGNAL(currentIndexChanged(int)), this, SLOT(launchItemChanged(int)));
    connect(launcher_apps_browse_button_, SIGNAL(clicked()), this, SLOT(on_pushButtonAppList_clicked()));

    //////////////////////////////////////////////////////////////////
    // Working folder
    QTreeWidgetItem *launcher_folder_item = new QTreeWidgetItem();
    launcher_folder_item->setText(0, "Working Directory");
    launcher_parent->addChild(launcher_folder_item);

    launcher_working_ = new QLineEdit();
    launcher_working_->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    launcher_working_->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui_->launchTree->setItemWidget(launcher_folder_item, 1, launcher_working_);
    launcher_working_->setReadOnly(false);

    // Comming soon
    //    pLaunchWorkingFolderButton = new QPushButton();
    //    pLaunchWorkingFolderButton->setText("...");
    //    pLaunchWorkingFolderButton->setMinimumWidth(32);
    //    ui->launchTree->setItemWidget(pLauncherFolder, 2, pLaunchWorkingFolderButton);

    //////////////////////////////////////////////////////////////////
    // Command line arguments
    QTreeWidgetItem *launcher_arguments_item = new QTreeWidgetItem();
    launcher_arguments_item->setText(0, "Command-line Arguments");
    launcher_parent->addChild(launcher_arguments_item);

    launch_arguments_ = new QLineEdit();
    launch_arguments_->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    launch_arguments_->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui_->launchTree->setItemWidget(launcher_arguments_item, 1, launch_arguments_);
    connect(launch_arguments_, SIGNAL(textEdited(const QString &)), this, SLOT(launchArgsEdited(const QString &)));

    // Comming soon
    //    pButton = new QPushButton();
    //    pButton->setText("...");
    //    ui->launchTree->setItemWidget(pLauncherCMD, 2, pButton);

    //////////////////////////////////////////////////////////////////
    // LOG FILE
    QTreeWidgetItem *launcher_log_file_item = new QTreeWidgetItem();
    launcher_log_file_item->setText(0, "Output Log");
    launcher_parent->addChild(launcher_log_file_item);

    launcher_log_file_edit_ = new QLineEdit();
    launcher_log_file_edit_->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    launcher_log_file_edit_->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui_->launchTree->setItemWidget(launcher_log_file_item, 1, launcher_log_file_edit_);

    launcher_log_file_button_ = new QPushButton();
    launcher_log_file_button_->setText("...");
    launcher_log_file_button_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    launcher_log_file_button_->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    ui_->launchTree->setItemWidget(launcher_log_file_item, 2, launcher_log_file_button_);
    connect(launcher_log_file_button_, SIGNAL(clicked()), this, SLOT(launchSetLogFile()));

    //////////////////////////////////////////////////////////////////
    ui_->launchTree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    ui_->launchTree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);

    ui_->launchTree->setColumnWidth(0, LAUNCH_COLUMN0_SIZE);
    ui_->launchTree->setColumnWidth(
        1, ui_->launchTree->rect().width() - LAUNCH_COLUMN0_SIZE - LAUNCH_COLUMN2_SIZE - LAUNCH_SPACING_SIZE);
    ui_->launchTree->setColumnWidth(2, LAUNCH_COLUMN2_SIZE);

    if(settings_.value("launcherCollapsed").toBool())
        launchItemCollapsed(nullptr);
    else
      ui_->launchTree->expandItem(launcher_parent);

    ui_->launchTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->launchTree->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ResetLaunchOptions();
}

////////////////////////////////////////////////////////////////////
// Expanding the tree also grows the tree to match
void MainWindow::launchItemExpanded(QTreeWidgetItem *item) {
    (void)item;
    ui_->launchTree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    ui_->launchTree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    settings_.setValue("launcherCollapsed", false);

}

////////////////////////////////////////////////////////////////////
// Collapsing the tree also shrinks the tree to match and show only
// the first line
void MainWindow::launchItemCollapsed(QTreeWidgetItem *item) {
    (void)item;
    ui_->launchTree->setMinimumHeight(LAUNCH_ROW_HEIGHT + 6);
    ui_->launchTree->setMaximumHeight(LAUNCH_ROW_HEIGHT + 6);
    settings_.setValue("launcherCollapsed", true);
}

////////////////////////////////////////////////////////////////////
void MainWindow::launchSetLogFile() {
    int current_application_index = launcher_apps_combo_->currentIndex();
    Q_ASSERT(current_application_index >= 0);

    const QString log_file =
        QDir::toNativeSeparators(QFileDialog::getSaveFileName(this, tr("Set Log File To..."), ".", tr("Log text(*.txt)")));

    Configurator &configurator = Configurator::Get();
    configurator.overridden_application_list[current_application_index]->log_file = log_file;

    if (log_file.isEmpty())
        launcher_log_file_edit_->setText("");
    else
        launcher_log_file_edit_->setText(log_file);

    configurator.SaveOverriddenApplicationList();
}

////////////////////////////////////////////////////////////////////
/// Launch app change
void MainWindow::launchItemChanged(int application_index) {
    Configurator &configurator = Configurator::Get();

    if (application_index < 0 || application_index >= configurator.overridden_application_list.size()) return;

    launch_arguments_->setText(configurator.overridden_application_list[application_index]->arguments);
    launcher_working_->setText(configurator.overridden_application_list[application_index]->working_folder);
    launcher_log_file_edit_->setText(configurator.overridden_application_list[application_index]->log_file);

    configurator.SelectLaunchApplication(application_index);
    configurator.SaveSettings();
}

/////////////////////////////////////////////////////////////////////
/// New command line arguments. Update them.
void MainWindow::launchArgsEdited(const QString &arguments) {
    int application_index = launcher_apps_combo_->currentIndex();
    if (application_index < 0) return;

    Configurator &configurator = Configurator::Get();
    configurator.overridden_application_list[application_index]->arguments = arguments;
    configurator.SaveOverriddenApplicationList();
}

//////////////////////////////////////////////////////////////////////
// Clear the browser window
void MainWindow::on_pushButtonClearLog_clicked() {
    ui_->logBrowser->clear();
    ui_->logBrowser->update();
    ui_->pushButtonClearLog->setEnabled(false);
}

//////////////////////////////////////////////////////////////////////
bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    Configurator &configurator = Configurator::Get();

    // When no application are overridden in the application list,
    // we disable override_application_list_only to reflect to the
    // user that overrides will apply to all applications.
    if (configurator.override_application_list_updated && configurator.override_application_list_only) {
        configurator.override_application_list_updated = false;

        if (!configurator.HasOverriddenApplications()) {
            configurator.override_application_list_only = false;
            ui_->checkBoxApplyList->setChecked(false);
            ui_->pushButtonAppList->setEnabled(false);
        }
    }

    // Launch tree does some fancy resizing and since it's down in
    // layouts and splitters, we can't just relay on the resize method
    // of this window.
    if (target == ui_->launchTree) {
        if (event->type() == QEvent::Resize) {
            QRect rect = ui_->launchTree->rect();
            ui_->launchTree->setColumnWidth(0, LAUNCH_COLUMN0_SIZE);
            ui_->launchTree->setColumnWidth(1, rect.width() - LAUNCH_COLUMN0_SIZE - LAUNCH_COLUMN2_SIZE - LAUNCH_SPACING_SIZE);
            ui_->launchTree->setColumnWidth(2, LAUNCH_COLUMN2_SIZE);
            return false;
        }
    }

    // Context menus for layer configuration files
    if (target == ui_->profileTree) {
        QContextMenuEvent *right_click = dynamic_cast<QContextMenuEvent *>(event);
        if (right_click && event->type() == QEvent::ContextMenu) {
            // Which item were we over?
            QTreeWidgetItem *configuration_item = ui_->profileTree->itemAt(right_click->pos());
            ContigurationListItem *item = dynamic_cast<ContigurationListItem *>(configuration_item);

            // Create context menu here
            QMenu menu(ui_->profileTree);

            QAction *new_action = new QAction("New Layers Configuration...");
            new_action->setEnabled(true);
            menu.addAction(new_action);

            QAction *duplicate_action = new QAction("Duplicate the Layers Configuration");
            duplicate_action->setEnabled(item != nullptr);
            menu.addAction(duplicate_action);

            QAction *remove_action = new QAction("Remove the Layers Configuration");
            remove_action->setEnabled(item != nullptr);
            menu.addAction(remove_action);

            QAction *rename_action = new QAction("Rename the Layers Configuration");
            rename_action->setEnabled(item != nullptr);
            menu.addAction(rename_action);

            menu.addSeparator();

            QAction *import_action = new QAction("Import a Layers Configuration...");
            import_action->setEnabled(true);
            menu.addAction(import_action);

            QAction *export_action = new QAction("Export the Layers Configuration...");
            export_action->setEnabled(item != nullptr);
            menu.addAction(export_action);

            menu.addSeparator();

            QAction *custom_path_action = new QAction("Edit Layers Custom Path...");
            custom_path_action->setEnabled(true);
            menu.addAction(custom_path_action);

            QPoint point(right_click->globalX(), right_click->globalY());
            QAction *action = menu.exec(point);

            // Pointer compares made me throw up in my mouth at least a little
            // less than doing a full string compare. Setting up signal/slot for
            // all of these just seemed ridiculous. Every problem is not a nail,
            // put the hammer away....
            // New Profile...
            if (action == new_action) {
                settings_tree_manager_.CleanupGUI();
                NewClicked();
                ui_->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Duplicate
            if (action == duplicate_action) {
                settings_tree_manager_.CleanupGUI();
                DuplicateClicked(item);
                settings_tree_manager_.CleanupGUI();
                ui_->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Remove this profile....
            if (action == remove_action) {
                settings_tree_manager_.CleanupGUI();
                RemoveClicked(item);
                ui_->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Rename this profile...
            if (action == rename_action) {
                RenameClicked(item);
                settings_tree_manager_.CleanupGUI();
                ui_->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Export this profile (copy the .json)
            if (action == export_action) {
                settings_tree_manager_.CleanupGUI();
                ExportClicked(item);
                ui_->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Import a profile (copy a json)
            if (action == import_action) {
                settings_tree_manager_.CleanupGUI();
                ImportClicked(item);
                ui_->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Edit Layer custom paths
            if (action == custom_path_action) {
                settings_tree_manager_.CleanupGUI();
                EditCustomPathsClicked(item);
                ui_->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
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
void MainWindow::on_pushButtonLaunch_clicked() {
    // Are we already monitoring a running app? If so, terminate it
    if (launch_application_ != nullptr) {
        launch_application_->terminate();
        launch_application_->deleteLater();
        launch_application_ = nullptr;
        ui_->pushButtonLaunch->setText(tr("Launch"));
        return;
    }

    // Is there an app selected?
    int current_application_index = launcher_apps_combo_->currentIndex();

    // Launch the test application
    launch_application_ = new QProcess(this);
    connect(launch_application_, SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));

    connect(launch_application_, SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));

    connect(launch_application_, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processClosed(int, QProcess::ExitStatus)));

    Configurator &configurator = Configurator::Get();
    launch_application_->setProgram(configurator.overridden_application_list[current_application_index]->executable_path);
    launch_application_->setWorkingDirectory(configurator.overridden_application_list[current_application_index]->working_folder);

    if (!configurator.overridden_application_list[current_application_index]->arguments.isEmpty()) {
        const QStringList args = configurator.overridden_application_list[current_application_index]->arguments.split(" ");
        launch_application_->setArguments(args);
    }

    // Some of these may have changed
    configurator.SaveSettings();

    launch_application_->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    launch_application_->setProcessChannelMode(QProcess::MergedChannels);
    launch_application_->closeWriteChannel();

    // We are logging, let's add that we've launched a new application
    QString launch_log = "Launching Vulkan Application:\n";

    const Application &current_application = *configurator.overridden_application_list[current_application_index];

    if (configurator.GetActiveConfiguration() == nullptr) {
        launch_log += QString().asprintf("- Layers fully controlled by the application.\n");
    } else if (!configurator.GetActiveConfiguration()->IsValid()) {
        launch_log += QString().asprintf("- No layers override. The active \"%s\" configuration is missing a layer.\n",
                                         configurator.GetActiveConfiguration()->name.toUtf8().constData());
    } else if (configurator.override_active) {
        if (configurator.override_application_list_only && configurator.HasOverriddenApplications() &&
            !current_application.override_layers) {
            launch_log +=
                QString().asprintf("- Layers fully controlled by the application. Application excluded from layers override.\n");
        } else {
            launch_log += QString().asprintf("- Layers overridden by \"%s\" configuration.\n",
                                             configurator.GetActiveConfiguration()->name.toUtf8().constData());
        }
    }

    launch_log += QString().asprintf("- Executable Path: %s\n", current_application.executable_path.toUtf8().constData());
    launch_log += QString().asprintf("- Working Directory: %s\n", current_application.working_folder.toUtf8().constData());
    launch_log += QString().asprintf("- Command-line Arguments: %s\n", current_application.arguments.toUtf8().constData());
    launch_log += QString().asprintf("- Log file: %s\n", current_application.log_file.toUtf8().constData());

    if (!current_application.log_file.isEmpty()) {
        // This should never happen... but things that should never happen do in
        // fact happen... so just a sanity check.
        if (log_file_ != nullptr) {
            log_file_->close();
            log_file_ = nullptr;
        }

        // Start logging
        log_file_ = new QFile(current_application.log_file);

        // Open and append, or open and truncate?
        QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
        if (!ui_->checkBoxClearOnLaunch->isChecked()) mode |= QIODevice::Append;

        if (!current_application.log_file.isEmpty()) {
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
            log_file_->write((launch_log + "\n").toUtf8().constData(), launch_log.length());
        }
    }

    if (ui_->checkBoxClearOnLaunch->isChecked()) ui_->logBrowser->clear();
    ui_->logBrowser->append(launch_log);
    ui_->pushButtonClearLog->setEnabled(true);

    // Wait... did we start? Give it 4 seconds, more than enough time
    if (!launch_application_->waitForStarted(4000)) {
        launch_application_->waitForStarted();
        launch_application_->deleteLater();
        launch_application_ = nullptr;

        QString failed_log = QString().asprintf("Failed to launch %s!\n", current_application.executable_path.toUtf8().constData());

        ui_->logBrowser->append(failed_log);
        if (log_file_) log_file_->write(failed_log.toUtf8().constData(), failed_log.length());

        return;
    }

    // We are off to the races....
    ui_->pushButtonLaunch->setText(tr("Terminate"));
}

/////////////////////////////////////////////////////////////////////////////
/// The process we are following is closed. We don't actually care about the
/// exit status/code, we just need to know to destroy the QProcess object
/// and set it back to nullptr so that we know we can launch a new app.
/// Also, if we are logging, it's time to close the log file.
void MainWindow::processClosed(int exit_code, QProcess::ExitStatus status) {
    (void)exit_code;
    (void)status;

    // Not likely, but better to be sure...
    if (launch_application_ == nullptr) return;

    disconnect(launch_application_, SIGNAL(finished(int, QProcess::ExitStatus)), this,
               SLOT(processClosed(int, QProcess::ExitStatus)));

    disconnect(launch_application_, SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));

    disconnect(launch_application_, SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));

    ui_->pushButtonLaunch->setText(tr("Launch"));

    if (log_file_) {
        log_file_->close();
        delete log_file_;
        log_file_ = nullptr;
    }

    delete launch_application_;
    launch_application_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// This signal get's raised whenever the spawned Vulkan appliction writes
/// to stdout and there is data to be read. The layers flush after all stdout
/// writes, so we should see layer output here in realtime, as we just read
/// the string and append it to the text browser.
/// If a log file is open, we also write the output to the log.
void MainWindow::standardOutputAvailable() {
    if (launch_application_ == nullptr) return;

    QString log = launch_application_->readAllStandardOutput();
    ui_->logBrowser->append(log);
    ui_->pushButtonClearLog->setEnabled(true);

    // Are we logging?
    if (log_file_) log_file_->write(log.toUtf8().constData(), log.length());
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::errorOutputAvailable() {
    if (launch_application_ == nullptr) return;

    QString log = launch_application_->readAllStandardError();
    ui_->logBrowser->append(log);
    ui_->pushButtonClearLog->setEnabled(true);

    // Are we logging?
    if (log_file_) log_file_->write(log.toUtf8().constData(), log.length());
}
