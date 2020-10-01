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

#include "alert.h"

#include "dialog_about.h"
#include "dialog_vulkan_analysis.h"
#include "dialog_vulkan_info.h"
#include "dialog_layers.h"
#include "dialog_applications.h"
#include "dialog_custom_paths.h"

#include "configurator.h"
#include "vulkan.h"

#include "../vkconfig_core/util.h"
#include "../vkconfig_core/version.h"
#include "../vkconfig_core/platform.h"

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
      ui(std::make_unique<Ui::MainWindow>()),
      _launch_application(nullptr),
      _log_file(nullptr),
      _launcher_apps_combo(nullptr),
      _launcher_arguments(nullptr) {
    ui->setupUi(this);
    ui->launcher_tree->installEventFilter(this);
    ui->configuration_tree->installEventFilter(this);

    SetupLauncherTree();

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutVkConfig(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));
    connect(ui->actionHelp, SIGNAL(triggered(bool)), this, SLOT(helpShowHelp(bool)));
    connect(ui->actionVulkan_specification, SIGNAL(triggered(bool)), this, SLOT(helpShowVulkanSpec(bool)));
    connect(ui->actionVulkan_Layer_Specification, SIGNAL(triggered(bool)), this, SLOT(helpShowLayerSpec(bool)));

    connect(ui->actionCustom_Layer_Paths, SIGNAL(triggered(bool)), this, SLOT(toolsSetCustomPaths(bool)));

    connect(ui->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui->actionRestore_Default_Configurations, SIGNAL(triggered(bool)), this, SLOT(toolsResetToDefault(bool)));

    connect(ui->configuration_tree, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationItemChanged(QTreeWidgetItem *, int)));
    connect(ui->configuration_tree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(OnConfigurationTreeChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui->configuration_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationTreeClicked(QTreeWidgetItem *, int)));

    connect(ui->settings_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(editorExpanded(QTreeWidgetItem *)));
    connect(ui->settings_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnSettingsTreeClicked(QTreeWidgetItem *, int)));

    connect(ui->launcher_tree, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(launchItemCollapsed(QTreeWidgetItem *)));
    connect(ui->launcher_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(launchItemExpanded(QTreeWidgetItem *)));

    Configurator &configurator = Configurator::Get();
    const Environment &environment = configurator.environment;

    // Restore window geometry from last launch
    restoreGeometry(environment.Get(LAYOUT_MAIN_GEOMETRY));
    restoreState(environment.Get(LAYOUT_MAIN_WINDOW_STATE));
    ui->splitter->restoreState(environment.Get(LAYOUT_MAIN_SPLITTER1));
    ui->splitter_2->restoreState(environment.Get(LAYOUT_MAIN_SPLITTER2));
    ui->splitter_3->restoreState(environment.Get(LAYOUT_MAIN_SPLITTER3));

    // We need to resetup the new profile for consistency sake.
    auto current_configuration = FindConfiguration(configurator.available_configurations, environment.Get(ACTIVE_CONFIGURATION));
    if (environment.UseOverride()) {
        configurator.SetActiveConfiguration(current_configuration);
    }

    LoadConfigurationList();

    // Resetting this from the default prevents the log window (a QTextEdit) from overflowing.
    // Whenever the control surpasses this block count, old blocks are discarded.
    // Note: We could make this a user configurable setting down the road should this be
    // insufficinet.
    ui->log_browser->document()->setMaximumBlockCount(2048);
    ui->log_browser->append("Vulkan Development Status:");
    ui->log_browser->append(GenerateVulkanStatus());
    ui->configuration_tree->scrollToItem(ui->configuration_tree->topLevelItem(0), QAbstractItemView::PositionAtTop);

    if (current_configuration != configurator.available_configurations.end()) {
        _settings_tree_manager.CreateGUI(ui->settings_tree, &(*current_configuration));
    }

    UpdateUI();
}

MainWindow::~MainWindow() { ResetLaunchApplication(); }

static std::string GetMainWindowTitle(bool active) {
    std::string title = format("%s %s", VKCONFIG_NAME, Version::VKCONFIG.str().c_str());
    if (active) title += " <ACTIVE>";
    return title;
}

