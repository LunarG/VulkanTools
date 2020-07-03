/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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

#include <QDialog>
#include <QTreeWidgetItem>

#include <configurator.h>

namespace Ui {
class dlgCreateAssociation;
}

class dlgCreateAssociation : public QDialog {
    Q_OBJECT

   public:
    explicit dlgCreateAssociation(QWidget *parent = nullptr);
    ~dlgCreateAssociation();

    static void GetExecutableFromAppBundle(QString &path);
    int GetSelectedLaunchApplicationIndex() const { return last_selected_application_index; }

   private:
    virtual void closeEvent(QCloseEvent *) override;
    virtual bool eventFilter(QObject *target, QEvent *event) override;

    Ui::dlgCreateAssociation *ui_;
    int last_selected_application_index;

   public Q_SLOTS:
    void on_pushButtonAdd_clicked();                                                // Pick the application
    void on_pushButtonRemove_clicked();                                             // Remove
    void selectedPathChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);  // Used to enable remove button
    void itemChanged(QTreeWidgetItem *item, int column_index);
    void itemClicked(bool clicked);

    void editCommandLine(const QString &cmd_line);
    void editWorkingFolder(const QString &working_folder);
    void editLogFile(const QString &log_file);
};
