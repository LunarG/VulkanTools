/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

#include "widget_setting_list.h"
#include "widget_setting.h"

#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/util.h"

#include <cassert>

WidgetSettingList::WidgetSettingList(const SettingMetaList &setting_meta, SettingDataList &setting_data)
    : QWidget(nullptr), setting_meta(setting_meta), setting_data(setting_data), _list_widget(new QListWidget(this)) {
    assert(&setting_meta);
    assert(&setting_data);

    _list_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _list_widget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _list_widget->show();

    // Load with existing settings
    if (!this->setting_data.values.empty()) {
        QStringList list;
        for (std::size_t i = 0, n = this->setting_data.values.size(); i < n; ++i) {
            list.append(this->setting_data.values[i].key.c_str());
        }

        _list_widget->addItems(list);
        _list_widget->setCurrentRow(_list_widget->count() - 1);
    }
}

void WidgetSettingList::resizeEvent(QResizeEvent *event) {
    assert(event);

    const int button_height = ITEM_HEIGHT;
    QSize parent_size = event->size();
    _list_widget->setGeometry(0, 0, parent_size.width(), parent_size.height() - button_height);
}

void WidgetSettingList::addItem(const QString &item) {
    _list_widget->addItem(item);
    _list_widget->setCurrentRow(_list_widget->count() - 1);

    // Update Setting
    // AppendString(this->setting_data.value, item.toStdString());
    emit itemChanged();
}

void WidgetSettingList::removePushed() {
    const int row = _list_widget->currentRow();
    if (row < 0) return;

    const QString &item_name = _list_widget->currentItem()->text();
    _list_widget->takeItem(row);

    // Update Setting
    // RemoveString(this->setting_data.value, item_name.toStdString());
    emit itemChanged();
    emit itemRemoved(item_name);
}
