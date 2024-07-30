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

#include "dialog_vulkan_analysis.h"
#include "dialog_vulkan_info.h"

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/alert.h"
#include "../vkconfig_core/util.h"
#include "../vkconfig_core/version.h"
#include "../vkconfig_core/help.h"
#include "../vkconfig_core/doc.h"
#include "../vkconfig_core/date.h"
#include "../vkconfig_core/type_platform.h"
#include "../vkconfig_core/type_log.h"
#include "../vkconfig_core/type_hide_message.h"

#include <QMenu>
#include <QProcess>
#include <QMessageBox>
#include <QFrame>
#include <QComboBox>
#include <QVariant>
#include <QContextMenuEvent>
#include <QLineEdit>
#include <QDesktopServices>

#include <cassert>

#if VKC_PLATFORM == VKC_PLATFORM_LINUX || VKC_PLATFORM == VKC_PLATFORM_MACOS
#include <unistd.h>

#elif VKC_PLATFORM == VKC_PLATFORM_WINDOWS
#include <windows.h>

// From Stack Overflow.
#define MKPTR(p1, p2) ((DWORD_PTR)(p1) + (DWORD_PTR)(p2))

typedef enum _pe_architecture {
    PE_ARCHITECTURE_UNKNOWN = 0x0000,
    PE_ARCHITECTURE_ANYCPU = 0x0001,
    PE_ARCHITECTURE_X86 = 0x010B,
    PE_ARCHITECTURE_x64 = 0x020B
} PE_ARCHITECTURE;

static PE_ARCHITECTURE GetImageArchitecture(void *pImageBase) {
    // Parse and validate the DOS header
    IMAGE_DOS_HEADER *pDosHd = (IMAGE_DOS_HEADER *)pImageBase;
    if (IsBadReadPtr(pDosHd, sizeof(pDosHd->e_magic)) || pDosHd->e_magic != IMAGE_DOS_SIGNATURE) return PE_ARCHITECTURE_UNKNOWN;

    // Parse and validate the NT header
    IMAGE_NT_HEADERS *pNtHd = (IMAGE_NT_HEADERS *)MKPTR(pDosHd, pDosHd->e_lfanew);
    if (IsBadReadPtr(pNtHd, sizeof(pNtHd->Signature)) || pNtHd->Signature != IMAGE_NT_SIGNATURE) return PE_ARCHITECTURE_UNKNOWN;

    // First, naive, check based on the 'Magic' number in the Optional Header.
    PE_ARCHITECTURE architecture = (PE_ARCHITECTURE)pNtHd->OptionalHeader.Magic;

    return architecture;
}

// End Stack Overflow
#endif  // VKC_PLATFORM

/// Utility function to see if the file is 32-bit
static bool IsDLL32Bit(const std::string full_path) {
#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
    if (full_path.empty()) {
        return false;
    }

    QFile file(full_path.c_str());
    if (!file.open(QIODevice::ReadOnly)) {
        return false;  // punt...
    }

    // Not gonna lie, just guessed 1024 and it was enough.
    // This is the minimum page size on any OS (I might be wrong,
    // it could be 4096), so going lower is not really helpful.
    // Changed to 4096 on a whim; looking for crashing bug I can't debug
    void *header = file.map(0, 4096, QFileDevice::MapPrivateOption);

    // Another punt as we may not be able to map the file
    if (header == nullptr) {
        return false;
    }

    PE_ARCHITECTURE arch = GetImageArchitecture(header);

    file.unmap((uchar *)header);
    file.close();

    return (PE_ARCHITECTURE_X86 == arch);
#else
    (void)full_path;
    return false;
#endif
}

static const int LAUNCH_COLUMN0_SIZE = 220;
static const int LAUNCH_COLUMN2_SIZE = 32;
static const int LAUNCH_SPACING_SIZE = 2;
#if VKC_PLATFORM == VKC_PLATFORM_MACOS
static const int LAUNCH_ROW_HEIGHT = 26;
#else
static const int LAUNCH_ROW_HEIGHT = 28;
#endif

