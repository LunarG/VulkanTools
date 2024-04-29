/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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
#include <QPropertyAnimation>

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
      _tray_icon(nullptr),
      _tray_icon_menu(nullptr),
      _tray_restore_action(nullptr),
      _tray_quit_action(nullptr),
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
    connect(ui->configuration_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationItemDoubleClicked(QTreeWidgetItem *, int)));
    connect(ui->configuration_tree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(OnConfigurationTreeChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui->configuration_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationTreeClicked(QTreeWidgetItem *, int)));

    connect(ui->combo_box_layers_controlled, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboBoxModeChanged(int)));

    connect(ui->settings_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(editorExpanded(QTreeWidgetItem *)));
    // No need to do something when clicking on the setting tree, individual setting will update the configuration files
    // connect(ui->settings_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(OnSettingsTreeClicked(QTreeWidgetItem *,
    // int)));

    connect(ui->launcher_tree, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(launchItemCollapsed(QTreeWidgetItem *)));
    connect(ui->launcher_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(launchItemExpanded(QTreeWidgetItem *)));

    connect(ui->launcher_loader_debug, SIGNAL(currentIndexChanged(int)), this, SLOT(OnLauncherLoaderMessageChanged(int)));

    Configurator &configurator = Configurator::Get();
    Environment &environment = configurator.environment;

    // Restore window geometry from last launch
    restoreGeometry(environment.Get(VKCONFIG2_LAYOUT_MAIN_GEOMETRY));
    restoreState(environment.Get(VKCONFIG2_LAYOUT_MAIN_WINDOW_STATE));
    ui->splitter->restoreState(environment.Get(VKCONFIG2_LAYOUT_MAIN_SPLITTER1));
    ui->splitter_2->restoreState(environment.Get(VKCONFIG2_LAYOUT_MAIN_SPLITTER2));
    ui->splitter_3->restoreState(environment.Get(VKCONFIG2_LAYOUT_MAIN_SPLITTER3));

    ui->check_box_persistent->setToolTip("Keep Vulkan Configurator running in system tray when closing the main window");
    ui->check_box_persistent->setVisible(QSystemTrayIcon::isSystemTrayAvailable());

    LoadConfigurationList();

    // Resetting this from the default prevents the log window (a QTextEdit) from overflowing.
    // Whenever the control surpasses this block count, old blocks are discarded.
    // Note: We could make this a user configurable setting down the road should this be
    // insufficinet.
    ui->log_browser->document()->setMaximumBlockCount(2048);
    ui->configuration_tree->scrollToItem(ui->configuration_tree->topLevelItem(0), QAbstractItemView::PositionAtTop);

    this->InitTray();
    this->UpdateTray();
    this->UpdateUI();
}

MainWindow::~MainWindow() { ResetLaunchApplication(); }

void MainWindow::InitTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        this->_tray_quit_action = new QAction("&Quit", this);
        connect(this->_tray_quit_action, &QAction::triggered, qApp, &QCoreApplication::quit);

        this->_tray_restore_action = new QAction("Open &Vulkan Configurator", this);
        connect(this->_tray_restore_action, &QAction::triggered, this, &MainWindow::trayActionRestore);

        this->_tray_layers_controlled_by_applications = new QAction("Layers Controlled by the Vulkan Applications", this);
        this->_tray_layers_controlled_by_applications->setCheckable(true);
        connect(this->_tray_layers_controlled_by_applications, &QAction::triggered, this,
                &MainWindow::trayActionControlledByApplications);

        this->_tray_layers_controlled_by_configurator = new QAction("Layers Controlled by the Vulkan Configurator", this);
        this->_tray_layers_controlled_by_configurator->setCheckable(true);
        connect(this->_tray_layers_controlled_by_configurator, &QAction::triggered, this,
                &MainWindow::trayActionControlledByConfigurator);

        this->_tray_layers_disabled_by_configurator = new QAction("Layers Disabled by the Vulkan Configurator", this);
        this->_tray_layers_disabled_by_configurator->setCheckable(true);
        connect(this->_tray_layers_disabled_by_configurator, &QAction::triggered, this,
                &MainWindow::trayActionDisabledByApplications);

        this->_tray_icon_menu = new QMenu(this);
        this->_tray_icon_menu->addAction(this->_tray_restore_action);
        this->_tray_icon_menu->addSeparator();
        this->_tray_icon_menu->addAction(this->_tray_layers_controlled_by_applications);
        this->_tray_icon_menu->addAction(this->_tray_layers_controlled_by_configurator);
        this->_tray_icon_menu->addAction(this->_tray_layers_disabled_by_configurator);
        this->_tray_icon_menu->addSeparator();
        this->_tray_icon_menu->addAction(this->_tray_quit_action);

        this->_tray_icon = new QSystemTrayIcon(this);
        this->_tray_icon->setContextMenu(this->_tray_icon_menu);
        this->UpdateTray();
        this->_tray_icon->show();

        this->connect(this->_tray_icon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    }
}

