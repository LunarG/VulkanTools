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

#include "widget_setting_flags.h"
#include "widget_setting.h"

#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/util.h"

#include <QPushButton>

#include <cassert>

WidgetSettingFlag::WidgetSettingFlag(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaFlags& meta,
                                     SettingDataSet& data_set, const std::string& flag)
    : WidgetSettingBase(tree, item), meta(meta), data_set(data_set), flag(flag), field(new QCheckBox(this)) {
    assert(!flag.empty());

    const SettingEnumValue* enum_value = FindByKey(meta.enum_values, flag.c_str());
    assert(enum_value);

    const std::string status = meta.status == STATUS_STABLE ? "" : std::string(" (") + GetToken(enum_value->status) + ")";

    this->field->setText((enum_value->label + status).c_str());
    this->field->setToolTip(enum_value->description.c_str());
    this->field->setFont(tree->font());
    this->field->show();
    this->connect(this->field, SIGNAL(clicked(bool)), this, SLOT(OnClicked(bool)));

    this->item->setExpanded(enum_value->expanded);
    this->item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    this->tree->setItemWidget(this->item, 0, this);

    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

void WidgetSettingFlag::Refresh(RefreshAreas refresh_areas) {
    const SettingDependenceMode enabled = ::CheckDependence(this->meta, data_set);

    this->item->setHidden(enabled == SETTING_DEPENDENCE_HIDE);
    this->item->setDisabled(enabled != SETTING_DEPENDENCE_ENABLE);
    this->field->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);
    this->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);

    if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
        if (::CheckSettingOverridden(this->meta)) {
            this->DisplayOverride(this->field, this->meta);
        }

        const std::vector<std::string>& value = this->data().value;

        this->field->blockSignals(true);
        this->field->setChecked(std::find(value.begin(), value.end(), flag) != value.end());
        this->field->blockSignals(false);
    }
}

void WidgetSettingFlag::OnClicked(bool checked) {
    if (checked) {
        AppendString(this->data().value, flag);
    } else {
        RemoveString(this->data().value, flag);
    }

    emit itemChanged();
}

SettingDataFlags& WidgetSettingFlag::data() {
    SettingDataFlags* data = FindSetting<SettingDataFlags>(this->data_set, this->meta.key.c_str());
    assert(data != nullptr);
    return *data;
}
