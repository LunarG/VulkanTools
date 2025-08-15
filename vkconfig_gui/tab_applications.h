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

#include <memory>

#include <QProcess>
#include <QFile>

class TabApplications : public Tab {
    Q_OBJECT

   public:
    TabApplications(MainWindow& window, std::shared_ptr<Ui::MainWindow> ui);
    virtual ~TabApplications();

    virtual void UpdateUI(UpdateUIMode mode) override;
    virtual void CleanUI() override;
    virtual bool EventFilter(QObject* target, QEvent* event) override;

    void ResetLaunchApplication();

   public Q_SLOTS:
    void on_launch_executable_list_activated(int index);
    void on_launch_executable_list_textEdited(const QString& text);
    void on_launch_executable_search_pressed();
    void on_launch_executable_append_pressed();
    void on_launch_executable_remove_pressed();

    void on_launch_options_list_activated(int index);
    void on_launch_options_list_textEdited(const QString& text);
    void on_launch_options_append_pressed();
    void on_launch_options_remove_pressed();

    void on_launch_options_dir_textEdited(const QString& text);
    void on_launch_options_dir_pressed();
    void on_launch_options_args_textEdited(const QString& text);
    void on_launch_options_envs_textEdited(const QString& text);
    void on_launch_options_log_textEdited(const QString& text);
    void on_launch_options_log_pressed();
    void on_launch_options_log_open_pressed();

    void on_context_menu(const QPoint& pos);
    void on_export_file();

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

    void on_launch_clear_at_launch_toggled(bool checked);
    void on_launch_clear_log_pressed();
    void on_launch_button_pressed();

    void standardOutputAvailable();                                 // stdout output is available
    void errorOutputAvailable();                                    // Layeroutput is available
    void processClosed(int exitCode, QProcess::ExitStatus status);  // app died

   private:
    void EnableOptions();
    void RebuildOptions();
    void SearchFind(bool prev);

    void Log(const QString& log, bool flush);

    std::unique_ptr<QProcess> _launch_application;  // Keeps track of the monitored app
    QFile _log_file;                                // Log file for layer output

    std::string search_text;
    bool search_case = false;
    bool search_whole = false;
    bool search_regex = false;
};
