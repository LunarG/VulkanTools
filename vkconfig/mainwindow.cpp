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

#include "../vkconfig_core/util.h"
#include "../vkconfig_core/version.h"
#include "../vkconfig_core/platform.h"
#include "../vkconfig_core/vulkan.h"

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

#if PLATFORM_LINUX || PLATFORM_MACOS
#include <unistd.h>
#endif

#include <cassert>

// This is what happens when programmers can touch type....
bool been_warned_about_old_loader = false;

static const int LAUNCH_COLUMN0_SIZE = 220;
static const int LAUNCH_COLUMN2_SIZE = 32;
static const int LAUNCH_SPACING_SIZE = 2;
#if PLATFORM_MACOS
static const int LAUNCH_ROW_HEIGHT = 26;
#else
static const int LAUNCH_ROW_HEIGHT = 28;
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      _launch_application(nullptr),
      _log_file(nullptr),
      _vk_via(nullptr),
      _vk_info(nullptr),
      _selected_configuration_item(nullptr),
      _launcher_apps_combo(nullptr),
      _launch_arguments(nullptr) {
    ui->setupUi(this);
    ui->launchTree->installEventFilter(this);
    ui->profileTree->installEventFilter(this);

    SetupLaunchTree();

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutVkConfig(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));
    connect(ui->actionHelp, SIGNAL(triggered(bool)), this, SLOT(helpShowHelp(bool)));
    connect(ui->actionVulkan_specification, SIGNAL(triggered(bool)), this, SLOT(helpShowVulkanSpec(bool)));
    connect(ui->actionVulkan_Layer_Specification, SIGNAL(triggered(bool)), this, SLOT(helpShowLayerSpec(bool)));

    connect(ui->actionCustom_Layer_Paths, SIGNAL(triggered(bool)), this, SLOT(toolsSetCustomPaths(bool)));

    connect(ui->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui->actionRestore_Default_Configurations, SIGNAL(triggered(bool)), this, SLOT(toolsResetToDefault(bool)));

    connect(ui->profileTree, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(profileItemChanged(QTreeWidgetItem *, int)));
    connect(ui->profileTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(profileTreeChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui->profileTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationTreeClicked(QTreeWidgetItem *, int)));

    connect(ui->layerSettingsTree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(editorExpanded(QTreeWidgetItem *)));
    connect(ui->layerSettingsTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationSettingsTreeClicked(QTreeWidgetItem *, int)));

    connect(ui->launchTree, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(launchItemCollapsed(QTreeWidgetItem *)));
    connect(ui->launchTree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(launchItemExpanded(QTreeWidgetItem *)));

    Configurator &configurator = Configurator::Get();
    const Environment &environment = configurator.environment;

    // Restore window geometry from last launch
    restoreGeometry(environment.Get(LAYOUT_GEOMETRY));
    restoreState(environment.Get(LAYOUT_WINDOW_STATE));
    ui->splitter->restoreState(environment.Get(LAYOUT_SPLITTER1));
    ui->splitter_2->restoreState(environment.Get(LAYOUT_SPLITTER2));
    ui->splitter_3->restoreState(environment.Get(LAYOUT_SPLITTER3));

    LoadConfigurationList();

    // We need to resetup the new profile for consistency sake.
    Configuration *current_configuration = configurator.FindConfiguration(environment.Get(ACTIVE_CONFIGURATION));
    if (environment.UseOverride()) {
        configurator.SetActiveConfiguration(current_configuration);
    }

    // All else is done, highlight and activeate the current profile on startup
    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
            ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->profileTree->topLevelItem(i));
            if (item != nullptr)
                if (&item->configuration == configuration) {  // Ding ding ding... we have a winner
                    ui->profileTree->setCurrentItem(item);
                }
        }
    }

    ui->logBrowser->append("Vulkan Development Status:");
    ui->logBrowser->append(GenerateVulkanStatus());
    ui->profileTree->scrollToItem(ui->profileTree->topLevelItem(0), QAbstractItemView::PositionAtTop);

    // Resetting this from the default prevents the log window (a QTextEdit) from overflowing.
    // Whenever the control surpasses this block count, old blocks are discarded.
    // Note: We could make this a user configurable setting down the road should this be
    // insufficinet.
    ui->logBrowser->document()->setMaximumBlockCount(2048);

    UpdateUI();
}

MainWindow::~MainWindow() { delete ui; }

static std::string GetMainWindowTitle(bool active) {
    std::string title = format("%s %s", VKCONFIG_NAME, Version::VKCONFIG.str().c_str());
    if (active) title += " <ACTIVE>";
    return title;
}