void MainWindow::UpdateUI() {
    Configurator &configurator = Configurator::Get();
    const Environment &environment = Configurator::Get().environment;
    const bool has_active_configuration = configurator.HasActiveConfiguration();
    const QString &active_contiguration_name = environment.Get(ACTIVE_CONFIGURATION);

    ui->configuration_tree->blockSignals(true);

    // Mode states
    ui->radio_override->setChecked(environment.UseOverride());
    ui->radio_fully->setChecked(!environment.UseOverride());

    // Update configurations
    ui->group_box_configurations->setEnabled(environment.UseOverride());

    for (int i = 0, n = ui->configuration_tree->topLevelItemCount(); i < n; ++i) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->configuration_tree->topLevelItem(i));
        assert(item);
        assert(!item->configuration_name.isEmpty());

        auto configuration = FindConfiguration(configurator.available_configurations, item->configuration_name);
        if (configuration == configurator.available_configurations.end()) continue;

        if (!HasMissingParameter(configuration->parameters, configurator.available_layers)) {
            item->setText(1, item->configuration_name);
            item->radio_button->setToolTip(configuration->_description);
        } else {
            item->setText(1, item->configuration_name + " (Invalid)");
            item->radio_button->setToolTip(
                "Missing Vulkan Layer to use this configuration, try to add Custom Path to locate the layers");
        }

        if (&item->configuration_name == active_contiguration_name) {
            ui->configuration_tree->setCurrentItem(item);
            item->radio_button->setChecked(true);
        }
    }

    // Update settings
    ui->settings_tree->setEnabled(environment.UseOverride() && has_active_configuration);
    ui->group_box_settings->setTitle(active_contiguration_name.isEmpty() ? "Configuration Settings"
                                                                         : active_contiguration_name + " Settings");

    // Handle application lists states
    if (been_warned_about_old_loader) {
        Version loader_version;
        const bool support_application_list = configurator.SupportApplicationList(false, &loader_version);

        ui->check_box_apply_list->setEnabled(support_application_list && environment.UseOverride());
        ui->check_box_apply_list->setChecked(support_application_list && environment.UseApplicationListOverrideMode());
        if (!support_application_list) {
            const std::string version = GetVulkanLoaderVersion().str();
            const std::string message =
                format("The detected Vulkan loader version is %s but version 1.2.141 or newer is required", version.c_str());
            ui->check_box_apply_list->setToolTip(message.c_str());
        }
    } else {
        ui->check_box_apply_list->setEnabled(environment.UseOverride());
        ui->check_box_apply_list->setChecked(environment.UseApplicationListOverrideMode());
    }
    ui->push_button_applications->setEnabled(ui->check_box_apply_list->isChecked());

    _launcher_apps_combo->blockSignals(true);
    _launcher_apps_combo->clear();

    const std::vector<Application> &applications = environment.GetApplications();
    if (applications.empty()) {
        _launcher_arguments->setText("");
        _launcher_working->setText("");
        _launcher_log_file_edit->setText("");
    } else {
        for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
            _launcher_apps_combo->addItem(applications[i].executable_path);
        }
        _launcher_apps_combo->setCurrentIndex(environment.GetActiveApplicationIndex());

        const Application &application = environment.GetActiveApplication();
        _launcher_arguments->setText(application.arguments);
        _launcher_working->setText(application.working_folder);
        _launcher_log_file_edit->setText(application.log_file);
    }

    _launcher_apps_combo->blockSignals(false);

    // Handle persistent states
    ui->check_box_persistent->setEnabled(environment.UseOverride());
    ui->check_box_persistent->setChecked(environment.UsePersistentOverrideMode());

    // Launcher states
    const bool has_application_list = !environment.GetApplications().empty();
    ui->push_button_launcher->setEnabled(has_application_list);
    ui->push_button_launcher->setText(_launch_application ? "Terminate" : "Launch");
    ui->check_box_clear_on_launch->setChecked(environment.Get(LAYOUT_LAUNCHER_NOT_CLEAR) != "true");
    if (_launcher_working_browse_button) {
        _launcher_working_browse_button->setEnabled(has_application_list);
    }
    if (_launcher_log_file_browse_button) {
        _launcher_log_file_browse_button->setEnabled(has_application_list);
    }
    if (_launcher_apps_combo) {
        _launcher_apps_combo->setEnabled(has_application_list);
    }
    if (_launcher_arguments) {
        _launcher_arguments->setEnabled(has_application_list);
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

    ui->configuration_tree->blockSignals(false);
}

void MainWindow::UpdateConfiguration() {}

///////////////////////////////////////////////////////////////////////////////
// Load or refresh the list of profiles. Any profile that uses a layer that
// is not detected on the system is disabled.
void MainWindow::LoadConfigurationList() {
    // There are lots of ways into this, and in none of them
    // can we have an active editor running.
    _settings_tree_manager.CleanupGUI();
    ui->configuration_tree->blockSignals(true);  // No signals firing off while we do this
    ui->configuration_tree->clear();

    Configurator &configurator = Configurator::Get();

    for (std::size_t i = 0, n = configurator.available_configurations.size(); i < n; i++) {
        const Configuration &configuration = configurator.available_configurations[i];

        ConfigurationListItem *item = new ConfigurationListItem(configuration.name);
        ui->configuration_tree->addTopLevelItem(item);
        item->radio_button = new QRadioButton();
#if PLATFORM_MACOS  // Mac OS does not leave enough space without this
        item->radio_button->setText(" ");
#endif
        item->radio_button->setToolTip(configuration._description);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->configuration_tree->setItemWidget(item, 0, item->radio_button);
        connect(item->radio_button, SIGNAL(clicked(bool)), this, SLOT(OnConfigurationItemClicked(bool)));
    }

    ui->configuration_tree->blockSignals(false);
    ui->configuration_tree->resizeColumnToContents(0);
    ui->configuration_tree->resizeColumnToContents(1);

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
    for (int i = 0; i < ui->configuration_tree->topLevelItemCount(); i++) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->configuration_tree->topLevelItem(i));

        if (item != nullptr)
            if (item->radio_button->isChecked()) return item;
    }

    return nullptr;
}

//////////////////////////////////////////////////////////
/// Use the active profile as the override
void MainWindow::on_radio_override_clicked() {
    Configurator &configurator = Configurator::Get();

    configurator.environment.SetMode(OVERRIDE_MODE_ACTIVE, true);

    // This just doesn't work. Make a function to look for the radio button checked.
    ConfigurationListItem *item = GetCheckedItem();
    auto configuration = item == nullptr ? configurator.available_configurations.end()
                                         : FindConfiguration(configurator.available_configurations, item->configuration_name);

    configurator.SetActiveConfiguration(configuration);

    UpdateUI();
}

//////////////////////////////////////////////////////////
// No override at all, fully controlled by the application
void MainWindow::on_radio_fully_clicked() {
    Configurator &configurator = Configurator::Get();

    configurator.environment.SetMode(OVERRIDE_MODE_ACTIVE, false);
    configurator.SetActiveConfiguration(configurator.available_configurations.end());

    UpdateUI();
}

