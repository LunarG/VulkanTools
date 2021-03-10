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

#include "widget_setting.h"

WidgetSetting::WidgetSetting(QTreeWidget* tree, QTreeWidgetItem* parent, const SettingMeta& setting_meta)
    : tree(tree), item(new QTreeWidgetItem()) {
    assert(tree);
    assert(parent);
    assert(&setting_meta);

    this->item->setText(0, setting_meta.label.c_str());
    this->item->setToolTip(0, setting_meta.description.c_str());
    this->item->setSizeHint(0, QSize(0, 24));
    this->item->setFont(0, tree->font());

    parent->addChild(this->item);

    tree->setItemWidget(this->item, 0, this);
}

void EnableItem(QTreeWidgetItem* item, bool enable) {
    if (item == nullptr) return;

    if (enable) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    } else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
}