void MainWindow::UpdateUI() {
    Configurator &configurator = Configurator::Get();
    const Environment &environment = Configurator::Get().environment;
    const bool has_active_configuration =
        configurator.GetActiveConfiguration() != nullptr ? configurator.IsValid(*configurator.GetActiveConfiguration()) : false;

    // Mode states
    ui->radioOverride->setChecked(environment.UseOverride());
    ui->radioFully->setChecked(!environment.UseOverride());

    // Update configurations
    ui->groupBoxProfiles->setEnabled(environment.UseOverride());

    // Update settings
    ui->groupBoxEditor->setEnabled(environment.UseOverride() && has_active_configuration);
    ui->groupBoxEditor->setTitle(environment.Get(ACTIVE_CONFIGURATION).isEmpty()
                                     ? "Configuration Settings"
                                     : environment.Get(ACTIVE_CONFIGURATION) + " Settings");

    // Handle application lists states
    if (been_warned_about_old_loader) {
        Version loader_version;
        const bool support_application_list = configurator.SupportApplicationList(false, &loader_version);

        ui->checkBoxApplyList->setEnabled(support_application_list && environment.UseOverride());
        ui->checkBoxApplyList->setChecked(support_application_list && environment.UseApplicationListOverrideMode());
        if (!support_application_list) {
            const std::string version = GetVulkanLoaderVersion().str();
            const std::string message =
                format("The detected Vulkan loader version is %s but version 1.2.141 or newer is required", version.c_str());
            ui->checkBoxApplyList->setToolTip(message.c_str());
        }
    } else {
        ui->checkBoxApplyList->setEnabled(environment.UseOverride());
        ui->checkBoxApplyList->setChecked(environment.UseApplicationListOverrideMode());
    }
    ui->pushButtonAppList->setEnabled(ui->checkBoxApplyList->isChecked());

    _launcher_apps_combo->blockSignals(true);
    _launcher_apps_combo->clear();

    if (configurator._overridden_applications.isEmpty()) {
        _launch_arguments->setText("");
        _launcher_working->setText("");
        _launcher_log_file_edit->setText("");
    } else {
        for (int i = 0, n = configurator._overridden_applications.size(); i < n; i++) {
            _launcher_apps_combo->addItem(configurator._overridden_applications[i]->executable_path);
        }

        int launch_application_index = configurator.GetLaunchApplicationIndex();
        assert(launch_application_index >= 0);
        configurator.SelectLaunchApplication(launch_application_index);
        const Application &application = *configurator._overridden_applications[launch_application_index];

        _launcher_apps_combo->setCurrentIndex(launch_application_index);
        _launch_arguments->setText(application.arguments);
        _launcher_working->setText(application.working_folder);
        _launcher_log_file_edit->setText(application.log_file);
    }

    _launcher_apps_combo->blockSignals(false);

    // Handle persistent states
    ui->checkBoxPersistent->setEnabled(environment.UseOverride());
    ui->checkBoxPersistent->setChecked(environment.UsePersistentOverrideMode());

    // Launcher states
    const bool has_application_list = !configurator._overridden_applications.empty();
    ui->pushButtonLaunch->setEnabled(has_application_list);
    ui->pushButtonLaunch->setText(_launch_application == nullptr ? "Launch" : "Terminate");
    ui->checkBoxClearOnLaunch->setChecked(environment.Get(LAYOUT_LAUNCHER_CLEAR_ON) == "true");
    if (_launcher_working_browse_button) {
        _launcher_working_browse_button->setEnabled(has_application_list);
    }
    if (_launcher_log_file_browse_button) {
        _launcher_log_file_browse_button->setEnabled(has_application_list);
    }
    if (_launcher_apps_combo) {
        _launcher_apps_combo->setEnabled(has_application_list);
    }
    if (_launch_arguments) {
        _launch_arguments->setEnabled(has_application_list);
    }
    if (_launcher_working) {
        _launcher_working->setEnabled(has_application_list);
    }
    if (_launcher_log_file_edit) {
        _launcher_log_file_edit->setEnabled(has_application_list);
    }

    // Update title bar
    if (has_active_configuration && configurator.environment.UseOverride()) {
        setWindowTitle(GetMainWindowTitle(true).c_str());
    } else {
        setWindowTitle(GetMainWindowTitle(false).c_str());
    }
}

void MainWindow::UpdateConfiguration() {}

///////////////////////////////////////////////////////////////////////////////
// Load or refresh the list of profiles. Any profile that uses a layer that
// is not detected on the system is disabled.
void MainWindow::LoadConfigurationList() {
    // There are lots of ways into this, and in none of them
    // can we have an active editor running.
    _settings_tree_manager.CleanupGUI();
    ui->profileTree->blockSignals(true);  // No signals firing off while we do this
    ui->profileTree->clear();

    Configurator &configurator = Configurator::Get();

    const QString &active_configuration_name = configurator.environment.Get(ACTIVE_CONFIGURATION);

    for (int i = 0, n = configurator._available_configurations.size(); i < n; i++) {
        // Add to list
        ConfigurationListItem *item = new ConfigurationListItem(*configurator._available_configurations[i]);
        ui->profileTree->addTopLevelItem(item);
        item->radio_button = new QRadioButton();
#if PLATFORM_MACOS  // Mac OS does not leave enough space without this
        item->radio_button->setText(" ");
#endif
        item->radio_button->setToolTip(configurator._available_configurations[i]->_description);
        item->setText(1, configurator._available_configurations[i]->_name);

        if (!configurator.IsValid(*configurator._available_configurations[i])) {
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
            item->radio_button->setEnabled(false);
            item->radio_button->setChecked(false);
            item->radio_button->setToolTip(
                "Missing Vulkan Layer to use this configuration, try to add Custom Path to locate the layers");
        }

        // Check if this is the current config... but... depending on how we came into this
        // function, this configuration may no longer be active. So double check that. Simply,
        // if you make a current config invalid and come back in... it can't be active any
        // longer
        if (active_configuration_name == configurator._available_configurations[i]->_name) {
            if (configurator.IsValid(*configurator._available_configurations[i]))
                item->radio_button->setChecked(true);
            else
                configurator.SetActiveConfiguration(nullptr);
        }

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->profileTree->setItemWidget(item, 0, item->radio_button);
        connect(item->radio_button, SIGNAL(clicked(bool)), this, SLOT(profileItemClicked(bool)));
    }

    configurator.SetActiveConfiguration(configurator.GetActiveConfiguration());
    ui->profileTree->blockSignals(false);
    ui->profileTree->resizeColumnToContents(0);
    ui->profileTree->resizeColumnToContents(1);

    UpdateUI();
}

//////////////////////////////////////////////////////////
/// Okay, because we are using custom controls, some of
/// the signaling is not happening as expected. So, we cannot
/// always get an accurate answer to the currently selected
/// item, but we do often need to know what has been checked
/// when an event occurs. This unambigously answers that question.
ConfigurationListItem *MainWindow::GetCheckedItem() {
    // Just go through all the top level items
    for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->profileTree->topLevelItem(i));

        if (item != nullptr)
            if (item->radio_button->isChecked()) return item;
    }

    return nullptr;
}

