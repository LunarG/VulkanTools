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
#include "widget_resize_button.h"

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>
#include <QMessageBox>

TabDiagnostics::TabDiagnostics(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_DIAGNOSTIC, window, ui) {
    this->connect(this->ui->diagnostic_keep_running, SIGNAL(toggled(bool)), this, SLOT(on_diagnostic_keep_running_toggled(bool)));
    this->connect(this->ui->diagnostic_vk_home_text, SIGNAL(returnPressed()), this, SLOT(on_diagnostic_vk_home_text_pressed()));
    this->connect(this->ui->diagnostic_vk_home_browse, SIGNAL(clicked()), this, SLOT(on_diagnostic_vk_home_browse_pressed()));

    Configurator &configurator = Configurator::Get();

    this->ui->diagnostic_keep_running->blockSignals(true);
    this->ui->diagnostic_keep_running->setChecked(configurator.GetUseSystemTray());
    this->ui->diagnostic_keep_running->blockSignals(false);

    this->ui->diagnostic_vk_home_text->blockSignals(true);
    this->ui->diagnostic_vk_home_text->setText(::Get(Path::HOME).RelativePath().c_str());
    this->ui->diagnostic_vk_home_text->setToolTip(::Get(Path::HOME).AbsolutePath().c_str());
    this->ui->diagnostic_vk_home_text->blockSignals(false);

    this->ui->diagnostic_status_text->document()->setMaximumBlockCount(65536);

    this->widget_refresh = new ResizeButton(this->ui->diagnostic_group_box_refresh, 0);
    this->widget_refresh->setMinimumSize(24, 24);
    this->widget_refresh->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->widget_refresh->adjustSize();
    this->ui->diagnostic_group_box_refresh->installEventFilter(this->widget_refresh);
    this->connect(this->widget_refresh, SIGNAL(clicked()), this, SLOT(on_diagnostic_refresh_pressed()));

    this->widget_export = new ResizeButton(this->ui->diagnostic_group_box_refresh, 1);
    this->widget_export->setMinimumSize(24, 24);
    this->widget_export->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->widget_export->setIcon(QIcon(":/resourcefiles/file_export.png"));
    this->widget_export->setToolTip("Saving the 'Vulkan Development Status' to a file...");
    this->widget_export->adjustSize();
    this->ui->diagnostic_group_box_refresh->installEventFilter(this->widget_export);
    this->connect(this->widget_export, SIGNAL(clicked()), this, SLOT(on_diagnostic_export_pressed()));

    this->widget_reset_hard = new ResizeButton(this->ui->diagnostic_group_box_settings, 0);
    this->widget_reset_hard->setMinimumSize(24, 24);
    this->widget_reset_hard->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->widget_reset_hard->setIcon(QIcon(":/resourcefiles/reset.png"));
    this->widget_reset_hard->setToolTip("Resetting Vulkan Configurator to default...");
    this->widget_reset_hard->adjustSize();
    this->ui->diagnostic_group_box_settings->installEventFilter(this->widget_reset_hard);
    this->connect(this->widget_reset_hard, SIGNAL(clicked()), this, SLOT(on_diagnostic_reset_hard_pressed()));

    this->on_diagnostic_refresh_pressed();
}

TabDiagnostics::~TabDiagnostics() {}

void TabDiagnostics::UpdateUI(UpdateUIMode mode) {
    (void)mode;

    this->status.clear();
    this->on_diagnostic_refresh_pressed();
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
        message.setInformativeText(format("Restoring the previous path '%s'.", ::Get(Path::HOME).AbsolutePath().c_str()).c_str());
        message.exec();

        this->ui->diagnostic_vk_home_text->setText(::Get(Path::HOME).AbsolutePath().c_str());
    }
}

void TabDiagnostics::on_diagnostic_vk_home_browse_pressed() {
    Configurator &configurator = Configurator::Get();
    const QString selected_path = QFileDialog::getExistingDirectory(
        this->ui->diagnostic_vk_home_browse, "Select the Vulkan Home Default Working Folder (Set ${VK_HOME} value)...",
        ::Get(Path::HOME).AbsolutePath().c_str());

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
        Configurator &configurator = Configurator::Get();
        configurator.Reset(true);
    }
}

void TabDiagnostics::on_diagnostic_refresh_pressed() {
    Configurator &configurator = Configurator::Get();

    if (this->status.empty()) {
        configurator.Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

        this->status = configurator.GenerateVulkanStatus();
        this->ui->diagnostic_status_text->setText(this->status.c_str());
        this->widget_refresh->setIcon(QIcon(":/resourcefiles/clear.png"));
        this->widget_refresh->setToolTip("Clear 'Vulkan Development Status'");

        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
    } else {
        this->status.clear();
        this->ui->diagnostic_status_text->clear();
        this->widget_refresh->setIcon(QIcon(":/resourcefiles/reload.png"));
        this->widget_refresh->setToolTip("Refresh 'Vulkan Development Status'");
    }
}

void TabDiagnostics::on_diagnostic_export_pressed() {
    Configurator &configurator = Configurator::Get();

    const QString selected_path = QFileDialog::getSaveFileName(this->ui->diagnostic_group_box_refresh, "Select Log file...",
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

        this->status.clear();
        this->on_diagnostic_refresh_pressed();
        file.write(this->status.c_str());
        file.close();

        configurator.last_path_status = selected_path.toStdString();
    }
}
