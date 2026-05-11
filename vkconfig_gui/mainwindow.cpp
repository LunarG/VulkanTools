/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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
#include "style.h"

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/util.h"
#include "../vkconfig_core/ui.h"
#include "../vkconfig_core/version.h"
#include "../vkconfig_core/date.h"
#include "../vkconfig_core/type_platform.h"
#include "../vkconfig_core/type_log.h"
#include "../vkconfig_core/type_hide_message.h"

#include <QMenu>
#include <QMessageBox>
#include <QFrame>
#include <QComboBox>
#include <QVariant>
#include <QContextMenuEvent>
#include <QLineEdit>
#include <QFileDialog>
#include <QDesktopServices>
#include <QSettings>
#include <QWidgetAction>
#include <QShortcut>
#include <QSignalMapper>

#include <cassert>

MainWindow::MainWindow(QApplication &app, QWidget *parent)
    : QMainWindow(parent), _tray_icon(new QSystemTrayIcon(this)), app(app), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->tabs[TAB_DIAGNOSTIC].reset(new TabDiagnostics(*this, ui));
    this->tabs[TAB_APPLICATIONS].reset(new TabApplications(*this, ui));
    this->tabs[TAB_LAYERS_PATHS].reset(new TabLayers(*this, ui));
    this->tabs[TAB_CONFIGURATIONS].reset(new TabConfigurations(*this, ui));
    this->tabs[TAB_DOCUMENTATION].reset(new TabDocumentation(*this, ui));
    this->tabs[TAB_DRIVERS].reset(new TabDrivers(*this, ui));
    this->tabs[TAB_PREFERENCES].reset(new TabPreferences(*this, ui));
    this->tabs[TAB_ABOUT].reset(new TabAbout(*this, ui));

    this->connect(qApp, &QGuiApplication::commitDataRequest, this, &MainWindow::commitDataRequest);

    for (int i = TAB_FIRST, l = TAB_LAST; i <= l; ++i) {
        this->ui->tab_widget->setTabText(i, GetLabel(static_cast<TabType>(i)));
    }

    Configurator &configurator = Configurator::Get();

    if (!configurator.window_geometry.isEmpty()) {
        this->restoreGeometry(configurator.window_geometry);
        this->restoreState(configurator.window_state);
    }

    this->ui->tab_widget->setCurrentIndex(configurator.active_tab);
    this->ui->tab_widget->installEventFilter(this);

    this->connect(ui->tab_widget, SIGNAL(currentChanged(int)), this, SLOT(on_tab_widget_currentChanged(int)));

    this->UpdateUI(UPDATE_REBUILD_UI);

    configurator.Override(OVERRIDE_AREA_ALL);
}

MainWindow::~MainWindow() {}

void MainWindow::commitDataRequest(QSessionManager &manager) {
    (void)manager;

    Configurator &configurator = Configurator::Get();
    configurator.Surrender(OVERRIDE_AREA_ALL);

    Configurator::Release();
}

void MainWindow::UpdateUI(UpdateUIMode mode) {
    if (this->tabs[this->ui->tab_widget->currentIndex()] != nullptr) {
        this->tabs[this->ui->tab_widget->currentIndex()]->UpdateUI(mode);
    }

    this->UpdateUI_Status();
}