void MainWindow::UpdateTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        QApplication::setQuitOnLastWindowClosed(!ui->check_box_persistent->isChecked());

        Configurator &configurator = Configurator::Get();

        const Environment &environment = configurator.environment;

        const bool use_override = environment.GetMode() != LAYERS_MODE_BY_APPLICATIONS;
        const bool active =
            configurator.configurations.HasActiveConfiguration(configurator.layers.available_layers) && use_override;

        switch (environment.GetMode()) {
            default:
            case LAYERS_MODE_BY_APPLICATIONS:
                this->_tray_layers_controlled_by_applications->setChecked(true);
                this->_tray_layers_controlled_by_configurator->setChecked(false);
                this->_tray_layers_disabled_by_configurator->setChecked(false);
                break;
            case LAYERS_MODE_BY_CONFIGURATOR_RUNNING:
                this->_tray_layers_controlled_by_applications->setChecked(false);
                this->_tray_layers_controlled_by_configurator->setChecked(true);
                this->_tray_layers_disabled_by_configurator->setChecked(false);
                break;
            case LAYERS_MODE_BY_CONFIGURATOR_ALL_DISABLED:
                this->_tray_layers_controlled_by_applications->setChecked(false);
                this->_tray_layers_controlled_by_configurator->setChecked(false);
                this->_tray_layers_disabled_by_configurator->setChecked(true);
                break;
        }

        if (active) {
            const QIcon icon(":/resourcefiles/vkconfig-on.png");

            this->setWindowIcon(icon);
            this->_tray_icon->setIcon(icon);
            this->_tray_icon->setToolTip("Layers controlled by the Vulkan Configurator");
        } else {
            const QIcon icon(":/resourcefiles/vkconfig-off.png");

            this->setWindowIcon(icon);
            this->_tray_icon->setIcon(icon);
            this->_tray_icon->setToolTip("Layers controlled by the Vulkan Applications");
        }
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        default:
            break;
        case QSystemTrayIcon::Context:
            break;
        case QSystemTrayIcon::DoubleClick:
            Qt::WindowStates window_states = this->windowState();

            const bool is_minimized = this->isMinimized();
            const bool is_visible = this->isVisible();
            const bool is_hidden = this->isHidden();

            if (this->isMinimized() || this->isHidden()) {
                this->setVisible(true);
                this->showNormal();
                this->setWindowState(Qt::WindowState::WindowActive);
            } else {
                this->hide();
            }

            this->UpdateTray();
            break;
    }
}

void MainWindow::trayActionRestore() {
    this->hide();
    this->showNormal();

    this->UpdateUI();
    this->UpdateTray();
}

void MainWindow::trayActionControlledByApplications() {
    Configurator &configurator = Configurator::Get();
    configurator.environment.SetMode(LAYERS_MODE_BY_APPLICATIONS);
    configurator.configurations.Configure(configurator.layers.available_layers);

    this->UpdateUI();
    this->UpdateTray();
}

void MainWindow::trayActionControlledByConfigurator() {
    Configurator &configurator = Configurator::Get();
    configurator.environment.SetMode(LAYERS_MODE_BY_CONFIGURATOR_RUNNING);
    configurator.configurations.Configure(configurator.layers.available_layers);

    this->UpdateUI();
    this->UpdateTray();
}