/* TODO

Path PathManager::SelectPathImpl(QWidget* parent, PathType path, const Path& suggested_path) {
    assert(parent);
    assert(path >= PATH_FIRST && path <= PATH_LAST);
    assert(!suggested_path.Empty());

    switch (path) {
        case PATH_LAUNCHER_LOG_FILE: {
            const std::string selected_path =
                QFileDialog::getSaveFileName(parent, "Set Log File To...", suggested_path.AbsolutePath().c_str(), "Log text(*.txt)")
                    .toStdString();
            if (selected_path.empty())  // The user cancelled
                return Path("");

            SetPath(path, QFileInfo(selected_path.c_str()).absolutePath().toStdString());
            return GetFullPath(path, QFileInfo(selected_path.c_str()).baseName().toStdString());
        } break;
        case PATH_EXECUTABLE: {
            static const char* TABLE[] = {
                "Applications (*.exe)",     // PLATFORM_WINDOWS
                "Applications (*)",         // PLATFORM_LINUX
                "Applications (*.app, *)",  // PLATFORM_MACOS
                "N/A"                       // PLATFORM_ANDROID
            };
            static_assert(std::size(TABLE) == PLATFORM_COUNT,
                          "The tranlation table size doesn't match the enum number of elements");

            const std::string filter = TABLE[VKC_PLATFORM];
            const std::string selected_path = QFileDialog::getOpenFileName(parent, "Select a Vulkan Executable...",
                                                                           suggested_path.AbsolutePath().c_str(), filter.c_str())
                                                  .toStdString();
            if (selected_path.empty())  // The user cancelled
                return Path("");

            SetPath(path, QFileInfo(selected_path.c_str()).absolutePath().toStdString());
            return GetFullPath(path, QFileInfo(selected_path.c_str()).fileName().toStdString());
        }
        case PATH_WORKING_DIR: {
            const std::string selected_path =
                QFileDialog::getExistingDirectory(parent, "Set Working Folder To...", suggested_path.AbsolutePath().c_str())
                    .toStdString();
            if (selected_path.empty())  // The user cancelled
                return Path("");

            SetPath(path, selected_path);
            return Path(GetPath(path));
        }
        case PATH_USER_DEFINED_LAYERS_GUI: {
            const std::string selected_path =
                QFileDialog::getExistingDirectory(parent, "Add User-Defined Layers Folder...",
                                                  suggested_path.AbsolutePath().c_str(), QFileDialog::DontUseNativeDialog)
                    .toStdString();
            if (selected_path.empty())  // The user cancelled
                return Path("");

            SetPath(path, selected_path);
            return Path(GetPath(path));
        }
        case PATH_IMPORT_CONFIGURATION: {
            const std::string selected_path =
                QFileDialog::getOpenFileName(parent, "Import Layers Configuration File", suggested_path.AbsolutePath().c_str(),
                                             "JSON configuration(*.json)")
                    .toStdString();
            if (selected_path.empty())  // The user cancelled
                return Path("");

            SetPath(path, QFileInfo(selected_path.c_str()).absolutePath().toStdString());
            return Path(selected_path);
        }
        case PATH_EXPORT_CONFIGURATION: {
            const std::string selected_path =
                QFileDialog::getSaveFileName(parent, "Export Layers Configuration File", suggested_path.AbsolutePath().c_str(),
                                             "JSON configuration(*.json)")
                    .toStdString();
            if (selected_path.empty())  // The user cancelled
                return Path("");

            SetPath(path, QFileInfo(selected_path.c_str()).absolutePath().toStdString());
            return Path(selected_path);
        }
        default:
            assert(0);
            return Path("");
    }
}
*/

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
      _tray_layers_controlled_by_applications(nullptr),
      _tray_layers_controlled_by_configurator(nullptr),
      _tray_layers_disabled_by_configurator(nullptr),
      _tray_quit_action(nullptr),
      ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->launcher_tree->installEventFilter(this);
    ui->configurations_tree->installEventFilter(this);
    ui->layers_tree->installEventFilter(this);
    ui->settings_tree->installEventFilter(this);
    //    ui->splitter_settings->setVisible(false);

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
    /*
        connect(ui->comboBox_configurations, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this,
                SLOT(OnConfigurationItemChanged(QTreeWidgetItem *, int)));
        connect(ui->comboBox_configurations, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
                SLOT(OnConfigurationTreeChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
        connect(ui->comboBox_configurations, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
                SLOT(OnConfigurationTreeClicked(QTreeWidgetItem *, int)));
    */
    connect(ui->configurations_tree, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationItemChanged(QTreeWidgetItem *, int)));
    connect(ui->configurations_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationItemDoubleClicked(QTreeWidgetItem *, int)));
    connect(ui->configurations_tree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(OnConfigurationTreeChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui->configurations_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationTreeClicked(QTreeWidgetItem *, int)));

    // connect(ui->layers_tree, SIGNAL(itemChanged(QListWidgetItem *, int)), this, SLOT(OnLayersItemChanged(QListWidgetItem *,
    // int))); connect(ui->layers_tree, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this,
    // SLOT(OnLayersTreeChanged(QListWidgetItem *, QListWidgetItem *))); connect(ui->layers_tree, SIGNAL(indexesMoved(int)), this,
    // SLOT(OnLayersIndexesMoved(const QModelIndexList &)));
    connect(ui->layers_tree, SIGNAL(currentRowChanged(int)), this, SLOT(OnLayerCurrentRowChanged(int)));

    connect(ui->settings_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(editorExpanded(QTreeWidgetItem *)));
    connect(ui->settings_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnSettingsTreeClicked(QTreeWidgetItem *, int)));

    connect(ui->launcher_tree, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(launchItemCollapsed(QTreeWidgetItem *)));
    connect(ui->launcher_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(launchItemExpanded(QTreeWidgetItem *)));

    connect(ui->launcher_loader_debug, SIGNAL(currentIndexChanged(int)), this, SLOT(OnLauncherLoaderMessageChanged(int)));

    connect(_launcher_apps_browse_button, SIGNAL(clicked()), this, SLOT(on_push_button_applications_clicked()));

    this->InitTray();

    Configurator &configurator = Configurator::Get();
    Environment &environment = configurator.environment;

    ui->combo_box_layers_view->setCurrentIndex(configurator.GetActiveConfiguration()->view_advanced_layers);

    ui->combo_box_mode->setCurrentIndex(environment.GetActiveConfigurationInfo().GetMode());

    // Update configuration application area

    if (environment.GetApplications().empty()) {
        environment.SetPerApplicationConfig(false);

        ui->check_box_per_application->setEnabled(false);
        ui->check_box_per_application->setVisible(false);
        ui->combo_box_applications->setVisible(false);
    } else {
        ui->check_box_per_application->setEnabled(true);

        ui->combo_box_applications->blockSignals(true);
        ui->combo_box_applications->clear();
        for (std::size_t i = 0, n = environment.GetApplications().size(); i < n; ++i) {
            const Application &application = environment.GetApplications()[i];

            ui->combo_box_applications->addItem(application.executable_path.RelativePath().c_str());
        }
        ui->combo_box_applications->blockSignals(false);

        this->on_check_box_per_application_toggled(environment.GetPerApplicationConfig());
        this->on_combo_box_applications_currentIndexChanged(environment.GetActiveApplicationIndex());
        ui->combo_box_applications->setCurrentIndex(environment.GetActiveApplicationIndex());
    }

    // ui->edit_executable->setText(application.executable_path.c_str());
    /*
        ui->edit_dir->setText(application.working_folder.c_str());
        ui->edit_arguments->setText(application.arguments.c_str());
        ui->edit_env->setText(application.env.c_str());
        ui->edit_log->setText(ReplaceBuiltInVariable(application.log_file.c_str()).c_str());
    */
    ui->execute_closer_application_label->setVisible(true);
    ui->execute_closer_driver_label->setVisible(true);

    // ui->check_box_persistent->setToolTip("Keep Vulkan Configurator running in system tray when closing the main window");
    // ui->check_box_persistent->setVisible(QSystemTrayIcon::isSystemTrayAvailable());

    LoadConfigurationList();

    // Resetting this from the default prevents the log window (a QTextEdit) from overflowing.
    // Whenever the control surpasses this block count, old blocks are discarded.
    // Note: We could make this a user configurable setting down the road should this be
    // insufficinet.
    ui->log_browser->document()->setMaximumBlockCount(2048);
    // ui->configuration_tree->scrollToItem(ui->configuration_tree->topLevelItem(0), QAbstractItemView::PositionAtTop);

    // Restore window geometry from last launch
    // restoreGeometry(environment.Get(VKCONFIG3_LAYOUT_MAIN_GEOMETRY));
    // restoreState(environment.Get(VKCONFIG3_LAYOUT_MAIN_WINDOW_STATE));
    // ui->splitter_main->restoreState(environment.Get(VKCONFIG3_LAYOUT_MAIN_SPLITTER1));
    // ui->splitter_configurations->restoreState(environment.Get(VKCONFIG3_LAYOUT_MAIN_SPLITTER2));
    // ui->splitter_settings->restoreState(environment.Get(VKCONFIG3_LAYOUT_MAIN_SPLITTER3));

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
        connect(this->_tray_layers_controlled_by_applications, &QAction::toggled, this,
                &MainWindow::trayActionControlledByApplications);

        this->_tray_layers_controlled_by_configurator = new QAction("Layers Controlled by the Vulkan Configurator", this);
        this->_tray_layers_controlled_by_configurator->setCheckable(true);
        connect(this->_tray_layers_controlled_by_configurator, &QAction::toggled, this,
                &MainWindow::trayActionControlledByConfigurator);

        this->_tray_layers_disabled_by_configurator = new QAction("Layers Disabled by the Vulkan Configurator", this);
        this->_tray_layers_disabled_by_configurator->setCheckable(true);
        connect(this->_tray_layers_disabled_by_configurator, &QAction::toggled, this,
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
        // QApplication::setQuitOnLastWindowClosed(!ui->check_box_persistent->isChecked());

        const Environment &environment = Configurator::Get().environment;

        switch (environment.GetActiveConfigurationInfo().GetMode()) {
            default:
            case LAYERS_CONTROLLED_BY_APPLICATIONS:
                this->_tray_layers_controlled_by_applications->setChecked(true);
                this->_tray_layers_controlled_by_configurator->setChecked(false);
                this->_tray_layers_disabled_by_configurator->setChecked(false);
                break;
            case LAYERS_CONTROLLED_BY_CONFIGURATOR:
                this->_tray_layers_controlled_by_applications->setChecked(false);
                this->_tray_layers_controlled_by_configurator->setChecked(true);
                this->_tray_layers_disabled_by_configurator->setChecked(false);
                break;
            case LAYERS_DISABLED_BY_CONFIGURATOR:
                this->_tray_layers_controlled_by_applications->setChecked(false);
                this->_tray_layers_controlled_by_configurator->setChecked(false);
                this->_tray_layers_disabled_by_configurator->setChecked(true);
                break;
        }

        if (environment.GetActiveConfigurationInfo().GetMode() != LAYERS_CONTROLLED_BY_APPLICATIONS) {
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

void MainWindow::trayActionControlledByApplications(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();

        if (configurator.environment.GetPerApplicationConfig()) {
            configurator.environment.GetActiveApplication().configuration.SetMode(LAYERS_CONTROLLED_BY_APPLICATIONS);
        } else {
            configurator.environment.global_configuration.SetMode(LAYERS_CONTROLLED_BY_APPLICATIONS);
        }
        configurator.Override();

        this->UpdateUI();
        this->UpdateTray();
    }
}

void MainWindow::trayActionControlledByConfigurator(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();

        if (configurator.environment.GetPerApplicationConfig()) {
            configurator.environment.GetActiveApplication().configuration.SetMode(LAYERS_CONTROLLED_BY_CONFIGURATOR);
        } else {
            configurator.environment.global_configuration.SetMode(LAYERS_CONTROLLED_BY_CONFIGURATOR);
        }

        this->UpdateUI();
        this->UpdateTray();
    }
}

