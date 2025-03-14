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

#include "setting_group.h"

// SettingMetaGroup

const SettingType SettingMetaGroup::TYPE(SETTING_GROUP);

SettingMetaGroup::SettingMetaGroup(Layer& layer, const std::string& key) : SettingMeta(layer, key, TYPE) {}

SettingData* SettingMetaGroup::Instantiate() {
    SettingData* setting_data = new SettingDataGroup(this);
    setting_data->Reset();
    this->instances.push_back(setting_data);
    return setting_data;
}

bool SettingMetaGroup::Load(const QJsonObject& json_setting) {
    (void)json_setting;

    return true;
}

std::string SettingMetaGroup::Export(ExportMode export_mode) const {
    (void)export_mode;
    return "";
}

// SettingDataGroup

SettingDataGroup::SettingDataGroup(const SettingMetaGroup* meta) : SettingData(meta->key, SettingMetaGroup::TYPE), meta(meta) {}

void SettingDataGroup::Reset() { assert(this->meta != nullptr); }

void SettingDataGroup::Copy(const SettingData* data) { (void)data; }

bool SettingDataGroup::Load(const QJsonObject& json_setting) {
    if (json_setting.value("expanded") != QJsonValue::Undefined) {
        this->expanded = ReadBoolValue(json_setting, "expanded");
    }

    return true;
}
bool SettingDataGroup::Save(QJsonObject& json_setting) const {
    json_setting.insert("expanded", this->expanded);

    return true;
}

std::string SettingDataGroup::Export(ExportMode export_mode) const {
    (void)export_mode;
    return "";
}