void MainWindow::trayActionDisabledByApplications() {
    Configurator &configurator = Configurator::Get();
    configurator.environment.SetMode(LAYERS_MODE_BY_CONFIGURATOR_ALL_DISABLED);
    configurator.configurations.Configure(configurator.layers.available_layers);

    this->UpdateUI();
    this->UpdateTray();
}

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
    static int check_recurse = 0;
    ++check_recurse;
    assert(check_recurse <= 2);

    Configurator &configurator = Configurator::Get();
    const Environment &environment = Configurator::Get().environment;
    const std::string &selected_contiguration_name = environment.GetSelectedConfiguration();
    const bool has_selected_configuration = !selected_contiguration_name.empty();

    this->blockSignals(true);
    this->ClearLog();

    ui->configuration_tree->blockSignals(true);

    ui->combo_box_layers_controlled->blockSignals(true);
    ui->combo_box_layers_controlled->setCurrentIndex(environment.GetMode());
    ui->combo_box_layers_controlled->blockSignals(false);

    const bool has_active_configuration = configurator.configurations.HasActiveConfiguration(configurator.layers.available_layers);

    // Mode states
    this->UpdateTray();

    // Update configurations
    ui->group_box_configurations->setEnabled(environment.GetMode() == LAYERS_MODE_BY_CONFIGURATOR_RUNNING);
    ui->configuration_tree->setCurrentItem(nullptr);

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
        item->radio_button->blockSignals(true);

        if (item->configuration_name == selected_contiguration_name) {
            item->radio_button->setChecked(true);
            ui->configuration_tree->setCurrentItem(item);
        } else {
            // item->radio_button->setChecked(false);
        }

        item->radio_button->blockSignals(false);
    }

    // Update settings
    ui->push_button_edit->setEnabled(has_selected_configuration);
    ui->push_button_remove->setEnabled(has_selected_configuration);
    ui->push_button_duplicate->setEnabled(has_selected_configuration);
    ui->push_button_new->setEnabled(true);
    if (has_selected_configuration) {
        ui->group_box_settings->setTitle((selected_contiguration_name + " Settings").c_str());
    } else {
        ui->group_box_settings->setTitle("Configuration Settings");
    }

    // Handle application lists states
    ui->check_box_apply_list->setEnabled(!been_warned_about_old_loader &&
                                         ui->combo_box_layers_controlled->currentIndex() != LAYERS_MODE_BY_APPLICATIONS);
    ui->check_box_apply_list->setChecked(!been_warned_about_old_loader && environment.GetUseApplicationList());
    ui->push_button_applications->setEnabled(!been_warned_about_old_loader &&
                                             ui->combo_box_layers_controlled->currentIndex() != LAYERS_MODE_BY_APPLICATIONS);

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
        this->UpdateApplicationUI(application);
    }

    _launcher_apps_combo->blockSignals(false);

    // Handle persistent states
    ui->check_box_persistent->setChecked(environment.GetUseSystemTray());

    // Launcher states
    const bool has_application_list = !environment.GetApplications().empty();
    ui->push_button_launcher->setEnabled(has_application_list);
    ui->push_button_launcher->setText(_launch_application ? "Terminate" : "Launch");
    ui->check_box_clear_on_launch->setChecked(environment.Get(LAYOUT_LAUNCHER_NOT_CLEAR) != "true");
    ui->launcher_loader_debug->blockSignals(true);  // avoid calling again UpdateUI
    ui->launcher_loader_debug->setCurrentIndex(GetLoaderMessageType(environment.GetLoaderMessageTypes()));
    ui->launcher_loader_debug->blockSignals(false);

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

    ui->settings_tree->setEnabled(environment.GetMode() == LAYERS_MODE_BY_CONFIGURATOR_RUNNING && has_selected_configuration);
    if (has_selected_configuration) {
        _settings_tree_manager.CreateGUI(ui->settings_tree);
    } else {
        _settings_tree_manager.CleanupGUI();
    }

    // Update title bar
    setWindowTitle(GetMainWindowTitle(has_active_configuration).c_str());

    ui->configuration_tree->blockSignals(false);
    this->blockSignals(false);

    --check_recurse;
}

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
        // connect(item->radio_button, SIGNAL(clicked(bool)), this, SLOT(OnConfigurationItemClicked(bool)));
        connect(item->radio_button, SIGNAL(toggled(bool)), this, SLOT(OnConfigurationItemClicked(bool)));
    }

    ui->configuration_tree->blockSignals(false);
    ui->configuration_tree->resizeColumnToContents(0);
    ui->configuration_tree->resizeColumnToContents(1);
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

void MainWindow::OnComboBoxModeChanged(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.environment.SetMode(static_cast<LayersMode>(ui->combo_box_layers_controlled->currentIndex()));

    this->UpdateUI();
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
        configurator.environment.SetUseApplicationList(false);
        been_warned_about_old_loader = true;

        return;
    }

    configurator.environment.SetUseApplicationList(ui->check_box_apply_list->isChecked());

    // Handle the case where no application with active override is present
    const bool application_list_requires_update = !configurator.environment.HasOverriddenApplications();
    if (ui->check_box_apply_list->isChecked() && application_list_requires_update) {
        ApplicationsDialog dialog(this);
        dialog.exec();
    }

    configurator.configurations.Configure(configurator.layers.available_layers);

    UpdateUI();
}

void MainWindow::on_check_box_persistent_clicked() {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return;
    }

    Environment &environment = Configurator::Get().environment;

    // Alert the user to the current state of the vulkan configurator and
    // give them the option to not shutdown.
    QSettings settings;
    if (ui->check_box_persistent->isChecked() && !settings.value("vkconfig_system_tray_stay_on_close", false).toBool()) {
        const QPalette saved_palette = ui->check_box_persistent->palette();
        QPalette modified_palette = saved_palette;
        modified_palette.setColor(QPalette::ColorRole::WindowText, QColor(255, 0, 0, 255));
        ui->check_box_persistent->setPalette(modified_palette);

        const std::string message = "Vulkan Layers will remain controlled by Vulkan Configurator while active in the system tray.";

        QMessageBox alert(this);
        alert.setWindowTitle("Vulkan Configurator behavior when closing the main window");
        alert.setText(message.c_str());
        alert.setIcon(QMessageBox::Warning);
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setDefaultButton(QMessageBox::No);
        alert.setCheckBox(new QCheckBox("Do not show again."));
        alert.setInformativeText(
            "Do you want to keep Vulkan Configurator running in the system tray when closing the main window?");

        int ret_val = alert.exec();
        settings.setValue("vkconfig_system_tray_stay_on_close", alert.checkBox()->isChecked());

        ui->check_box_persistent->setPalette(saved_palette);

        if (ret_val == QMessageBox::No) {
            ui->check_box_persistent->setChecked(false);
            return;
        }
    }

    environment.SetUseSystemTray(ui->check_box_persistent->isChecked());
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

    this->UpdateUI();
}