void MainWindow::UpdateUI_Status() {
    const Configurator &configurator = Configurator::Get();

    this->setWindowTitle(GetMainWindowTitle().c_str());

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        // Device
        {
            if (this->_tray_icon_menu != nullptr) {
                delete this->_tray_icon_menu;
            }
            this->_tray_icon_menu = new QMenu(this);

            QSignalMapper *mapper_device = new QSignalMapper(this);
            QSignalMapper *mapper_layers = new QSignalMapper(this);

            QAction *tray_restore_action = new QAction("&Show Vulkan Configurator UI", this);
            tray_restore_action->setIcon(QIcon(":/resourcefiles/vkconfig-on.png"));
            QFont font = tray_restore_action->font();
            font.setBold(true);
            tray_restore_action->setFont(font);
            this->connect(tray_restore_action, &QAction::triggered, this, &MainWindow::OnTrayActionShow);
            this->_tray_icon_menu->addAction(tray_restore_action);

            this->_tray_icon_menu->addSeparator();
            const bool enabled_device = configurator.driver_override_enabled;

            QAction *tray_override = new QAction("Override System Vulkan &Device with:", this);
            tray_override->setFont(font);
            tray_override->setCheckable(true);
            tray_override->setChecked(enabled_device);

            this->connect(tray_override, &QAction::toggled, this, &MainWindow::OnTrayActionOverrideDevice);
            this->_tray_icon_menu->addAction(tray_override);

            for (std::size_t i = 0, n = configurator.vulkan_system_info.physicalDevices.size(); i < n; ++i) {
                const VulkanPhysicalDeviceInfo &info = configurator.vulkan_system_info.physicalDevices[i];
                const DeviceInfo &device_info = ::GetDeviceInfo(info);

                QAction *action = new QAction(format("%s (%s)", info.GetLabel().c_str(), info.GetVersion().c_str()).c_str());
                action->setCheckable(true);
                action->setChecked(device_info == configurator.driver_override_info);
                action->setEnabled(enabled_device);

                this->_tray_icon_menu->addAction(action);

                this->connect(action, SIGNAL(triggered()), mapper_device, SLOT(map()));
                mapper_device->setMapping(action, i);
            }

            this->connect(mapper_device, &QSignalMapper::mappedInt, this, &MainWindow::OnDeviceChanged);

            this->_tray_icon_menu->addSeparator();
            const bool enabled_layers = configurator.layers_override_enabled && configurator.GetExecutableScope() != EXECUTABLE_PER;

            QAction *tray_override_layers = new QAction("Override System Vulkan &Layers Configuration with:", this);
            tray_override_layers->setFont(font);
            tray_override_layers->setCheckable(true);
            tray_override_layers->setChecked(enabled_layers);

            this->connect(tray_override_layers, &QAction::toggled, this, &MainWindow::OnTrayActionOverrideLayers);
            this->_tray_icon_menu->addAction(tray_override_layers);

            for (std::size_t i = 0, n = configurator.configurations.available_configurations.size(); i < n; ++i) {
                const Configuration &configuration = configurator.configurations.available_configurations[i];

                QAction *action = new QAction(configuration.key.c_str(), this);
                action->setCheckable(true);
                action->setChecked(configuration.key == configurator.GetSelectedGlobalConfiguration());
                action->setEnabled(enabled_layers);

                this->_tray_icon_menu->addAction(action);

                this->connect(action, SIGNAL(triggered()), mapper_layers, SLOT(map()));
                mapper_layers->setMapping(action, i);
            }

            this->connect(mapper_layers, &QSignalMapper::mappedInt, this, &MainWindow::OnLayersChanged);

            this->_tray_icon_menu->addSeparator();

            QAction *tray_override_loader = new QAction("Override System Vulkan Loader Log", this);
            tray_override_loader->setFont(font);
            tray_override_loader->setCheckable(true);
            tray_override_loader->setChecked(configurator.loader_log_enabled);
            this->connect(tray_override_loader, &QAction::toggled, this, &MainWindow::OnTrayActionOverrideLog);
            this->_tray_icon_menu->addAction(tray_override_loader);

            this->_tray_icon_menu->addSeparator();

            QAction *tray_quit_action = new QAction("&Quit Vulkan Configurator", this);
            tray_quit_action->setIcon(::Get(configurator.current_theme_mode, ::ICON_EXIT));
            tray_quit_action->setFont(font);
            this->connect(tray_quit_action, &QAction::triggered, qApp, &QCoreApplication::quit);
            this->_tray_icon_menu->addAction(tray_quit_action);

            /*
                        if (VKC_ENV != VKC_ENV_WIN32) {
                            if (this->_tray_icon != nullptr) {
                                delete this->_tray_icon;
                            }
                            this->_tray_icon = new QSystemTrayIcon(this);
                        }
            */

            this->_tray_icon->setContextMenu(this->_tray_icon_menu);
            this->connect(this->_tray_icon, &QSystemTrayIcon::activated, this, &MainWindow::OnIconActivated);

            if (configurator.layers_override_enabled || configurator.driver_override_enabled || configurator.loader_log_enabled) {
                this->_tray_icon->setIcon(QIcon(":/resourcefiles/vkconfig-on.png"));
            } else {
                this->_tray_icon->setIcon(QIcon(":/resourcefiles/vkconfig-off.png"));
            }

            this->_tray_icon->show();
        }
    }

    /*
        if (QSystemTrayIcon::isSystemTrayAvailable()) {
            // Layers
            {
                QMenu *menu_layers = new QMenu(this);
                QSignalMapper *mapper = new QSignalMapper(this);

                QAction *tray_restore_action = new QAction("&Show Vulkan Configurator UI", this);
                tray_restore_action->setIcon(QIcon(":/resourcefiles/vkconfig-on.png"));
                QFont font = tray_restore_action->font();
                font.setBold(true);
                tray_restore_action->setFont(font);
                this->connect(tray_restore_action, &QAction::triggered, this, &MainWindow::OnTrayActionShow);
                menu_layers->addAction(tray_restore_action);

                menu_layers->addSeparator();
                const bool enabled_layers = configurator.layers_override_enabled && configurator.GetExecutableScope() !=
       EXECUTABLE_PER;

                QAction *tray_override_layers = new QAction("Override System Vulkan &Layers Configuration with:", this);
                tray_override_layers->setFont(font);
                tray_override_layers->setCheckable(true);
                tray_override_layers->setChecked(enabled_layers);

                this->connect(tray_override_layers, &QAction::toggled, this, &MainWindow::OnTrayActionOverrideLayers);
                menu_layers->addAction(tray_override_layers);

                for (std::size_t i = 0, n = configurator.configurations.available_configurations.size(); i < n; ++i) {
                    const Configuration &configuration = configurator.configurations.available_configurations[i];

                    QAction *action = new QAction(configuration.key.c_str(), this);
                    action->setCheckable(true);
                    action->setChecked(configuration.key == configurator.GetSelectedGlobalConfiguration());
                    action->setEnabled(enabled_layers);

                    // this->connect(action, &QAction::toggled, this, &MainWindow::OnTrayActionOverrideLayers);
                    menu_layers->addAction(action);

                    this->connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
                    mapper->setMapping(action, i);
                }

                this->connect(mapper, &QSignalMapper::mappedInt, this, &MainWindow::OnLayersChanged);

                menu_layers->addSeparator();

                QAction *tray_quit_action = new QAction("&Quit Vulkan Configurator", this);
                tray_quit_action->setIcon(::Get(configurator.current_theme_mode, ::ICON_EXIT));
                tray_quit_action->setFont(font);
                this->connect(tray_quit_action, &QAction::triggered, qApp, &QCoreApplication::quit);
                menu_layers->addAction(tray_quit_action);

                if (this->_tray_icon_layers != nullptr) {
                    delete this->_tray_icon_layers;
                }

                this->_tray_icon_layers = new QSystemTrayIcon(this);
                this->_tray_icon_layers->setContextMenu(menu_layers);
                this->_tray_icon_layers->show();
                this->connect(this->_tray_icon_layers, &QSystemTrayIcon::activated, this, &MainWindow::OnIconActivated);

                if (configurator.layers_override_enabled) {
                    this->_tray_icon_layers->setIcon(::Get(configurator.current_theme_mode, ::ICON_LAYERS_ON));
                } else {
                    this->_tray_icon_layers->setIcon(::Get(configurator.current_theme_mode, ::ICON_LAYERS_OFF));
                }
            }

            // Device
            {
                QMenu *menu = new QMenu(this);
                QSignalMapper *mapper = new QSignalMapper(this);

                QAction *tray_restore_action = new QAction("&Show Vulkan Configurator UI", this);
                tray_restore_action->setIcon(QIcon(":/resourcefiles/vkconfig-on.png"));
                QFont font = tray_restore_action->font();
                font.setBold(true);
                tray_restore_action->setFont(font);
                this->connect(tray_restore_action, &QAction::triggered, this, &MainWindow::OnTrayActionShow);
                menu->addAction(tray_restore_action);

                menu->addSeparator();
                const bool enabled_device = configurator.driver_override_enabled;

                QAction *tray_override = new QAction("Override System Vulkan &Device with:", this);
                tray_override->setFont(font);
                tray_override->setCheckable(true);
                tray_override->setChecked(enabled_device);

                this->connect(tray_override, &QAction::toggled, this, &MainWindow::OnTrayActionOverrideDevice);
                menu->addAction(tray_override);

                for (std::size_t i = 0, n = configurator.vulkan_system_info.physicalDevices.size(); i < n; ++i) {
                    const VulkanPhysicalDeviceInfo &info = configurator.vulkan_system_info.physicalDevices[i];
                    const DeviceInfo &device_info = ::GetDeviceInfo(info);

                    QAction *action = new QAction(format("%s (%s)", info.GetLabel().c_str(), info.GetVersion().c_str()).c_str());
                    action->setCheckable(true);
                    action->setChecked(device_info == configurator.driver_override_info);
                    action->setEnabled(enabled_device);

                    menu->addAction(action);

                    this->connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
                    mapper->setMapping(action, i);
                }

                this->connect(mapper, &QSignalMapper::mappedInt, this, &MainWindow::OnDeviceChanged);

                menu->addSeparator();

                QAction *tray_quit_action = new QAction("&Quit Vulkan Configurator", this);
                tray_quit_action->setIcon(::Get(configurator.current_theme_mode, ::ICON_EXIT));
                tray_quit_action->setFont(font);
                this->connect(tray_quit_action, &QAction::triggered, qApp, &QCoreApplication::quit);
                menu->addAction(tray_quit_action);

                if (this->_tray_icon_device != nullptr) {
                    delete this->_tray_icon_device;
                }

                this->_tray_icon_device = new QSystemTrayIcon(this);
                this->_tray_icon_device->setContextMenu(menu);
                this->_tray_icon_device->show();
                this->connect(this->_tray_icon_device, &QSystemTrayIcon::activated, this, &MainWindow::OnIconActivated);

                if (configurator.driver_override_enabled) {
                    this->_tray_icon_device->setIcon(::Get(configurator.current_theme_mode, ::ICON_DEVICE_ON));
                } else {
                    this->_tray_icon_device->setIcon(::Get(configurator.current_theme_mode, ::ICON_DEVICE_OFF));
                }
            }
        }
    */
    if (configurator.layers_override_enabled || configurator.driver_override_enabled) {
        this->setWindowIcon(QIcon(":/resourcefiles/vkconfig-on.png"));
    } else {
        this->setWindowIcon(QIcon(":/resourcefiles/vkconfig-off.png"));
    }
}

