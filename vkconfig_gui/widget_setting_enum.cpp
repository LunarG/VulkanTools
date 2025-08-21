/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/setting_filesystem.h"

#include <cassert>

static const int MIN_FIELD_WIDTH = 80;

static std::vector<std::string> GetProfileNames(SettingDataSet& data_set) {
    std::vector<std::string> profiles_name;

    SettingDataFileLoad* data_file_load = static_cast<SettingDataFileLoad*>(FindSetting(data_set, "profile_file"));
    if (data_file_load != nullptr) {
        profiles_name = data_file_load->profile_names;
    }

    SettingDataFolderLoad* data_folder_load = static_cast<SettingDataFolderLoad*>(FindSetting(data_set, "profile_dirs"));
    if (data_folder_load != nullptr) {
        profiles_name = data_folder_load->profile_names;
    }

    return profiles_name;
}

WidgetSettingEnum::WidgetSettingEnum(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaEnum& meta,
                                     SettingDataSet& data_set)
    : WidgetSettingBase(tree, item), meta(meta), data_set(data_set), field(new ComboBox(this)), last_resize(0, 0) {
    this->field->setFocusPolicy(Qt::StrongFocus);
    this->field->show();

    this->item->setText(0, GetLabel(this->meta).c_str());
    this->item->setFont(0, this->tree->font());
    this->item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    this->tree->setItemWidget(this->item, 0, this);

    this->Refresh(REFRESH_ENABLE_AND_STATE);

    this->connect(this->field, SIGNAL(currentIndexChanged(int)), this, SLOT(OnIndexChanged(int)));
}

void WidgetSettingEnum::Refresh(RefreshAreas refresh_areas) {
    const SettingDependenceMode enabled = ::CheckDependence(this->meta, data_set);

    this->blockSignals(true);
    this->field->blockSignals(true);

    this->item->setHidden(enabled == SETTING_DEPENDENCE_HIDE);
    this->item->setDisabled(enabled != SETTING_DEPENDENCE_ENABLE);
    this->field->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);
    this->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);

    if (meta.default_value == "${VP_DEFAULT}") {
        if (::CheckSettingOverridden(this->meta)) {
            this->DisplayOverride(this->field, this->meta);
        }

        this->field->clear();
        this->enum_indexes.clear();

        const std::vector<std::string>& profiles = GetProfileNames(data_set);
        this->item->setHidden(enabled == SETTING_DEPENDENCE_HIDE);

        int selection = 0;
        const std::string value = this->data().GetValue();
        for (std::size_t i = 0, n = profiles.size(); i < n; ++i) {
            this->field->addItem(profiles[i].c_str());
            if (profiles[i] == value) {
                selection = static_cast<int>(this->enum_indexes.size());
            }
            this->enum_indexes.push_back(i);
        }
        this->field->setCurrentIndex(selection);
        if (!profiles.empty()) {
            OnIndexChanged(selection);
        }

        // Ensure this->field size match the profiles names size
        this->Resize();
    } else if (meta.default_value == "${VP_PHYSICAL_DEVICES}") {
        if (::CheckSettingOverridden(this->meta)) {
            this->DisplayOverride(this->field, this->meta);
        }

        this->field->clear();
        this->enum_indexes.clear();

        const std::vector<VulkanPhysicalDeviceInfo>& physical_device_infos = Configurator::Get().vulkan_system_info.physicalDevices;

        int selection = 0;
        const std::string value = this->data().GetValue();
        for (std::size_t i = 0, n = physical_device_infos.size(); i < n; ++i) {
            this->field->addItem(physical_device_infos[i].deviceName.c_str());
            if (physical_device_infos[i].deviceName == value || "${VP_PHYSICAL_DEVICES}" == value) {
                this->data().SetValue(physical_device_infos[i].deviceName.c_str());
                selection = static_cast<int>(this->enum_indexes.size());
            }
            this->enum_indexes.push_back(i);
        }
        this->field->setCurrentIndex(selection);
        if (!physical_device_infos.empty()) {
            OnIndexChanged(selection);
        }
    } else if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
        if (::CheckSettingOverridden(this->meta)) {
            this->DisplayOverride(this->field, this->meta);
        }

        this->field->clear();
        this->enum_indexes.clear();

        int selection = 0;
        const std::string value = this->data().GetValue();

        for (std::size_t i = 0, n = this->meta.enum_values.size(); i < n; ++i) {
            if (!IsSupported(&this->meta.enum_values[i])) {
                continue;
            }

            this->field->addItem(this->meta.enum_values[i].label.c_str());
            if (this->meta.enum_values[i].key == value) {
                selection = static_cast<int>(this->enum_indexes.size());
            }
            this->enum_indexes.push_back(i);
        }
        this->field->setCurrentIndex(selection);
    }

    this->blockSignals(false);
    this->field->blockSignals(false);
}

