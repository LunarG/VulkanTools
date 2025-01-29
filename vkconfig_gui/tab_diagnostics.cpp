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

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>

TabDiagnostics::TabDiagnostics(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_DIAGNOSTIC, window, ui) {
    this->ui->diagnostic_status_text->installEventFilter(&window);

    this->connect(this->ui->diagnostic_keep_running, SIGNAL(toggled(bool)), this, SLOT(on_diagnostic_keep_running_toggled(bool)));
    this->connect(this->ui->diagnostic_vk_home_text, SIGNAL(returnPressed()), this, SLOT(on_diagnostic_vk_home_text_pressed()));
    this->connect(this->ui->diagnostic_vk_home_browse, SIGNAL(clicked()), this, SLOT(on_diagnostic_vk_home_browse_pressed()));
    this->connect(this->ui->diagnostic_reset, SIGNAL(clicked()), this, SLOT(on_diagnostic_reset_hard_pressed()));

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

    Configurator &configurator = Configurator::Get();

    this->ui->diagnostic_keep_running->blockSignals(true);
    this->ui->diagnostic_keep_running->setChecked(configurator.GetUseSystemTray());
    this->ui->diagnostic_keep_running->blockSignals(false);

    this->ui->diagnostic_vk_home_text->blockSignals(true);
    this->ui->diagnostic_vk_home_text->setText(::Path(Path::HOME).RelativePath().c_str());
    this->ui->diagnostic_vk_home_text->setToolTip(::Path(Path::HOME).AbsolutePath().c_str());
    this->ui->diagnostic_vk_home_text->blockSignals(false);
}

void TabDiagnostics::CleanUI() {}

bool TabDiagnostics::EventFilter(QObject *target, QEvent *event) {
    (void)target;
    (void)event;

    return false;
}

void TabDiagnostics::on_diagnostic_keep_running_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();
    configurator.SetUseSystemTray(checked);
}

void TabDiagnostics::on_diagnostic_vk_home_text_pressed() {
    Configurator &configurator = Configurator::Get();

    Path path(this->ui->diagnostic_vk_home_text->text().toStdString());
    if (path.Exists()) {
        ::SetHomePath(this->ui->diagnostic_vk_home_text->text().toStdString());
    } else {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setWindowTitle("Invalid ${VK_HOME} path...");
        message.setText(
            format("'%s' is not a valid, it doesn't exist.", this->ui->diagnostic_vk_home_text->text().toStdString().c_str())
                .c_str());
        message.setInformativeText(format("Restoring the previous path '%s'.", ::Path(Path::HOME).AbsolutePath().c_str()).c_str());
        message.exec();

        this->ui->diagnostic_vk_home_text->setText(::Path(Path::HOME).AbsolutePath().c_str());
    }
}

void TabDiagnostics::on_diagnostic_vk_home_browse_pressed() {
    Configurator &configurator = Configurator::Get();
    const QString selected_path = QFileDialog::getExistingDirectory(
        this->ui->diagnostic_vk_home_browse, "Select the Vulkan Home Default Working Folder (Set ${VK_HOME} value)...",
        ::Path(Path::HOME).AbsolutePath().c_str());

    if (!selected_path.isEmpty()) {
        this->ui->diagnostic_vk_home_text->setText(selected_path);

        ::SetHomePath(selected_path.toStdString());
    }
}

void TabDiagnostics::on_diagnostic_reset_hard_pressed() {
    QMessageBox message;
    message.setIcon(QMessageBox::Critical);
    message.setWindowTitle(format("Restoring and Resetting %s to default...", VKCONFIG_NAME).c_str());
    message.setText(
        "You are about to delete all the user-defined configurations and resetting all default configurations to their default "
        "state.");
    message.setInformativeText("Do you want to continue?");
    message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    message.setDefaultButton(QMessageBox::No);
    if (message.exec() == QMessageBox::Yes) {
        Configurator::Get().Reset(true);
        qApp->quit();
    }
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
