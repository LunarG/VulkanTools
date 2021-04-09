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
#include "widget_setting.h"

#include <cassert>

static const int MIN_FIELD_WIDTH = 80;

WidgetSettingEnum::WidgetSettingEnum(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaEnum& meta, SettingDataEnum& data)
    : tree(tree), item(item), meta(meta), data(data), field(new QComboBox(this)) {
    assert(item);
    assert(&meta);
    assert(&data);

    item->setText(0, meta.label.c_str());
    item->setFont(0, tree->font());
    item->setToolTip(0, meta.description.c_str());
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));

    int selection = 0;
    for (std::size_t i = 0, n = meta.enum_values.size(); i < n; ++i) {
        if (!IsPlatformSupported(meta.enum_values[i].platform_flags)) continue;
        if (meta.enum_values[i].view == SETTING_VIEW_HIDDEN) continue;

        this->field->addItem(meta.enum_values[i].label.c_str());
        if (meta.enum_values[i].key == data.value) {
            selection = static_cast<int>(this->enum_indexes.size());
        }
        this->enum_indexes.push_back(i);
    }

    this->field->setCurrentIndex(selection);
    this->field->show();

    this->connect(this->field, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));

    tree->setItemWidget(item, 0, this);
}

void WidgetSettingEnum::resizeEvent(QResizeEvent* event) {
    int width = MIN_FIELD_WIDTH;

    const QFontMetrics fm = this->field->fontMetrics();
    for (std::size_t i = 0, n = this->meta.enum_values.size(); i < n; ++i) {
        width = std::max(width, HorizontalAdvance(fm, (this->meta.enum_values[i].label + "0000").c_str()));
    }

    const QRect button_rect = QRect(event->size().width() - width, 0, width, event->size().height());
    this->field->setGeometry(button_rect);
}

void WidgetSettingEnum::indexChanged(int index) {
    assert(index >= 0 && index < static_cast<int>(this->meta.enum_values.size()));

    this->data.value = this->meta.enum_values[enum_indexes[static_cast<std::size_t>(index)]].key;
    emit itemChanged();
}
