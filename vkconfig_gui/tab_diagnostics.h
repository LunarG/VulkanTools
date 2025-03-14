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

#include "tab.h"

#include <QPushButton>

class TabDiagnostics : public Tab {
    Q_OBJECT

   public:
    TabDiagnostics(MainWindow& window, std::shared_ptr<Ui::MainWindow> ui);
    virtual ~TabDiagnostics();

    virtual void UpdateUI(UpdateUIMode mode) override;
    virtual void CleanUI() override;
    virtual bool EventFilter(QObject* target, QEvent* event) override;

   public Q_SLOTS:
    void on_customContextMenuRequested(const QPoint& pos);

   private:
    QPushButton* widget_refresh = nullptr;
    QPushButton* widget_export = nullptr;
    std::string status;

    void UpdateStatus();
};