void MainWindow::OnIconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        default:
            break;
        case QSystemTrayIcon::Context:
            break;
        case QSystemTrayIcon::DoubleClick:
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

void MainWindow::OnTrayActionShow() {
    this->hide();
    this->showNormal();
    this->UpdateUI(UPDATE_REBUILD_UI);
}

void MainWindow::OnTrayActionOverrideLayers(bool toggled) {
    Configurator &configurator = Configurator::Get();
    configurator.layers_override_enabled = toggled;
    configurator.Override(OVERRIDE_AREA_ALL);

    this->tabs[TAB_CONFIGURATIONS]->UpdateUI(UPDATE_REBUILD_UI);
}

void MainWindow::OnLayersChanged(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.SetActiveConfigurationName(configurator.configurations.available_configurations[index].key);
    configurator.Override(OVERRIDE_AREA_ALL);

    this->tabs[TAB_CONFIGURATIONS]->UpdateUI(UPDATE_REBUILD_UI);
}

void MainWindow::OnTrayActionOverrideDevice(bool toggled) {
    Configurator &configurator = Configurator::Get();
    configurator.driver_override_mode = DRIVER_MODE_SINGLE;
    configurator.driver_override_enabled = toggled;
    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    this->tabs[TAB_DRIVERS]->UpdateUI(UPDATE_REBUILD_UI);
}

