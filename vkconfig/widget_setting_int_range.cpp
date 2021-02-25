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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "widget_setting_int_range.h"

#include <cassert>

WidgetSettingIntRange::WidgetSettingIntRange(QTreeWidgetItem* item, const SettingMetaIntRange& setting_meta,
                                             SettingDataIntRange& setting_data)
    : setting_meta(setting_meta), setting_data(setting_data) {
    assert(item);
    assert(&setting_meta);
    assert(&setting_data);

    item->setText(0, setting_meta.label.c_str());
    item->setToolTip(0, setting_meta.description.c_str());
    if (setting_data.min_value < setting_data.max_value) {
        this->setText(format("%d-%d", setting_data.min_value, setting_data.max_value).c_str());
    }
    connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(itemEdited(const QString&)));
}

void WidgetSettingIntRange::itemEdited(const QString& value) {
    if (value.isEmpty()) {
        this->setting_data.min_value = this->setting_meta.default_min_value;
        this->setting_data.max_value = this->setting_meta.default_max_value;
    } else {
        std::sscanf(value.toStdString().c_str(), "%d-%d", &this->setting_data.min_value, &this->setting_data.max_value);
    }
    emit itemChanged();
}