void MainWindow::trayActionDisabledByApplications(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();

        if (configurator.environment.GetPerApplicationConfig()) {
            configurator.environment.GetActiveApplication().configuration.SetMode(LAYERS_DISABLED_BY_CONFIGURATOR);
        } else {
            configurator.environment.global_configuration.SetMode(LAYERS_DISABLED_BY_CONFIGURATOR);
        }

        configurator.Override();

        this->UpdateUI();
        this->UpdateTray();
    }
}

static std::string GetMainWindowTitle(bool active) {
#if VKCONFIG_DATE
    std::string title = format("%s %s-%s", VKCONFIG_NAME, Version::VKCONFIG.str().c_str(), GetBuildDate().c_str());
#else
    std::string title = format("%s %s", VKCONFIG_NAME, Version::VKCONFIG.str().c_str());
#endif
    if (active) {
        title += " <ACTIVE>";
    }
    return title;
}

void MainWindow::AddLayerPathItem(const std::string &layer_path) {
    TreeWidgetItemParameter *item_state = new TreeWidgetItemParameter(layer_path.c_str());

    item_state->setFlags(item_state->flags() | Qt::ItemIsSelectable);
    LayerPathWidget *layer_path_widget = new LayerPathWidget(layer_path, ui->tree_layers_paths, item_state);

    item_state->widget = layer_path_widget;

    // Add the top level item
    ui->tree_layers_paths->addItem(item_state);
    ui->tree_layers_paths->setItemWidget(item_state, layer_path_widget);
}

void MainWindow::AddLayerItem(Parameter &parameter, bool advanced_view) {
    assert(!parameter.key.empty());

    Configurator &configurator = Configurator::Get();

    const Layer *layer = configurator.layers.Find(parameter.key);

    TreeWidgetItemParameter *item_state = new TreeWidgetItemParameter(parameter.key.c_str());
    item_state->setFlags(item_state->flags() | Qt::ItemIsSelectable);
    ui->layers_tree->addItem(item_state);

    std::vector<const Layer *> layers;
    if (layer != nullptr) {
        layers.push_back(layer);
    }

    ConfigurationLayerWidget *layer_widget = new ConfigurationLayerWidget(layers, parameter, advanced_view);
    item_state->widget = layer_widget;

    if (parameter.control == LAYER_CONTROL_APPLICATIONS_API) {
        item_state->widget->setToolTip("Located and Enabled Layers using the Vulkan API by the Vulkan Application");
    } else if (parameter.control == LAYER_CONTROL_APPLICATIONS_ENV) {
        item_state->widget->setToolTip("Located and Enabled Layers using Environment Variables by the Vulkan Application");
    }

    ui->layers_tree->setItemWidget(item_state, layer_widget);
    if (configurator.environment.selected_layer_name == parameter.key) {
        ui->layers_tree->setCurrentItem(item_state);
    }
}

