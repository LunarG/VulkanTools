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

#include "widget_tab_layers_path.h"

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>

LayersPathWidget::LayersPathWidget(LayersPathInfo* path_info) : path_info(path_info) {
    this->setChecked(this->path_info->enabled);

    this->buttom_remove = new QPushButton(this);
    this->buttom_remove->setText("-");
    this->buttom_remove->show();

    this->setText(path_info->path.RelativePath().c_str());
    this->setToolTip("Load layers in this location");

    this->connect(this, SIGNAL(toggled(bool)), this, SLOT(on_toggled(bool)));
    this->connect(this->buttom_remove, SIGNAL(clicked(bool)), this, SLOT(on_buttom_remove_clicked(bool)));
}

void LayersPathWidget::resizeEvent(QResizeEvent* event) {
    QSize size = event->size();

    const QFontMetrics fm = this->buttom_remove->fontMetrics();
    const int button_width_state = 30;

    const QRect remove_button_rect = QRect(size.width() - button_width_state, 0, button_width_state, size.height());
    this->buttom_remove->setGeometry(remove_button_rect);
}

void LayersPathWidget::on_buttom_remove_clicked(bool checked) {
    Configurator& configurator = Configurator::Get();
    configurator.layers.RemovePath(*this->path_info);

    emit toggled(checked);
}

void LayersPathWidget::on_toggled(bool checked) {
    this->path_info->enabled = checked;

    Configurator& configurator = Configurator::Get();
    configurator.layers.UpdatePath(*this->path_info);
}
