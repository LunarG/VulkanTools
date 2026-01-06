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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "tab.h"

#include "../vkconfig_core/type_diagnostic_mode.h"
#include "../vkconfig_core/path.h"

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
    void on_context_menu(const QPoint& pos);
    void on_mode_changed(int index);
    void on_mode_options_changed(int index);
    void on_export_folder();
    void on_export_file();
    void on_refresh_log();

    void on_focus_search();
    void on_search_textEdited(const QString& text);
    void on_search_clear_pressed();
    void on_search_next_pressed();
    void on_search_prev_pressed();
    void on_search_case_toggled(bool checked);
    void on_search_whole_toggled(bool checked);
    void on_search_regex_toggled(bool checked);
    void on_search_case_activated();
    void on_search_whole_activated();
    void on_search_regex_activated();

    void on_diagnostic_loader_messages_toggled(bool checked);
    void on_diagnostic_loader_errors_toggled(bool checked);
    void on_diagnostic_loader_warns_toggled(bool checked);
    void on_diagnostic_loader_infos_toggled(bool checked);
    void on_diagnostic_loader_debug_toggled(bool checked);
    void on_diagnostic_loader_layers_toggled(bool checked);
    void on_diagnostic_loader_drivers_toggled(bool checked);
    void on_diagnostic_dir_sdk_pressed();
    void on_diagnostic_dir_home_pressed();
    void on_diagnostic_dir_system_pressed();
    void on_diagnostic_dir_info_pressed();

    void standardOutputAvailable();                                 // stdout output is available
    void errorOutputAvailable();                                    // Layeroutput is available
    void processClosed(int exitCode, QProcess::ExitStatus status);  // app died

   private:
    Path log_path;
    DiagnosticMode mode = DIAGNOSTIC_VULKAN_STATUS;
    std::string status;
    std::string diagnostic_search_text;
    bool search_case = false;
    bool search_whole = false;
    bool search_regex = false;

    void OnCheckedLoaderMessageTypes(bool checked);

    std::string BuildStatus(DiagnosticMode selected_mode, std::size_t mode_index);
    void UpdateStatus();
    void SearchFind(bool prev);

    std::unique_ptr<QProcess> process;  // Keeps track of the monitored app for diagnostic generation
};