void MainWindow::UpdateUI() {
    static int check_recurse = 0;
    ++check_recurse;
    // assert(check_recurse <= 1);

    Configurator &configurator = Configurator::Get();
    const Environment &environment = Configurator::Get().environment;
    const std::string &selected_contiguration_name = environment.GetActiveConfigurationInfo().GetName();
    const bool has_selected_configuration = !selected_contiguration_name.empty();

    this->blockSignals(true);
    ui->configurations_tree->blockSignals(true);

    // Add applications
    ui->combo_box_applications->blockSignals(true);
    ui->combo_box_applications->clear();
    ui->combo_box_applications->setEnabled(ui->check_box_per_application->isChecked());
    const std::vector<Application> &applications = environment.GetApplications();
    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        ui->combo_box_applications->addItem(applications[i].executable_path.RelativePath().c_str());
    }
    ui->combo_box_applications->setCurrentIndex(environment.GetActiveApplicationIndex());
    ui->combo_box_applications->blockSignals(false);

    const bool has_active_configuration = configurator.HasActiveConfiguration();

    // Mode states
    this->UpdateTray();
    // ui->tree_layers_paths->blockSignals(true);

    // Update configurations
    // ui->layout_layers_buttons->setEnabled(ui->radio_vulkan_configurator->isChecked() && !environment.mode_disable_layers);
    /*
        ui->configuration_tree->setEnabled(enable_layer_ui);
        ui->configuration_tree->clear();
        for (std::size_t i = 0, n = configurator.configurations.available_configurations.size(); i < n; ++i) {
            Configuration *configuration = &configurator.configurations.available_configurations[i];

            ui->comboBox_configurations->addItem(configuration->key.c_str());

            if (configuration->key == selected_contiguration_name) {
                ui->comboBox_configurations->setCurrentIndex(static_cast<int>(i));
            }
        }
    */
    /*
        ui->tree_configurations->setEnabled(ui->radio_vulkan_configurator->isChecked() && !environment.mode_disable_layers);
        ui->tree_configurations->setCurrentItem(nullptr);
        ui->tree_configurations->setSelectionMode(ui->radio_vulkan_configurator->isChecked() ? QAbstractItemView::SingleSelection
                                                                                             : QAbstractItemView::NoSelection);

        for (int i = 0, n = ui->tree_configurations->topLevelItemCount(); i < n; ++i) {
            ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->tree_configurations->topLevelItem(i));

            assert(item);
            assert(!item->configuration_name.empty());

            Configuration *configuration =
                FindByKey(configurator.configurations.available_configurations, item->configuration_name.c_str());
            if (configuration == nullptr) continue;

            item->setText(1, item->configuration_name.c_str());
            item->setToolTip(1, configuration->description.c_str());
            item->radio_button->setToolTip(configuration->description.c_str());

            if (item->configuration_name == active_contiguration_name) {
                ui->tree_configurations->setCurrentItem(item);
                item->radio_button->setChecked(true);
            } else {
                item->radio_button->setChecked(false);
            }
        }
    */

    // Update configurations
    ui->configurations_tree->setCurrentItem(nullptr);

    for (int i = 0, n = ui->configurations_tree->topLevelItemCount(); i < n; ++i) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->configurations_tree->topLevelItem(i));
        assert(item);
        assert(!item->configuration_name.empty());

        Configuration *configuration = configurator.configurations.FindConfiguration(item->configuration_name);
        if (configuration == nullptr) {
            continue;
        }

        item->radio_button->setToolTip(configuration->description.c_str());
        item->radio_button->blockSignals(true);

        if (item->configuration_name == selected_contiguration_name) {
            item->radio_button->setChecked(true);
            ui->configurations_tree->setCurrentItem(item);
        } else {
            // item->radio_button->setChecked(false);
        }

        item->radio_button->blockSignals(false);
    }

    // ui->tree_layers_paths->setEnabled(enable_layer_ui);
    ui->tree_layers_paths->clear();
    /*
        std::vector<Path> layer_paths = configurator.layers.BuildPathList();
        for (std::size_t path_index = 0, count = layer_paths.size(); path_index < count; ++path_index) {
            const std::string user_defined_path(layer_paths[path_index].RelativePath().c_str());

            AddLayerPathItem(user_defined_path);
        }
    */
    ui->tree_layers_paths->update();

    // Load Layers items
    // ui->layers_tree->setEnabled(enable_layer_ui);
    ui->layers_tree->clear();

    if (has_selected_configuration) {
        Configuration *configuration = configurator.configurations.FindConfiguration(selected_contiguration_name);
        if (configuration != nullptr) {
            for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
                Parameter &parameter = configuration->parameters[i];

                if (!configuration->view_advanced_layers) {
                    if (parameter.control == LAYER_CONTROL_AUTO) {
                        continue;
                    }
                }

                this->AddLayerItem(parameter, configuration->view_advanced_layers);
            }
            resizeEvent(nullptr);

            ui->combo_box_layers_view->setCurrentIndex(configuration->view_advanced_layers);

            ui->layers_tree->update();
        }
    }

    // Handle application lists states
    /*
        ui->check_box_apply_list->setEnabled(!been_warned_about_old_loader &&
                                             ui->combo_box_mode->currentIndex() != LAYERS_MODE_BY_APPLICATIONS);
        ui->check_box_apply_list->setChecked(!been_warned_about_old_loader && environment.GetUseApplicationList());
        ui->push_button_applications->setEnabled(!been_warned_about_old_loader &&
                                                 ui->combo_box_mode->currentIndex() != LAYERS_MODE_BY_APPLICATIONS);
    */
    _launcher_apps_combo->blockSignals(true);
    _launcher_apps_combo->clear();
    /*
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
    */
    _launcher_apps_combo->blockSignals(false);

    // Handle persistent states
    /*
    ui->tree_layers_paths->setEnabled(!environment.mode_disable_layers);
    ui->tree_layers_list->setEnabled(!environment.mode_disable_layers);
    ui->push_button_new->setEnabled(!environment.mode_disable_layers);
    ui->push_button_edit->setEnabled(!environment.mode_disable_layers);
    ui->push_button_duplicate->setEnabled(!environment.mode_disable_layers);
    ui->push_button_remove->setEnabled(!environment.mode_disable_layers);
    */

    // Launcher states
    const bool has_application_list = !environment.GetApplications().empty();
    // ui->push_button_launcher->setEnabled(ui->check_box_apply_list->isChecked());
    ui->push_button_launcher->setText(_launch_application ? "Terminate" : "Launch");
    ui->check_box_clear_on_launch->setChecked(environment.Get(LAYOUT_LAUNCHER_NOT_CLEAR) != "true");
    // ui->launcher_loader_debug->setCurrentIndex(environment.GetLoaderMessage());

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

    // ui->settings_tree->setEnabled(environment.GetMode() == LAYERS_MODE_CONTROLLED_BY_CONFIGURATOR && has_selected_configuration);
    if (has_selected_configuration) {
        this->_settings_tree_manager.CreateGUI(ui->settings_tree);
    } else {
        this->_settings_tree_manager.CleanupGUI();
    }

    // Update title bar
    this->setWindowTitle(GetMainWindowTitle(has_active_configuration).c_str());

    ui->configurations_tree->blockSignals(false);
    this->blockSignals(false);

    --check_recurse;
}

// Load or refresh the list of configuration. Any configuration that uses a layer that
// is not detected on the system is disabled.
void MainWindow::LoadConfigurationList() {
    // There are lots of ways into this, and in none of them
    // can we have an active editor running.
    _settings_tree_manager.CleanupGUI();
    ui->configurations_tree->blockSignals(true);  // No signals firing off while we do this
    ui->configurations_tree->clear();
    ui->configurations_tree->header()->setSectionResizeMode(1, QHeaderView::Stretch);

    Configurator &configurator = Configurator::Get();

    for (std::size_t i = 0, n = configurator.configurations.available_configurations.size(); i < n; ++i) {
        const Configuration &configuration = configurator.configurations.available_configurations[i];

        ConfigurationListItem *item = new ConfigurationListItem(configuration.key);
        item->setText(1, item->configuration_name.c_str());
        item->setToolTip(1, configuration.description.c_str());
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->radio_button = new QRadioButton();
        item->radio_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        item->radio_button->setFixedSize(QSize(24, 24));
        item->radio_button->setToolTip(configuration.description.c_str());
        ui->configurations_tree->addTopLevelItem(item);
        ui->configurations_tree->setItemWidget(item, 0, item->radio_button);
        connect(item->radio_button, SIGNAL(toggled(bool)), this, SLOT(OnConfigurationItemClicked(bool)));
    }

    ui->configurations_tree->blockSignals(false);
    ui->configurations_tree->setColumnWidth(0, 24);
    ui->configurations_tree->setColumnWidth(2, 24);
    ui->configurations_tree->setColumnWidth(3, 24);
    ui->configurations_tree->setColumnWidth(4, 24);
}