//////////////////////////////////////////////////////////
/// Use the active profile as the override
void MainWindow::on_radioOverride_clicked() {
    Configurator &configurator = Configurator::Get();

    configurator.environment.SetMode(OVERRIDE_MODE_ACTIVE, true);

    // This just doesn't work. Make a function to look for the radio button checked.
    ConfigurationListItem *item = GetCheckedItem();
    if (item == nullptr)
        configurator.SetActiveConfiguration(nullptr);
    else
        configurator.SetActiveConfiguration(&item->configuration);

    UpdateUI();
}

//////////////////////////////////////////////////////////
// No override at all, fully controlled by the application
void MainWindow::on_radioFully_clicked() {
    Configurator &configurator = Configurator::Get();

    configurator.environment.SetMode(OVERRIDE_MODE_ACTIVE, false);
    configurator.SetActiveConfiguration(nullptr);

    UpdateUI();
}

///////////////////////////////////////////////////////////////////////
// We want to apply to just the app list... hang on there. Doe we have
// the new loader?
void MainWindow::on_checkBoxApplyList_clicked() {
    Configurator &configurator = Configurator::Get();

    // Handle old loader case
    if (!configurator.SupportApplicationList()) {
        ::been_warned_about_old_loader = true;
        UpdateUI();
        return;
    }

    configurator.environment.SetMode(OVERRIDE_MODE_LIST, ui->checkBoxApplyList->isChecked());

    // Handle the case where no application with active override is present
    const bool application_list_requires_update = !configurator.HasActiveOverriddenApplications();
    if (ui->checkBoxApplyList->isChecked() && application_list_requires_update) {
        dlgCreateAssociation dlg(this);
        dlg.exec();
    }

    // Checking the list, the configuration need to be updated to the system
    if (configurator.GetActiveConfiguration()) {
        configurator.SetActiveConfiguration(configurator.GetActiveConfiguration());
    }

    UpdateUI();
}

void MainWindow::on_checkBoxPersistent_clicked() {
    Configurator::Get().environment.SetMode(OVERRIDE_MODE_PERISTENT, ui->checkBoxPersistent->isChecked());
}

void MainWindow::on_checkBoxClearOnLaunch_clicked() {
    Configurator::Get().environment.Set(LAYOUT_LAUNCHER_CLEAR_ON, ui->checkBoxClearOnLaunch->isChecked() ? "true" : "false");
}

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
    QDir dir(configurator.path.GetPath(PATH_CONFIGURATION));
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
    configurator.environment.first_run = true;

    // Now we need to kind of restart everything
    _settings_tree_manager.CleanupGUI();
    configurator.LoadAllConfigurations();

    // Find the "Validation - Standard" configuration and make it current if we are active
    Configuration *active_configuration = configurator.FindConfiguration("Validation - Standard");
    if (configurator.environment.UseOverride()) {
        configurator.SetActiveConfiguration(active_configuration);
    }

    LoadConfigurationList();

    // Active or not, set it in the tree so we can see the settings.
    for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->profileTree->topLevelItem(i));
        if (item == nullptr) continue;
        if (&item->configuration == active_configuration) ui->profileTree->setCurrentItem(item);
    }

    configurator.UpdateDefaultApplications(true);

    ui->logBrowser->clear();
    ui->logBrowser->append("Vulkan Development Status:");
    ui->logBrowser->append(GenerateVulkanStatus());

    UpdateUI();
}

////////////////////////////////////////////////////////////////////////////
/// Thist signal actually comes from the radio button
void MainWindow::profileItemClicked(bool checked) {
    (void)checked;
    // Someone just got checked, they are now the current profile
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *item = GetCheckedItem();
    if (item == nullptr) return;

    // This appears redundant on Windows, but under linux it is needed
    // to ensure the new item is "selected"
    ui->profileTree->setCurrentItem(item);

    // Do we go ahead and activate it?
    if (Configurator::Get().environment.UseOverride()) {
        Configurator::Get().SetActiveConfiguration(&item->configuration);
    }
}

