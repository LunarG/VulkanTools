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

#include "setting_filesystem.h"
#include "path.h"

// SettingMetaFilesystem

SettingMetaFilesystem::SettingMetaFilesystem(Layer& layer, const std::string& key, const SettingType& type)
    : SettingMeta(layer, key, type) {}

bool SettingMetaFilesystem::Load(const QJsonObject& json_setting) {
    this->default_value = ReadStringValue(json_setting, "default");
    if (json_setting.value("filter") != QJsonValue::Undefined) {
        this->filter = ReadStringValue(json_setting, "filter");
    }
    if (json_setting.value("format") != QJsonValue::Undefined) {
        this->format = ReadStringValue(json_setting, "format");
    }
    return true;
}

std::string SettingMetaFilesystem::Export(ExportMode export_mode) const {
    switch (export_mode) {
        default:
            assert(0);
            return "";
        case EXPORT_MODE_OVERRIDE:
            return ReplaceBuiltInVariable(this->default_value).c_str();
        case EXPORT_MODE_DOC:
            return this->default_value;
    }
}

bool SettingMetaFilesystem::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    const SettingMetaFilesystem& meta = static_cast<const SettingMetaFilesystem&>(other);

    return this->default_value == meta.default_value && this->filter == meta.filter;
}

// SettingDataFilesystem

SettingDataFilesystem::SettingDataFilesystem(const std::string& key, const SettingType& type) : SettingDataString(key, type) {}

std::string SettingDataFilesystem::Export(ExportMode export_mode) const {
    switch (export_mode) {
        default:
            assert(0);
            return "";
        case EXPORT_MODE_OVERRIDE:
            return ReplaceBuiltInVariable(this->value).c_str();
        case EXPORT_MODE_DOC:
            return this->value;
    }
}

// SettingMetaFileLoad

const SettingType SettingMetaFileLoad::TYPE(SETTING_LOAD_FILE);

SettingMetaFileLoad::SettingMetaFileLoad(Layer& layer, const std::string& key) : SettingMetaFilesystem(layer, key, TYPE) {}

SettingData* SettingMetaFileLoad::Instantiate() {
    SettingData* setting_data = new SettingDataFileLoad(this);
    setting_data->Reset();
    this->instances.push_back(setting_data);
    return setting_data;
}

// SettingDataFileLoad

SettingDataFileLoad::SettingDataFileLoad(const SettingMetaFileLoad* meta)
    : SettingDataFilesystem(meta->key, meta->type), meta(meta) {}

void SettingDataFileLoad::Copy(const SettingData* data) {
    if (data->type != this->type) return;

    const SettingDataFileLoad* setting_data = static_cast<const SettingDataFileLoad*>(data);
    this->value = setting_data->value;
    this->profile_names = setting_data->profile_names;
}

bool SettingDataFileLoad::Load(const QJsonObject& json_setting) {
    SettingDataString::Load(json_setting);

    if (this->meta->format == "PROFILE") {
        this->profile_names = GetProfileNames(this->value);
    }
    return true;
}

void SettingDataFileLoad::Reset() { this->value = this->meta->default_value; }

// SettingMetaFileSave

const SettingType SettingMetaFileSave::TYPE(SETTING_SAVE_FILE);

SettingMetaFileSave::SettingMetaFileSave(Layer& layer, const std::string& key) : SettingMetaFilesystem(layer, key, TYPE) {}

SettingData* SettingMetaFileSave::Instantiate() {
    SettingData* setting_data = new SettingDataFileSave(this);
    setting_data->Reset();
    this->instances.push_back(setting_data);
    return setting_data;
}

// SettingDataFileSave

SettingDataFileSave::SettingDataFileSave(const SettingMetaFileSave* meta)
    : SettingDataFilesystem(meta->key, meta->type), meta(meta) {}

void SettingDataFileSave::Copy(const SettingData* data) {
    if (data->type != this->type) return;

    const SettingDataFileSave* setting_data = static_cast<const SettingDataFileSave*>(data);
    this->value = setting_data->value;
}

void SettingDataFileSave::Reset() { this->value = this->meta->default_value; }

// SettingMetaFolderSave

const SettingType SettingMetaFolderSave::TYPE(SETTING_SAVE_FOLDER);

SettingMetaFolderSave::SettingMetaFolderSave(Layer& layer, const std::string& key) : SettingMetaFilesystem(layer, key, TYPE) {}

SettingData* SettingMetaFolderSave::Instantiate() {
    SettingData* setting_data = new SettingDataFolderSave(this);
    setting_data->Reset();
    this->instances.push_back(setting_data);
    return setting_data;
}

// SettingDataFolderSave

SettingDataFolderSave::SettingDataFolderSave(const SettingMetaFolderSave* meta)
    : SettingDataFilesystem(meta->key, meta->type), meta(meta) {}

void SettingDataFolderSave::Copy(const SettingData* data) {
    if (data->type != this->type) return;

    const SettingDataFolderSave* setting_data = static_cast<const SettingDataFolderSave*>(data);
    this->value = setting_data->value;
}

void SettingDataFolderSave::Reset() { this->value = this->meta->default_value; }