/*
// Load or refresh the list of configuration. Any configuration that uses a layer that
// is not detected on the system is disabled.
void MainWindow::LoadConfigurationList() {
    // There are lots of ways into this, and in none of them
    // can we have an active editor running.
    _settings_tree_manager.CleanupGUI();
    // ui->tree_configurations->blockSignals(true);  // No signals firing off while we do this
    // ui->tree_configurations->clear();

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
        // ui->tree_configurations->addTopLevelItem(item);
        // ui->tree_configurations->setItemWidget(item, 0, item->radio_button);
        connect(item->radio_button, SIGNAL(clicked(bool)), this, SLOT(OnConfigurationItemClicked(bool)));
    }

    // ui->tree_configurations->blockSignals(false);
    // ui->tree_configurations->resizeColumnToContents(0);
    // ui->tree_configurations->resizeColumnToContents(1);
}
*/
/// Okay, because we are using custom controls, some of
/// the signaling is not happening as expected. So, we cannot
/// always get an accurate answer to the currently selected
/// item, but we do often need to know what has been checked
/// when an event occurs. This unambigously answers that question.
ConfigurationListItem *MainWindow::GetCheckedItem() {
    // Just go through all the top level items
    for (int i = 0, n = ui->configurations_tree->topLevelItemCount(); i < n; ++i) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->configurations_tree->topLevelItem(i));

        if (item == nullptr) {
            continue;
        }
        if (item->radio_button->isChecked()) {
            return item;
        }
    }

    return nullptr;
}

/*
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
*/

void MainWindow::on_combo_box_mode_currentIndexChanged(int index) {
    Configurator &configurator = Configurator::Get();

    configurator.environment.GetActiveConfigurationInfo().SetMode(static_cast<LayersMode>(index));
    configurator.Override();

    const bool enabled_ui = index == LAYERS_CONTROLLED_BY_CONFIGURATOR;

    ui->group_box_configurations->setEnabled(enabled_ui);
    ui->group_box_settings->setEnabled(enabled_ui);
    ui->group_box_layers->setEnabled(enabled_ui);

    this->UpdateTray();
    this->UpdateUI();
}

void MainWindow::on_combo_box_applications_currentIndexChanged(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.environment.SelectActiveApplication(index);

    Application &application = configurator.environment.GetApplication(index);
    ui->combo_box_applications->setToolTip(application.executable_path.AbsolutePath().c_str());
    ui->combo_box_mode->setCurrentIndex(configurator.environment.GetActiveConfigurationInfo().GetMode());

    this->UpdateTray();
    this->UpdateUI();
}

void MainWindow::on_check_box_per_application_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();
    configurator.environment.SetPerApplicationConfig(checked);

    ui->combo_box_applications->setEnabled(configurator.environment.GetPerApplicationConfig());
    ui->combo_box_mode->setCurrentIndex(configurator.environment.GetActiveConfigurationInfo().GetMode());

    this->UpdateTray();
    this->UpdateUI();
}

void MainWindow::on_check_box_clear_on_launch_clicked() {
    Configurator::Get().environment.Set(LAYOUT_LAUNCHER_NOT_CLEAR, ui->check_box_clear_on_launch->isChecked() ? "false" : "true");
}

void MainWindow::on_combo_box_layers_view_currentIndexChanged(int index) {
    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    configuration->view_advanced_layers = index != 0;

    this->UpdateUI();
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
    ConfigurationListItem *configuration_item = GetCheckedItem();
    if (configuration_item == nullptr) {
        return;
    }

    this->_settings_tree_manager.CleanupGUI();

    Configurator &configurator = Configurator::Get();
    configurator.environment.GetActiveConfigurationInfo().SetName(configuration_item->configuration_name);
    configurator.environment.selected_layer_name.clear();
    configurator.Override();

    UpdateUI();
}

void MainWindow::OnConfigurationTreeClicked(QTreeWidgetItem *item, int column) {
    (void)column;

    Configurator &configurator = Configurator::Get();

    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(item);
    if (configuration_item != nullptr) {
        this->_settings_tree_manager.CleanupGUI();

        configurator.environment.GetActiveConfigurationInfo().SetName(configuration_item->configuration_name);
        configurator.environment.selected_layer_name.clear();
        configurator.Override();
    }

    UpdateUI();
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
        } else if (!IsPortableFilename(new_name)) {
            valid_new_name = false;
            Alert::ConfigurationNameInvalid();
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
            configurator.configurations.SaveAllConfigurations();
            configurator.environment.GetActiveConfigurationInfo().SetName(new_name);
            configurator.environment.selected_layer_name.clear();

            LoadConfigurationList();
        } else {
            // If the configurate name is empty or the configuration name is taken, keep old configuration name

            ui->configurations_tree->blockSignals(true);
            item->setText(1, old_name.c_str());
            ui->configurations_tree->blockSignals(false);

            configurator.environment.GetActiveConfigurationInfo().SetName(old_name);
            configurator.environment.selected_layer_name.clear();
        }

        configurator.Override();

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
    if (configurator.environment.GetActiveConfigurationInfo().GetName() != configuration_item->configuration_name) {
        this->_settings_tree_manager.CleanupGUI();

        configurator.environment.GetActiveConfigurationInfo().SetName(configuration_item->configuration_name);

        configurator.Override();
        this->UpdateUI();
    }
}

