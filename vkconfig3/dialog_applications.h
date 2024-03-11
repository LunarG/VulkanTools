/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

#include "ui_dialog_applications.h"

#include "../vkconfig_core/application.h"

#include <memory>

class ApplicationsDialog : public QDialog {
    Q_OBJECT

   public:
    explicit ApplicationsDialog(QWidget *parent = nullptr);

    int GetSelectedLaunchApplicationIndex() const { return _last_selected_application_index; }

   private:
    QTreeWidgetItem *CreateApplicationItem(const Application &application) const;

    void closeEvent(QCloseEvent *) override;
    bool eventFilter(QObject *target, QEvent *event) override;

   public Q_SLOTS:
    void on_pushButtonAdd_clicked();                                                // Pick the application
    void on_pushButtonRemove_clicked();                                             // Remove
    void on_pushButtonSelect_clicked();                                             // Make this the current launcher app
    void selectedPathChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);  // Used to enable remove button
    void itemChanged(QTreeWidgetItem *item, int column_index);
    void itemClicked(bool clicked);

    void editAppName(const QString &name);
    void editExecutable(const QString &name);
    void editCommandLine(const QString &cmd_line);
    void editWorkingFolder(const QString &working_folder);
    void editLogFile(const QString &log_file);

   private:
    ApplicationsDialog(const ApplicationsDialog &) = delete;
    ApplicationsDialog &operator=(const ApplicationsDialog &) = delete;

    std::unique_ptr<Ui::dialog_applications> ui;
    int _last_selected_application_index;
};
