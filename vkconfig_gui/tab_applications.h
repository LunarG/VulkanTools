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
    void on_applications_remove_application_pushButton_pressed();
    void on_applications_append_application_pushButton_pressed();
    void on_applications_list_comboBox_activated(int index);
    void on_applications_list_comboBox_textEdited(const QString& text);

    void on_applications_options_remove_pushButton_pressed();
    void on_applications_options_append_pushButton_pressed();
    void on_applications_options_comboBox_activated(int index);
    void on_applications_options_comboBox_textEdited(const QString& text);

    void on_applications_configuration_comboBox_activated(int index);

    void on_applications_directory_edit_textEdited(const QString& text);
    void on_applications_directory_edit_pushButton_pressed();
    void on_applications_args_list_textEdited(const QString& text);
    void on_applications_envs_list_textEdited(const QString& text);
    void on_applications_output_log_edit_textEdited(const QString& text);
    void on_applications_output_log_pushButton_pressed();

    void on_applications_launcher_pushButton_pressed();
    void on_applications_clear_log_pushButton_pressed();
    void on_check_box_clear_on_launch_clicked();

    void standardOutputAvailable();                                 // stdout output is available
    void errorOutputAvailable();                                    // Layeroutput is available
    void processClosed(int exitCode, QProcess::ExitStatus status);  // app died

   private:
    void EnableOptions();

    void Log(const std::string& log);

    std::unique_ptr<QProcess> _launch_application;  // Keeps track of the monitored app
    QFile _log_file;                                // Log file for layer output
};
