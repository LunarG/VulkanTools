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

WidgetSettingBool::WidgetSettingBool(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaBool& meta,
                                     SettingDataSet& data_set)
    : WidgetSettingBase(tree, item), meta(meta), data_set(data_set), field(new QCheckBox(this)) {
    assert(&this->meta);

    this->field->setText(GetLabel(this->meta).c_str());
    this->field->setFont(this->tree->font());
    this->field->setToolTip(this->meta.description.c_str());
    this->field->show();
    this->connect(this->field, SIGNAL(clicked()), this, SLOT(OnClicked()));

    this->item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    this->item->setExpanded(this->meta.expanded);
    this->tree->setItemWidget(this->item, 0, this);

    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

void WidgetSettingBool::Refresh(RefreshAreas refresh_areas) {
    const SettingDependenceMode enabled = ::CheckDependence(this->meta, this->data_set);

    this->item->setHidden(enabled == SETTING_DEPENDENCE_HIDE);
    this->item->setDisabled(enabled != SETTING_DEPENDENCE_ENABLE);
    this->field->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);
    this->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);

    if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
        if (::CheckSettingOverridden(this->meta)) {
            this->DisplayOverride(this->field, this->meta);
        }

        this->field->blockSignals(true);
        this->field->setChecked(this->data().value);
        this->field->blockSignals(false);
    }
}

void WidgetSettingBool::OnClicked() {
    this->data().value = this->field->isChecked();

    emit itemChanged();
}

SettingDataBool& WidgetSettingBool::data() {
    SettingDataBool* data = FindSetting<SettingDataBool>(this->data_set, this->meta.key.c_str());
    assert(data != nullptr);
    return *data;
}
