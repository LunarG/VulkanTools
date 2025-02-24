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

#include "tab_preferences.h"
#include "style.h"

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>
#include <QMessageBox>

TabPreferences::TabPreferences(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_DIAGNOSTIC, window, ui) {
    this->ui->settings_reset->setIcon(::Get(::ICON_RESET));
    this->ui->settings_vk_home_browse->setIcon(::Get(::ICON_FOLDER_SEARCH));

    this->connect(this->ui->settings_keep_running, SIGNAL(toggled(bool)), this, SLOT(on_keep_running_toggled(bool)));
    this->connect(this->ui->settings_vk_home_text, SIGNAL(returnPressed()), this, SLOT(on_vk_home_text_pressed()));
    this->connect(this->ui->settings_vk_home_browse, SIGNAL(clicked()), this, SLOT(on_vk_home_browse_pressed()));
    this->connect(this->ui->settings_reset, SIGNAL(clicked()), this, SLOT(on_reset_hard_pressed()));
    this->connect(this->ui->settings_layer_dev_mode, SIGNAL(toggled(bool)), this, SLOT(on_layer_dev_mode_toggled(bool)));
}

TabPreferences::~TabPreferences() {}

void TabPreferences::UpdateUI(UpdateUIMode mode) {
    (void)mode;

    Configurator &configurator = Configurator::Get();

    this->ui->settings_keep_running->blockSignals(true);
    this->ui->settings_keep_running->setChecked(configurator.GetUseSystemTray());
    this->ui->settings_keep_running->blockSignals(false);

    this->ui->settings_vk_home_text->blockSignals(true);
    this->ui->settings_vk_home_text->setText(::Path(Path::HOME).RelativePath().c_str());
    this->ui->settings_vk_home_text->setToolTip(::Path(Path::HOME).AbsolutePath().c_str());
    this->ui->settings_vk_home_text->blockSignals(false);

    this->ui->settings_layer_dev_mode->blockSignals(true);
    this->ui->settings_layer_dev_mode->setChecked(configurator.GetUseLayerDevMode());
    this->ui->settings_layer_dev_mode->blockSignals(false);
}

void TabPreferences::CleanUI() {}

bool TabPreferences::EventFilter(QObject *target, QEvent *event) {
    (void)target;
    (void)event;

    return false;
}

void TabPreferences::on_keep_running_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();
    configurator.SetUseSystemTray(checked);
}

void TabPreferences::on_vk_home_text_pressed() {
    Path path(this->ui->settings_vk_home_text->text().toStdString());
    if (path.Exists()) {
        ::SetHomePath(this->ui->settings_vk_home_text->text().toStdString());
    } else {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setWindowTitle("Invalid ${VK_HOME} path...");
        message.setText(
            format("'%s' is not a valid, it doesn't exist.", this->ui->settings_vk_home_text->text().toStdString().c_str())
                .c_str());
        message.setInformativeText(format("Restoring the previous path '%s'.", ::Path(Path::HOME).AbsolutePath().c_str()).c_str());
        message.exec();

        this->ui->settings_vk_home_text->setText(::Path(Path::HOME).AbsolutePath().c_str());
    }
}

void TabPreferences::on_vk_home_browse_pressed() {
    const QString selected_path = QFileDialog::getExistingDirectory(
        this->ui->settings_vk_home_browse, "Select the Vulkan Home Default Working Folder (Set ${VK_HOME} value)...",
        ::Path(Path::HOME).AbsolutePath().c_str());

    if (!selected_path.isEmpty()) {
        this->ui->settings_vk_home_text->setText(selected_path);

        ::SetHomePath(selected_path.toStdString());
    }
}

void TabPreferences::on_reset_hard_pressed() {
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

void TabPreferences::on_layer_dev_mode_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();
    configurator.SetUseLayerDevMode(checked);
}