///////////////////////////////////////////////////////////////////////
// We want to apply to just the app list... hang on there. Doe we have
// the new loader?
void MainWindow::on_check_box_apply_list_clicked() {
    Configurator &configurator = Configurator::Get();

    // Handle old loader case
    if (!configurator.SupportApplicationList()) {
        ::been_warned_about_old_loader = true;
        UpdateUI();
        return;
    }

    configurator.environment.SetMode(OVERRIDE_MODE_LIST, ui->check_box_apply_list->isChecked());

    // Handle the case where no application with active override is present
    const bool application_list_requires_update = !configurator.environment.HasOverriddenApplications();
    if (ui->check_box_apply_list->isChecked() && application_list_requires_update) {
        ApplicationsDialog dialog(this);
        dialog.exec();
    }

    configurator.RefreshConfiguration();

    UpdateUI();
}

void MainWindow::on_check_box_persistent_clicked() {
    Configurator::Get().environment.SetMode(OVERRIDE_MODE_PERISTENT, ui->check_box_persistent->isChecked());
}

void MainWindow::on_check_box_clear_on_launch_clicked() {
    Configurator::Get().environment.Set(LAYOUT_LAUNCHER_NOT_CLEAR, ui->check_box_clear_on_launch->isChecked() ? "false" : "true");
}

void MainWindow::toolsResetToDefault(bool checked) {
    (void)checked;

    // Let make sure...
    QMessageBox alert;
    alert.setIcon(QMessageBox::Warning);
    alert.setWindowTitle("Restoring and Resetting all Layers Configurations to default");
    alert.setText(
        "You are about to delete all the user-defined configurations and resetting all default configurations to their default "
        "state.");
    alert.setInformativeText("Are you sure you want to continue?");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::Yes);
    if (alert.exec() == QMessageBox::No) return;

    _settings_tree_manager.CleanupGUI();

    Configurator &configurator = Configurator::Get();
    configurator.ResetDefaultsConfigurations();

    // Find the "Validation - Standard" configuration and make it current if we are active
    auto active_configuration =
        FindConfiguration(configurator.available_configurations, configurator.environment.Get(ACTIVE_CONFIGURATION));
    if (configurator.environment.UseOverride()) {
        configurator.SetActiveConfiguration(active_configuration);
    }

    LoadConfigurationList();

    if (active_configuration != configurator.available_configurations.end()) {
        _settings_tree_manager.CreateGUI(ui->settings_tree, &(*active_configuration));
    }

    ui->log_browser->clear();
    ui->log_browser->append("Vulkan Development Status:");
    ui->log_browser->append(GenerateVulkanStatus());

    UpdateUI();
}

////////////////////////////////////////////////////////////////////////////
/// Thist signal actually comes from the radio button
void MainWindow::OnConfigurationItemClicked(bool checked) {
    (void)checked;
    // Someone just got checked, they are now the current profile
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *item = GetCheckedItem();
    if (item == nullptr) return;

    // This appears redundant on Windows, but under linux it is needed
    // to ensure the new item is "selected"
    ui->configuration_tree->setCurrentItem(item);

    Configurator &configurator = Configurator::Get();
    configurator.SetActiveConfiguration(FindConfiguration(configurator.available_configurations, item->configuration_name));
}