void MainWindow::StartTool(Tool tool) {
    Configurator &configurator = Configurator::Get();

    configurator.Surrender();

    switch (tool) {
        case TOOL_VULKAN_INFO:
            vk_info_dialog.reset(new VulkanInfoDialog(this));
            break;
        case TOOL_VULKAN_INSTALL:
            vk_installation_dialog.reset(new VulkanAnalysisDialog(this));
            break;
    }

    configurator.Override();
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
    if (environment.GetUseSystemTray()) {
        if (environment.hide_message_boxes_flags & GetBit(HIDE_MESSAGE_USE_SYSTEM_TRAY)) {
            std::string shut_down_state;

            if (environment.GetActiveConfigurationInfo().GetMode() == LAYERS_CONTROLLED_BY_CONFIGURATOR) {
                shut_down_state =
                    "Vulkan Layers override will remain in effect while Vulkan Configurator remain active in the system tray.";
            } else {
                shut_down_state =
                    "No Vulkan layers override will be active when Vulkan Configurator remain active in the system tray.";
            }

            QMessageBox alert(this);
            alert.setWindowTitle("Vulkan Configurator will remain in the system tray");
            alert.setText(shut_down_state.c_str());
            alert.setIcon(QMessageBox::Question);
            alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            alert.setCheckBox(new QCheckBox("Do not show again."));
            alert.setInformativeText("Are you still ready to move Vulkan Configurator in the system tray?");

            int ret_val = alert.exec();
            if (alert.checkBox()->isChecked()) {
                environment.hide_message_boxes_flags |= GetBit(HIDE_MESSAGE_USE_SYSTEM_TRAY);
            }

            if (ret_val == QMessageBox::No) {
                event->ignore();
                return;
            }
        }
    }

    // If a child process is still running, destroy it
    if (_launch_application) {
        ResetLaunchApplication();
    }

    _settings_tree_manager.CleanupGUI();

    environment.Set(VKCONFIG3_LAYOUT_MAIN_GEOMETRY, saveGeometry());
    environment.Set(VKCONFIG3_LAYOUT_MAIN_WINDOW_STATE, saveState());
    environment.Set(VKCONFIG3_LAYOUT_MAIN_SPLITTER1, ui->splitter_main->saveState());
    environment.Set(VKCONFIG3_LAYOUT_MAIN_SPLITTER2, ui->splitter_configurations->saveState());
    environment.Set(VKCONFIG3_LAYOUT_MAIN_SPLITTER3, ui->splitter_settings->saveState());

    QMainWindow::closeEvent(event);
}

/// Resizing needs a little help. Yes please, there has to be
/// a better way of doing this.
void MainWindow::resizeEvent(QResizeEvent *event) {
    if (event != nullptr) event->accept();

    const QFontMetrics fm = ui->layers_tree->fontMetrics();
    const int combo_width = (fm.size(Qt::TextSingleLine, "Auto").width() * 1.6);
    const int width = ui->layers_tree->width() - combo_width;

    // ui->tree_layers_list->setColumnWidth(0, width);
}

void MainWindow::showEvent(QShowEvent *event) {
    (void)event;

    UpdateUI();

    event->accept();
}

/// Edit the list of apps that can be filtered.
void MainWindow::on_push_button_applications_clicked() {
    // ApplicationsDialog dlg(this);
    // dlg.exec();

    Configurator &configurator = Configurator::Get();
    configurator.Override();

    UpdateUI();
}

void MainWindow::on_push_button_new_clicked() { this->NewClicked(); }

void MainWindow::on_push_button_rename_clicked() {
    ConfigurationListItem *configuration_item = GetCheckedItem();
    if (configuration_item == nullptr) {
        return;
    }

    ui->configurations_tree->editItem(configuration_item, 1);
}

void MainWindow::on_push_button_remove_clicked() {
    ConfigurationListItem *configuration_item = GetCheckedItem();
    if (configuration_item == nullptr) {
        return;
    }

    this->RemoveConfiguration(configuration_item->configuration_name);
}

void MainWindow::on_push_button_duplicate_clicked() {
    Configurator &configurator = Configurator::Get();

    const Configuration *configutation = configurator.GetActiveConfiguration();
    assert(configutation != nullptr);

    const Configuration &duplicated_configuration =
        configurator.configurations.CreateConfiguration(configurator.layers.selected_layers, configutation->key, true);

    configurator.environment.GetActiveConfigurationInfo().SetName(duplicated_configuration.key);
    configurator.Override();

    LoadConfigurationList();

    this->UpdateUI();
}

void MainWindow::NewClicked() {
    Configurator &configurator = Configurator::Get();

    Configuration &new_configuration =
        configurator.configurations.CreateConfiguration(configurator.layers.selected_layers, "New Configuration");

    configurator.environment.GetActiveConfigurationInfo().SetName(new_configuration.key);
    configurator.Override();

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
    if (alert.exec() == QMessageBox::No) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    configurator.configurations.RemoveConfiguration(configurator.layers.selected_layers, configuration_name);
    configurator.environment.GetActiveConfigurationInfo().SetName("");

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
    if (alert.exec() == QMessageBox::No) return;

    configuration->Reset(configurator.layers.selected_layers);

    LoadConfigurationList();
}

void MainWindow::RenameClicked(ConfigurationListItem *item) {
    assert(item);

    ui->configurations_tree->editItem(item, 1);
}

void MainWindow::DuplicateClicked(ConfigurationListItem *item) {
    assert(item);
    assert(!item->configuration_name.empty());

    Configurator &configurator = Configurator::Get();
    const Configuration &duplicated_configuration =
        configurator.configurations.CreateConfiguration(configurator.layers.selected_layers, item->configuration_name, true);

    item->configuration_name = duplicated_configuration.key;

    configurator.environment.GetActiveConfigurationInfo().SetName(duplicated_configuration.key);
    configurator.Override();

    LoadConfigurationList();

    /*
        ConfigurationListItem *new_item = nullptr;
        for (int i = 0, n = ui->tree_configurations->topLevelItemCount(); i < n; ++i) {
            ConfigurationListItem *searched_item = dynamic_cast<ConfigurationListItem *>(ui->tree_configurations->topLevelItem(i));
            assert(searched_item);

            if (searched_item->configuration_name != duplicated_configuration.key) continue;

            new_item = searched_item;
            break;
        }
        assert(new_item);
        ui->tree_configurations->editItem(new_item, 1);
    */
}

void MainWindow::ImportClicked(ConfigurationListItem *item) {
    (void)item;  // We don't need this

    Configurator &configurator = Configurator::Get();

    const Path &path_import = configurator.environment.path_import;
    /*
    const Path full_import_path = configurator.path.SelectPath(this, PATH_IMPORT_CONFIGURATION);
    if (full_import_path.Empty()) {
        return;
    }

    configurator.configurations.ImportConfiguration(configurator.layers.selected_layers, full_import_path);
    */
    LoadConfigurationList();
}

void MainWindow::ExportClicked(ConfigurationListItem *item) {
    assert(item);

    Configurator &configurator = Configurator::Get();

    const Path &path_export = configurator.environment.path_export;
    /* TODO
    const Path full_export_path = configurator.path.SelectPath(this, PATH_EXPORT_CONFIGURATION, full_suggested_path);
    if (full_export_path.Empty()) {
        return;
    }

    configurator.configurations.ExportConfiguration(configurator.layers.selected_layers, full_export_path,
                                                    item->configuration_name);
    */
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

void MainWindow::OnLayerCurrentRowChanged(int currentRow) {
    this->_settings_tree_manager.CleanupGUI();

    if (currentRow != -1) {
        Configurator &configurator = Configurator::Get();
        QWidget *widget = ui->layers_tree->itemWidget(ui->layers_tree->item(currentRow));
        std::string layer_name = static_cast<LayerPathWidget *>(widget)->text().toStdString();
        Configuration *configuration = configurator.GetActiveConfiguration();
        if (configuration != nullptr) {
            for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
                Parameter &parameter = configuration->parameters[i];
                if (layer_name.find(parameter.key) != std::string::npos) {
                    configurator.environment.selected_layer_name = parameter.key;
                }
            }
        }
    }

    //    ui->splitter_settings->setVisible(currentRow != -1);
    this->_settings_tree_manager.CreateGUI(ui->settings_tree);
}

