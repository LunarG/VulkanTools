/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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
            return this->default_value.AbsolutePath();
        case EXPORT_MODE_DOC:
            return ::ConvertStandardSeparators(this->default_value.RelativePath());
    }
}

bool SettingMetaFilesystem::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) {
        return false;
    }

    const SettingMetaFilesystem& meta = static_cast<const SettingMetaFilesystem&>(other);

    return this->default_value == meta.default_value && this->filter == meta.filter;
}

// SettingDataFilesystem

SettingDataFilesystem::SettingDataFilesystem(const SettingMetaFilesystem* meta) : SettingData(meta->key, meta->type), meta(meta) {
    assert(meta != nullptr);
    this->Reset();
}

SettingDataFilesystem::SettingDataFilesystem(const std::string& key, const SettingType& type) : SettingData(key, type) {}

void SettingDataFilesystem::Reset() {
    assert(meta != nullptr);
    this->value = this->meta->default_value;
}

void SettingDataFilesystem::Copy(const SettingData* data) {
    if (data->type != this->type) return;

    const SettingDataFilesystem* setting_data = static_cast<const SettingDataFilesystem*>(data);
    this->value = setting_data->value;
}

bool SettingDataFilesystem::Load(const QJsonObject& json_setting) {
    this->value = ReadStringValue(json_setting, "value");

    if (json_setting.value("expanded") != QJsonValue::Undefined) {
        this->expanded = ReadBoolValue(json_setting, "expanded");
    }

    return true;
}

bool SettingDataFilesystem::Save(QJsonObject& json_setting) const {
    json_setting.insert("value", this->value.RelativePath().c_str());
    json_setting.insert("expanded", this->expanded);
    return true;
}

std::string SettingDataFilesystem::Export(ExportMode export_mode) const {
    switch (export_mode) {
        default:
            assert(0);
            return "";
        case EXPORT_MODE_CPP_DECLARATION_AND_INIT: {
            return format("std::string %s = \"%s\";\n", this->key.c_str(),
                          ::ConvertStandardSeparators(this->value.RelativePath()).c_str());
        }
        case EXPORT_MODE_OVERRIDE: {
            Path file(this->value);
            return ::ConvertStandardSeparators(file.AbsolutePath());
        }
        case EXPORT_MODE_DOC:
            return ::ConvertStandardSeparators(Path(this->value).RelativePath());
    }
}

const Path& SettingDataFilesystem::GetValue() const { return this->value; }

void SettingDataFilesystem::SetValue(const Path& value) { this->value = value; }

bool SettingDataFilesystem::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    return this->value == static_cast<const SettingDataFilesystem&>(other).value;
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
    if (data->type != this->type) {
        return;
    }

    const SettingDataFileLoad* setting_data = static_cast<const SettingDataFileLoad*>(data);
    this->value = setting_data->value;
    this->profile_names = setting_data->profile_names;
}

bool SettingDataFileLoad::Load(const QJsonObject& json_setting) {
    const bool result = this->SettingDataFilesystem::Load(json_setting);

    if (this->meta->format == "PROFILE") {
        this->profile_names = CollectProfileNamesFromFile(this->value);
    }

    return result;
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
    if (data->type != this->type) {
        return;
    }

    const SettingDataFileSave* setting_data = static_cast<const SettingDataFileSave*>(data);
    this->value = setting_data->value;
}

void SettingDataFileSave::Reset() { this->value = this->meta->default_value; }

// SettingMetaFolderLoad

const SettingType SettingMetaFolderLoad::TYPE(SETTING_LOAD_FOLDER);

SettingMetaFolderLoad::SettingMetaFolderLoad(Layer& layer, const std::string& key) : SettingMetaFilesystem(layer, key, TYPE) {}

SettingData* SettingMetaFolderLoad::Instantiate() {
    SettingData* setting_data = new SettingDataFolderLoad(this);
    setting_data->Reset();
    this->instances.push_back(setting_data);
    return setting_data;
}

// SettingDataFolderLoad

SettingDataFolderLoad::SettingDataFolderLoad(const SettingMetaFolderLoad* meta)
    : SettingDataFilesystem(meta->key, meta->type), meta(meta) {}

void SettingDataFolderLoad::Copy(const SettingData* data) {
    if (data->type != this->type) return;

    const SettingDataFolderLoad* setting_data = static_cast<const SettingDataFolderLoad*>(data);
    this->value = setting_data->value;
}

bool SettingDataFolderLoad::Load(const QJsonObject& json_setting) {
    this->SettingDataFilesystem::Load(json_setting);

    if (this->meta->format == "PROFILE") {
        this->profile_names = CollectProfileNamesFromDir(this->value);
    }
    return true;
}

void SettingDataFolderLoad::Reset() { this->value = this->meta->default_value; }

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
    if (data->type != this->type) {
        return;
    }

    const SettingDataFolderSave* setting_data = static_cast<const SettingDataFolderSave*>(data);
    this->value = setting_data->value;
}

void SettingDataFolderSave::Reset() { this->value = this->meta->default_value; }