// Thist signal actually comes from the radio button
void MainWindow::OnConfigurationItemClicked(bool checked) {
    (void)checked;
    // Someone just got checked, they are now the current profile
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *configuration_item = GetCheckedItem();
    if (configuration_item == nullptr) {
        return;
    }

    // This appears redundant on Windows, but under linux it is needed
    // to ensure the new item is "selected"
    ui->configuration_tree->setCurrentItem(configuration_item);

    Configurator &configurator = Configurator::Get();
    if (configurator.environment.GetSelectedConfiguration() != configuration_item->configuration_name) {
        configurator.ActivateConfiguration(configuration_item->configuration_name);
        this->UpdateUI();
    }
}

void MainWindow::OnConfigurationTreeClicked(QTreeWidgetItem *item, int column) {
    (void)column;

    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(item);
    if (configuration_item == nullptr) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    if (configurator.environment.GetSelectedConfiguration() != configuration_item->configuration_name) {
        configurator.ActivateConfiguration(configuration_item->configuration_name);
        this->UpdateUI();
    }
}

/// This gets called with keyboard selections and clicks that do not necessarily
/// result in a radio button change (but it may). So we need to do two checks here, one
/// for the radio button, and one to change the editor/information at lower right.
void MainWindow::OnConfigurationTreeChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    (void)previous;

    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(current);
    if (configuration_item == nullptr) {
        return;
    }
    configuration_item->radio_button->setChecked(true);

    Configurator &configurator = Configurator::Get();
    if (configurator.environment.GetSelectedConfiguration() != configuration_item->configuration_name) {
        configurator.ActivateConfiguration(configuration_item->configuration_name);
        this->UpdateUI();
    }
}

void MainWindow::OnConfigurationItemDoubleClicked(QTreeWidgetItem *item, int column) {
    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(item);
    if (configuration_item == nullptr) return;

    EditClicked(configuration_item);
}

/// An item has been changed. Check for edit of the items name (configuration name)
void MainWindow::OnConfigurationItemChanged(QTreeWidgetItem *item, int column) {
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(item);
    if (configuration_item == nullptr) {
        return;
    }

    if (column == 1) {  // configuration name
        _settings_tree_manager.CleanupGUI();
        Configurator &configurator = Configurator::Get();

        // We are renaming the file. Things can go wrong here...
        // This is the name of the configuratin we are changing
        // const std::string full_path =
        //    ConvertNativeSeparators(GetPath(BUILTIN_PATH_CONFIG_LAST) + "/" + configuration_item->configuration_name + ".json");

        // This is the new name we want to use for the configuration
        const std::string &new_name = configuration_item->text(1).toStdString();
        bool valid_new_name = true;

        if (new_name.empty()) {
            valid_new_name = false;
            Alert::ConfigurationNameEmpty();
        } else {
            if (!IsPortableFilename(new_name)) {
                valid_new_name = false;
                Alert::ConfigurationNameInvalid();
            }
            if (valid_new_name &&
                configuration_item->text(1).contains(QRegularExpression(QStringLiteral("[^\\x{0000}-\\x{007F}]")))) {
                valid_new_name = false;
                Alert::ConfigurationNameASCII();
            }
        }

        Configuration *duplicate_configuration = FindByKey(configurator.configurations.available_configurations, new_name.c_str());
        if (duplicate_configuration != nullptr) {
            valid_new_name = false;
            Alert::ConfigurationRenamingFailed();
        }

        // Find existing configuration using it's old name
        const std::string old_name = configuration_item->configuration_name;
        Configuration *configuration = FindByKey(configurator.configurations.available_configurations, old_name.c_str());

        if (valid_new_name) {
            // Rename configuration ; Remove old configuration file ; change the name of the configuration
            configurator.configurations.RemoveConfigurationFile(old_name);
            configuration->key = configuration_item->configuration_name = new_name;
            configurator.configurations.SaveAllConfigurations(configurator.layers.available_layers);

            configurator.ActivateConfiguration(new_name);

            LoadConfigurationList();
        } else {
            // If the configurate name is empty or the configuration name is taken, keep old configuration name

            ui->configuration_tree->blockSignals(true);
            item->setText(1, old_name.c_str());
            ui->configuration_tree->blockSignals(false);

            configurator.ActivateConfiguration(old_name);
        }

        this->UpdateUI();
    }
}

