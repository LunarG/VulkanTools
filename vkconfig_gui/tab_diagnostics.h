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

#include "../vkconfig_core/type_diagnostic_mode.h"

#include <QProcess>
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
    void on_mode_changed(int index);
    void on_export();
    void on_focus_search();
    void on_search_textEdited(const QString& text);
    void on_search_next_pressed();
    void on_search_prev_pressed();
    void on_search_case_toggled(bool checked);
    void on_search_whole_toggled(bool checked);
    void on_search_regex_toggled(bool checked);

    void standardOutputAvailable();                                 // stdout output is available
    void errorOutputAvailable();                                    // Layeroutput is available
    void processClosed(int exitCode, QProcess::ExitStatus status);  // app died

   private:
    DiagnosticMode mode = DIAGNOSTIC_VULKAN_STATUS;
    std::string status;
    bool search_case = false;
    bool search_whole = false;
    bool search_regex = false;

    void UpdateStatus();
    void SearchFind(bool prev);

    std::unique_ptr<QProcess> launch_application;  // Keeps track of the monitored app for diagnostic generation
};