/////////////////////////////////////////////////////////////////////////////
/// An item has been changed. Check for edit of the items name (configuration name)
void MainWindow::OnConfigurationItemChanged(QTreeWidgetItem *item, int column) {
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(item);
    if (configuration_item == nullptr) return;

    if (column == 1) {  // configuration name
        _settings_tree_manager.CleanupGUI();
        Configurator &configurator = Configurator::Get();

        // We are renaming the file. Things can go wrong here...
        // This is the name of the configuratin we are changing
        const QString full_path(configurator.path.GetFullPath(PATH_CONFIGURATION, configuration_item->configuration_name));

        // This is the new name we want to use for the configuration
        const QString &new_configuration_name = configuration_item->text(1);

        if (new_configuration_name.isEmpty()) {
            Alert::ConfigurationNameEmpty();
        }

        auto end = configurator.available_configurations.end();
        auto duplicate_configuration = new_configuration_name.isEmpty()
                                           ? end
                                           : FindConfiguration(configurator.available_configurations, new_configuration_name);
        if (duplicate_configuration != end) {
            Alert::ConfigurationRenamingFailed();
        }

        // Find existing configuration using it's old name
        auto configuration = FindConfiguration(configurator.available_configurations, configuration_item->configuration_name);

        if (new_configuration_name.isEmpty() || duplicate_configuration != end) {
            // If the configurate name is empty or the configuration name is taken, keep old configuration name

            ui->configuration_tree->blockSignals(true);
            item->setText(1, configuration_item->configuration_name);
            ui->configuration_tree->blockSignals(false);
        } else {
            // Rename configuration ; Remove old configuration file ; Create new configuration file

            configuration->name = configuration_item->configuration_name = new_configuration_name;

            remove(full_path.toUtf8().constData());
            const bool result = configuration->Save(configurator.path.GetFullPath(PATH_CONFIGURATION, new_configuration_name));
            assert(result);
        }

        configurator.SetActiveConfiguration(configuration);

        _settings_tree_manager.CreateGUI(ui->settings_tree, &(*configuration));
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
/// This gets called with keyboard selections and clicks that do not necessarily
/// result in a radio button change (but it may). So we need to do two checks here, one
/// for the radio button, and one to change the editor/information at lower right.
void MainWindow::OnConfigurationTreeChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    (void)previous;
    _settings_tree_manager.CleanupGUI();
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(current);
    if (configuration_item == nullptr) return;

    configuration_item->radio_button->setChecked(true);
    auto configuration = FindConfiguration(Configurator::Get().available_configurations, configuration_item->configuration_name);
    Configurator::Get().SetActiveConfiguration(configuration);

    _settings_tree_manager.CreateGUI(ui->settings_tree, &(*configuration));

    ui->settings_tree->resizeColumnToContents(0);
}

////////////////////////////////////////////////////
// Unused flag, just display the about Qt dialog
void MainWindow::aboutVkConfig(bool checked) {
    (void)checked;

    AboutDialog dlg(this);
    dlg.exec();
}

//////////////////////////////////////////////////////////
/// Create the VulkanInfo dialog if it doesn't already
/// exits & show it.
void MainWindow::toolsVulkanInfo(bool checked) {
    (void)checked;

    vk_info_dialog.reset(new VulkanInfoDialog(this));
}

//////////////////////////////////////////////////////////
/// Create the VulkanTools dialog if it doesn't already
/// exist & show it.
void MainWindow::toolsVulkanInstallation(bool checked) {
    (void)checked;

    vk_installation_dialog.reset(new VulkanAnalysisDialog(this));
}

void MainWindow::helpShowHelp(bool checked) {
    (void)checked;
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/windows/vkconfig.html"));
}

void MainWindow::helpShowVulkanSpec(bool checked) {
    (void)checked;
    if (PLATFORM_WINDOWS)
        QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/windows/1.2-extensions/vkspec.html"));
    else if (PLATFORM_MACOS)
        QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/mac/1.2-extensions/vkspec.html"));
    else if (PLATFORM_LINUX)
        QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/linux/1.2-extensions/vkspec.html"));
    else
        assert(0);  //"Unknown platform"
}

void MainWindow::helpShowLayerSpec(bool checked) {
    (void)checked;
    if (PLATFORM_WINDOWS)
        QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/windows/layer_configuration.html"));
    else if (PLATFORM_MACOS)
        QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/mac/layer_configuration.html"));
    else if (PLATFORM_LINUX)
        QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/linux/layer_configuration.html"));
    else
        assert(0);  //"Unknown platform"
}

/// The only thing we need to do here is clear the configuration if
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
        ResetLaunchApplication();
    }

    _settings_tree_manager.CleanupGUI();
    if (!configurator.environment.UsePersistentOverrideMode()) {
        auto active_configuration = configurator.GetActiveConfiguration();
        const QString active_configuration_name =
            active_configuration == configurator.available_configurations.end() ? "" : active_configuration->name;
        configurator.SetActiveConfiguration(configurator.available_configurations.end());
        configurator.environment.Set(ACTIVE_CONFIGURATION, active_configuration_name);
    }

    configurator.environment.Set(LAYOUT_MAIN_GEOMETRY, saveGeometry());
    configurator.environment.Set(LAYOUT_MAIN_WINDOW_STATE, saveState());
    configurator.environment.Set(LAYOUT_MAIN_SPLITTER1, ui->splitter->saveState());
    configurator.environment.Set(LAYOUT_MAIN_SPLITTER2, ui->splitter_2->saveState());
    configurator.environment.Set(LAYOUT_MAIN_SPLITTER3, ui->splitter_3->saveState());

    QMainWindow::closeEvent(event);
}

/// Resizing needs a little help. Yes please, there has to be
/// a better way of doing this.
void MainWindow::resizeEvent(QResizeEvent *event) {
    if (event != nullptr) event->accept();
}

void MainWindow::showEvent(QShowEvent *event) {
    (void)event;

    SaveLastItem();

    UpdateUI();

    event->accept();
}

/// Edit the list of apps that can be filtered.
void MainWindow::on_push_button_applications_clicked() {
    ApplicationsDialog dlg(this);
    dlg.exec();

    Configurator::Get().RefreshConfiguration();

    UpdateUI();
}

/// Just resave the list anytime we go into the editor
void MainWindow::on_push_button_select_configuration_clicked() {
    ConfigurationListItem *item = SaveLastItem();
    if (item == nullptr) return;

    Configurator &configurator = Configurator::Get();

    // Save current state before we go in
    _settings_tree_manager.CleanupGUI();

    auto configuration = configurator.GetActiveConfiguration();
    assert(configuration != configurator.available_configurations.end());

    LayersDialog dlg(this, *configuration);
    dlg.exec();

    configurator.LoadAllConfigurations();
    configurator.SetActiveConfiguration(dlg.GetConfigurationName());
    LoadConfigurationList();

    RestoreLastItem();
    _settings_tree_manager.CreateGUI(ui->settings_tree, &(*configurator.GetActiveConfiguration()));
}

// When changes are made to the layer list, it forces a reload
// of the configuration list. This wipes everything out, so we
// need a way to restore the currently selected item whenever
// certain kinds of edits occur. These push/pop functions
// accomplish that. If nothing can be found it should simply
// leave nothing selected.
ConfigurationListItem *MainWindow::SaveLastItem() {
    // Who is selected?
    ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->configuration_tree->currentItem());
    if (item == nullptr) return nullptr;

    assert(!item->configuration_name.isEmpty());
    _last_item = item->configuration_name;
    return item;
}