void MainWindow::OnSettingsTreeClicked(QTreeWidgetItem *item, int column) {
    (void)column;
    (void)item;

    Configurator &configurator = Configurator::Get();
    configurator.environment.GetActiveConfigurationInfo().ForceUpdate(UPDATE_LAYERS_SETTINGS_BIT);
    configurator.Override();
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

    // configurator.environment.SetLoaderMessage(static_cast<LoaderMessageLevel>(level));
    // configurator.environment.SetLoaderMessageFlags(GetBit(static_cast<LogBit>(level)));

    if (ui->check_box_clear_on_launch->isChecked()) {
        this->UpdateUI();
    }
}

void MainWindow::launchSetExecutable() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    Application &application = configurator.environment.GetApplication(current_application_index);
    /* TODO
    const Path exe = configurator.path.SelectPath(this, PATH_EXECUTABLE, application.executable_path);

    // The user has cancel the operation
    if (exe.Empty()) {
        return;
    }

    application.executable_path = exe;
    */
    _launcher_executable->setText(application.executable_path.RelativePath().c_str());
}

void MainWindow::launchSetLogFile() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    Application &application = configurator.environment.GetApplication(current_application_index);
    ApplicationOptions &options = application.GetActiveOptions();
    /* TODO
    const Path &path = configurator.path.SelectPath(this, PATH_LAUNCHER_LOG_FILE, options.log_file);

    // The user has cancel the operation
    if (path.Empty()) {
        return;
    }

    options.log_file = path;
    */
    _launcher_log_file_edit->setText(options.log_file.RelativePath().c_str());
}

void MainWindow::launchSetWorkingFolder() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    Application &application = configurator.environment.GetApplication(current_application_index);
    ApplicationOptions &options = application.GetActiveOptions();

    /* TODO
    const Path path = configurator.path.SelectPath(this, PATH_WORKING_DIR, options.working_folder);

    // The user has cancel the operation
    if (path.Empty()) {
        return;
    }

    options.working_folder = path;
    */
    _launcher_working->setText(options.working_folder.RelativePath().c_str());
}

// Log file path edited manually.
void MainWindow::launchChangeLogFile(const QString &log_file) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Application &application = Configurator::Get().environment.GetApplication(current_application_index);
    ApplicationOptions &options = application.GetActiveOptions();
    options.log_file = log_file.toStdString();
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
    ApplicationOptions &options = application.GetActiveOptions();
    options.working_folder = working_folder.toStdString();
}

// Launch app change
void MainWindow::launchItemChanged(int application_index) {
    if (application_index < 0) return;

    Environment &environment = Configurator::Get().environment;

    environment.SelectActiveApplication(application_index);

    Application &application = environment.GetApplication(application_index);
    ApplicationOptions &options = application.GetActiveOptions();

    _launcher_executable->setText(application.executable_path.RelativePath().c_str());
    _launcher_working->setText(options.working_folder.RelativePath().c_str());
    _launcher_arguments->setText(Merge(options.arguments, " ").c_str());
    _launcher_log_file_edit->setText(options.log_file.RelativePath().c_str());
}

/// New command line arguments. Update them.
void MainWindow::launchArgsEdited(const QString &arguments) {
    int application_index = _launcher_apps_combo->currentIndex();
    if (application_index < 0) return;

    Application &application = Configurator::Get().environment.GetApplication(application_index);
    ApplicationOptions &options = application.GetActiveOptions();
    options.arguments = SplitSpace(arguments.toStdString());
}

// Clear the browser window
void MainWindow::on_push_button_clear_log_clicked() {
    ui->log_browser->clear();
    ui->log_browser->update();
    ui->push_button_clear_log->setEnabled(false);
}

