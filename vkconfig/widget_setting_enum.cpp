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
#include "configurator.h"

#include "../vkconfig_core/setting_filesystem.h"

#include <cassert>

static const int MIN_FIELD_WIDTH = 80;

static const std::vector<std::string>& GetProfileNames(SettingDataSet& data_set) {
    SettingDataFileLoad* data_file_load = static_cast<SettingDataFileLoad*>(FindSetting(data_set, "profile_file"));

    return data_file_load->profile_names;
}

WidgetSettingEnum::WidgetSettingEnum(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaEnum& meta,
                                     SettingDataSet& data_set)
    : WidgetSettingBase(tree, item), meta(meta), data_set(data_set), field(new ComboBox(this)) {
    assert(&meta);

    this->field->setFocusPolicy(Qt::StrongFocus);
    this->field->show();

    this->item->setText(0, GetLabel(this->meta).c_str());
    this->item->setFont(0, this->tree->font());
    this->item->setToolTip(0, this->meta.description.c_str());
    this->item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    this->item->setExpanded(this->meta.expanded);
    this->tree->setItemWidget(this->item, 0, this);

    this->Refresh(REFRESH_ENABLE_AND_STATE);

    this->connect(this->field, SIGNAL(currentIndexChanged(int)), this, SLOT(OnIndexChanged(int)));
}

void WidgetSettingEnum::Refresh(RefreshAreas refresh_areas) {
    const SettingDependenceMode enabled = ::CheckDependence(this->meta, data_set);

    this->item->setHidden(enabled == SETTING_DEPENDENCE_HIDE);
    this->item->setDisabled(enabled != SETTING_DEPENDENCE_ENABLE);
    this->field->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);
    this->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);

    if (meta.default_value == "${VP_DEFAULT}") {
        if (::CheckSettingOverridden(this->meta)) {
            this->DisplayOverride(this->field, this->meta);
        }

        this->field->blockSignals(true);
        this->field->clear();
        this->enum_indexes.clear();

        const std::vector<std::string>& profiles = GetProfileNames(data_set);
        this->item->setHidden(profiles.size() <= 1 || enabled == SETTING_DEPENDENCE_HIDE);

        int selection = 0;
        const std::string value = this->data().value;
        for (std::size_t i = 0, n = profiles.size(); i < n; ++i) {
            this->field->addItem(profiles[i].c_str());
            if (profiles[i] == value) {
                selection = static_cast<int>(this->enum_indexes.size());
            }
            this->enum_indexes.push_back(i);
        }
        this->field->setCurrentIndex(selection);
        this->field->blockSignals(false);
    } else if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
        if (::CheckSettingOverridden(this->meta)) {
            this->DisplayOverride(this->field, this->meta);
        }

        this->field->blockSignals(true);
        this->field->clear();
        this->enum_indexes.clear();

        int selection = 0;
        const std::string value = this->data().value;

        for (std::size_t i = 0, n = this->meta.enum_values.size(); i < n; ++i) {
            if (!IsSupported(&this->meta.enum_values[i])) continue;

            this->field->addItem(this->meta.enum_values[i].label.c_str());
            if (this->meta.enum_values[i].key == value) {
                selection = static_cast<int>(this->enum_indexes.size());
            }
            this->enum_indexes.push_back(i);
        }
        this->field->setCurrentIndex(selection);
        this->field->blockSignals(false);
    }
}

void WidgetSettingEnum::resizeEvent(QResizeEvent* event) {
    int width = MIN_FIELD_WIDTH;

    const QFontMetrics fm = this->field->fontMetrics();

    if (meta.default_value == "${VP_DEFAULT}") {
        const std::vector<std::string>& profiles = GetProfileNames(data_set);
        for (std::size_t i = 0, n = profiles.size(); i < n; ++i) {
            width = std::max(width, HorizontalAdvance(fm, (profiles[i] + "0000").c_str()));
        }
        this->item->setHidden(profiles.size() <= 1);
    } else {
        for (std::size_t i = 0, n = this->meta.enum_values.size(); i < n; ++i) {
            width = std::max(width, HorizontalAdvance(fm, (this->meta.enum_values[i].label + "0000").c_str()));
        }
    }

    const QRect button_rect = QRect(event->size().width() - width, 0, width, event->size().height());
    this->field->setGeometry(button_rect);
}

void WidgetSettingEnum::OnIndexChanged(int index) {
    if (meta.default_value == "${VP_DEFAULT}") {
        const std::vector<std::string>& profiles = GetProfileNames(data_set);
        assert(index >= 0 && index < static_cast<int>(profiles.size()));

        this->data().value = profiles[index];
        this->item->setHidden(profiles.size() <= 1);
    } else {
        assert(index >= 0 && index < static_cast<int>(this->meta.enum_values.size()));

        const std::size_t value_index = enum_indexes[static_cast<std::size_t>(index)];
        this->data().value = this->meta.enum_values[value_index].key;
        this->field->setToolTip(this->meta.enum_values[value_index].description.c_str());
    }

    emit itemChanged();
}

SettingDataEnum& WidgetSettingEnum::data() {
    SettingDataEnum* data = FindSetting<SettingDataEnum>(this->data_set, this->meta.key.c_str());
    assert(data != nullptr);
    return *data;
}
