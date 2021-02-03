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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "widget_setting_int.h"

#include <cassert>

WidgetSettingInt::WidgetSettingInt(QTreeWidgetItem* item, const SettingMetaInt& setting_meta, SettingDataInt& setting_data)
    : setting_meta(setting_meta), setting_data(setting_data) {
    assert(item);
    assert(&setting_meta);
    assert(&setting_data);

    item->setText(0, setting_meta.label.c_str());
    item->setToolTip(0, setting_meta.description.c_str());
    this->setText(format("%d", setting_data.value).c_str());
    connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(itemEdited(const QString&)));
}

void WidgetSettingInt::itemEdited(const QString& new_string) {
    if (new_string.isEmpty()) {
        this->setting_data.value = this->setting_meta.default_value;
    } else {
        this->setting_data.value = std::atoi(new_string.toStdString().c_str());
    }
    emit itemChanged();
}