bool MainWindow::SelectConfigurationItem(const QString &configuration_name) {
    assert(!configuration_name.isEmpty());

    for (int i = 0, n = ui->configuration_tree->topLevelItemCount(); i < n; ++i) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->configuration_tree->topLevelItem(i));
        assert(item != nullptr);
        assert(!item->configuration_name.isEmpty());

        if (item->configuration_name == configuration_name) {
            ui->configuration_tree->setCurrentItem(item);
            return true;
        }
    }

    assert(0);
    return false;
}

// Partner for above function. Returns false if the last config could not be found.
bool MainWindow::RestoreLastItem(const char *configuration_override) {
    if (configuration_override != nullptr) _last_item = configuration_override;

    // Reset the current item
    for (int i = 0; i < ui->configuration_tree->topLevelItemCount(); i++) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->configuration_tree->topLevelItem(i));
        if (item == nullptr) continue;

        assert(!item->configuration_name.isEmpty());
        if (item->configuration_name == _last_item) {
            ui->configuration_tree->setCurrentItem(item);
            return true;
        }
    }
    return false;
}

/// Allow addition or removal of custom layer paths. Afterwards reset the list
/// of loaded layers, but only if something was changed.
void MainWindow::addCustomPaths() {
    // SaveLastItem();
    // Get the tree state and clear it.
    // This looks better aesthetically after the dialog
    // but the dialog changes the pointers to the
    // configs and it will cause a crash.
    _settings_tree_manager.CleanupGUI();

    CustomPathsDialog dlg(this);
    dlg.exec();

    LoadConfigurationList();  // Force a reload
    RestoreLastItem();
}

// Edit the layers for the given configuration.
void MainWindow::EditClicked(ConfigurationListItem *item) {
    assert(item);
    assert(!item->configuration_name.isEmpty());

    Configurator &configurator = Configurator::Get();

    SaveLastItem();
    _settings_tree_manager.CleanupGUI();

    LayersDialog dlg(this, *FindConfiguration(configurator.available_configurations, item->configuration_name));
    dlg.exec();

    configurator.LoadAllConfigurations();
    configurator.SetActiveConfiguration(dlg.GetConfigurationName());
    LoadConfigurationList();

    RestoreLastItem();
    _settings_tree_manager.CreateGUI(ui->settings_tree, &(*configurator.GetActiveConfiguration()));
}

void MainWindow::NewClicked() {
    // SaveLastItem();
    _settings_tree_manager.CleanupGUI();
    Configurator &configurator = Configurator::Get();

    Configuration configuration;
    configuration.name = MakeConfigurationName(configurator.available_configurations, "New Configuration");

    LayersDialog dlg(this, configuration);
    if (QDialog::Accepted == dlg.exec()) {
        configurator.LoadAllConfigurations();
        configurator.SetActiveConfiguration(dlg.GetConfigurationName());
        LoadConfigurationList();

        RestoreLastItem(dlg.GetConfigurationName().toUtf8().constData());
        _settings_tree_manager.CreateGUI(ui->settings_tree, &(*configurator.GetActiveConfiguration()));
    }
}

void MainWindow::RemoveClicked(ConfigurationListItem *item) {
    assert(item);
    assert(!item->configuration_name.isEmpty());

    // Let make sure...
    QMessageBox alert;
    alert.setWindowTitle(VKCONFIG_NAME);
    alert.setText("Are you sure you want to remove this configuration?");
    alert.setInformativeText(item->configuration_name);
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::Yes);
    alert.setIcon(QMessageBox::Question);
    if (alert.exec() == QMessageBox::No) return;

    SaveLastItem();
    _settings_tree_manager.CleanupGUI();
    // What if this is the active profile? We will go boom boom soon...
    Configurator &configurator = Configurator::Get();
    if (configurator.GetActiveConfiguration()->name == &item->configuration_name) {
        configurator.SetActiveConfiguration(configurator.available_configurations.end());
    }

    // Delete the configuration file
    const QString full_path(configurator.path.GetFullPath(PATH_CONFIGURATION, item->configuration_name));
    remove(full_path.toUtf8().constData());

    configurator.LoadAllConfigurations();
    LoadConfigurationList();
    RestoreLastItem();
}

void MainWindow::RenameClicked(ConfigurationListItem *item) {
    assert(item);

    SaveLastItem();
    ui->configuration_tree->editItem(item, 1);
}

void MainWindow::DuplicateClicked(ConfigurationListItem *item) {
    assert(item);

    Configurator &configurator = Configurator::Get();

    assert(!item->configuration_name.isEmpty());

    auto configuration = FindConfiguration(configurator.available_configurations, item->configuration_name);
    assert(configuration != configurator.available_configurations.end());

    const QString &new_name = MakeConfigurationName(configurator.available_configurations, item->configuration_name);
    assert(new_name != item->configuration_name);

    configuration->name = item->configuration_name = new_name;
    const bool result = configuration->Save(configurator.path.GetFullPath(PATH_CONFIGURATION, item->configuration_name));
    assert(result);

    _settings_tree_manager.CleanupGUI();

    configurator.LoadAllConfigurations();
    configurator.SetActiveConfiguration(new_name);
    LoadConfigurationList();

    ConfigurationListItem *new_item = nullptr;
    for (int i = 0, n = ui->configuration_tree->topLevelItemCount(); i < n; ++i) {
        ConfigurationListItem *searched_item = dynamic_cast<ConfigurationListItem *>(ui->configuration_tree->topLevelItem(i));
        assert(searched_item);

        if (searched_item->configuration_name != new_name) continue;

        new_item = searched_item;
        break;
    }
    assert(new_item);
    ui->configuration_tree->editItem(new_item, 1);

    _settings_tree_manager.CreateGUI(ui->settings_tree, &(*configurator.GetActiveConfiguration()));
}