/////////////////////////////////////////////////////////////////////////////
/// An item has been changed. Check for edit of the items name (configuration name)
void MainWindow::profileItemChanged(QTreeWidgetItem *item, int column) {
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(item);
    if (configuration_item == nullptr) return;

    if (column == 1) {  // configuration name
        _settings_tree_manager.CleanupGUI();
        Configurator &configurator = Configurator::Get();

        // We are renaming the file. Things can go wrong here...
        // This is the name of the configuratin we are changing
        const QString full_path(configurator.path.GetFullPath(PATH_CONFIGURATION, configuration_item->configuration._name));

        // This is the new name we want to use
        QString new_name = configuration_item->text(1);

        // Make sure we do not have a duplicate
        Configuration *duplicate = configurator.FindConfiguration(new_name);
        if (duplicate != nullptr) {
            QMessageBox alert;
            alert.setText("This name is already taken by another configuration.");
            alert.setWindowTitle("Duplicate Name");
            alert.setIcon(QMessageBox::Warning);
            alert.exec();

            // Reset the name
            ui->profileTree->blockSignals(true);
            item->setText(1, configuration_item->configuration._name);
            ui->profileTree->blockSignals(false);
            return;
        }

        // Proceed
        remove(full_path.toUtf8().constData());
        configuration_item->configuration._name = new_name;
        const bool result = configuration_item->configuration.Save(configurator.path.GetFullPath(PATH_CONFIGURATION, new_name));
        assert(result);

        RestoreLastItem(configuration_item->configuration._name.toUtf8().constData());
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
/// This gets called with keyboard selections and clicks that do not necessarily
/// result in a radio button change (but it may). So we need to do two checks here, one
/// for the radio button, and one to change the editor/information at lower right.
void MainWindow::profileTreeChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    (void)previous;
    _settings_tree_manager.CleanupGUI();
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(current);
    if (configuration_item == nullptr) return;

    if (!Preferences::Get()._use_separated_select_and_activate) {
        configuration_item->radio_button->setChecked(true);
        Configurator::Get().SetActiveConfiguration(&configuration_item->configuration);
    }

    _settings_tree_manager.CreateGUI(ui->layerSettingsTree, &configuration_item->configuration);

    ui->layerSettingsTree->resizeColumnToContents(0);
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

    if (_vk_info == nullptr) _vk_info = new dlgVulkanInfo(this);

    _vk_info->RunTool();
}

//////////////////////////////////////////////////////////
/// Create the VulkanTools dialog if it doesn't already
/// exist & show it.
void MainWindow::toolsVulkanInstallation(bool checked) {
    (void)checked;
    if (_vk_via == nullptr) _vk_via = new dlgVulkanAnalysis(this);

    _vk_via->RunTool();
}

////////////////////////////////////////////////////////////////
/// Show help, which is just a rich text file
void MainWindow::helpShowHelp(bool checked) {
    (void)checked;
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/windows/vkconfig.html"));
}

////////////////////////////////////////////////////////////////
// Open the web browser to the Vulkan specification
void MainWindow::helpShowVulkanSpec(bool checked) {
    (void)checked;
#if PLATFORM_WINDOWS
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/windows/1.2-extensions/vkspec.html"));
#elif PLATFORM_MACOS
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/mac/1.2-extensions/vkspec.html"));
#elif PLATFORM_LINUX
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/linux/1.2-extensions/vkspec.html"));
#else
#error "Unknown platform"
#endif
}

////////////////////////////////////////////////////////////////
// Open the web browser to the Vulkan Layers specification
void MainWindow::helpShowLayerSpec(bool checked) {
    (void)checked;
#if PLATFORM_WINDOWS
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/windows/layer_configuration.html"));
#elif PLATFORM_MACOS
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/mac/layer_configuration.html"));
#elif PLATFORM_LINUX
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/linux/layer_configuration.html"));
#else
#error "Unknown platform"
#endif
}

////////////////////////////////////////////////////////////////
/// The only thing we need to do here is clear the profile if
/// the user does not want it active.
void MainWindow::closeEvent(QCloseEvent *event) {
    Configurator &configurator = Configurator::Get();

    // Alert the user to the current state of the vulkan configurator and
    // give them the option to not shutdown.
    const bool exec_event = configurator.environment.Notify(NOTIFICATION_EXIT);
    if (!exec_event) {
        event->ignore();
        return;
    }

    // If a child process is still running, destroy it
    if (_launch_application) {
        _launch_application->terminate();
        _launch_application->waitForFinished();
        delete _launch_application;
        _launch_application = nullptr;
    }

    _settings_tree_manager.CleanupGUI();
    if (!configurator.environment.UsePersistentOverrideMode()) {
        Configuration *active_configuration = configurator.GetActiveConfiguration();
        const QString active_configuration_name = active_configuration == nullptr ? "" : active_configuration->_name;
        configurator.SetActiveConfiguration(nullptr);
        configurator.environment.Set(ACTIVE_CONFIGURATION, active_configuration_name);
    }

    configurator.SaveOverriddenApplicationList();

    configurator.environment.Set(LAYOUT_GEOMETRY, saveGeometry());
    configurator.environment.Set(LAYOUT_WINDOW_STATE, saveState());
    configurator.environment.Set(LAYOUT_SPLITTER1, ui->splitter->saveState());
    configurator.environment.Set(LAYOUT_SPLITTER2, ui->splitter_2->saveState());
    configurator.environment.Set(LAYOUT_SPLITTER3, ui->splitter_3->saveState());

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
    ConfigurationListItem *item = SaveLastItem();
    if (item == nullptr) ui->groupBoxEditor->setEnabled(false);

    //  resizeEvent(nullptr); // Fake to get controls to do the right thing
    event->accept();
}

///////////////////////////////////////////////////////////////////////////////
/// Edit the list of apps that can be filtered.
void MainWindow::on_pushButtonAppList_clicked() {
    dlgCreateAssociation dlg(this);
    dlg.exec();

    Configurator &configurator = Configurator::Get();

    if (Preferences::Get()._use_last_selected_application_in_launcher) {
        configurator.SelectLaunchApplication(dlg.GetSelectedLaunchApplicationIndex());
    }

    configurator.SaveOverriddenApplicationList();

    if (configurator.GetActiveConfiguration()) {
        configurator.SetActiveConfiguration(configurator.GetActiveConfiguration());
    }

    UpdateUI();
}

///////////////////////////////////////////////////////////////////////////////
/// Just resave the list anytime we go into the editor
void MainWindow::on_pushButtonEditProfile_clicked() {
    // If not item are selected we disable the configuration settings
    ConfigurationListItem *item = SaveLastItem();
    if (item == nullptr) {
        ui->groupBoxEditor->setEnabled(false);
        return;
    }
    // Save current state before we go in
    _settings_tree_manager.CleanupGUI();

    assert(&item->configuration);
    dlgProfileEditor dlg(this, &item->configuration);
    dlg.exec();

    Configurator::Get().LoadAllConfigurations();
    LoadConfigurationList();

    RestoreLastItem();
}

///////////////////////////////////////////////////////////////
// When changes are made to the layer list, it forces a reload
// of the configuration list. This wipes everything out, so we
// need a way to restore the currently selected item whenever
// certain kinds of edits occur. These push/pop functions
// accomplish that. If nothing can be found it should simply
// leave nothing selected.
ConfigurationListItem *MainWindow::SaveLastItem() {
    // Who is selected?
    ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->profileTree->currentItem());
    if (item == nullptr) return nullptr;

    assert(&item->configuration);
    _last_item = item->configuration._name;
    return item;
}

