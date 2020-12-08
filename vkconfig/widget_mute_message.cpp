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

#include "widget_mute_message.h"

#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/util.h"

#include <cassert>

MuteMessageWidget::MuteMessageWidget(LayerSettingData &layer_setting_data)
    : QWidget(nullptr), layer_setting_data(layer_setting_data) {
    assert(&layer_setting_data);

    _list_widget = new QListWidget(this);
    _list_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _list_widget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _list_widget->show();
    _remove_button = new QPushButton(this);
    _remove_button->setText("Remove");
    _remove_button->show();

    // Load with existing settings
    if (!layer_setting_data.value.empty()) {
        const QStringList list = QString(layer_setting_data.value.c_str()).split(",");
        _list_widget->addItems(list);
        _list_widget->setCurrentRow(_list_widget->count() - 1);
    } else
        _remove_button->setEnabled(false);

    connect(_remove_button, SIGNAL(pressed()), this, SLOT(removePushed()));
}

void MuteMessageWidget::resizeEvent(QResizeEvent *event) {
    assert(event);

    const int button_height = 26;
    QSize parent_size = event->size();
    _list_widget->setGeometry(0, 0, parent_size.width(), parent_size.height() - button_height);
    _remove_button->setGeometry(0, parent_size.height() - button_height, parent_size.width(), button_height);
}

void MuteMessageWidget::addItem(const QString &item) {
    _list_widget->addItem(item);
    _list_widget->setCurrentRow(_list_widget->count() - 1);

    // Update Setting
    AppendString(layer_setting_data.value, item.toStdString());
    _remove_button->setEnabled(true);
    emit itemChanged();
}

void MuteMessageWidget::removePushed() {
    const int row = _list_widget->currentRow();
    if (row < 0) return;

    const QString &item_name = _list_widget->currentItem()->text();
    _list_widget->takeItem(row);

    // Update Setting
    RemoveString(layer_setting_data.value, item_name.toStdString());
    emit itemChanged();
    emit itemRemoved(item_name);
}