void MainWindow::ImportClicked(ConfigurationListItem *item) {
    (void)item;  // We don't need this
    Configurator &configurator = Configurator::Get();

    const QString full_import_path = configurator.path.SelectPath(this, PATH_IMPORT_CONFIGURATION);
    if (full_import_path.isEmpty()) return;

    _settings_tree_manager.CleanupGUI();

    Configurator::Get().ImportConfiguration(full_import_path);
    LoadConfigurationList();
}

void MainWindow::ExportClicked(ConfigurationListItem *item) {
    assert(item);

    Configurator &configurator = Configurator::Get();

    const QString full_suggested_path = configurator.path.GetFullPath(PATH_EXPORT_CONFIGURATION, item->configuration_name);
    const QString full_export_path = configurator.path.SelectPath(this, PATH_EXPORT_CONFIGURATION, full_suggested_path);
    if (full_export_path.isEmpty()) return;

    configurator.ExportConfiguration(item->configuration_name + ".json", full_export_path);
}

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
    ui->settings_tree->resizeColumnToContents(0);
}

void MainWindow::OnConfigurationItemExpanded(QTreeWidgetItem *item) {
    (void)item;
    ui->settings_tree->resizeColumnToContents(0);
    ui->settings_tree->resizeColumnToContents(1);
}

void MainWindow::OnConfigurationTreeClicked(QTreeWidgetItem *item, int column) {
    (void)column;

    Configurator &configurator = Configurator::Get();

    configurator.environment.Notify(NOTIFICATION_RESTART);

    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(item);
    auto configuration = item == nullptr
                             ? configurator.available_configurations.end()
                             : FindConfiguration(configurator.available_configurations, configuration_item->configuration_name);

    configurator.SetActiveConfiguration(configuration);
    SaveLastItem();

    UpdateUI();
}

void MainWindow::OnSettingsTreeClicked(QTreeWidgetItem *item, int column) {
    (void)column;
    (void)item;

    Configurator::Get().environment.Notify(NOTIFICATION_RESTART);
    Configurator::Get().RefreshConfiguration();
    SaveLastItem();

    UpdateUI();
}

void MainWindow::SetupLauncherTree() {
    // Executable
    QTreeWidgetItem *launcher_parent = new QTreeWidgetItem();
    launcher_parent->setText(0, "Executable Path");
    ui->launcher_tree->addTopLevelItem(launcher_parent);

    _launcher_apps_combo = new QComboBox();
    _launcher_apps_combo->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_apps_combo->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launcher_tree->setItemWidget(launcher_parent, 1, _launcher_apps_combo);

    _launcher_apps_browse_button = new QPushButton();
    _launcher_apps_browse_button->setText("...");
    _launcher_apps_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _launcher_apps_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    _launcher_apps_browse_button->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_apps_browse_button->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launcher_tree->setItemWidget(launcher_parent, 2, _launcher_apps_browse_button);
    connect(_launcher_apps_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(launchItemChanged(int)));
    connect(_launcher_apps_browse_button, SIGNAL(clicked()), this, SLOT(on_push_button_applications_clicked()));

    // Working folder
    QTreeWidgetItem *launcher_folder_item = new QTreeWidgetItem();
    launcher_folder_item->setText(0, "Working Directory");
    launcher_parent->addChild(launcher_folder_item);

    _launcher_working = new QLineEdit();
    _launcher_working->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_working->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launcher_tree->setItemWidget(launcher_folder_item, 1, _launcher_working);
    _launcher_working->setReadOnly(false);

    _launcher_working_browse_button = new QPushButton();
    _launcher_working_browse_button->setText("...");
    _launcher_working_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _launcher_working_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    _launcher_working_browse_button->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_working_browse_button->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launcher_tree->setItemWidget(launcher_folder_item, 2, _launcher_working_browse_button);
    connect(_launcher_working_browse_button, SIGNAL(clicked()), this, SLOT(launchSetWorkingFolder()));

    // Command line arguments
    QTreeWidgetItem *launcher_arguments_item = new QTreeWidgetItem();
    launcher_arguments_item->setText(0, "Command-line Arguments");
    launcher_parent->addChild(launcher_arguments_item);

    _launcher_arguments = new QLineEdit();
    _launcher_arguments->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_arguments->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launcher_tree->setItemWidget(launcher_arguments_item, 1, _launcher_arguments);
    connect(_launcher_arguments, SIGNAL(textEdited(const QString &)), this, SLOT(launchArgsEdited(const QString &)));

    // Log file
    QTreeWidgetItem *launcher_log_file_item = new QTreeWidgetItem();
    launcher_log_file_item->setText(0, "Output Log");
    launcher_parent->addChild(launcher_log_file_item);

    _launcher_log_file_edit = new QLineEdit();
    _launcher_log_file_edit->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_log_file_edit->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launcher_tree->setItemWidget(launcher_log_file_item, 1, _launcher_log_file_edit);
    connect(_launcher_log_file_edit, SIGNAL(textEdited(const QString &)), this, SLOT(launchChangeLogFile(const QString &)));
    connect(_launcher_working, SIGNAL(textEdited(const QString &)), this, SLOT(launchChangeWorkingFolder(const QString &)));

    _launcher_log_file_browse_button = new QPushButton();
    _launcher_log_file_browse_button->setText("...");
    _launcher_log_file_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _launcher_log_file_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    ui->launcher_tree->setItemWidget(launcher_log_file_item, 2, _launcher_log_file_browse_button);
    connect(_launcher_log_file_browse_button, SIGNAL(clicked()), this, SLOT(launchSetLogFile()));

    // Launcher tree
    ui->launcher_tree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    ui->launcher_tree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);

    ui->launcher_tree->setColumnWidth(0, LAUNCH_COLUMN0_SIZE);
    ui->launcher_tree->setColumnWidth(
        1, ui->launcher_tree->rect().width() - LAUNCH_COLUMN0_SIZE - LAUNCH_COLUMN2_SIZE - LAUNCH_SPACING_SIZE);
    ui->launcher_tree->setColumnWidth(2, LAUNCH_COLUMN2_SIZE);

    if (Configurator::Get().environment.Get(LAYOUT_LAUNCHER_COLLAPSED) == "true")
        launchItemCollapsed(nullptr);
    else
        ui->launcher_tree->expandItem(launcher_parent);

    ui->launcher_tree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->launcher_tree->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

