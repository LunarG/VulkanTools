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

#include "widget_enum_setting.h"

#include <cassert>

EnumSettingWidget::EnumSettingWidget(QTreeWidgetItem* item, const LayerSettingMeta& setting_meta, LayerSettingData& setting_data)
    : setting_meta(setting_meta), setting_data(setting_data) {
    assert(item);
    assert(&setting_data);

    item->setText(0, setting_meta.label.c_str());
    item->setToolTip(0, setting_meta.description.c_str());

    int selection = 0;
    for (int i = 0; i < setting_meta.enum_labels.size(); i++) {
        this->addItem(setting_meta.enum_labels[i]);
        if (setting_meta.enum_values[i] == setting_data.value.c_str()) {
            selection = i;
        }
    }

    setCurrentIndex(selection);

    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
}

void EnumSettingWidget::indexChanged(int index) {
    setting_data.value = setting_meta.enum_values[index].toStdString();
    emit itemChanged();
}