void MainWindow::OnDeviceChanged(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.driver_override_info = ::GetDeviceInfo(configurator.vulkan_system_info.physicalDevices[index]);
    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    this->tabs[TAB_DRIVERS]->UpdateUI(UPDATE_REBUILD_UI);
}

void MainWindow::OnTrayActionOverrideLog(bool toggled) {
    Configurator &configurator = Configurator::Get();
    configurator.loader_log_enabled = toggled;
    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    this->tabs[TAB_DIAGNOSTIC]->UpdateUI(UPDATE_REBUILD_UI);
}

/// The only thing we need to do here is clear the configuration if
/// the user does not want it active.
void MainWindow::closeEvent(QCloseEvent *event) {
    Configurator &configurator = Configurator::Get();

    QGuiApplication::setQuitOnLastWindowClosed(!configurator.GetUseSystemTray());

    if (configurator.GetUseSystemTray()) {
        // Alert the user to the current state of the vulkan configurator and
        // give them the option to not shutdown.
        if (!(configurator.Get(HIDE_MESSAGE_USE_SYSTEM_TRAY))) {
            std::string shut_down_state;

            if (configurator.layers_override_enabled) {
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
            alert.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            alert.setDefaultButton(QMessageBox::Ok);
            alert.setCheckBox(new QCheckBox("Do not show again."));

            QPalette palette, palette_saved = this->ui->preferences_keep_running->palette();
            palette.setColor(QPalette::WindowText, QColor(Qt::red));
            this->ui->preferences_keep_running->setPalette(palette);

            this->ui->tab_widget->setCurrentIndex(TAB_PREFERENCES);

            int ret_val = alert.exec();
            if (alert.checkBox()->isChecked()) {
                configurator.Set(HIDE_MESSAGE_USE_SYSTEM_TRAY);
            }

            this->ui->preferences_keep_running->setPalette(palette_saved);

            if (ret_val == QMessageBox::Cancel) {
                event->ignore();  // Not closing the window
                return;
            }
        }
    }

    this->tabs[this->ui->tab_widget->currentIndex()]->CleanUI();

    configurator.window_geometry = this->saveGeometry();
    configurator.window_state = this->saveState();

    QMainWindow::closeEvent(event);
}

void MainWindow::showEvent(QShowEvent *event) {
    (void)event;

    this->UpdateUI(UPDATE_REBUILD_UI);

    event->accept();
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    if (this->tabs[this->ui->tab_widget->currentIndex()] == nullptr) {
        return true;
    }

    return this->tabs[this->ui->tab_widget->currentIndex()]->EventFilter(target, event);
}

void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::ThemeChange) {
        Configurator &configurator = Configurator::Get();

        static_cast<TabPreferences *>(this->tabs[TAB_PREFERENCES].get())->on_theme_mode_changed(configurator.current_theme_mode);
    }
}

