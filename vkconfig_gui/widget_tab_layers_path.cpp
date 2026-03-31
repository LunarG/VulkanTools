/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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

#include "widget_tab_layers_path.h"
#include "style.h"

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>
#include <QMessageBox>

LayerWidget::LayerWidget(const Layer& layer) : layer_id(layer.GetId()) {
    const Configurator& configurator = Configurator::Get();

    const std::string status = layer.status == STATUS_STABLE ? "" : format(" (%s)", ::GetToken(layer.status));
    const std::string text = format("%s - %s%s - %s layer", layer.key.c_str(), layer.api_version.str().c_str(), status.c_str(),
                                    ::ToLowerCase(::GetToken(layer.type)).c_str());

    this->setText(text.c_str());
    this->setToolTip(format("%s", layer.manifest_path.AbsolutePath().c_str()).c_str());
    this->setChecked(layer.descriptor.enabled);

    this->buttom_remove = new QPushButton(this);
    this->buttom_remove->setIcon(::Get(configurator.current_theme_mode, ::ICON_FOLDER_REMOVE));
    this->buttom_remove->setToolTip("Remove the Vulkan layer from Vulkan Configurator");
    this->buttom_remove->setFixedSize(24, 24);

    if (layer.descriptor.recent) {
        QPalette palette = this->palette();
        palette.setColor(QPalette::Active, QPalette::WindowText, QColor(255, 0, 0));
        this->setPalette(palette);
    }

    this->connect(this, SIGNAL(toggled(bool)), this, SLOT(on_toggled(bool)));
    this->connect(this->buttom_remove, SIGNAL(clicked(bool)), this, SLOT(on_buttom_remove_clicked(bool)));
}

void LayerWidget::resizeEvent(QResizeEvent* event) {
    QSize size = event->size();

    const QFontMetrics fm = this->buttom_remove->fontMetrics();
    const int button_width_state = this->buttom_remove->width();

    const QRect remove_button_rect = QRect(size.width() - button_width_state, 0, button_width_state, size.height());
    this->buttom_remove->setGeometry(remove_button_rect);
}

void LayerWidget::on_buttom_remove_clicked(bool checked) {
    (void)checked;

    Configurator& configurator = Configurator::Get();

    if (!(configurator.Get(HIDE_MESSAGE_QUESTION_REMOVING_LAYERS_PATH))) {
        QMessageBox alert;
        alert.setWindowTitle(
            format("Removing %s %s layer...", this->layer_id.key.c_str(), this->layer_id.api_version.str().c_str()).c_str());
        alert.setIcon(QMessageBox::Question);
        alert.setDefaultButton(QMessageBox::No);
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setCheckBox(new QCheckBox("Do not show again."));
        alert.setText(format("Are you sure you want to remove %s %s from %s?", this->layer_id.key.c_str(),
                             this->layer_id.api_version.str().c_str(), VKCONFIG_NAME)
                          .c_str());
        alert.setInformativeText(format("The layer is located: %s", this->layer_id.manifest_path.AbsolutePath().c_str()).c_str());
        int ret_val = alert.exec();
        if (alert.checkBox()->isChecked()) {
            configurator.Set(HIDE_MESSAGE_QUESTION_REMOVING_LAYERS_PATH);
        }
        if (ret_val == QMessageBox::No) {
            return;
        }
    }

    configurator.layers.RemoveLayer(this->layer_id);

    emit itemChanged();
}

void LayerWidget::on_toggled(bool checked) {
    Configurator& configurator = Configurator::Get();
    configurator.layers.EnableLayer(this->layer_id, checked);

    emit itemToggled();
}
