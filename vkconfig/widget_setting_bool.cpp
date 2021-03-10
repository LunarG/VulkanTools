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

#include "widget_setting_bool.h"

#include <QCheckBox>

#include <cassert>

WidgetSettingBool::WidgetSettingBool(QTreeWidgetItem* item, const SettingMetaBool& setting_meta, SettingDataBool& setting_data)
    : setting_meta(setting_meta), setting_data(setting_data) {
    assert(&setting_meta);
    assert(&setting_data);

    item->setText(0, setting_meta.label.c_str());
    item->setToolTip(0, setting_meta.description.c_str());

    this->field = new QCheckBox(this);
    this->field->setChecked(setting_data.value);
    this->field->show();

    this->connect(this->field, SIGNAL(clicked()), this, SLOT(itemToggled()));
}

void WidgetSettingBool::resizeEvent(QResizeEvent* event) {
    if (this->field == nullptr) return;

    const QRect button_rect = QRect(event->size().width() - 16, 0, 16, event->size().height());
    this->field->setGeometry(button_rect);
}

void WidgetSettingBool::itemToggled() {
    this->setting_data.value = this->field->isChecked();

    emit itemChanged();
}