void MainWindow::on_tab_widget_currentChanged(int index) {
    assert(index >= 0);

    Configurator &configurator = Configurator::Get();
    configurator.active_tab = static_cast<TabType>(index);

    switch (configurator.active_tab) {
        case TAB_CONFIGURATIONS: {
        } break;
        case TAB_LAYERS_PATHS: {
            // QShortcut *shortcut_search = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this->ui->tab_widget);
            // this->connect(shortcut_search, SIGNAL(activated()), this, SLOT(on_focus_search()));

            // QShortcut *shortcut_open = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_O), this->ui->layers_list);
            // this->connect(shortcut_open, SIGNAL(activated()), this, SLOT(on_layers_browse_pressed()));
        } break;
        case TAB_DRIVERS: {
            // QShortcut *shortcut_search = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this->ui->tab_widget);
            // this->connect(shortcut_search, SIGNAL(activated()), this, SLOT(on_focus_search()));
        } break;
        case TAB_APPLICATIONS: {
            // QShortcut *shortcut_search = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this->ui->tab_widget);
            // this->connect(shortcut_search, SIGNAL(activated()), this, SLOT(on_focus_search()));
        } break;
        case TAB_DIAGNOSTIC: {
        } break;
        case TAB_DOCUMENTATION: {
        } break;
        case TAB_PREFERENCES: {
        } break;
        case TAB_ABOUT: {
        } break;
    }

    this->tabs[index]->UpdateUI(UPDATE_REBUILD_UI);
}
