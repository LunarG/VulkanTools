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

#include "tab_diagnostics.h"
#include "mainwindow.h"
#include "widget_resize_button.h"
#include "style.h"

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>

TabDiagnostics::TabDiagnostics(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_DIAGNOSTIC, window, ui) {
    this->ui->diagnostic_status_text->installEventFilter(&window);
    this->ui->diagnostic_status_text->document()->setMaximumBlockCount(65536);
    this->ui->diagnostic_status_text->setContextMenuPolicy(Qt::CustomContextMenu);
    this->connect(this->ui->diagnostic_status_text, SIGNAL(customContextMenuRequested(QPoint)), this,
                  SLOT(on_customContextMenuRequested(const QPoint &)));

    this->UpdateStatus();
}

TabDiagnostics::~TabDiagnostics() {}

void TabDiagnostics::UpdateStatus() {
    Configurator &configurator = Configurator::Get();

    configurator.Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    this->status = configurator.GenerateVulkanStatus();
    this->ui->diagnostic_status_text->setText(this->status.c_str());

    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
}

void TabDiagnostics::UpdateUI(UpdateUIMode mode) {
    (void)mode;

    this->UpdateStatus();
}

void TabDiagnostics::CleanUI() {}

bool TabDiagnostics::EventFilter(QObject *target, QEvent *event) {
    (void)target;
    (void)event;

    return false;
}

void TabDiagnostics::on_customContextMenuRequested(const QPoint &pos) {
    Configurator &configurator = Configurator::Get();

    QMenu *menu = this->ui->diagnostic_status_text->createStandardContextMenu();
    menu->addSeparator();

    QAction *action_refresh = new QAction(this->status.empty() ? "Refresh" : "Clear", nullptr);
    action_refresh->setEnabled(true);
    menu->addAction(action_refresh);

    QAction *action_save = new QAction("Save...", nullptr);
    action_save->setEnabled(!this->status.empty());
    menu->addAction(action_save);

    QAction *action = menu->exec(this->ui->diagnostic_status_text->mapToGlobal(pos));

    if (action == action_refresh) {
        if (this->status.empty()) {
            configurator.Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

            this->status = configurator.GenerateVulkanStatus();
            this->ui->diagnostic_status_text->setText(this->status.c_str());

            configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
        } else {
            this->status.clear();
            this->ui->diagnostic_status_text->clear();
        }
    } else if (action == action_save) {
        const QString selected_path =
            QFileDialog::getSaveFileName(this->ui->diagnostic_group_box_refresh, "Select Log file...",
                                         configurator.last_path_status.AbsolutePath().c_str(), "Log (*.txt)");

        if (!selected_path.isEmpty()) {
            QFile file(selected_path.toStdString().c_str());
            const bool result = file.open(QFile::WriteOnly);
            if (!result) {
                QMessageBox message;
                message.setIcon(QMessageBox::Critical);
                message.setWindowTitle("Failed to save 'Vulkan Development Status'!");
                message.setText(format("Couldn't write to '%s'.", selected_path.toStdString().c_str()).c_str());
                message.setInformativeText("Select a file path with 'write' rights.");
                return;
            }

            file.write(this->status.c_str());
            file.close();

            configurator.last_path_status = selected_path.toStdString();
        }
    }

    menu->deleteLater();
}