// Unused flag, just display the about Qt dialog
void MainWindow::OnHelpAbout(bool checked) {
    (void)checked;

    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::StartTool(Tool tool) {
    Configurator &configurator = Configurator::Get();

    LayersMode saved_mode = configurator.environment.GetMode();
    configurator.environment.SetMode(LAYERS_MODE_BY_APPLICATIONS);
    configurator.configurations.Configure(configurator.layers.available_layers);

    switch (tool) {
        case TOOL_VULKAN_INFO:
            vk_info_dialog.reset(new VulkanInfoDialog(this));
            break;
        case TOOL_VULKAN_INSTALL:
            vk_installation_dialog.reset(new VulkanAnalysisDialog(this));
            break;
    }

    configurator.environment.SetMode(saved_mode);
    configurator.configurations.Configure(configurator.layers.available_layers);
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

    // If a child process is still running, destroy it
    if (_launch_application) {
        ResetLaunchApplication();
    }

    _settings_tree_manager.CleanupGUI();

    environment.Set(VKCONFIG2_LAYOUT_MAIN_GEOMETRY, saveGeometry());
    environment.Set(VKCONFIG2_LAYOUT_MAIN_WINDOW_STATE, saveState());
    environment.Set(VKCONFIG2_LAYOUT_MAIN_SPLITTER1, ui->splitter->saveState());
    environment.Set(VKCONFIG2_LAYOUT_MAIN_SPLITTER2, ui->splitter_2->saveState());
    environment.Set(VKCONFIG2_LAYOUT_MAIN_SPLITTER3, ui->splitter_3->saveState());

    environment.Save();

    if (QSystemTrayIcon::isSystemTrayAvailable() && environment.GetUseSystemTray()) {
        QGuiApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        event->ignore();
    } else {
        QGuiApplication::setQuitOnLastWindowClosed(true);
        QMainWindow::closeEvent(event);
    }
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
    configurator.configurations.Configure(configurator.layers.available_layers);

    UpdateUI();
}

void MainWindow::on_push_button_new_clicked() {
    Configurator &configurator = Configurator::Get();
    const std::string selected_configuration = configurator.environment.GetSelectedConfiguration();

    Configuration &new_configuration =
        configurator.configurations.CreateConfiguration(configurator.layers.available_layers, "New Configuration");

    std::string activate_configuration;

    LayersDialog dlg(this, new_configuration);
    switch (dlg.exec()) {
        case QDialog::Accepted:
            activate_configuration = new_configuration.key;
            break;
        case QDialog::Rejected:
            activate_configuration = selected_configuration;
            configurator.configurations.RemoveConfiguration(configurator.layers.available_layers, new_configuration.key);
            break;
        default:
            assert(0);
            break;
    }

    configurator.ActivateConfiguration(activate_configuration);

    LoadConfigurationList();

    this->UpdateUI();
}

void MainWindow::on_push_button_remove_clicked() {
    Configurator &configurator = Configurator::Get();

    this->RemoveConfiguration(configurator.environment.GetSelectedConfiguration());
}

void MainWindow::on_push_button_duplicate_clicked() {
    Configurator &configurator = Configurator::Get();

    Configuration *configutation = configurator.configurations.FindActiveConfiguration();
    assert(configutation != nullptr);

    const Configuration &duplicated_configuration =
        configurator.configurations.CreateConfiguration(configurator.layers.available_layers, configutation->key, true);

    configurator.ActivateConfiguration(duplicated_configuration.key);

    LoadConfigurationList();

    this->UpdateUI();
}

void MainWindow::on_push_button_edit_clicked() {
    Configurator &configurator = Configurator::Get();
    Configuration *configuration = configurator.configurations.FindActiveConfiguration();
    assert(configuration != nullptr);

    LayersDialog dlg(this, *configuration);
    if (dlg.exec() == QDialog::Accepted) {
        configurator.ActivateConfiguration(configuration->key);

        LoadConfigurationList();

        this->UpdateUI();
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
        configurator.configurations.SortConfigurations();
        configurator.ActivateConfiguration(configuration->key);

        LoadConfigurationList();

        this->UpdateUI();
    }
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
    if (alert.exec() == QMessageBox::No) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    configurator.configurations.RemoveConfiguration(configurator.layers.available_layers, configuration_name);
    configurator.environment.SetSelectedConfiguration("");

    LoadConfigurationList();

    this->UpdateUI();
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
    if (alert.exec() == QMessageBox::No) {
        return;
    }

    configuration->Reset(configurator.layers.available_layers, configurator.path);

    configurator.ActivateConfiguration(configuration->key);

    LoadConfigurationList();

    this->UpdateUI();
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

    this->UpdateUI();
}

void MainWindow::ImportClicked(ConfigurationListItem *item) {
    (void)item;  // We don't need this

    Configurator &configurator = Configurator::Get();

    const std::string full_import_path = configurator.path.SelectPath(this, PATH_IMPORT_CONFIGURATION);
    if (full_import_path.empty()) return;

    const std::string imported_configuration =
        configurator.configurations.ImportConfiguration(configurator.layers.available_layers, full_import_path);
    if (imported_configuration.empty()) {
        return;
    }

    configurator.ActivateConfiguration(imported_configuration);

    LoadConfigurationList();

    this->UpdateUI();
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

        configurator.ActivateConfiguration(configurator.environment.GetSelectedConfiguration());

        LoadConfigurationList();

        this->UpdateUI();
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
    configurator.configurations.Configure(configurator.layers.available_layers);

    // Don't update UI here. It's not useful and cause setting tree rebuild
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

    configurator.environment.SetLoaderMessageTypes(GetLoaderMessageFlags(static_cast<LoaderMessageType>(level)));

    if (ui->check_box_clear_on_launch->isChecked()) {
        this->UpdateUI();
    }
}

void MainWindow::launchSetExecutable() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    Application &application = configurator.environment.GetApplication(current_application_index);
    std::string new_path = _launcher_executable->text().toStdString();
    if (!QFileInfo(ReplaceBuiltInVariable(new_path).c_str()).exists()) {
        new_path = application.executable_path.c_str();
    }

    const std::string selected_path = configurator.path.SelectPath(this, PATH_EXECUTABLE, ReplaceBuiltInVariable(new_path));

    // The user has cancel the operation
    if (selected_path.empty()) {
        return;
    }

    // The path didn't change, preserve the built-in variables
    if (ReplaceBuiltInVariable(new_path) == selected_path) {
        return;
    }

    application.executable_path = selected_path;
    _launcher_executable->setText(selected_path.c_str());
}

