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

#include "widget_setting_enum.h"

#include <cassert>

WidgetSettingEnum::WidgetSettingEnum(QTreeWidgetItem* item, const SettingMetaEnum& setting_meta, SettingDataEnum& setting_data)
    : setting_meta(setting_meta), setting_data(setting_data) {
    assert(item);
    assert(&setting_data);

    item->setText(0, setting_meta.label.c_str());
    item->setToolTip(0, setting_meta.description.c_str());

    int selection = 0;
    for (std::size_t i = 0, n = setting_meta.enum_values.size(); i < n; ++i) {
        if (!IsPlatformSupported(setting_meta.enum_values[i].platform_flags)) continue;
        if (setting_meta.enum_values[i].view == SETTING_VIEW_HIDDEN) continue;

        this->addItem(setting_meta.enum_values[i].label.c_str());
        if (setting_meta.enum_values[i].key == setting_data.value) {
            selection = static_cast<int>(i);
        }
    }

    setCurrentIndex(selection);

    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
}

void WidgetSettingEnum::indexChanged(int index) {
    assert(index >= 0 && index < static_cast<int>(setting_meta.enum_values.size()));

    setting_data.value = setting_meta.enum_values[index].key;
    emit itemChanged();
}