static const Layer *GetLayer(QListWidget *tree, QListWidgetItem *item) {
    if (item == nullptr) return nullptr;

    TreeWidgetItemParameter *item_parameter = static_cast<TreeWidgetItemParameter *>(item);

    const std::string &text = item_parameter->layer_name;
    if (!text.empty()) {
        Configurator &configurator = Configurator::Get();

        for (std::size_t i = 0, n = configurator.layers.selected_layers.size(); i < n; ++i) {
            const Layer &layer = configurator.layers.selected_layers[i];
            if (text.find(layer.key) != std::string::npos) {
                return &layer;
            }
        }
    }

    return nullptr;
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    QEvent::Type event_type = event->type();

    if (event_type == QEvent::Wheel) {
        return true;
    }

    if (event_type == QEvent::ChildRemoved) {
        // Layers were reordered, we need to update the configuration
        Configurator::Get().Override();
        return true;
    }

    // Launch tree does some fancy resizing and since it's down in
    // layouts and splitters, we can't just rely on the resize method
    // of this window. Any resize coming through needs to trigger this
    // or we get drawing artifacts on macOS.
    /*
        if (event->type() == QEvent::Resize) {
            const QRect rect = ui->launcher_tree->rect();
            ui->launcher_tree->setColumnWidth(0, LAUNCH_COLUMN0_SIZE);
            ui->launcher_tree->setColumnWidth(1, rect.width() - LAUNCH_COLUMN0_SIZE - LAUNCH_COLUMN2_SIZE - LAUNCH_SPACING_SIZE);
            ui->launcher_tree->setColumnWidth(2, LAUNCH_COLUMN2_SIZE);
            return false;
        }
    */

    if (target == ui->settings_tree) {
        QContextMenuEvent *right_click = dynamic_cast<QContextMenuEvent *>(event);
        if (right_click) {
            QTreeWidgetItem *item = ui->settings_tree->itemAt(right_click->pos());

            QMenu menu(ui->settings_tree);
            QFont subtitle_font = menu.font();
            subtitle_font.setBold(true);

            QAction *title_action = new QAction("settings context menu", nullptr);
            title_action->setFont(subtitle_font);
            menu.addAction(title_action);

            QPoint point(right_click->globalX(), right_click->globalY());
            QAction *action = menu.exec(point);

            if (action == title_action) {
            }
        }
    } else if (target == ui->configurations_tree) {
        QContextMenuEvent *right_click = dynamic_cast<QContextMenuEvent *>(event);
        if (right_click) {
            ConfigurationListItem *item = static_cast<ConfigurationListItem *>(ui->configurations_tree->itemAt(right_click->pos()));

            std::string name;

            if (item != nullptr) {
                name = item->configuration_name;
            }

            QMenu menu(ui->configurations_tree);
            QFont subtitle_font = menu.font();
            subtitle_font.setBold(true);

            QAction *action_new = new QAction("Create a new Configuration", nullptr);
            action_new->setEnabled(true);
            menu.addAction(action_new);

            QAction *action_import = new QAction("Import a new Configuration...", nullptr);
            action_import->setEnabled(true);
            menu.addAction(action_import);

            menu.addSeparator();

            QAction *action_rename = new QAction("Rename the Configuration", nullptr);
            action_rename->setEnabled(item != nullptr);
            menu.addAction(action_rename);

            QAction *action_duplicate = new QAction("Duplicate the Configuration", nullptr);
            action_duplicate->setEnabled(item != nullptr);
            menu.addAction(action_duplicate);

            QAction *action_delete = new QAction("Delete the Configuration", nullptr);
            action_delete->setEnabled(item != nullptr);
            menu.addAction(action_delete);

            menu.addSeparator();

            QAction *action_reset = new QAction("Reset the Configuration", nullptr);
            action_reset->setEnabled(item != nullptr);
            action_reset->setToolTip("Reset the Configuration using the built-in configuration");
            menu.addAction(action_reset);

            QAction *action_reload = new QAction("Reload the Configuration", nullptr);
            action_reload->setEnabled(item != nullptr);
            action_reset->setToolTip("Reload the Configuration, discarding all changes from this session");
            menu.addAction(action_reload);

            menu.addSeparator();

            QAction *action_export = new QAction("Export the Configuration...", nullptr);
            action_export->setEnabled(item != nullptr);
            menu.addAction(action_export);

            QAction *action_export_settings = new QAction("Export the Layers Settings...", nullptr);
            action_export_settings->setEnabled(item != nullptr);
            menu.addAction(action_export_settings);

            QPoint point(right_click->globalX(), right_click->globalY());
            QAction *action = menu.exec(point);

            if (action == action_new) {
            } else if (action == action_delete) {
            } else if (action == action_duplicate) {
            } else if (action == action_rename) {
            } else if (action == action_import) {
            } else if (action == action_export) {
            }
        }
    } else if (target == ui->layers_tree) {
        QContextMenuEvent *right_click = dynamic_cast<QContextMenuEvent *>(event);
        if (right_click) {
            QListWidgetItem *item = ui->layers_tree->itemAt(right_click->pos());

            const Layer *layer = GetLayer(ui->layers_tree, item);

            QMenu menu(ui->layers_tree);

            QAction *action_all = new QAction("Show All Layers", nullptr);
            action_all->setEnabled(item != nullptr);
            action_all->setCheckable(true);
            action_all->setChecked(true);
            menu.addAction(action_all);

            menu.addSeparator();

            QAction *action_description = new QAction("Open the Layer Description...", nullptr);
            action_description->setEnabled(item != nullptr);
            menu.addAction(action_description);

            QAction *export_html_action = new QAction("Open the Layer HTML Documentation...", nullptr);
            export_html_action->setEnabled(item != nullptr);
            menu.addAction(export_html_action);

            QAction *export_markdown_action = new QAction("Open the Layer Markdown Documentation...", nullptr);
            export_markdown_action->setEnabled(item != nullptr);
            menu.addAction(export_markdown_action);

            QAction *visit_layer_website_action = new QAction("Visit the Layer Website...", nullptr);
            visit_layer_website_action->setEnabled(layer != nullptr ? !layer->url.empty() : false);
            menu.addAction(visit_layer_website_action);

            QPoint point(right_click->globalX(), right_click->globalY());
            QAction *action = menu.exec(point);

            if (action == action_description) {
                Alert::LayerProperties(layer);
            } else if (action == visit_layer_website_action) {
                QDesktopServices::openUrl(QUrl(layer->url.c_str()));
            } else if (action == export_html_action) {
                const std::string path = format("%s/%s.html", AbsolutePath(Path::APPDATA).c_str(), layer->key.c_str());
                ExportHtmlDoc(*layer, path);
                QDesktopServices::openUrl(QUrl(("file:///" + path).c_str()));
            } else if (action == export_markdown_action) {
                const std::string path = format("%s/%s.md", AbsolutePath(Path::APPDATA).c_str(), layer->key.c_str());
                ExportMarkdownDoc(*layer, path);
                QDesktopServices::openUrl(QUrl(("file:///" + path).c_str()));
            }
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

QStringList MainWindow::BuildEnvVariables() const {
    Configurator &configurator = Configurator::Get();

    QStringList env = QProcess::systemEnvironment();
    env << (QString("VK_LOADER_DEBUG=") + ::GetLogString(configurator.environment.GetLoaderMessageFlags()).c_str());
    return env;
}

void MainWindow::on_tab_widget_currentChanged(int index) {
    switch (index) {
        case TAB_DIAGNOSTIC: {
        } break;
        case TAB_APPLICATIONS: {
        } break;
        case TAB_LAYERS: {
        } break;
        case TAB_CONFIGURATIONS: {
        } break;
        case TAB_PREFERENCES: {
        } break;
        case TAB_HELP: {
        } break;
    }
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

    assert(!active_application.executable_path.Empty());
    launch_log += format("- Executable: %s\n", active_application.executable_path.AbsolutePath().c_str());
    if (!active_application.executable_path.Exists()) {
        Alert::PathInvalid(
            active_application.executable_path,
            format("The '%s' application will fail to launch.", active_application.executable_path.AbsolutePath().c_str()).c_str());
    }

    const ApplicationOptions &options = active_application.GetActiveOptions();

    launch_log += format("- Working Directory: %s\n", options.working_folder.AbsolutePath().c_str());
    if (!options.working_folder.Exists()) {
        Alert::PathInvalid(
            options.working_folder,
            format("The '%s' application will fail to launch.", active_application.executable_path.AbsolutePath().c_str()).c_str());
    }

    if (!_launcher_arguments->text().isEmpty()) {
        launch_log += format("- Command-line Arguments: %s\n", _launcher_arguments->text().toStdString().c_str());
    }

    if (!options.log_file.Empty()) {
        launch_log += format("- Log file: %s\n", options.log_file.AbsolutePath().c_str());
    }

    if (!options.log_file.Empty()) {
        // Start logging
        // Make sure the log file is not already opened. This can occur if the
        // launched application is closed from the applicaiton.
        if (!_log_file.isOpen()) {
            _log_file.setFileName(options.log_file.AbsolutePath().c_str());

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

    _launch_application->setProgram(ui->edit_executable->text());
    _launch_application->setWorkingDirectory(ui->edit_dir->text());
    _launch_application->setEnvironment(BuildEnvVariables() + ui->edit_env->text().split(","));

    if (!options.arguments.empty()) {
        const QStringList args = ConvertString(options.arguments);
        _launch_application->setArguments(args);
    }

    _launch_application->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    _launch_application->setProcessChannelMode(QProcess::MergedChannels);
    _launch_application->closeWriteChannel();

    // Wait... did we start? Give it 4 seconds, more than enough time
    if (!_launch_application->waitForStarted(4000)) {
        _launch_application->deleteLater();
        _launch_application = nullptr;

        const std::string failed_log =
            std::string("Failed to launch ") + active_application.executable_path.AbsolutePath().c_str() + "!\n";
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
