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

#include "widget_tab_driver_path.h"
#include "style.h"

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>
#include <QMessageBox>

DriverPathWidget::DriverPathWidget(const Path& path, bool enabled) : path(path), enabled(enabled) {
    this->setChecked(enabled);

    const Configurator& configurator = Configurator::Get();

    this->buttom_remove = new QPushButton(this);
    this->buttom_remove->setIcon(::Get(configurator.current_theme_mode, ::ICON_FOLDER_REMOVE));
    this->buttom_remove->setFixedSize(24, 24);

    this->setText(format("%s", this->path.RelativePath().c_str()).c_str());
    this->setToolTip(format("%s", this->path.AbsolutePath().c_str()).c_str());

    this->connect(this, SIGNAL(toggled(bool)), this, SLOT(on_toggled(bool)));
    this->connect(this->buttom_remove, SIGNAL(clicked(bool)), this, SLOT(on_buttom_remove_clicked(bool)));
}

void DriverPathWidget::resizeEvent(QResizeEvent* event) {
    QSize size = event->size();

    const QFontMetrics fm = this->buttom_remove->fontMetrics();
    const int button_width_state = this->buttom_remove->width();

    const QRect remove_button_rect = QRect(size.width() - button_width_state, 0, button_width_state, size.height());
    this->buttom_remove->setGeometry(remove_button_rect);
}

void DriverPathWidget::on_buttom_remove_clicked(bool checked) {
    (void)checked;

    Configurator& configurator = Configurator::Get();

    if (!(configurator.Get(HIDE_MESSAGE_QUESTION_REMOVING_LAYERS_PATH))) {
        QMessageBox alert;
        alert.setWindowTitle("Removing a driver path...");
        alert.setIcon(QMessageBox::Question);
        alert.setDefaultButton(QMessageBox::No);
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setCheckBox(new QCheckBox("Do not show again."));
        alert.setText(format("Are you sure you want to remove the following path from %s?", VKCONFIG_NAME).c_str());
        alert.setInformativeText(this->path.AbsolutePath().c_str());
        int ret_val = alert.exec();
        if (alert.checkBox()->isChecked()) {
            configurator.Set(HIDE_MESSAGE_QUESTION_REMOVING_LAYERS_PATH);
        }
        if (ret_val == QMessageBox::No) {
            return;
        }
    }

    configurator.driver_paths.erase(this->path);
    if (std::find(configurator.driver_paths_removed.begin(), configurator.driver_paths_removed.end(), this->path) ==
        configurator.driver_paths_removed.end()) {
        configurator.driver_paths_removed.push_back(this->path);
    }

    emit itemChanged();
}

void DriverPathWidget::on_toggled(bool checked) {
    Configurator& configurator = Configurator::Get();
    auto it = configurator.driver_paths.find(this->path);

    if (it != configurator.driver_paths.end()) {
        it->second = checked;

        emit itemToggled();
    }
}