void MainWindow::launchSetLogFile() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    Application &application = configurator.environment.GetApplication(current_application_index);
    std::string new_path = _launcher_log_file_edit->text().toStdString();
    if (!QFileInfo(ReplaceBuiltInVariable(new_path).c_str()).exists()) {
        new_path = application.log_file.c_str();
    }

    const std::string selected_path = configurator.path.SelectPath(this, PATH_LAUNCHER_LOG_FILE, ReplaceBuiltInVariable(new_path));

    // The user has cancel the operation
    if (selected_path.empty()) {
        return;
    }

    // The path didn't change, preserve the built-in variables
    if (ReplaceBuiltInVariable(new_path) == selected_path) {
        return;
    }

    application.log_file = selected_path;
    _launcher_log_file_edit->setText(selected_path.c_str());
}

void MainWindow::launchSetWorkingFolder() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    Application &application = configurator.environment.GetApplication(current_application_index);
    std::string new_path = _launcher_working->text().toStdString();
    if (!QFileInfo(ReplaceBuiltInVariable(new_path).c_str()).exists()) {
        new_path = application.working_folder.c_str();
    }

    const std::string selected_path = configurator.path.SelectPath(this, PATH_WORKING_DIR, ReplaceBuiltInVariable(new_path));

    // The user has cancel the operation
    if (selected_path.empty()) {
        return;
    }

    // The path didn't change, preserve the built-in variables
    if (ReplaceBuiltInVariable(new_path) == selected_path) {
        return;
    }

    application.working_folder = selected_path;
    _launcher_working->setText(selected_path.c_str());
}

// Log file path edited manually.
void MainWindow::launchChangeLogFile(const QString &log_file) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    const std::string replaced_path = ReplaceBuiltInVariable(log_file.toStdString());

    if (QFileInfo(replaced_path.c_str()).exists()) {
        Application &application = Configurator::Get().environment.GetApplication(current_application_index);
        application.log_file = log_file.toStdString();
    }
}

void MainWindow::launchChangeExecutable(const QString &exe_path) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    const std::string replaced_path = ReplaceBuiltInVariable(exe_path.toStdString());

    if (QFileInfo(replaced_path.c_str()).exists()) {
        Application &application = Configurator::Get().environment.GetApplication(current_application_index);
        application.executable_path = exe_path.toStdString();
    }
}

void MainWindow::launchChangeWorkingFolder(const QString &working_folder) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    const std::string replaced_path = ReplaceBuiltInVariable(working_folder.toStdString());

    if (QFileInfo(replaced_path.c_str()).exists()) {
        Application &application = Configurator::Get().environment.GetApplication(current_application_index);
        application.working_folder = working_folder.toStdString();
    }
}

void MainWindow::UpdateApplicationUI(const Application &application) {
    _launcher_executable->setText(application.executable_path.c_str());
    _launcher_executable->setToolTip(ReplaceBuiltInVariable(application.executable_path.c_str()).c_str());
    _launcher_arguments->setText(application.arguments.c_str());
    _launcher_working->setText(application.working_folder.c_str());
    _launcher_working->setToolTip(ReplaceBuiltInVariable(application.working_folder.c_str()).c_str());
    _launcher_log_file_edit->setText(application.log_file.c_str());
    _launcher_log_file_edit->setToolTip(ReplaceBuiltInVariable(application.log_file.c_str()).c_str());
}

