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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "../vkconfig_core/type_tab.h"

#include "ui_mainwindow.h"

#include <QObject>
#include <QSystemTrayIcon>

#include <memory>
#include <cassert>

class MainWindow;

enum UpdateUIMode {
    UPDATE_REBUILD_UI = 0,
    UPDATE_REFRESH_UI,
};

class Tab : public QObject {
    Q_OBJECT

   public:
    Tab(TabType type, MainWindow& window, std::shared_ptr<Ui::MainWindow> ui);
    virtual ~Tab();

    Tab(const Tab&) = delete;
    Tab& operator=(const Tab&) = delete;

    const TabType type;

    virtual void UpdateUI(UpdateUIMode mode) = 0;
    virtual void CleanUI() = 0;
    virtual bool EventFilter(QObject* target, QEvent* event) = 0;

   protected:
    std::shared_ptr<Ui::MainWindow> ui;
    MainWindow& window;
};