////////////////////////////////////////////////////////////////
// Partner for above function. Returns false if the last config
// could not be found.
bool MainWindow::RestoreLastItem(const char *configuration_override) {
    if (configuration_override != nullptr) _last_item = configuration_override;

    // Reset the current item
    for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->profileTree->topLevelItem(i));
        if (item == nullptr) continue;

        assert(&item->configuration);
        if (item->configuration._name == _last_item) {
            ui->profileTree->setCurrentItem(item);
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
// Edit the layers for the given configuration.
void MainWindow::EditClicked(ConfigurationListItem *item) {
    SaveLastItem();
    _settings_tree_manager.CleanupGUI();
    dlgProfileEditor dlg(this, &item->configuration);
    dlg.exec();

    Configurator::Get().LoadAllConfigurations();
    LoadConfigurationList();
    RestoreLastItem();
}

////////////////////////////////////////////////////////////////////////////////
// Create a new blank configuration
void MainWindow::NewClicked() {
    SaveLastItem();
    _settings_tree_manager.CleanupGUI();
    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.CreateEmptyConfiguration();
    dlgProfileEditor dlg(this, configuration);
    if (QDialog::Accepted == dlg.exec()) {
        configurator.LoadAllConfigurations();
        LoadConfigurationList();
    }
    RestoreLastItem();
}

///////////////////////////////////////////////////////////////////////////////
/// Allow addition or removal of custom layer paths. Afterwards reset the list
/// of loaded layers, but only if something was changed.
void MainWindow::addCustomPaths() {
    SaveLastItem();

    // Get the tree state and clear it.
    // This looks better aesthetically after the dialog
    // but the dialog changes the pointers to the
    // configs and it will cause a crash.
    _settings_tree_manager.CleanupGUI();

    dlgCustomPaths dlg(this);
    dlg.exec();

    LoadConfigurationList();  // Force a reload
    RestoreLastItem();
}

//////////////////////////////////////////////////////////////////////////////
/// Remove the currently selected user defined profile.
// This option does not automatically select another profile when you
// delete the current one. Since it's not possible to select it without
// making it current, this is the only reasonable option I see.
void MainWindow::RemoveClicked(ConfigurationListItem *item) {
    assert(item);
    assert(&item->configuration);

    // Let make sure...
    QMessageBox msg;
    msg.setInformativeText(item->configuration._name);
    msg.setText(tr("Are you sure you want to remove this configuration?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    if (msg.exec() == QMessageBox::No) return;

    SaveLastItem();
    _settings_tree_manager.CleanupGUI();
    // What if this is the active profile? We will go boom boom soon...
    Configurator &configurator = Configurator::Get();
    if (configurator.GetActiveConfiguration() == &item->configuration) {
        configurator.SetActiveConfiguration(nullptr);
    }

    // Delete the file
    const QString full_path(configurator.path.GetFullPath(PATH_CONFIGURATION, item->configuration._name));
    remove(full_path.toUtf8().constData());

    // Reload profiles
    configurator.LoadAllConfigurations();
    LoadConfigurationList();
    RestoreLastItem();
}

/////////////////////////////////////////////////////////////////////////////
void MainWindow::RenameClicked(ConfigurationListItem *item) {
    assert(item);

    SaveLastItem();
    ui->profileTree->editItem(item, 1);
}

/////////////////////////////////////////////////////////////////////////////
// Copy the current configuration
void MainWindow::DuplicateClicked(ConfigurationListItem *item) {
    assert(item);

    SaveLastItem();
    Configurator &configurator = Configurator::Get();

    // We need a new name that is not already used. Simply append '2' until
    // it is unique.
    assert(&item->configuration);
    QString new_name = item->configuration._name;
    while (configurator.FindConfiguration(new_name) != nullptr) new_name += "2";

    _settings_tree_manager.CleanupGUI();

    item->configuration._name = new_name;
    const bool result = item->configuration.Save(configurator.path.GetFullPath(PATH_CONFIGURATION, item->configuration._name));
    assert(result);

    configurator.LoadAllConfigurations();
    LoadConfigurationList();

    // Good enough? Nope, I want to select it and edit the name.
    // Find it.
    for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->profileTree->topLevelItem(i));
        if (item->configuration._name == new_name) {
            ui->profileTree->editItem(item, 1);
            return;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Import a configuration file. File copy followed by a reload.
void MainWindow::ImportClicked(ConfigurationListItem *item) {
    (void)item;  // We don't need this
    Configurator &configurator = Configurator::Get();

    const QString full_import_path = configurator.path.SelectPath(this, PATH_IMPORT_CONFIGURATION);
    if (full_import_path.isEmpty()) return;

    SaveLastItem();
    _settings_tree_manager.CleanupGUI();
    Configurator::Get().ImportConfiguration(full_import_path);
    LoadConfigurationList();
    RestoreLastItem();
}

/////////////////////////////////////////////////////////////////////////////
// Export a configuration file. Basically just a file copy
void MainWindow::ExportClicked(ConfigurationListItem *item) {
    assert(item);

    Configurator &configurator = Configurator::Get();

    const QString full_suggested_path = configurator.path.GetFullPath(PATH_EXPORT_CONFIGURATION, item->configuration._name);
    const QString full_export_path = configurator.path.SelectPath(this, PATH_EXPORT_CONFIGURATION, full_suggested_path);
    if (full_export_path.isEmpty()) return;

    configurator.ExportConfiguration(item->configuration._name + ".json", full_export_path);
}

/////////////////////////////////////////////////////////////////////////////
// Export a configuration file. Basically just a file copy
void MainWindow::EditCustomPathsClicked(ConfigurationListItem *item) {
    (void)item;
    addCustomPaths();
}

void MainWindow::toolsSetCustomPaths(bool checked) {
    (void)checked;
    addCustomPaths();
}

void MainWindow::editorExpanded(QTreeWidgetItem *item) {
    (void)item;
    ui->layerSettingsTree->resizeColumnToContents(0);
}

void MainWindow::profileItemExpanded(QTreeWidgetItem *item) {
    (void)item;
    ui->layerSettingsTree->resizeColumnToContents(0);
    ui->layerSettingsTree->resizeColumnToContents(1);
}

void MainWindow::OnConfigurationTreeClicked(QTreeWidgetItem *item, int column) {
    (void)item;
    (void)column;
    Configurator::Get().environment.Notify(NOTIFICATION_RESTART);
}

void MainWindow::OnConfigurationSettingsTreeClicked(QTreeWidgetItem *item, int column) {
    (void)item;
    (void)column;

    Configurator::Get().environment.Notify(NOTIFICATION_RESTART);
}

///////////////////////////////////////////////////////////////////
/// Launch and log area
void MainWindow::SetupLaunchTree() {
    /////////////////////////////////////////////////////////////////
    // Executable
    QTreeWidgetItem *launcher_parent = new QTreeWidgetItem();
    launcher_parent->setText(0, "Executable Path");
    ui->launchTree->addTopLevelItem(launcher_parent);

    _launcher_apps_combo = new QComboBox();
    _launcher_apps_combo->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_apps_combo->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(launcher_parent, 1, _launcher_apps_combo);

    _launcher_apps_browse_button = new QPushButton();
    _launcher_apps_browse_button->setText("...");
    _launcher_apps_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _launcher_apps_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    _launcher_apps_browse_button->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_apps_browse_button->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(launcher_parent, 2, _launcher_apps_browse_button);
    connect(_launcher_apps_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(launchItemChanged(int)));
    connect(_launcher_apps_browse_button, SIGNAL(clicked()), this, SLOT(on_pushButtonAppList_clicked()));

    //////////////////////////////////////////////////////////////////
    // Working folder
    QTreeWidgetItem *launcher_folder_item = new QTreeWidgetItem();
    launcher_folder_item->setText(0, "Working Directory");
    launcher_parent->addChild(launcher_folder_item);

    _launcher_working = new QLineEdit();
    _launcher_working->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_working->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(launcher_folder_item, 1, _launcher_working);
    _launcher_working->setReadOnly(false);

    _launcher_working_browse_button = new QPushButton();
    _launcher_working_browse_button->setText("...");
    _launcher_working_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _launcher_working_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    _launcher_working_browse_button->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_working_browse_button->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(launcher_folder_item, 2, _launcher_working_browse_button);
    connect(_launcher_working_browse_button, SIGNAL(clicked()), this, SLOT(launchSetWorkingFolder()));

    //////////////////////////////////////////////////////////////////
    // Command line arguments
    QTreeWidgetItem *launcher_arguments_item = new QTreeWidgetItem();
    launcher_arguments_item->setText(0, "Command-line Arguments");
    launcher_parent->addChild(launcher_arguments_item);

    _launch_arguments = new QLineEdit();
    _launch_arguments->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launch_arguments->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(launcher_arguments_item, 1, _launch_arguments);
    connect(_launch_arguments, SIGNAL(textEdited(const QString &)), this, SLOT(launchArgsEdited(const QString &)));

    //////////////////////////////////////////////////////////////////
    // LOG FILE
    QTreeWidgetItem *launcher_log_file_item = new QTreeWidgetItem();
    launcher_log_file_item->setText(0, "Output Log");
    launcher_parent->addChild(launcher_log_file_item);

    _launcher_log_file_edit = new QLineEdit();
    _launcher_log_file_edit->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_log_file_edit->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(launcher_log_file_item, 1, _launcher_log_file_edit);
    connect(_launcher_log_file_edit, SIGNAL(textEdited(const QString &)), this, SLOT(launchChangeLogFile(const QString &)));
    connect(_launcher_working, SIGNAL(textEdited(const QString &)), this, SLOT(launchChangeWorkingFolder(const QString &)));

    _launcher_log_file_browse_button = new QPushButton();
    _launcher_log_file_browse_button->setText("...");
    _launcher_log_file_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _launcher_log_file_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    ui->launchTree->setItemWidget(launcher_log_file_item, 2, _launcher_log_file_browse_button);
    connect(_launcher_log_file_browse_button, SIGNAL(clicked()), this, SLOT(launchSetLogFile()));

    //////////////////////////////////////////////////////////////////
    ui->launchTree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    ui->launchTree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);

    ui->launchTree->setColumnWidth(0, LAUNCH_COLUMN0_SIZE);
    ui->launchTree->setColumnWidth(
        1, ui->launchTree->rect().width() - LAUNCH_COLUMN0_SIZE - LAUNCH_COLUMN2_SIZE - LAUNCH_SPACING_SIZE);
    ui->launchTree->setColumnWidth(2, LAUNCH_COLUMN2_SIZE);

    if (Configurator::Get().environment.Get(LAYOUT_LAUNCHER_COLLAPSED) == "true")
        launchItemCollapsed(nullptr);
    else
        ui->launchTree->expandItem(launcher_parent);

    ui->launchTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->launchTree->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

////////////////////////////////////////////////////////////////////
// Expanding the tree also grows the tree to match
void MainWindow::launchItemExpanded(QTreeWidgetItem *item) {
    (void)item;
    ui->launchTree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    ui->launchTree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    Configurator::Get().environment.Set(LAYOUT_LAUNCHER_COLLAPSED, QByteArray("false"));
}

////////////////////////////////////////////////////////////////////
// Collapsing the tree also shrinks the tree to match and show only
// the first line
void MainWindow::launchItemCollapsed(QTreeWidgetItem *item) {
    (void)item;
    ui->launchTree->setMinimumHeight(LAUNCH_ROW_HEIGHT + 6);
    ui->launchTree->setMaximumHeight(LAUNCH_ROW_HEIGHT + 6);
    Configurator::Get().environment.Set(LAYOUT_LAUNCHER_COLLAPSED, QByteArray("true"));
}

void MainWindow::launchSetLogFile() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();

    Application *application = configurator._overridden_applications[current_application_index];
    const QString path = configurator.path.SelectPath(this, PATH_LAUNCHER_LOG_FILE, application->log_file);

    // The user has cancel the operation
    if (path.isEmpty()) return;

    application->log_file = path;
    _launcher_log_file_edit->setText(path);

    configurator.SaveOverriddenApplicationList();
}

void MainWindow::launchSetWorkingFolder() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();

    Application *application = configurator._overridden_applications[current_application_index];
    const QString path = configurator.path.SelectPath(this, PATH_EXECUTABLE, application->working_folder);

    // The user has cancel the operation
    if (path.isEmpty()) return;

    application->working_folder = path;
    _launcher_working->setText(path);

    configurator.SaveOverriddenApplicationList();
}

///////////////////////////////////////////////////////////////////
// Log file path edited manually.
void MainWindow::launchChangeLogFile(const QString &log_file) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    configurator._overridden_applications[current_application_index]->log_file = log_file;
    configurator.SaveOverriddenApplicationList();
}

////////////////////////////////////////////////////////////////////
void MainWindow::launchChangeWorkingFolder(const QString &working_folder) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    configurator._overridden_applications[current_application_index]->working_folder = working_folder;
    configurator.SaveOverriddenApplicationList();
}