// Expanding the tree also grows the tree to match
void MainWindow::launchItemExpanded(QTreeWidgetItem *item) {
    (void)item;
    ui->launcher_tree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    ui->launcher_tree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    Configurator::Get().environment.Set(LAYOUT_LAUNCHER_COLLAPSED, QByteArray("false"));
}

// Collapsing the tree also shrinks the tree to match and show only the first line
void MainWindow::launchItemCollapsed(QTreeWidgetItem *item) {
    (void)item;
    ui->launcher_tree->setMinimumHeight(LAUNCH_ROW_HEIGHT + 6);
    ui->launcher_tree->setMaximumHeight(LAUNCH_ROW_HEIGHT + 6);
    Configurator::Get().environment.Set(LAYOUT_LAUNCHER_COLLAPSED, QByteArray("true"));
}

void MainWindow::launchSetLogFile() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Application &application = Configurator::Get().environment.GetApplication(current_application_index);
    const QString path = Configurator::Get().path.SelectPath(this, PATH_LAUNCHER_LOG_FILE, application.log_file);

    // The user has cancel the operation
    if (path.isEmpty()) return;

    application.log_file = path;
    _launcher_log_file_edit->setText(path);
}

void MainWindow::launchSetWorkingFolder() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Application &application = Configurator::Get().environment.GetApplication(current_application_index);
    const QString path = Configurator::Get().path.SelectPath(this, PATH_WORKING_DIR, application.working_folder);

    // The user has cancel the operation
    if (path.isEmpty()) return;

    application.working_folder = path;
    _launcher_working->setText(path);
}

// Log file path edited manually.
void MainWindow::launchChangeLogFile(const QString &log_file) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Application &application = Configurator::Get().environment.GetApplication(current_application_index);
    application.log_file = log_file;
}

void MainWindow::launchChangeWorkingFolder(const QString &working_folder) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Application &application = Configurator::Get().environment.GetApplication(current_application_index);
    application.working_folder = working_folder;
}

// Launch app change
void MainWindow::launchItemChanged(int application_index) {
    if (application_index < 0) return;

    Environment &environment = Configurator::Get().environment;

    environment.SelectActiveApplication(application_index);

    Application &application = environment.GetApplication(application_index);
    _launcher_arguments->setText(application.arguments);
    _launcher_working->setText(application.working_folder);
    _launcher_log_file_edit->setText(application.log_file);
}

/// New command line arguments. Update them.
void MainWindow::launchArgsEdited(const QString &arguments) {
    int application_index = _launcher_apps_combo->currentIndex();
    if (application_index < 0) return;

    Application &application = Configurator::Get().environment.GetApplication(application_index);
    application.arguments = arguments;
}

// Clear the browser window
void MainWindow::on_push_button_clear_log_clicked() {
    ui->log_browser->clear();
    ui->log_browser->update();
    ui->push_button_clear_log->setEnabled(false);
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    // Launch tree does some fancy resizing and since it's down in
    // layouts and splitters, we can't just rely on the resize method
    // of this window. Any resize coming through needs to trigger this
    // or we get drawing artifacts on macOS.
    if (event->type() == QEvent::Resize) {
        const QRect rect = ui->launcher_tree->rect();
        ui->launcher_tree->setColumnWidth(0, LAUNCH_COLUMN0_SIZE);
        ui->launcher_tree->setColumnWidth(1, rect.width() - LAUNCH_COLUMN0_SIZE - LAUNCH_COLUMN2_SIZE - LAUNCH_SPACING_SIZE);
        ui->launcher_tree->setColumnWidth(2, LAUNCH_COLUMN2_SIZE);
        return false;
    }

    // Context menus for layer configuration files
    if (target == ui->configuration_tree) {
        QContextMenuEvent *right_click = dynamic_cast<QContextMenuEvent *>(event);
        if (right_click) {  // && event->type() == QEvent::ContextMenu) {
            // Which item were we over?
            QTreeWidgetItem *configuration_item = ui->configuration_tree->itemAt(right_click->pos());
            ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(configuration_item);

            // Create context menu here
            QMenu menu(ui->configuration_tree);

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

            if (action == edit_action) {
                EditClicked(item);
            } else if (action == new_action) {
                NewClicked();
            } else if (action == duplicate_action) {
                DuplicateClicked(item);
            } else if (action == remove_action) {
                RemoveClicked(item);
            } else if (action == rename_action) {
                RenameClicked(item);
            } else if (action == export_action) {
                ExportClicked(item);
            } else if (action == import_action) {
                ImportClicked(item);
            } else if (action == custom_path_action) {
                EditCustomPathsClicked(item);
            } else {
                assert(0);  // Unknown action
            }

            // Do not pass on
            return true;
        }
    }

    // Pass it on
    return false;
}

