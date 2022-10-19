/*
 * Copyright (c) 2020-2022 Valve Corporation
 * Copyright (c) 2020-2022 LunarG, Inc.
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

#include "dialog_about.h"
#include "dialog_vulkan_analysis.h"
#include "dialog_vulkan_info.h"
#include "dialog_layers.h"
#include "dialog_applications.h"

#include "configurator.h"
#include "vulkan_util.h"

#include "../vkconfig_core/alert.h"
#include "../vkconfig_core/util.h"
#include "../vkconfig_core/version.h"
#include "../vkconfig_core/platform.h"
#include "../vkconfig_core/help.h"
#include "../vkconfig_core/doc.h"
#include "../vkconfig_core/date.h"

#include <QProcess>
#include <QMessageBox>
#include <QFrame>
#include <QComboBox>
#include <QVariant>
#include <QContextMenuEvent>
#include <QLineEdit>
#include <QSettings>
#include <QDesktopServices>

#if VKC_PLATFORM == VKC_PLATFORM_LINUX || VKC_PLATFORM == VKC_PLATFORM_MACOS
#include <unistd.h>
#endif

#include <cassert>

static const int LAUNCH_COLUMN0_SIZE = 220;
static const int LAUNCH_COLUMN2_SIZE = 32;
static const int LAUNCH_SPACING_SIZE = 2;
#if VKC_PLATFORM == VKC_PLATFORM_MACOS
static const int LAUNCH_ROW_HEIGHT = 26;
#else
static const int LAUNCH_ROW_HEIGHT = 28;
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _launch_application(nullptr),
      _log_file(nullptr),
      _launcher_apps_combo(nullptr),
      _launcher_executable(nullptr),
      _launcher_arguments(nullptr),
      _launcher_working(nullptr),
      _launcher_log_file_edit(nullptr),
      _launcher_apps_browse_button(nullptr),
      _launcher_executable_browse_button(nullptr),
      _launcher_working_browse_button(nullptr),
      _launcher_log_file_browse_button(nullptr),
      ui(new Ui::MainWindow),
      been_warned_about_old_loader(false) {
    ui->setupUi(this);
    ui->launcher_tree->installEventFilter(this);
    ui->configuration_tree->installEventFilter(this);
    ui->settings_tree->installEventFilter(this);

    SetupLauncherTree();

    connect(ui->action_find_more_layers, SIGNAL(triggered(bool)), this, SLOT(OnHelpFindLayers(bool)));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(OnHelpAbout(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));
    connect(ui->action_readme, SIGNAL(triggered(bool)), this, SLOT(OnHelpReadme(bool)));
    connect(ui->action_changelog, SIGNAL(triggered(bool)), this, SLOT(OnHelpChangelog(bool)));
    connect(ui->actionVulkan_specification, SIGNAL(triggered(bool)), this, SLOT(OnHelpVulkanSpec(bool)));
    connect(ui->actionVulkan_Layer_Specification, SIGNAL(triggered(bool)), this, SLOT(OnHelpLayerSpec(bool)));
    connect(ui->actionGPU_Info_Reports, SIGNAL(triggered(bool)), this, SLOT(OnHelpGPUInfo(bool)));

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

    connect(ui->launcher_loader_debug, SIGNAL(currentIndexChanged(int)), this, SLOT(OnLauncherLoaderMessageChanged(int)));

    Configurator &configurator = Configurator::Get();
    Environment &environment = configurator.environment;

    // Restore window geometry from last launch
    restoreGeometry(environment.Get(LAYOUT_MAIN_GEOMETRY));
    restoreState(environment.Get(LAYOUT_MAIN_WINDOW_STATE));
    ui->splitter->restoreState(environment.Get(LAYOUT_MAIN_SPLITTER1));
    ui->splitter_2->restoreState(environment.Get(LAYOUT_MAIN_SPLITTER2));
    ui->splitter_3->restoreState(environment.Get(LAYOUT_MAIN_SPLITTER3));

    LoadConfigurationList();

    // Resetting this from the default prevents the log window (a QTextEdit) from overflowing.
    // Whenever the control surpasses this block count, old blocks are discarded.
    // Note: We could make this a user configurable setting down the road should this be
    // insufficinet.
    ui->log_browser->document()->setMaximumBlockCount(2048);
    ui->configuration_tree->scrollToItem(ui->configuration_tree->topLevelItem(0), QAbstractItemView::PositionAtTop);

    if (configurator.configurations.HasSelectConfiguration()) {
        _settings_tree_manager.CreateGUI(ui->settings_tree);
    }

    UpdateUI();
}

MainWindow::~MainWindow() { ResetLaunchApplication(); }

static std::string GetMainWindowTitle(bool active) {
#if VKCONFIG_DATE
    std::string title = format("%s %s-%s", VKCONFIG_NAME, Version::VKCONFIG.str().c_str(), GetBuildDate().c_str());
#else
    std::string title = format("%s %s", VKCONFIG_NAME, Version::VKCONFIG.str().c_str());
#endif
    if (active) title += " <ACTIVE>";
    return title;
}

void MainWindow::UpdateUI() {
    Configurator &configurator = Configurator::Get();
    const Environment &environment = Configurator::Get().environment;
    const bool has_select_configuration = configurator.configurations.HasSelectConfiguration();
    const std::string &active_contiguration_name = environment.Get(ACTIVE_CONFIGURATION);

    ui->configuration_tree->blockSignals(true);

    // Mode states
    ui->radio_override->setChecked(environment.UseOverride());
    ui->radio_fully->setChecked(!environment.UseOverride());

    // Update configurations
    ui->group_box_configurations->setEnabled(environment.UseOverride());

    ui->configuration_tree->setCurrentItem(nullptr);
    // ui->configuration_tree->setSelectionMode(has_active_configuration ? QAbstractItemView::SingleSelection
    //                                                                  : QAbstractItemView::NoSelection);
    for (int i = 0, n = ui->configuration_tree->topLevelItemCount(); i < n; ++i) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->configuration_tree->topLevelItem(i));
        assert(item);
        assert(!item->configuration_name.empty());

        Configuration *configuration =
            FindByKey(configurator.configurations.available_configurations, item->configuration_name.c_str());
        if (configuration == nullptr) continue;

        item->setText(1, item->configuration_name.c_str());
        item->setToolTip(1, configuration->description.c_str());
        item->radio_button->setToolTip(configuration->description.c_str());

        if (item->configuration_name == active_contiguration_name) {
            ui->configuration_tree->setCurrentItem(item);
            item->radio_button->setChecked(true);
        } else {
            item->radio_button->setChecked(false);
        }
    }

    // Update settings
    ui->push_button_edit->setEnabled(environment.UseOverride() && has_select_configuration);
    ui->push_button_remove->setEnabled(environment.UseOverride() && has_select_configuration);
    ui->push_button_duplicate->setEnabled(environment.UseOverride() && has_select_configuration);
    ui->push_button_new->setEnabled(environment.UseOverride());
    ui->settings_tree->setEnabled(environment.UseOverride() && has_select_configuration);
    ui->group_box_settings->setTitle(has_select_configuration ? (active_contiguration_name + " Settings").c_str()
                                                              : "Configuration Settings");

    // Handle application lists states
    ui->check_box_apply_list->setEnabled(!been_warned_about_old_loader && environment.UseOverride());
    ui->check_box_apply_list->setChecked(!been_warned_about_old_loader && environment.UseApplicationListOverrideMode());
    ui->push_button_applications->setEnabled(!been_warned_about_old_loader && ui->check_box_apply_list->isChecked());

    _launcher_apps_combo->blockSignals(true);
    _launcher_apps_combo->clear();

    const std::vector<Application> &applications = environment.GetApplications();
    if (applications.empty()) {
        _launcher_executable->setText("");
        _launcher_arguments->setText("");
        _launcher_working->setText("");
        _launcher_log_file_edit->setText("");
    } else {
        for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
            _launcher_apps_combo->addItem(applications[i].app_name.c_str());
        }
        _launcher_apps_combo->setCurrentIndex(environment.GetActiveApplicationIndex());

        const Application &application = environment.GetActiveApplication();
        _launcher_executable->setText(application.executable_path.c_str());
        _launcher_arguments->setText(application.arguments.c_str());
        _launcher_working->setText(application.working_folder.c_str());
        _launcher_log_file_edit->setText(ReplaceBuiltInVariable(application.log_file.c_str()).c_str());
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
    ui->launcher_loader_debug->setCurrentIndex(environment.GetLoaderMessage());

    // ui->launcher_loader_debug
    if (_launcher_executable_browse_button) {
        _launcher_executable_browse_button->setEnabled(has_application_list);
    }
    if (_launcher_working_browse_button) {
        _launcher_working_browse_button->setEnabled(has_application_list);
    }
    if (_launcher_log_file_browse_button) {
        _launcher_log_file_browse_button->setEnabled(has_application_list);
    }
    if (_launcher_apps_combo) {
        _launcher_apps_combo->setEnabled(has_application_list);
    }
    if (_launcher_executable) {
        _launcher_executable->setEnabled(has_application_list);
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

    if (configurator.request_vulkan_status) {
        ui->log_browser->clear();

        ui->log_browser->setPlainText(("Vulkan Development Status:\n" + GenerateVulkanStatus()).c_str());
        ui->push_button_clear_log->setEnabled(true);
        configurator.request_vulkan_status = false;

        if (configurator.configurations.HasActiveConfiguration(configurator.layers.available_layers)) {
            _settings_tree_manager.CreateGUI(ui->settings_tree);
        }
    }

    // Update title bar
    setWindowTitle(GetMainWindowTitle(configurator.configurations.HasActiveConfiguration(configurator.layers.available_layers) &&
                                      configurator.environment.UseOverride())
                       .c_str());

    ui->configuration_tree->blockSignals(false);
}

void MainWindow::UpdateConfiguration() {}

// Load or refresh the list of configuration. Any configuration that uses a layer that
// is not detected on the system is disabled.
void MainWindow::LoadConfigurationList() {
    // There are lots of ways into this, and in none of them
    // can we have an active editor running.
    _settings_tree_manager.CleanupGUI();
    ui->configuration_tree->blockSignals(true);  // No signals firing off while we do this
    ui->configuration_tree->clear();

    Configurator &configurator = Configurator::Get();

    for (std::size_t i = 0, n = configurator.configurations.available_configurations.size(); i < n; ++i) {
        const Configuration &configuration = configurator.configurations.available_configurations[i];

        // Hide built-in configuration when the layer is missing. The Vulkan user may have not installed the necessary layer
        // if (configuration.IsBuiltIn() && HasMissingLayer(configuration.parameters, configurator.layers.available_layers))
        // continue;

        ConfigurationListItem *item = new ConfigurationListItem(configuration.key);
        item->setToolTip(0, configuration.description.c_str());
        item->radio_button = new QRadioButton();
        item->radio_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        item->radio_button->setFixedSize(QSize(24, 24));
        item->radio_button->setToolTip(configuration.description.c_str());
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->configuration_tree->addTopLevelItem(item);
        ui->configuration_tree->setItemWidget(item, 0, item->radio_button);
        connect(item->radio_button, SIGNAL(clicked(bool)), this, SLOT(OnConfigurationItemClicked(bool)));
    }

    ui->configuration_tree->blockSignals(false);
    ui->configuration_tree->resizeColumnToContents(0);
    ui->configuration_tree->resizeColumnToContents(1);

    if (configurator.configurations.HasActiveConfiguration(configurator.layers.available_layers)) {
        _settings_tree_manager.CreateGUI(ui->settings_tree);
    }

    this->UpdateUI();
}

/// Okay, because we are using custom controls, some of
/// the signaling is not happening as expected. So, we cannot
/// always get an accurate answer to the currently selected
/// item, but we do often need to know what has been checked
/// when an event occurs. This unambigously answers that question.
ConfigurationListItem *MainWindow::GetCheckedItem() {
    // Just go through all the top level items
    for (int i = 0, n = ui->configuration_tree->topLevelItemCount(); i < n; ++i) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->configuration_tree->topLevelItem(i));

        if (item == nullptr) continue;
        if (item->radio_button->isChecked()) return item;
    }

    return nullptr;
}

/// Use the active profile as the override
void MainWindow::on_radio_override_clicked() {
    Configurator &configurator = Configurator::Get();

    configurator.environment.SetMode(OVERRIDE_MODE_ACTIVE, true);
    configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);
    configurator.request_vulkan_status = true;

    UpdateUI();
}

// No override at all, fully controlled by the application
void MainWindow::on_radio_fully_clicked() {
    Configurator &configurator = Configurator::Get();

    configurator.environment.SetMode(OVERRIDE_MODE_ACTIVE, false);
    configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);
    configurator.request_vulkan_status = true;

    UpdateUI();
}

// We want to apply to just the app list... hang on there. Doe we have the new loader?
void MainWindow::on_check_box_apply_list_clicked() {
    Configurator &configurator = Configurator::Get();

    // Handle old loader case
    Version loader_version;
    if (!configurator.SupportApplicationList(&loader_version)) {
        const std::string version = loader_version.str();
        const std::string message =
            format("The detected Vulkan loader version is %s but version 1.2.141 or newer is required", version.c_str());
        ui->check_box_apply_list->setToolTip(message.c_str());

        Alert::ApplicationListUnsupported(message.c_str());

        ui->check_box_apply_list->setEnabled(false);
        ui->check_box_apply_list->setChecked(false);
        ui->push_button_applications->setEnabled(false);
        configurator.environment.SetMode(OVERRIDE_MODE_LIST, false);
        been_warned_about_old_loader = true;

        return;
    }

    configurator.environment.SetMode(OVERRIDE_MODE_LIST, ui->check_box_apply_list->isChecked());

    // Handle the case where no application with active override is present
    const bool application_list_requires_update = !configurator.environment.HasOverriddenApplications();
    if (ui->check_box_apply_list->isChecked() && application_list_requires_update) {
        ApplicationsDialog dialog(this);
        dialog.exec();
    }

    configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);

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

    if (Alert::ConfiguratorResetAll() == QMessageBox::No) return;

    Configurator &configurator = Configurator::Get();
    configurator.ResetToDefault(true);

    LoadConfigurationList();

    UpdateUI();
}

// Thist signal actually comes from the radio button
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

    Configurator::Get().ActivateConfiguration(item->configuration_name);

    UpdateUI();
}

void MainWindow::OnConfigurationTreeClicked(QTreeWidgetItem *item, int column) {
    (void)column;

    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(item);
    if (configuration_item != nullptr) {
        Configurator::Get().ActivateConfiguration(configuration_item->configuration_name);
    }

    UpdateUI();
}

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
        // const std::string full_path =
        //    ConvertNativeSeparators(GetPath(BUILTIN_PATH_CONFIG_LAST) + "/" + configuration_item->configuration_name + ".json");

        // This is the new name we want to use for the configuration
        const std::string &new_name = configuration_item->text(1).toStdString();

        if (new_name.empty()) {
            Alert::ConfigurationNameEmpty();
        } else if (!IsPortableFilename(new_name)) {
            Alert::ConfigurationNameInvalid();
        }

        const bool failed = new_name.empty() || !IsPortableFilename(new_name);
        Configuration *duplicate_configuration =
            failed ? nullptr : FindByKey(configurator.configurations.available_configurations, new_name.c_str());

        if (duplicate_configuration != nullptr) {
            Alert::ConfigurationRenamingFailed();
        }

        // Find existing configuration using it's old name
        const std::string old_name = configuration_item->configuration_name;
        Configuration *configuration = FindByKey(configurator.configurations.available_configurations, old_name.c_str());

        if (failed || duplicate_configuration != nullptr) {
            // If the configurate name is empty or the configuration name is taken, keep old configuration name

            ui->configuration_tree->blockSignals(true);
            item->setText(1, old_name.c_str());
            ui->configuration_tree->blockSignals(false);

            configurator.ActivateConfiguration(old_name);
        } else {
            // Rename configuration ; Remove old configuration file ; change the name of the configuration
            configurator.configurations.RemoveConfigurationFile(old_name);
            configuration->key = configuration_item->configuration_name = new_name;
            configurator.configurations.SaveAllConfigurations(configurator.layers.available_layers);
            configurator.configurations.LoadAllConfigurations(configurator.layers.available_layers);

            configurator.ActivateConfiguration(new_name);

            LoadConfigurationList();
            SelectConfigurationItem(new_name.c_str());
        }
    }

    UpdateUI();
}

/// This gets called with keyboard selections and clicks that do not necessarily
/// result in a radio button change (but it may). So we need to do two checks here, one
/// for the radio button, and one to change the editor/information at lower right.
void MainWindow::OnConfigurationTreeChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    (void)previous;

    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(current);
    if (configuration_item == nullptr) return;

    Configurator &configurator = Configurator::Get();
    if (configurator.configurations.HasActiveConfiguration(configurator.layers.available_layers)) {
        if (configurator.configurations.GetActiveConfiguration()->key == configuration_item->configuration_name) return;
    }

    _settings_tree_manager.CleanupGUI();

    configuration_item->radio_button->setChecked(true);
    configurator.ActivateConfiguration(configuration_item->configuration_name);

    _settings_tree_manager.CreateGUI(ui->settings_tree);

    ui->settings_tree->resizeColumnToContents(0);
}

// Unused flag, just display the about Qt dialog
void MainWindow::OnHelpAbout(bool checked) {
    (void)checked;

    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::StartTool(Tool tool) {
    std::string active_configuration;

    Configurator &configurator = Configurator::Get();
    if (configurator.configurations.HasActiveConfiguration(configurator.layers.available_layers)) {
        active_configuration = configurator.configurations.GetActiveConfiguration()->key;
        configurator.configurations.SetActiveConfiguration(configurator.layers.available_layers, nullptr);
    }

    switch (tool) {
        case TOOL_VULKAN_INFO:
            vk_info_dialog.reset(new VulkanInfoDialog(this));
            break;
        case TOOL_VULKAN_INSTALL:
            vk_installation_dialog.reset(new VulkanAnalysisDialog(this));
            break;
    }

    if (!active_configuration.empty()) {
        configurator.configurations.SetActiveConfiguration(configurator.layers.available_layers, active_configuration);
    }
}

/// Create the VulkanInfo dialog if it doesn't already exits & show it.
void MainWindow::toolsVulkanInfo(bool checked) {
    (void)checked;

    this->StartTool(TOOL_VULKAN_INFO);
}

/// Create the VulkanTools dialog if it doesn't already exist & show it.
void MainWindow::toolsVulkanInstallation(bool checked) {
    (void)checked;

    this->StartTool(TOOL_VULKAN_INSTALL);
}

void MainWindow::OnHelpFindLayers(bool checked) {
    (void)checked;

    ShowDoc(DOC_FIND_LAYERS);
}

void MainWindow::OnHelpReadme(bool checked) {
    (void)checked;

    ShowDoc(DOC_VKCONFIG_README);
}

void MainWindow::OnHelpChangelog(bool checked) {
    (void)checked;

    ShowDoc(DOC_VKCONFIG_CHANGELOG);
}

void MainWindow::OnHelpVulkanSpec(bool checked) {
    (void)checked;

    ShowDoc(DOC_VULKAN_SPEC);
}

void MainWindow::OnHelpLayerSpec(bool checked) {
    (void)checked;

    ShowDoc(DOC_VULKAN_LAYERS);
}

void MainWindow::OnHelpGPUInfo(bool checked) {
    (void)checked;

    ShowDoc(DOC_GPU_INFO);
}

/// The only thing we need to do here is clear the configuration if
/// the user does not want it active.
void MainWindow::closeEvent(QCloseEvent *event) {
    Environment &environment = Configurator::Get().environment;

    // Alert the user to the current state of the vulkan configurator and
    // give them the option to not shutdown.
    QSettings settings;
    if (!settings.value("vkconfig_override", false).toBool()) {
        std::string shut_down_state;

        if (environment.UsePersistentOverrideMode()) {
            shut_down_state = "Vulkan Layers override will remain in effect when Vulkan Configurator closes.";

            if (environment.UseApplicationListOverrideMode())
                shut_down_state += " Overrides will be applied only to the application list.";
            else
                shut_down_state += " Overrides will be applied to ALL Vulkan applications.";
        } else {
            shut_down_state = "No Vulkan layers override will be active when Vulkan Configurator closes.";
        }

        QMessageBox alert(this);
        alert.setWindowTitle("Vulkan Layers configuration state on exit");
        alert.setText(shut_down_state.c_str());
        alert.setIcon(QMessageBox::Question);
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setCheckBox(new QCheckBox("Do not show again."));
        alert.setInformativeText("Are you still ready to close Vulkan Configurator?");

        int ret_val = alert.exec();
        settings.setValue("vkconfig_override", alert.checkBox()->isChecked());

        if (ret_val == QMessageBox::No) {
            event->ignore();
            return;
        }
    }

    // If a child process is still running, destroy it
    if (_launch_application) {
        ResetLaunchApplication();
    }

    _settings_tree_manager.CleanupGUI();

    environment.Set(LAYOUT_MAIN_GEOMETRY, saveGeometry());
    environment.Set(LAYOUT_MAIN_WINDOW_STATE, saveState());
    environment.Set(LAYOUT_MAIN_SPLITTER1, ui->splitter->saveState());
    environment.Set(LAYOUT_MAIN_SPLITTER2, ui->splitter_2->saveState());
    environment.Set(LAYOUT_MAIN_SPLITTER3, ui->splitter_3->saveState());

    QMainWindow::closeEvent(event);
}

/// Resizing needs a little help. Yes please, there has to be
/// a better way of doing this.
void MainWindow::resizeEvent(QResizeEvent *event) {
    if (event != nullptr) event->accept();
}

void MainWindow::showEvent(QShowEvent *event) {
    (void)event;

    UpdateUI();

    event->accept();
}

/// Edit the list of apps that can be filtered.
void MainWindow::on_push_button_applications_clicked() {
    ApplicationsDialog dlg(this);
    dlg.exec();

    Configurator &configurator = Configurator::Get();
    configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);

    UpdateUI();
}

void MainWindow::on_push_button_new_clicked() { this->NewClicked(); }

void MainWindow::on_push_button_remove_clicked() {
    Configurator &configurator = Configurator::Get();

    this->RemoveConfiguration(configurator.configurations.GetActiveConfiguration()->key);
}

void MainWindow::on_push_button_duplicate_clicked() {
    Configurator &configurator = Configurator::Get();

    Configuration *configutation = configurator.configurations.GetActiveConfiguration();
    assert(configutation != nullptr);

    const Configuration &duplicated_configuration =
        configurator.configurations.CreateConfiguration(configurator.layers.available_layers, configutation->key, true);

    configurator.ActivateConfiguration(duplicated_configuration.key);

    LoadConfigurationList();
}

void MainWindow::on_push_button_edit_clicked() {
    Configurator &configurator = Configurator::Get();
    Configuration *configuration = configurator.configurations.GetActiveConfiguration();
    assert(configuration != nullptr);

    const std::string configuration_name = configuration->key;

    LayersDialog dlg(this, *configuration);
    if (dlg.exec() == QDialog::Accepted) {
        LoadConfigurationList();
    }
}

// Edit the layers for the given configuration.
void MainWindow::EditClicked(ConfigurationListItem *item) {
    assert(item);
    assert(!item->configuration_name.empty());

    Configurator &configurator = Configurator::Get();
    Configuration *configuration =
        FindByKey(configurator.configurations.available_configurations, item->configuration_name.c_str());
    assert(configuration != nullptr);

    LayersDialog dlg(this, *configuration);
    if (dlg.exec() == QDialog::Accepted) {
        LoadConfigurationList();
    }
}

void MainWindow::NewClicked() {
    Configurator &configurator = Configurator::Get();
    const std::string active_configuration = configurator.environment.Get(ACTIVE_CONFIGURATION);

    Configuration &new_configuration =
        configurator.configurations.CreateConfiguration(configurator.layers.available_layers, "New Configuration");

    LayersDialog dlg(this, new_configuration);
    switch (dlg.exec()) {
        case QDialog::Accepted:
            break;
        case QDialog::Rejected:
            configurator.configurations.RemoveConfiguration(configurator.layers.available_layers, new_configuration.key);
            configurator.configurations.SetActiveConfiguration(configurator.layers.available_layers, active_configuration);
            break;
        default:
            assert(0);
            break;
    }

    LoadConfigurationList();
}

void MainWindow::RemoveConfiguration(const std::string &configuration_name) {
    assert(!configuration_name.empty());

    // Let make sure...
    QMessageBox alert;
    alert.setWindowTitle(format("Removing *%s* configuration...", configuration_name.c_str()).c_str());
    alert.setText(format("Are you sure you want to remove the *%s* configuration?", configuration_name.c_str()).c_str());
    alert.setInformativeText("All the data from this configuration will be lost.");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::Yes);
    alert.setIcon(QMessageBox::Warning);
    if (alert.exec() == QMessageBox::No) return;

    _settings_tree_manager.CleanupGUI();

    Configurator &configurator = Configurator::Get();
    configurator.configurations.RemoveConfiguration(configurator.layers.available_layers, configuration_name);
    configurator.request_vulkan_status = true;
    LoadConfigurationList();
}

void MainWindow::RemoveClicked(ConfigurationListItem *item) {
    assert(item);

    this->RemoveConfiguration(item->configuration_name);
}

void MainWindow::ResetClicked(ConfigurationListItem *item) {
    assert(item);
    assert(!item->configuration_name.empty());

    Configurator &configurator = Configurator::Get();
    Configuration *configuration =
        FindByKey(configurator.configurations.available_configurations, item->configuration_name.c_str());
    assert(configuration != nullptr);

    QMessageBox alert;
    alert.setWindowTitle(format("Resetting *%s* configuration...", configuration->key.c_str()).c_str());
    alert.setText(format("Are you sure you want to reset the *%s* configuration?", configuration->key.c_str()).c_str());
    if (configuration->IsBuiltIn())
        alert.setInformativeText(
            format("The configuration layers and settings will be restored to default built-in *%s* configuration.",
                   configuration->key.c_str())
                .c_str());
    else if (configurator.configurations.HasFile(*configuration))
        alert.setInformativeText(
            format("The configuration layers and settings will be reloaded using the *%s* saved file from previous %s run.",
                   configuration->key.c_str(), VKCONFIG_NAME)
                .c_str());
    else
        alert.setInformativeText("The configuration layers and settings will be reset to zero.");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::Yes);
    alert.setIcon(QMessageBox::Warning);
    if (alert.exec() == QMessageBox::No) return;

    configuration->Reset(configurator.layers.available_layers, configurator.path);

    LoadConfigurationList();
}

void MainWindow::RenameClicked(ConfigurationListItem *item) {
    assert(item);

    ui->configuration_tree->editItem(item, 1);
}

void MainWindow::DuplicateClicked(ConfigurationListItem *item) {
    assert(item);
    assert(!item->configuration_name.empty());

    Configurator &configurator = Configurator::Get();
    const Configuration &duplicated_configuration =
        configurator.configurations.CreateConfiguration(configurator.layers.available_layers, item->configuration_name, true);

    item->configuration_name = duplicated_configuration.key;

    configurator.ActivateConfiguration(duplicated_configuration.key);

    LoadConfigurationList();

    ConfigurationListItem *new_item = nullptr;
    for (int i = 0, n = ui->configuration_tree->topLevelItemCount(); i < n; ++i) {
        ConfigurationListItem *searched_item = dynamic_cast<ConfigurationListItem *>(ui->configuration_tree->topLevelItem(i));
        assert(searched_item);

        if (searched_item->configuration_name != duplicated_configuration.key) continue;

        new_item = searched_item;
        break;
    }
    assert(new_item);
    ui->configuration_tree->editItem(new_item, 1);
}

void MainWindow::ImportClicked(ConfigurationListItem *item) {
    (void)item;  // We don't need this

    Configurator &configurator = Configurator::Get();

    const std::string full_import_path = configurator.path.SelectPath(this, PATH_IMPORT_CONFIGURATION);
    if (full_import_path.empty()) return;

    configurator.configurations.ImportConfiguration(configurator.layers.available_layers, full_import_path);
    LoadConfigurationList();
}

void MainWindow::ExportClicked(ConfigurationListItem *item) {
    assert(item);

    Configurator &configurator = Configurator::Get();

    const std::string full_suggested_path = configurator.path.GetFullPath(PATH_EXPORT_CONFIGURATION, item->configuration_name);
    const std::string full_export_path = configurator.path.SelectPath(this, PATH_EXPORT_CONFIGURATION, full_suggested_path);
    if (full_export_path.empty()) return;

    configurator.configurations.ExportConfiguration(configurator.layers.available_layers, full_export_path,
                                                    item->configuration_name);
}

void MainWindow::ReloadDefaultClicked(ConfigurationListItem *item) {
    (void)item;

    if (Alert::ConfiguratorReloadDefault() == QMessageBox::Yes) {
        _settings_tree_manager.CleanupGUI();

        Configurator &configurator = Configurator::Get();
        configurator.configurations.ReloadDefaultsConfigurations(configurator.layers.available_layers);

        LoadConfigurationList();
        _settings_tree_manager.CreateGUI(ui->settings_tree);
    }
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

void MainWindow::OnSettingsTreeClicked(QTreeWidgetItem *item, int column) {
    (void)column;
    (void)item;

    Configurator &configurator = Configurator::Get();
    configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);

    this->UpdateUI();
}

void MainWindow::SetupLauncherTree() {
    // App Name
    QTreeWidgetItem *launcher_parent = new QTreeWidgetItem();
    launcher_parent->setText(0, "Application");
    ui->launcher_tree->addTopLevelItem(launcher_parent);

    _launcher_apps_combo = new QComboBox();
    _launcher_apps_combo->setFocusPolicy(Qt::StrongFocus);
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

    // Executable
    QTreeWidgetItem *launcher_executable_item = new QTreeWidgetItem();
    launcher_executable_item->setText(0, "Executable");
    launcher_parent->addChild(launcher_executable_item);

    _launcher_executable = new QLineEdit();
    _launcher_executable->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_executable->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launcher_tree->setItemWidget(launcher_executable_item, 1, _launcher_executable);
    _launcher_executable->setReadOnly(false);

    _launcher_executable_browse_button = new QPushButton();
    _launcher_executable_browse_button->setText("...");
    _launcher_executable_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _launcher_executable_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    _launcher_executable_browse_button->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_executable_browse_button->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launcher_tree->setItemWidget(launcher_executable_item, 2, _launcher_executable_browse_button);
    connect(_launcher_executable, SIGNAL(textEdited(const QString &)), this, SLOT(launchChangeExecutable(const QString &)));
    connect(_launcher_executable_browse_button, SIGNAL(clicked()), this, SLOT(launchSetExecutable()));

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
    connect(_launcher_working, SIGNAL(textEdited(const QString &)), this, SLOT(launchChangeWorkingFolder(const QString &)));
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

    _launcher_log_file_browse_button = new QPushButton();
    _launcher_log_file_browse_button->setText("...");
    _launcher_log_file_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _launcher_log_file_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    ui->launcher_tree->setItemWidget(launcher_log_file_item, 2, _launcher_log_file_browse_button);
    connect(_launcher_log_file_browse_button, SIGNAL(clicked()), this, SLOT(launchSetLogFile()));

    // Launcher tree
    ui->launcher_tree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 5 + 6);
    ui->launcher_tree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 5 + 6);

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
    ui->launcher_tree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 5 + 6);
    ui->launcher_tree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 5 + 6);
    Configurator::Get().environment.Set(LAYOUT_LAUNCHER_COLLAPSED, QByteArray("false"));
}

// Collapsing the tree also shrinks the tree to match and show only the first line
void MainWindow::launchItemCollapsed(QTreeWidgetItem *item) {
    (void)item;
    ui->launcher_tree->setMinimumHeight(LAUNCH_ROW_HEIGHT + 6);
    ui->launcher_tree->setMaximumHeight(LAUNCH_ROW_HEIGHT + 6);
    Configurator::Get().environment.Set(LAYOUT_LAUNCHER_COLLAPSED, QByteArray("true"));
}

void MainWindow::OnLauncherLoaderMessageChanged(int level) {
    Configurator &configurator = Configurator::Get();

    configurator.environment.SetLoaderMessage(static_cast<LoaderMessageLevel>(level));
    configurator.request_vulkan_status = true;

    this->UpdateUI();
}

void MainWindow::launchSetExecutable() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    Application &application = configurator.environment.GetApplication(current_application_index);
    const std::string exe = configurator.path.SelectPath(this, PATH_EXECUTABLE, application.executable_path.c_str());

    // The user has cancel the operation
    if (exe.empty()) return;

    application.executable_path = exe;
    _launcher_executable->setText(exe.c_str());
}

void MainWindow::launchSetLogFile() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    Application &application = configurator.environment.GetApplication(current_application_index);
    const std::string path = configurator.path.SelectPath(this, PATH_LAUNCHER_LOG_FILE, application.log_file.c_str());

    // The user has cancel the operation
    if (path.empty()) return;

    application.log_file = path;
    _launcher_log_file_edit->setText(path.c_str());
}

void MainWindow::launchSetWorkingFolder() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    Application &application = configurator.environment.GetApplication(current_application_index);
    const std::string path = configurator.path.SelectPath(this, PATH_WORKING_DIR, application.working_folder.c_str());

    // The user has cancel the operation
    if (path.empty()) return;

    application.working_folder = path;
    _launcher_working->setText(path.c_str());
}

// Log file path edited manually.
void MainWindow::launchChangeLogFile(const QString &log_file) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Application &application = Configurator::Get().environment.GetApplication(current_application_index);
    application.log_file = log_file.toStdString();
}

void MainWindow::launchChangeExecutable(const QString &exe) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Application &application = Configurator::Get().environment.GetApplication(current_application_index);
    application.executable_path = exe.toStdString();
}

void MainWindow::launchChangeWorkingFolder(const QString &working_folder) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Application &application = Configurator::Get().environment.GetApplication(current_application_index);
    application.working_folder = working_folder.toStdString();
}

// Launch app change
void MainWindow::launchItemChanged(int application_index) {
    if (application_index < 0) return;

    Environment &environment = Configurator::Get().environment;

    environment.SelectActiveApplication(application_index);

    Application &application = environment.GetApplication(application_index);
    _launcher_executable->setText(application.executable_path.c_str());
    _launcher_working->setText(application.working_folder.c_str());
    _launcher_arguments->setText(application.arguments.c_str());
    _launcher_log_file_edit->setText(ReplaceBuiltInVariable(application.log_file.c_str()).c_str());
}

/// New command line arguments. Update them.
void MainWindow::launchArgsEdited(const QString &arguments) {
    int application_index = _launcher_apps_combo->currentIndex();
    if (application_index < 0) return;

    Application &application = Configurator::Get().environment.GetApplication(application_index);
    application.arguments = arguments.toStdString();
}

// Clear the browser window
void MainWindow::on_push_button_clear_log_clicked() {
    ui->log_browser->clear();
    ui->log_browser->update();
    ui->push_button_clear_log->setEnabled(false);
}

const Layer *GetLayer(QTreeWidget *tree, QTreeWidgetItem *item) {
    if (item == tree->invisibleRootItem()) return nullptr;
    if (item == nullptr) return nullptr;

    const std::string &text = item->text(0).toStdString().c_str();
    if (!text.empty()) {
        Configurator &configurator = Configurator::Get();

        for (std::size_t i = 0, n = configurator.layers.available_layers.size(); i < n; ++i) {
            const Layer &layer = configurator.layers.available_layers[i];
            if (text.find(layer.key) != std::string::npos) return &layer;
        }
    }

    return GetLayer(tree, item->parent());
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    if (event->type() == QEvent::Wheel) {
        return true;
    }

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

    Configurator &configurator = Configurator::Get();

    // Context menus for layer configuration files
    if (target == ui->settings_tree) {
        QContextMenuEvent *right_click = dynamic_cast<QContextMenuEvent *>(event);
        if (right_click) {
            QTreeWidgetItem *setting_item = ui->settings_tree->itemAt(right_click->pos());

            const Layer *layer = GetLayer(ui->settings_tree, setting_item);
            if (layer == nullptr) {
                return false;  // Unhandled action
            }

            // Create context menu here
            QMenu menu(ui->settings_tree);
            QFont subtitle_font = menu.font();
            subtitle_font.setBold(true);

            QAction *title_action = new QAction(layer->key.c_str(), nullptr);
            title_action->setFont(subtitle_font);
            menu.addAction(title_action);

            QAction *visit_layer_website_action = new QAction("Visit Layer Website...", nullptr);
            visit_layer_website_action->setEnabled(!layer->url.empty());
            menu.addAction(visit_layer_website_action);

            QAction *export_html_action = new QAction("Open Layer HTML Documentation...", nullptr);
            menu.addAction(export_html_action);

            QAction *export_markdown_action = new QAction("Open Layer Markdown Documentation...", nullptr);
            menu.addAction(export_markdown_action);

            QAction *export_settings_action = new QAction("Open Layer vk_layers_settings.txt...", nullptr);
            menu.addAction(export_settings_action);

            static const char *table[] = {
                "N/A",            // LAYER_STATE_APPLICATION_CONTROLLED
                "Exclude Layer",  // LAYER_STATE_OVERRIDDEN
                "Override Layer"  // LAYER_STATE_EXCLUDED
            };
            static_assert(countof(table) == LAYER_STATE_COUNT,
                          "The tranlation table size doesn't match the enum number of elements");

            Configuration *configuration = configurator.configurations.GetActiveConfiguration();
            Parameter *parameter = FindByKey(configuration->parameters, layer->key.c_str());

            QAction *layer_state_action = new QAction(table[parameter->state], nullptr);
            menu.addAction(layer_state_action);

            menu.addSeparator();

            QAction *show_advanced_setting_action = new QAction("View Advanced Settings", nullptr);
            show_advanced_setting_action->setEnabled(true);
            show_advanced_setting_action->setCheckable(true);
            show_advanced_setting_action->setChecked(configuration->view_advanced_settings);
            menu.addAction(show_advanced_setting_action);

            QPoint point(right_click->globalX(), right_click->globalY());
            QAction *action = menu.exec(point);

            if (action == title_action) {
                Alert::LayerProperties(layer);
            } else if (action == visit_layer_website_action) {
                QDesktopServices::openUrl(QUrl(layer->url.c_str()));
            } else if (action == layer_state_action) {
                switch (parameter->state) {
                    case LAYER_STATE_OVERRIDDEN:
                        parameter->state = LAYER_STATE_EXCLUDED;
                        break;
                    case LAYER_STATE_EXCLUDED:
                        parameter->state = LAYER_STATE_OVERRIDDEN;
                        break;
                    default:
                        assert(0);
                        break;
                }
                configuration->setting_tree_state.clear();
                _settings_tree_manager.CreateGUI(ui->settings_tree);
            } else if (action == show_advanced_setting_action) {
                configuration->view_advanced_settings = action->isChecked();
                configuration->setting_tree_state.clear();
                _settings_tree_manager.CreateGUI(ui->settings_tree);
            } else if (action == export_html_action) {
                const std::string path = format("%s/%s.html", GetPath(BUILTIN_PATH_APPDATA).c_str(), layer->key.c_str());
                ExportHtmlDoc(*layer, path);
                QDesktopServices::openUrl(QUrl(("file:///" + path).c_str()));
            } else if (action == export_markdown_action) {
                const std::string path = format("%s/%s.md", GetPath(BUILTIN_PATH_APPDATA).c_str(), layer->key.c_str());
                ExportMarkdownDoc(*layer, path);
                QDesktopServices::openUrl(QUrl(("file:///" + path).c_str()));
            } else if (action == export_settings_action) {
                std::vector<Layer> layers = {*layer};
                const std::string path = GetPath(BUILTIN_PATH_OVERRIDE_SETTINGS);
                ExportSettingsDoc(layers, *configuration, path);
                QDesktopServices::openUrl(QUrl(("file:///" + path).c_str()));
            } else {
                return false;  // Unknown action
            }

            // Do not pass on
            return true;
        }
    } else if (target == ui->configuration_tree) {
        QContextMenuEvent *right_click = dynamic_cast<QContextMenuEvent *>(event);
        if (right_click) {  // && event->type() == QEvent::ContextMenu) {
            // Which item were we over?
            QTreeWidgetItem *configuration_item = ui->configuration_tree->itemAt(right_click->pos());
            ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(configuration_item);

            const Environment &environment = configurator.environment;
            const std::string &active_contiguration_name = environment.Get(ACTIVE_CONFIGURATION);

            const bool active = environment.UseOverride() && !active_contiguration_name.empty();

            // Create context menu here
            QMenu menu(ui->configuration_tree);

            QAction *edit_action = new QAction("Edit...", nullptr);
            edit_action->setEnabled(active && item != nullptr);
            menu.addAction(edit_action);

            menu.addSeparator();

            QAction *new_action = new QAction("New...", nullptr);
            new_action->setEnabled(active);
            menu.addAction(new_action);

            menu.addSeparator();

            QAction *duplicate_action = new QAction("Duplicate", nullptr);
            duplicate_action->setEnabled(active && item != nullptr);
            menu.addAction(duplicate_action);

            QAction *rename_action = new QAction("Rename", nullptr);
            rename_action->setEnabled(active && item != nullptr);
            menu.addAction(rename_action);

            QAction *remove_action = new QAction("Remove", nullptr);
            remove_action->setEnabled(active && item != nullptr);
            menu.addAction(remove_action);

            QAction *reset_action = new QAction("Reset", nullptr);
            reset_action->setEnabled(active && item != nullptr);
            menu.addAction(reset_action);

            menu.addSeparator();

            QAction *import_action = new QAction("Import...", nullptr);
            import_action->setEnabled(active);
            menu.addAction(import_action);

            QAction *export_action = new QAction("Export...", nullptr);
            export_action->setEnabled(active && item != nullptr);
            menu.addAction(export_action);

            menu.addSeparator();

            QAction *reload_default_action = new QAction("Reload Default Configurations", nullptr);
            reload_default_action->setEnabled(true);
            menu.addAction(reload_default_action);

            QPoint point(right_click->globalX(), right_click->globalY());
            QAction *action = menu.exec(point);

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
            } else if (action == reset_action) {
                ResetClicked(item);
            } else if (action == export_action) {
                ExportClicked(item);
            } else if (action == import_action) {
                ImportClicked(item);
            } else if (action == reload_default_action) {
                ReloadDefaultClicked(item);
            } else {
                return false;  // Unknown action
            }

            // Do not pass on
            return true;
        }
    }

    // Pass it on
    return false;
}

bool MainWindow::SelectConfigurationItem(const std::string &configuration_name) {
    assert(!configuration_name.empty());

    for (int i = 0, n = ui->configuration_tree->topLevelItemCount(); i < n; ++i) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->configuration_tree->topLevelItem(i));
        assert(item != nullptr);
        assert(!item->configuration_name.empty());
        if (item->configuration_name == configuration_name) {
            ui->configuration_tree->setCurrentItem(item);
            return true;
        }
    }

    assert(0);
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

QStringList MainWindow::BuildEnvVariables() const {
    Configurator &configurator = Configurator::Get();

    QStringList env = QProcess::systemEnvironment();
    env << (QString("VK_LOADER_DEBUG=") + GetLoaderDebugToken(configurator.environment.GetLoaderMessage()).c_str());
    return env;
}

void MainWindow::on_push_button_launcher_clicked() {
    // Are we already monitoring a running app? If so, terminate it
    if (_launch_application != nullptr) {
        ResetLaunchApplication();
        return;
    }

    // We are logging, let's add that we've launched a new application
    std::string launch_log = "Launching Vulkan Application:\n";

    Configurator &configurator = Configurator::Get();
    const Application &active_application = configurator.environment.GetActiveApplication();

    Configuration *configuration = configurator.configurations.GetActiveConfiguration();

    std::string missing_layer;
    if (configuration == nullptr) {
        launch_log += "- Layers fully controlled by the application.\n";
    } else if (HasMissingLayer(configuration->parameters, configurator.layers.available_layers, missing_layer)) {
        launch_log += format("- No layers override. The active \"%s\" configuration is missing '%s' layer.\n",
                             configuration->key.c_str(), missing_layer.c_str());
    } else if (configurator.environment.UseOverride()) {
        if (configurator.environment.UseApplicationListOverrideMode() && configurator.environment.HasOverriddenApplications() &&
            !active_application.override_layers) {
            launch_log += "- Layers fully controlled by the application. Application excluded from layers override.\n";
        } else {
            launch_log += format("- Layers overridden by \"%s\" configuration.\n", configuration->key.c_str());
        }
    }

    const std::string actual_log_file = ReplaceBuiltInVariable(active_application.log_file.c_str());

    assert(!active_application.app_name.empty());
    launch_log += format("- Application: %s\n", active_application.app_name.c_str());
    assert(!active_application.executable_path.empty());
    launch_log += format("- Executable: %s\n", active_application.executable_path.c_str());
    assert(!active_application.working_folder.empty());
    launch_log += format("- Working Directory: %s\n", active_application.working_folder.c_str());
    if (!active_application.arguments.empty())
        launch_log += format("- Command-line Arguments: %s\n", active_application.arguments.c_str());
    if (!actual_log_file.empty()) launch_log += format("- Log file: %s\n", actual_log_file.c_str());

    if (!actual_log_file.empty()) {
        // Start logging
        // Make sure the log file is not already opened. This can occur if the
        // launched application is closed from the applicaiton.
        if (!_log_file.isOpen()) {
            _log_file.setFileName(actual_log_file.c_str());

            // Open and append, or open and truncate?
            QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
            if (!ui->check_box_clear_on_launch->isChecked()) mode |= QIODevice::Append;

            if (!_log_file.open(mode)) {
                Alert::LogFileFailed();
            }
        }
    }

    if (ui->check_box_clear_on_launch->isChecked()) ui->log_browser->clear();
    Log(launch_log.c_str());

    // Launch the test application
    _launch_application.reset(new QProcess(this));
    connect(_launch_application.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));
    connect(_launch_application.get(), SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));
    connect(_launch_application.get(), SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(processClosed(int, QProcess::ExitStatus)));

    _launch_application->setProgram(active_application.executable_path.c_str());
    _launch_application->setWorkingDirectory(active_application.working_folder.c_str());
    _launch_application->setEnvironment(BuildEnvVariables());

    if (!active_application.arguments.empty()) {
        const QStringList args = QString(active_application.arguments.c_str()).split(" ");
        _launch_application->setArguments(args);
    }

    _launch_application->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    _launch_application->setProcessChannelMode(QProcess::MergedChannels);
    _launch_application->closeWriteChannel();

    // Wait... did we start? Give it 4 seconds, more than enough time
    if (!_launch_application->waitForStarted(4000)) {
        _launch_application->deleteLater();
        _launch_application = nullptr;

        const std::string failed_log = std::string("Failed to launch ") + active_application.executable_path.c_str() + "!\n";
        Log(failed_log);
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
        Log(_launch_application->readAllStandardOutput().toStdString());
    }
}

void MainWindow::errorOutputAvailable() {
    if (_launch_application) {
        Log(_launch_application->readAllStandardError().toStdString());
    }
}

void MainWindow::Log(const std::string &log) {
    ui->log_browser->setPlainText(ui->log_browser->toPlainText() + "\n" + log.c_str());
    ui->push_button_clear_log->setEnabled(true);

    if (_log_file.isOpen()) {
        _log_file.write(log.c_str(), log.size());
        _log_file.flush();
    }
}
