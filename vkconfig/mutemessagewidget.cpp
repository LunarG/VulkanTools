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

#include "mutemessagewidget.h"

MuteMessageWidget::MuteMessageWidget(LayerSettings *layer_settings) : QWidget(nullptr) {
    _layer_settings = layer_settings;
    _list_widget = new QListWidget(this);
    _list_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _list_widget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _list_widget->show();
    _remove_button = new QPushButton(this);
    _remove_button->setText("Remove");
    _remove_button->show();

    // Load with existing settings
    if (!_layer_settings->settings_value.isEmpty()) {
        QStringList list = _layer_settings->settings_value.split(",");
        _list_widget->addItems(list);
        _list_widget->setCurrentRow(_list_widget->count() - 1);
    } else
        _remove_button->setEnabled(false);

    connect(_remove_button, SIGNAL(pressed()), this, SLOT(removePushed()));
}

void MuteMessageWidget::resizeEvent(QResizeEvent *event) {
    int nButtonHeight = 26;
    QSize parentSize = event->size();
    _list_widget->setGeometry(0, 0, parentSize.width(), parentSize.height() - nButtonHeight);
    _remove_button->setGeometry(0, parentSize.height() - nButtonHeight, parentSize.width(), nButtonHeight);
}

void MuteMessageWidget::addItem(const QString &item) {
    _list_widget->addItem(item);
    _list_widget->setCurrentRow(_list_widget->count() - 1);

    // Update Setting
    AddString(_layer_settings->settings_value, item);
    _remove_button->setEnabled(true);
    emit itemChanged();
}

void MuteMessageWidget::removePushed() {
    int nRow = _list_widget->currentRow();
    if (nRow < 0) return;

    QString itemName = _list_widget->currentItem()->text();
    _list_widget->takeItem(nRow);

    // Update Setting
    RemoveString(_layer_settings->settings_value, itemName);
    emit itemChanged();
    emit itemRemoved(itemName);
}