void MainWindow::ResetLaunchApplication() {
    if (_launch_application) {
        _launch_application->kill();
        _launch_application->waitForFinished();
        _launch_application.reset();

        UpdateUI();
    }
}

void MainWindow::on_push_button_launch_clicked() {
    // Are we already monitoring a running app? If so, terminate it
    if (_launch_application != nullptr) {
        ResetLaunchApplication();
        return;
    }

    // We are logging, let's add that we've launched a new application
    QString launch_log = "Launching Vulkan Application:\n";

    Configurator &configurator = Configurator::Get();
    const Application &active_application = configurator.environment.GetActiveApplication();

    auto configuration = configurator.GetActiveConfiguration();

    if (configuration == configurator.available_configurations.end()) {
        launch_log += "- Layers fully controlled by the application.\n";
    } else if (HasMissingParameter(configuration->parameters, configurator.available_layers)) {
        launch_log += QString().asprintf("- No layers override. The active \"%s\" configuration is missing a layer.\n",
                                         configuration->name.toUtf8().constData());
    } else if (configurator.environment.UseOverride()) {
        if (configurator.environment.UseApplicationListOverrideMode() && configurator.environment.HasOverriddenApplications() &&
            !active_application.override_layers) {
            launch_log += "- Layers fully controlled by the application. Application excluded from layers override.\n";
        } else {
            launch_log +=
                QString().asprintf("- Layers overridden by \"%s\" configuration.\n", configuration->name.toUtf8().constData());
        }
    }

    assert(!active_application.executable_path.isEmpty());
    launch_log += QString().asprintf("- Executable Path: %s\n", active_application.executable_path.toUtf8().constData());
    assert(!active_application.working_folder.isEmpty());
    launch_log += QString().asprintf("- Working Directory: %s\n", active_application.working_folder.toUtf8().constData());
    if (!active_application.arguments.isEmpty())
        launch_log += QString().asprintf("- Command-line Arguments: %s\n", active_application.arguments.toUtf8().constData());
    if (!active_application.log_file.isEmpty())
        launch_log += QString().asprintf("- Log file: %s\n", active_application.log_file.toUtf8().constData());

    if (!active_application.log_file.isEmpty()) {
        // Start logging
        // Make sure the log file is not already opened. This can occur if the
        // launched application is closed from the applicaiton.
        if (!_log_file.isOpen()) {
            _log_file.setFileName(active_application.log_file);

            // Open and append, or open and truncate?
            QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
            if (!ui->check_box_clear_on_launch->isChecked()) mode |= QIODevice::Append;

            if (!_log_file.open(mode)) {
                QMessageBox err;
                err.setText("Cannot open log file");
                err.setIcon(QMessageBox::Warning);
                err.exec();
            }
        }
    }

    if (ui->check_box_clear_on_launch->isChecked()) ui->log_browser->clear();
    Log(launch_log);

    // Launch the test application
    _launch_application.reset(new QProcess(this));
    connect(_launch_application.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));
    connect(_launch_application.get(), SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));
    connect(_launch_application.get(), SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(processClosed(int, QProcess::ExitStatus)));

    _launch_application->setProgram(active_application.executable_path);
    _launch_application->setWorkingDirectory(active_application.working_folder);

    if (!active_application.arguments.isEmpty()) {
        const QStringList args = active_application.arguments.split(" ");
        _launch_application->setArguments(args);
    }

    _launch_application->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    _launch_application->setProcessChannelMode(QProcess::MergedChannels);
    _launch_application->closeWriteChannel();

    // Wait... did we start? Give it 4 seconds, more than enough time
    if (!_launch_application->waitForStarted(4000)) {
        _launch_application->deleteLater();
        _launch_application = nullptr;

        const QString failed_log = QString("Failed to launch ") + active_application.executable_path + "!\n";
        Log(failed_log);
        UpdateUI();
        return;
    }

    UpdateUI();
}

/// The process we are following is closed. We don't actually care about the
/// exit status/code, we just need to know to destroy the QProcess object
/// and set it back to nullptr so that we know we can launch a new app.
/// Also, if we are logging, it's time to close the log file.
void MainWindow::processClosed(int exit_code, QProcess::ExitStatus status) {
    (void)exit_code;
    (void)status;

    assert(_launch_application);

    disconnect(_launch_application.get(), SIGNAL(finished(int, QProcess::ExitStatus)), this,
               SLOT(processClosed(int, QProcess::ExitStatus)));
    disconnect(_launch_application.get(), SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));
    disconnect(_launch_application.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));

    Log("Process terminated");

    if (_log_file.isOpen()) {
        _log_file.close();
    }

    ResetLaunchApplication();
}

/// This signal get's raised whenever the spawned Vulkan appliction writes
/// to stdout and there is data to be read. The layers flush after all stdout
/// writes, so we should see layer output here in realtime, as we just read
/// the string and append it to the text browser.
/// If a log file is open, we also write the output to the log.
void MainWindow::standardOutputAvailable() {
    if (_launch_application) {
        Log(_launch_application->readAllStandardOutput());
    }
}

void MainWindow::errorOutputAvailable() {
    if (_launch_application) {
        Log(_launch_application->readAllStandardError());
    }
}

void MainWindow::Log(const QString &log) {
    ui->log_browser->append(log);
    ui->push_button_clear_log->setEnabled(true);

    if (_log_file.isOpen()) {
        _log_file.write(log.toUtf8().constData(), log.length());
        _log_file.flush();
    }
}
