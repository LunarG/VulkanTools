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

#pragma once

#include "../vkconfig_core/type_executable_mode.h"

#include "tab_configurations.h"
#include "tab_layers.h"
#include "tab_applications.h"
#include "tab_diagnostics.h"
#include "tab_documentation.h"
#include "tab_preferences.h"
#include "tab_about.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QShowEvent>
#include <QResizeEvent>
#include <QSystemTrayIcon>

#include <string>
#include <array>

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QApplication &app, QWidget *parent = nullptr);
    ~MainWindow();

    void UpdateUI(UpdateUIMode mode);

   private:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject *target, QEvent *event) override;
    void changeEvent(QEvent *event) override;

    QSystemTrayIcon *_tray_icon = nullptr;
    QMenu *_tray_icon_menu = nullptr;
    QAction *_tray_restore_action = nullptr;
    std::array<QAction *, EXECUTABLE_SCOPE_COUNT> _tray_layers;
    QAction *_tray_quit_action = nullptr;

   private slots:
    void trayActionRestore();
    void on_tray_none(bool checked);
    void on_tray_any(bool checked);
    void on_tray_all(bool checked);
    void on_tray_per(bool checked);

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

   public Q_SLOTS:
    void commitDataRequest(QSessionManager &manager);

    void on_tab_widget_currentChanged(int index);

    void UpdateUI_Status();

   private:
    MainWindow(const MainWindow &) = delete;
    MainWindow &operator=(const MainWindow &) = delete;

    void InitTray();

   public:
    QApplication &app;

   private:
    std::shared_ptr<Ui::MainWindow> ui;
    std::array<std::shared_ptr<Tab>, TAB_COUNT> tabs;
};