void WidgetSettingEnum::Resize() {
    // resizeEvent was never call yet
    if (this->last_resize.width() == 0 || this->last_resize.height() == 0) {
        return;
    }

    int width = MIN_FIELD_WIDTH;

    const QFontMetrics fm = this->field->fontMetrics();

    if (meta.default_value == "${VP_DEFAULT}") {
        const std::vector<std::string>& profiles = GetProfileNames(data_set);
        for (std::size_t i = 0, n = profiles.size(); i < n; ++i) {
            width = std::max(width, HorizontalAdvance(fm, (profiles[i] + "0000").c_str()));
        }
    } else if (meta.default_value == "${VP_PHYSICAL_DEVICES}") {
        const std::vector<VulkanPhysicalDeviceInfo>& physical_device_infos = Configurator::Get().vulkan_system_info.physicalDevices;
        for (std::size_t i = 0, n = physical_device_infos.size(); i < n; ++i) {
            width = std::max(width, HorizontalAdvance(fm, (physical_device_infos[i].deviceName + std::string("000")).c_str()));
        }
    } else {
        for (std::size_t i = 0, n = this->meta.enum_values.size(); i < n; ++i) {
            width = std::max(width, HorizontalAdvance(fm, (this->meta.enum_values[i].label + "0000").c_str()));
        }
    }

    const int prefix_width = HorizontalAdvance(fm, this->item->text(0) + "0");

    width = std::min(width, this->last_resize.width() - prefix_width);

    const QRect button_rect(this->last_resize.width() - width, 0, width, this->last_resize.height());
    this->field->setGeometry(button_rect);
}

void WidgetSettingEnum::resizeEvent(QResizeEvent* event) {
    this->last_resize = event->size();

    this->Resize();
}

void WidgetSettingEnum::OnIndexChanged(int index) {
    if (meta.default_value == "${VP_DEFAULT}") {
        const std::vector<std::string>& profiles = GetProfileNames(data_set);
        assert(index >= 0 && index < static_cast<int>(profiles.size()));

        this->data().SetValue(profiles[index].c_str());
        this->setToolTip(profiles[index].c_str());
    } else if (meta.default_value == "${VP_PHYSICAL_DEVICES}") {
        const std::vector<VulkanPhysicalDeviceInfo>& physical_device_infos = Configurator::Get().vulkan_system_info.physicalDevices;
        assert(index >= 0 && index < static_cast<int>(physical_device_infos.size()));

        this->data().SetValue(physical_device_infos[index].deviceName.c_str());
        this->setToolTip(physical_device_infos[index].deviceName.c_str());
    } else {
        assert(index >= 0 && index < static_cast<int>(this->meta.enum_values.size()));

        const std::size_t value_index = enum_indexes[static_cast<std::size_t>(index)];
        this->data().SetValue(this->meta.enum_values[value_index].key.c_str());

        std::string tooltip = this->meta.enum_values[value_index].description;
        if (this->meta.enum_values[value_index].status != STATUS_STABLE) {
            tooltip = format("(%s) %s", GetToken(this->meta.enum_values[value_index].status), tooltip.c_str());
        }
        this->setToolTip(tooltip.c_str());
    }

    emit refreshEnableOnly();
}

SettingDataEnum& WidgetSettingEnum::data() {
    SettingDataEnum* data = FindSetting<SettingDataEnum>(this->data_set, this->meta.key.c_str());
    assert(data != nullptr);
    return *data;
}
