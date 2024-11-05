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

#include "tab_diagnostics.h"
#include "mainwindow.h"

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>
#include <QMessageBox>

TabDiagnostics::TabDiagnostics(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_DIAGNOSTIC, window, ui) {
    this->connect(this->ui->diagnostic_keep_running, SIGNAL(toggled(bool)), this, SLOT(on_diagnostic_keep_running_toggled(bool)));
    this->connect(this->ui->diagnostic_vk_home_text, SIGNAL(returnPressed()), this, SLOT(on_diagnostic_vk_home_text_pressed()));
    this->connect(this->ui->diagnostic_vk_home_browse, SIGNAL(pressed()), this, SLOT(on_diagnostic_vk_home_browse_pressed()));
    this->connect(this->ui->diagnostic_reset_hard, SIGNAL(pressed()), this, SLOT(on_diagnostic_reset_hard_pressed()));

    Configurator &configurator = Configurator::Get();

    this->ui->diagnostic_reset_hard->setIcon(QIcon(":/resourcefiles/reset.png"));

    this->ui->diagnostic_keep_running->blockSignals(true);
    this->ui->diagnostic_keep_running->setChecked(configurator.GetUseSystemTray());
    this->ui->diagnostic_keep_running->blockSignals(false);

    this->ui->diagnostic_vk_home_text->blockSignals(true);
    this->ui->diagnostic_vk_home_text->setText(configurator.GetHomeSDK().RelativePath().c_str());
    this->ui->diagnostic_vk_home_text->setToolTip(configurator.GetHomeSDK().AbsolutePath().c_str());
    this->ui->diagnostic_vk_home_text->blockSignals(false);
}

TabDiagnostics::~TabDiagnostics() {}

void TabDiagnostics::UpdateUI(UpdateUIMode mode) {
    Configurator &configurator = Configurator::Get();
    this->ui->diagnostic_status_text->setMarkdown(configurator.GenerateVulkanStatus().c_str());
}

void TabDiagnostics::CleanUI() {}

bool TabDiagnostics::EventFilter(QObject *target, QEvent *event) { return false; }

void TabDiagnostics::on_diagnostic_keep_running_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();
    configurator.SetUseSystemTray(checked);
}

void TabDiagnostics::on_diagnostic_vk_home_text_pressed() {
    Configurator &configurator = Configurator::Get();

    Path path(this->ui->diagnostic_vk_home_text->text().toStdString());
    if (path.Exists()) {
        configurator.SetHomeSDK(this->ui->diagnostic_vk_home_text->text().toStdString().c_str());
    } else {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setWindowTitle("Invalid ${VK_HOME} path...");
        message.setText(
            format("'%s' is not a valid, it doesn't exist.", this->ui->diagnostic_vk_home_text->text().toStdString().c_str())
                .c_str());
        message.setInformativeText(
            format("Restoring the previous path '%s'.", configurator.GetHomeSDK().AbsolutePath().c_str()).c_str());
        message.exec();

        this->ui->diagnostic_vk_home_text->setText(configurator.GetHomeSDK().AbsolutePath().c_str());
    }
}

void TabDiagnostics::on_diagnostic_vk_home_browse_pressed() {
    Configurator &configurator = Configurator::Get();
    const QString selected_path = QFileDialog::getExistingDirectory(
        this->ui->diagnostic_vk_home_browse, "Select the Vulkan Home Default Working Folder (Set ${VK_HOME} value)...",
        configurator.GetHomeSDK().AbsolutePath().c_str());

    if (!selected_path.isEmpty()) {
        this->ui->diagnostic_vk_home_text->setText(selected_path);

        configurator.SetHomeSDK(selected_path.toStdString());
    }
}

void TabDiagnostics::on_diagnostic_reset_hard_pressed() {
    QMessageBox message;
    message.setIcon(QMessageBox::Critical);
    message.setWindowTitle(format("Restoring and Resetting %s to default...", VKCONFIG_NAME).c_str());
    message.setText(
        "You are about to delete all the user-defined configurations and resetting all default configurations to their default "
        "state.");
    message.setInformativeText("Are you sure you want to continue?");
    message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    message.setDefaultButton(QMessageBox::No);
    if (message.exec() == QMessageBox::Yes) {
        Configurator &configurator = Configurator::Get();
        configurator.Reset(true);
    }
}
