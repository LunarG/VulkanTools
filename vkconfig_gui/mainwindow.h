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

#pragma once

#include "../vkconfig_core/type_executable_mode.h"

#include "tab_configurations.h"
#include "tab_drivers.h"
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
#include <vector>

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

   public Q_SLOTS:
    void commitDataRequest(QSessionManager &manager);

    void on_tab_widget_currentChanged(int index);

    void OnIconActivated(QSystemTrayIcon::ActivationReason reason);
    void OnTrayActionShow();
    void OnTrayActionOverrideLayers(bool toggled);
    void OnTrayActionOverrideDevice(bool toggled);
    void OnTrayActionOverrideLog(bool toggled);
    void OnLayersChanged(int index);
    void OnDeviceChanged(int index);

    void UpdateUI_Status();

   private:
    MainWindow(const MainWindow &) = delete;
    MainWindow &operator=(const MainWindow &) = delete;

   public:
    QApplication &app;

   private:
    std::shared_ptr<Ui::MainWindow> ui;
    std::array<std::shared_ptr<Tab>, TAB_COUNT> tabs;
};
