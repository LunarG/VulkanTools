/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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

#include <cassert>

MainWindow::MainWindow(QApplication &app, QWidget *parent)
    : QMainWindow(parent),
      _tray_icon(nullptr),
      _tray_icon_menu(nullptr),
      _tray_restore_action(nullptr),
      _tray_layers{nullptr, nullptr, nullptr, nullptr},
      _tray_quit_action(nullptr),
      app(app),
      ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->tabs[TAB_DIAGNOSTIC].reset(new TabDiagnostics(*this, ui));
    this->tabs[TAB_APPLICATIONS].reset(new TabApplications(*this, ui));
    this->tabs[TAB_LAYERS].reset(new TabLayers(*this, ui));
    this->tabs[TAB_CONFIGURATIONS].reset(new TabConfigurations(*this, ui));
    this->tabs[TAB_DOCUMENTATION].reset(new TabDocumentation(*this, ui));
    this->tabs[TAB_PREFERENCES].reset(new TabPreferences(*this, ui));
    this->tabs[TAB_ABOUT].reset(new TabAbout(*this, ui));

    this->connect(qApp, &QGuiApplication::commitDataRequest, this, &MainWindow::commitDataRequest);

    for (int i = TAB_FIRST, l = TAB_LAST; i <= l; ++i) {
        this->ui->tab_widget->setTabText(i, GetLabel(static_cast<TabType>(i)));
    }

    const Configurator &configurator = Configurator::Get();

    if (!configurator.window_geometry.isEmpty()) {
        this->restoreGeometry(configurator.window_geometry);
        this->restoreState(configurator.window_state);
    }

    this->ui->tab_widget->setCurrentIndex(configurator.active_tab);

    this->InitTray();
    this->UpdateUI(UPDATE_REBUILD_UI);
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

void MainWindow::InitTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        this->_tray_quit_action = new QAction("&Quit", this);
        this->connect(this->_tray_quit_action, &QAction::triggered, qApp, &QCoreApplication::quit);
        this->_tray_restore_action = new QAction("Open &Vulkan Configurator", this);
        this->connect(this->_tray_restore_action, &QAction::triggered, this, &MainWindow::trayActionRestore);

        this->_tray_icon_menu = new QMenu(this);
        this->_tray_icon_menu->addAction(this->_tray_restore_action);
        this->_tray_icon_menu->addSeparator();
        for (int i = 0, n = EXECUTABLE_SCOPE_COUNT; i < n; ++i) {
            this->_tray_layers[i] = new QAction(::GetLabel(static_cast<ExecutableScope>(i)), this);
            this->_tray_layers[i]->setCheckable(true);
            this->_tray_icon_menu->addAction(this->_tray_layers[i]);
        }

        this->connect(this->_tray_layers[EXECUTABLE_NONE], &QAction::toggled, this, &MainWindow::on_tray_none);
        this->connect(this->_tray_layers[EXECUTABLE_ANY], &QAction::toggled, this, &MainWindow::on_tray_any);
        this->connect(this->_tray_layers[EXECUTABLE_ALL], &QAction::toggled, this, &MainWindow::on_tray_all);
        this->connect(this->_tray_layers[EXECUTABLE_PER], &QAction::toggled, this, &MainWindow::on_tray_per);

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

    this->setWindowTitle(GetMainWindowTitle().c_str());
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        for (int i = 0, n = EXECUTABLE_SCOPE_COUNT; i < n; ++i) {
            this->_tray_layers[i]->setChecked(configurator.GetExecutableScope() == i);
        }
    }

    if (configurator.GetExecutableScope() != EXECUTABLE_NONE) {
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

void MainWindow::on_tray_none(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();
        configurator.SetExecutableScope(EXECUTABLE_NONE);
        configurator.Override(OVERRIDE_AREA_ALL);

        this->UpdateUI(UPDATE_REBUILD_UI);
    }
}

void MainWindow::on_tray_any(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();
        configurator.SetExecutableScope(EXECUTABLE_ANY);
        configurator.Override(OVERRIDE_AREA_ALL);

        this->UpdateUI(UPDATE_REBUILD_UI);
    }
}

void MainWindow::on_tray_all(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();
        configurator.SetExecutableScope(EXECUTABLE_ALL);
        configurator.Override(OVERRIDE_AREA_ALL);

        this->UpdateUI(UPDATE_REBUILD_UI);
    }
}

void MainWindow::on_tray_per(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();
        configurator.SetExecutableScope(EXECUTABLE_PER);
        configurator.Override(OVERRIDE_AREA_ALL);

        this->UpdateUI(UPDATE_REBUILD_UI);
    }
}

void MainWindow::toolsResetToDefault(bool checked) {
    (void)checked;

    QMessageBox alert;
    alert.QDialog::setWindowTitle("Restoring and Resetting all Layers Configurations to default");
    alert.setText(
        "You are about to delete all the user-defined configurations and resetting all default configurations to their default "
        "state.");
    alert.setInformativeText("Do you want to continue?");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::Yes);
    alert.setIcon(QMessageBox::Warning);

    if (alert.exec() == QMessageBox::No) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    configurator.Reset(true);
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

            if (configurator.GetExecutableScope() != EXECUTABLE_NONE) {
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
        return false;
    }

    return this->tabs[this->ui->tab_widget->currentIndex()]->EventFilter(target, event);
}

void MainWindow::on_tab_widget_currentChanged(int index) {
    assert(index >= 0);

    Configurator &configurator = Configurator::Get();
    configurator.active_tab = static_cast<TabType>(index);

    this->tabs[index]->UpdateUI(UPDATE_REBUILD_UI);
}
