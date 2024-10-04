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
#include "../vkconfig_core/ui.h"
#include "../vkconfig_core/version.h"
#include "../vkconfig_core/help.h"
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
#include <QFileDialog>
#include <QDesktopServices>
#include <QSettings>

#include <cassert>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _tray_icon(nullptr),
      _tray_icon_menu(nullptr),
      _tray_restore_action(nullptr),
      _tray_layers_controlled_by_applications(nullptr),
      _tray_layers_controlled_by_configurator(nullptr),
      _tray_layers_disabled_by_configurator(nullptr),
      _tray_quit_action(nullptr),
      ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->InitTray();

    this->tabs[TAB_DIAGNOSTIC].reset(new TabDiagnostics(*this, ui));
    this->tabs[TAB_APPLICATIONS].reset(new TabApplications(*this, ui));
    this->tabs[TAB_LAYERS].reset(new TabLayers(*this, ui));
    this->tabs[TAB_CONFIGURATIONS].reset(new TabConfigurations(*this, ui));
    this->tabs[TAB_PREFERENCES].reset(new TabPreferences(*this, ui));
    this->tabs[TAB_DOCUMENTATION].reset(new TabDocumentation(*this, ui));
    this->tabs[TAB_ABOUT].reset(new TabAbout(*this, ui));

    connect(qApp, &QGuiApplication::commitDataRequest, this, &MainWindow::commitDataRequest);

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

    QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
    this->restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    this->restoreState(settings.value("mainwindow/state").toByteArray());

    const Configurator &configurator = Configurator::Get();

    this->ui->tab_widget->setCurrentIndex(configurator.environment.active_tab);
    this->UpdateUI(UPDATE_REBUILD_UI);
}

MainWindow::~MainWindow() {}

void MainWindow::commitDataRequest(QSessionManager &manager) {
    (void)manager;

    Configurator &configurator = Configurator::Get();
    configurator.~Configurator();
}

void MainWindow::UpdateUI(UpdateUIMode mode) {
    if (this->tabs[this->ui->tab_widget->currentIndex()] != nullptr) {
        this->tabs[this->ui->tab_widget->currentIndex()]->UpdateUI(mode);
    }

    this->UpdateUI_Status();
}

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
        this->_tray_icon->show();
        this->connect(this->_tray_icon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    }
}

void MainWindow::UpdateUI_Status() {
    const Configurator &configurator = Configurator::Get();
    const bool has_active_configuration = configurator.HasActiveConfiguration();

    // Update title bar
#ifdef VKCONFIG_DATE
    bool display_date = true;
#else
    bool display_date = false;
#endif

    const ConfigurationInfo *configuration_info = configurator.configurations.GetActiveConfigurationInfo();
    const LayersMode mode = configuration_info == nullptr ? LAYERS_CONTROLLED_BY_APPLICATIONS : configuration_info->mode;

    this->setWindowTitle(GetMainWindowTitle(has_active_configuration, display_date).c_str());
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        switch (mode) {
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
    }

    if (has_active_configuration && mode != LAYERS_CONTROLLED_BY_APPLICATIONS) {
        const QIcon icon(":/resourcefiles/vkconfig-on.png");
        this->setWindowIcon(icon);
        if (QSystemTrayIcon::isSystemTrayAvailable()) {
            this->_tray_icon->setIcon(icon);
            this->_tray_icon->setToolTip("Layers controlled by the Vulkan Configurator");
        }
    } else {
        const QIcon icon(":/resourcefiles/vkconfig-off.png");
        this->setWindowIcon(icon);
        if (QSystemTrayIcon::isSystemTrayAvailable()) {
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

            this->UpdateUI(UPDATE_REBUILD_UI);
            break;
    }
}

void MainWindow::trayActionRestore() {
    this->hide();
    this->showNormal();
    this->UpdateUI(UPDATE_REBUILD_UI);
}

void MainWindow::trayActionControlledByApplications(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();
        configurator.configurations.GetActiveConfigurationInfo()->mode = LAYERS_CONTROLLED_BY_APPLICATIONS;
        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

        this->UpdateUI(UPDATE_REBUILD_UI);
    }
}

void MainWindow::trayActionControlledByConfigurator(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();
        configurator.configurations.GetActiveConfigurationInfo()->mode = LAYERS_CONTROLLED_BY_CONFIGURATOR;
        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

        this->UpdateUI(UPDATE_REBUILD_UI);
    }
}

void MainWindow::trayActionDisabledByApplications(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();
        configurator.configurations.GetActiveConfigurationInfo()->mode = LAYERS_DISABLED_BY_CONFIGURATOR;
        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

        this->UpdateUI(UPDATE_REBUILD_UI);
    }
}

void MainWindow::toolsResetToDefault(bool checked) {
    (void)checked;

    if (Alert::ConfiguratorResetAll() == QMessageBox::No) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    configurator.Reset(true);

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void MainWindow::StartTool(Tool tool) {
    Configurator &configurator = Configurator::Get();

    if (!configurator.configurations.GetPerExecutableConfig()) {
        configurator.Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
    }

    switch (tool) {
        case TOOL_VULKAN_INFO:
            vk_info_dialog.reset(new VulkanInfoDialog(this));
            break;
        case TOOL_VULKAN_INSTALL:
            vk_installation_dialog.reset(new VulkanAnalysisDialog(this));
            break;
    }

    if (!configurator.configurations.GetPerExecutableConfig()) {
        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
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
    Configurator &configurator = Configurator::Get();
    Environment &environment = configurator.environment;

    // Alert the user to the current state of the vulkan configurator and
    // give them the option to not shutdown.
    if (configurator.configurations.GetUseSystemTray()) {
        if (environment.hide_message_boxes_flags & GetBit(HIDE_MESSAGE_USE_SYSTEM_TRAY)) {
            std::string shut_down_state;

            if (configurator.configurations.GetActiveConfigurationInfo()->mode == LAYERS_CONTROLLED_BY_CONFIGURATOR) {
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

    this->tabs[this->ui->tab_widget->currentIndex()]->CleanUI();

    QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
    settings.setValue("mainwindow/geometry", this->saveGeometry());
    settings.setValue("mainwindow/state", this->saveState());

    QMainWindow::closeEvent(event);
}

void MainWindow::showEvent(QShowEvent *event) {
    (void)event;

    this->UpdateUI(UPDATE_REBUILD_UI);

    event->accept();
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    if (this->tabs[this->ui->tab_widget->currentIndex()] == nullptr) {
        return false;
    }

    return this->tabs[this->ui->tab_widget->currentIndex()]->EventFilter(target, event);
}

void MainWindow::on_tab_widget_currentChanged(int index) {
    assert(index >= 0);

    Configurator &configurator = Configurator::Get();
    configurator.environment.active_tab = static_cast<TabType>(index);

    this->tabs[index]->UpdateUI(UPDATE_REBUILD_UI);
}