// Launch app change
void MainWindow::launchItemChanged(int application_index) {
    if (application_index < 0) return;

    Environment &environment = Configurator::Get().environment;

    environment.SelectActiveApplication(application_index);

    const Application &application = environment.GetApplication(application_index);
    this->UpdateApplicationUI(application);
}

/// New command line arguments. Update them.
void MainWindow::launchArgsEdited(const QString &arguments) {
    int application_index = _launcher_apps_combo->currentIndex();
    if (application_index < 0) return;

    Application &application = Configurator::Get().environment.GetApplication(application_index);
    application.arguments = arguments.toStdString();
}

// Clear the browser window
void MainWindow::ClearLog() {
    if (ui->check_box_clear_on_launch->isChecked()) {
        on_push_button_clear_log_clicked();
    }
}

void MainWindow::on_push_button_clear_log_clicked() {
    ui->log_browser->clear();
    ui->log_browser->update();
    ui->push_button_clear_log->setEnabled(false);
}

void MainWindow::on_push_button_status_clicked() { this->UpdateStatus(); }

void MainWindow::UpdateStatus() {
    ui->push_button_clear_log->setEnabled(true);

    QString text = ("Vulkan Development Status:\n" + GenerateVulkanStatus() + "\n").c_str();

    if (!ui->check_box_clear_on_launch->isChecked()) {
        text += ui->log_browser->toPlainText();
    }

    ui->log_browser->setPlainText(text);
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
            bool require_update_ui = false;

            QTreeWidgetItem *setting_item = ui->settings_tree->itemAt(right_click->pos());

            std::string layer_name = setting_item->toolTip(0).toStdString();

            const Layer *layer = GetLayer(ui->settings_tree, setting_item);
            if (layer == nullptr && layer_name.empty()) {
                return false;  // Unhandled action
            }

            if (layer != nullptr) {
                layer_name = layer->key;
            }

            // Create context menu here
            QMenu menu(ui->settings_tree);
            QFont subtitle_font = menu.font();
            subtitle_font.setBold(true);

            QAction *title_action = new QAction(setting_item->text(0).toStdString().c_str(), nullptr);
            title_action->setEnabled(layer != nullptr);
            title_action->setFont(subtitle_font);
            menu.addAction(title_action);

            QAction *visit_layer_website_action = new QAction("Visit Layer Website...", nullptr);
            visit_layer_website_action->setEnabled(layer != nullptr ? !layer->url.empty() : false);
            menu.addAction(visit_layer_website_action);

            QAction *export_html_action = new QAction("Open Layer HTML Documentation...", nullptr);
            export_html_action->setEnabled(layer != nullptr);
            menu.addAction(export_html_action);

            QAction *export_markdown_action = new QAction("Open Layer Markdown Documentation...", nullptr);
            export_markdown_action->setEnabled(layer != nullptr);
            menu.addAction(export_markdown_action);

            QAction *export_settings_action = new QAction("Open Layer vk_layers_settings.txt...", nullptr);
            export_settings_action->setEnabled(layer != nullptr);
            menu.addAction(export_settings_action);

            static const char *table[] = {
                "N/A",            // LAYER_STATE_APPLICATION_CONTROLLED
                "Exclude Layer",  // LAYER_STATE_OVERRIDDEN
                "Override Layer"  // LAYER_STATE_EXCLUDED
            };
            static_assert(countof(table) == LAYER_STATE_COUNT,
                          "The tranlation table size doesn't match the enum number of elements");

            Configuration *configuration = configurator.configurations.FindActiveConfiguration();
            Parameter *parameter = FindByKey(configuration->parameters, layer_name.c_str());

            QAction *layer_state_action = new QAction(layer != nullptr ? table[parameter->state] : "Remove Layer", nullptr);
            if (layer == nullptr) {
                layer_state_action->setFont(subtitle_font);
            }
            menu.addAction(layer_state_action);

            menu.addSeparator();

            QAction *show_advanced_setting_action = new QAction("View Advanced Settings", nullptr);
            show_advanced_setting_action->setEnabled(layer != nullptr);
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
                require_update_ui = true;
            } else if (action == show_advanced_setting_action) {
                configuration->view_advanced_settings = action->isChecked();
                configuration->setting_tree_state.clear();
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

            if (require_update_ui) {
                this->UpdateUI();
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
            const std::string &active_contiguration_name = environment.GetSelectedConfiguration();

            const bool active = environment.GetMode() != LAYERS_MODE_BY_APPLICATIONS && !active_contiguration_name.empty();

            // Create context menu here
            QMenu menu(ui->configuration_tree);

            QAction *rename_action = new QAction("Rename", nullptr);
            rename_action->setEnabled(active && item != nullptr);
            menu.addAction(rename_action);

            menu.addSeparator();

            QAction *import_action = new QAction("Import...", nullptr);
            import_action->setEnabled(true);
            menu.addAction(import_action);

            QAction *export_action = new QAction("Export...", nullptr);
            export_action->setEnabled(active && item != nullptr);
            menu.addAction(export_action);

            menu.addSeparator();

            QAction *reset_action = new QAction("Reset", nullptr);
            reset_action->setEnabled(active && item != nullptr);
            menu.addAction(reset_action);

            menu.addSeparator();

            QAction *reload_default_action = new QAction("Reload Default Configurations", nullptr);
            reload_default_action->setEnabled(true);
            menu.addAction(reload_default_action);

            QPoint point(right_click->globalX(), right_click->globalY());
            QAction *action = menu.exec(point);

            if (action == rename_action) {
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

        ui->push_button_launcher->setText("Launch");
    }
}

QStringList MainWindow::BuildEnvVariables() const {
    Configurator &configurator = Configurator::Get();

    QStringList env = QProcess::systemEnvironment();
    env << (QString("VK_LOADER_DEBUG=") + ::GetLoaderMessageTokens(configurator.environment.GetLoaderMessageTypes()).c_str());
    return env;
}

void MainWindow::on_push_button_launcher_clicked() {
    // Are we already monitoring a running app? If so, terminate it
    if (_launch_application != nullptr) {
        ResetLaunchApplication();
        return;
    }

    std::string launch_log;

    // Update the Vulkan Developemnt status to record the system configuration
    QString vulkan_status = ("Vulkan Development Status:\n" + GenerateVulkanStatus() + "\n").c_str();
    launch_log += vulkan_status.toStdString();

    // We are logging, let's add that we've launched a new application
    launch_log += "Launching Vulkan Application:\n";

    Configurator &configurator = Configurator::Get();
    const Application &active_application = configurator.environment.GetActiveApplication();

    assert(!active_application.app_name.empty());
    launch_log += format("- Application: %s\n", active_application.app_name.c_str());
    assert(!active_application.executable_path.empty());

    launch_log += format("- Executable: %s\n", ReplaceBuiltInVariable(_launcher_executable->text().toStdString()).c_str());
    if (!QFileInfo(ReplaceBuiltInVariable(_launcher_executable->text().toStdString()).c_str()).exists()) {
        Alert::PathInvalid(ReplaceBuiltInVariable(_launcher_executable->text().toStdString()).c_str(),
                           format("The '%s' application will fail to launch.", active_application.app_name.c_str()).c_str());
    }

    launch_log += format("- Working Directory: %s\n", ReplaceBuiltInVariable(_launcher_working->text().toStdString()).c_str());
    if (!QFileInfo(ReplaceBuiltInVariable(_launcher_working->text().toStdString()).c_str()).exists()) {
        Alert::PathInvalid(ReplaceBuiltInVariable(_launcher_working->text().toStdString()).c_str(),
                           format("The '%s' application will fail to launch.", active_application.app_name.c_str()).c_str());
    }

    if (!_launcher_arguments->text().isEmpty()) {
        launch_log += format("- Command-line Arguments: %s\n", _launcher_arguments->text().toStdString().c_str());
    }

    const std::string actual_log_file = ReplaceBuiltInVariable(_launcher_log_file_edit->text().toStdString());
    if (!actual_log_file.empty()) {
        launch_log += format("- Log file: %s\n", actual_log_file.c_str());
    }

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

    if (ui->check_box_clear_on_launch->isChecked()) {
        ui->log_browser->clear();
    }

    Log(launch_log.c_str());

    // Launch the test application
    _launch_application.reset(new QProcess(this));
    connect(_launch_application.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));
    connect(_launch_application.get(), SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));
    connect(_launch_application.get(), SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(processClosed(int, QProcess::ExitStatus)));

    _launch_application->setProgram(ReplaceBuiltInVariable(_launcher_executable->text().toStdString()).c_str());
    _launch_application->setWorkingDirectory(ReplaceBuiltInVariable(_launcher_working->text().toStdString()).c_str());
    _launch_application->setEnvironment(BuildEnvVariables());

    if (!_launcher_arguments->text().isEmpty()) {
        const QStringList args = ConvertString(SplitSpace(_launcher_arguments->text().toStdString()));
        _launch_application->setArguments(args);
    }

    ui->push_button_launcher->setText("Terminate");
    _launch_application->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    _launch_application->setProcessChannelMode(QProcess::MergedChannels);
    _launch_application->closeWriteChannel();

    // Wait... did we start? Give it 4 seconds, more than enough time
    if (!_launch_application->waitForStarted(4000)) {
        _launch_application->deleteLater();
        _launch_application = nullptr;

        ui->push_button_launcher->setText("Launch");

        const std::string failed_log =
            std::string("Failed to launch ") + ReplaceBuiltInVariable(_launcher_executable->text().toStdString()).c_str() + "!\n";
        Log(failed_log);
    }
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
