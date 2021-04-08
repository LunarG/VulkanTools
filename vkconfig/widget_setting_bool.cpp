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

#include <cassert>

WidgetSettingBool::WidgetSettingBool(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaBool& setting_meta,
                                     SettingDataBool& setting_data)
    : tree(tree), item(item), meta(setting_meta), data(setting_data) {
    assert(&setting_meta);
    assert(&setting_data);

    this->setText(setting_meta.label.c_str());
    this->setFont(tree->font());
    this->setToolTip(setting_meta.description.c_str());
    this->setChecked(setting_data.value);

    this->connect(this, SIGNAL(clicked()), this, SLOT(OnClicked()));

    tree->setItemWidget(item, 0, this);
    item->setExpanded(true);

    this->UpdateEnable(item, this->data.value);
}

void WidgetSettingBool::OnClicked() {
    this->data.value = isChecked();
    this->UpdateEnable(this->item, this->data.value);

    emit itemChanged();
}

void WidgetSettingBool::UpdateEnable(QTreeWidgetItem* parent, bool enabled) {
    assert(parent != nullptr);

    for (int i = 0, n = parent->childCount(); i < n; ++i) {
        QTreeWidgetItem* child = parent->child(i);
        QWidget* widget = tree->itemWidget(child, 0);
        if (widget != nullptr) {
            widget->setEnabled(enabled);
        }

        this->UpdateEnable(child, enabled);
        child->setDisabled(!enabled);
    }
}