////////////////////////////////////////////////////////////////////
/// Launch app change
void MainWindow::launchItemChanged(int application_index) {
    Configurator &configurator = Configurator::Get();

    if (application_index < 0 || application_index >= configurator._overridden_applications.size()) return;

    _launch_arguments->setText(configurator._overridden_applications[application_index]->arguments);
    _launcher_working->setText(configurator._overridden_applications[application_index]->working_folder);
    _launcher_log_file_edit->setText(configurator._overridden_applications[application_index]->log_file);

    configurator.SelectLaunchApplication(application_index);
}

/////////////////////////////////////////////////////////////////////
/// New command line arguments. Update them.
void MainWindow::launchArgsEdited(const QString &arguments) {
    int application_index = _launcher_apps_combo->currentIndex();
    if (application_index < 0) return;

    Configurator &configurator = Configurator::Get();
    configurator._overridden_applications[application_index]->arguments = arguments;
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
    UpdateUI();

    // Launch tree does some fancy resizing and since it's down in
    // layouts and splitters, we can't just rely on the resize method
    // of this window. Any resize coming through needs to trigger this
    // or we get drawing artifacts on macOS.
    if (event->type() == QEvent::Resize) {
        QRect rect = ui->launchTree->rect();
        ui->launchTree->setColumnWidth(0, LAUNCH_COLUMN0_SIZE);
        ui->launchTree->setColumnWidth(1, rect.width() - LAUNCH_COLUMN0_SIZE - LAUNCH_COLUMN2_SIZE - LAUNCH_SPACING_SIZE);
        ui->launchTree->setColumnWidth(2, LAUNCH_COLUMN2_SIZE);
        return false;
    }

    // Context menus for layer configuration files
    if (target == ui->profileTree) {
        QContextMenuEvent *right_click = dynamic_cast<QContextMenuEvent *>(event);
        if (right_click) {  // && event->type() == QEvent::ContextMenu) {
            // Which item were we over?
            QTreeWidgetItem *configuration_item = ui->profileTree->itemAt(right_click->pos());
            ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(configuration_item);

            // Create context menu here
            QMenu menu(ui->profileTree);

            QAction *new_action = new QAction("New...", nullptr);
            new_action->setEnabled(true);
            menu.addAction(new_action);

            menu.addSeparator();

            QAction *duplicate_action = new QAction("Duplicate", nullptr);
            duplicate_action->setEnabled(item != nullptr);
            menu.addAction(duplicate_action);

            QAction *remove_action = new QAction("Remove", nullptr);
            remove_action->setEnabled(item != nullptr);
            menu.addAction(remove_action);

            QAction *rename_action = new QAction("Rename", nullptr);
            rename_action->setEnabled(item != nullptr);
            menu.addAction(rename_action);

            menu.addSeparator();

            QAction *import_action = new QAction("Import...", nullptr);
            import_action->setEnabled(true);
            menu.addAction(import_action);

            QAction *export_action = new QAction("Export...", nullptr);
            export_action->setEnabled(item != nullptr);
            menu.addAction(export_action);

            menu.addSeparator();

            QAction *edit_action = new QAction("Select Layers...", nullptr);
            edit_action->setEnabled(item != nullptr);
            menu.addAction(edit_action);

            menu.addSeparator();

            QAction *custom_path_action = new QAction("Edit Layers Custom Path...", nullptr);
            custom_path_action->setEnabled(true);
            menu.addAction(custom_path_action);

            QPoint point(right_click->globalX(), right_click->globalY());
            QAction *action = menu.exec(point);

            // Pointer compares made me throw up in my mouth at least a little
            // less than doing a full string compare. Setting up signal/slot for
            // all of these just seemed ridiculous. Every problem is not a nail,
            // put the hammer away....

            // Edit the selected profile
            if (action == edit_action) {
                EditClicked(item);
                return true;
            }

            // New Profile...
            if (action == new_action) {
                NewClicked();
                return true;
            }

            // Duplicate
            if (action == duplicate_action) {
                DuplicateClicked(item);
                return true;
            }

            // Remove this profile....
            if (action == remove_action) {
                RemoveClicked(item);
                return true;
            }

            // Rename this profile...
            if (action == rename_action) {
                RenameClicked(item);
                return true;
            }

            // Export this profile (copy the .json)
            if (action == export_action) {
                ExportClicked(item);
                return true;
            }

            // Import a profile (copy a json)
            if (action == import_action) {
                ImportClicked(item);
                return true;
            }

            // Edit Layer custom paths
            if (action == custom_path_action) {
                EditCustomPathsClicked(item);
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
    if (_launch_application != nullptr) {
        _launch_application->terminate();
        _launch_application->waitForFinished();

        // This get's deleted by the slot called on termination.
        // do not delete it here.
        _launch_application = nullptr;
        // ui->pushButtonLaunch->setText("Launch");

        Log("Process terminated");

        if (_log_file.isOpen()) {
            _log_file.close();
        }

        UpdateUI();
        return;
    }

    // We are logging, let's add that we've launched a new application
    QString launch_log = "Launching Vulkan Application:\n";

    Configurator &configurator = Configurator::Get();
    int current_application_index = _launcher_apps_combo->currentIndex();
    const Application &current_application = *configurator._overridden_applications[current_application_index];

    Configuration *configuration = configurator.GetActiveConfiguration();

    if (configuration == nullptr) {
        launch_log += "- Layers fully controlled by the application.\n";
    } else if (!configurator.IsValid(*configuration)) {
        launch_log += QString().asprintf("- No layers override. The active \"%s\" configuration is missing a layer.\n",
                                         configuration->_name.toUtf8().constData());
    } else if (configurator.environment.UseOverride()) {
        if (configurator.environment.UseApplicationListOverrideMode() && configurator.HasActiveOverriddenApplications() &&
            !current_application.override_layers) {
            launch_log += "- Layers fully controlled by the application. Application excluded from layers override.\n";
        } else {
            launch_log +=
                QString().asprintf("- Layers overridden by \"%s\" configuration.\n", configuration->_name.toUtf8().constData());
        }
    }

    assert(!current_application.executable_path.isEmpty());
    launch_log += QString().asprintf("- Executable Path: %s\n", current_application.executable_path.toUtf8().constData());
    assert(!current_application.working_folder.isEmpty());
    launch_log += QString().asprintf("- Working Directory: %s\n", current_application.working_folder.toUtf8().constData());
    if (!current_application.arguments.isEmpty())
        launch_log += QString().asprintf("- Command-line Arguments: %s\n", current_application.arguments.toUtf8().constData());
    if (!current_application.log_file.isEmpty())
        launch_log += QString().asprintf("- Log file: %s\n", current_application.log_file.toUtf8().constData());

    if (!current_application.log_file.isEmpty()) {
        // Start logging
        // Make sure the log file is not already opened. This can occur if the
        // launched application is closed from the applicaiton.
        if (!_log_file.isOpen()) {
            _log_file.setFileName(current_application.log_file);

            // Open and append, or open and truncate?
            QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
            if (!ui->checkBoxClearOnLaunch->isChecked()) mode |= QIODevice::Append;

            if (!_log_file.open(mode)) {
                QMessageBox err;
                err.setText("Cannot open log file");
                err.setIcon(QMessageBox::Warning);
                err.exec();
            }
        }
    }

    if (ui->checkBoxClearOnLaunch->isChecked()) ui->logBrowser->clear();
    Log(launch_log);

    // Launch the test application
    _launch_application = new QProcess(this);
    connect(_launch_application, SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));
    connect(_launch_application, SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));
    connect(_launch_application, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processClosed(int, QProcess::ExitStatus)));

    _launch_application->setProgram(configurator._overridden_applications[current_application_index]->executable_path);
    _launch_application->setWorkingDirectory(configurator._overridden_applications[current_application_index]->working_folder);

    if (!current_application.arguments.isEmpty()) {
        const QStringList args = current_application.arguments.split(" ");
        _launch_application->setArguments(args);
    }

    _launch_application->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    _launch_application->setProcessChannelMode(QProcess::MergedChannels);
    _launch_application->closeWriteChannel();

    // Wait... did we start? Give it 4 seconds, more than enough time
    if (!_launch_application->waitForStarted(4000)) {
        _launch_application->deleteLater();
        _launch_application = nullptr;

        const QString failed_log = QString("Failed to launch ") + current_application.executable_path + "!\n";
        Log(failed_log);
        UpdateUI();
        return;
    }

    // We are off to the races....
    // ui->pushButtonLaunch->setText("Terminate");
    UpdateUI();
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
    if (_launch_application == nullptr) return;

    disconnect(_launch_application, SIGNAL(finished(int, QProcess::ExitStatus)), this,
               SLOT(processClosed(int, QProcess::ExitStatus)));
    disconnect(_launch_application, SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));
    disconnect(_launch_application, SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));

    // ui->pushButtonLaunch->setText(tr("Launch"));

    delete _launch_application;
    _launch_application = nullptr;

    UpdateUI();
}

////////////////////////////////////////////////////////////////////////////////
/// This signal get's raised whenever the spawned Vulkan appliction writes
/// to stdout and there is data to be read. The layers flush after all stdout
/// writes, so we should see layer output here in realtime, as we just read
/// the string and append it to the text browser.
/// If a log file is open, we also write the output to the log.
void MainWindow::standardOutputAvailable() {
    if (_launch_application == nullptr) return;

    Log(_launch_application->readAllStandardOutput());
    ui->pushButtonClearLog->setEnabled(true);
}

void MainWindow::errorOutputAvailable() { standardOutputAvailable(); }

void MainWindow::Log(const QString &log) {
    ui->logBrowser->append(log);
    ui->pushButtonClearLog->setEnabled(true);

    if (_log_file.isOpen()) {
        _log_file.write(log.toUtf8().constData(), log.length());
        _log_file.flush();
    }
}
