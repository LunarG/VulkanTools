/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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

#include "setting_meta.h"

SettingMeta::SettingMeta(const std::string& key, const SettingType type) : key(key), type(type) { assert(!this->key.empty()); }

bool SettingMeta::Equal(const SettingMeta& other) const {
    if (this->key != other.key)
        return false;
    else if (this->type != other.type)
        return false;
    else if (this->env != other.env)
        return false;
    else if (this->label != other.label)
        return false;
    else if (this->description != other.description)
        return false;
    else if (this->url != other.url)
        return false;
    else if (this->status != other.status)
        return false;
    else if (this->platform_flags != other.platform_flags)
        return false;
    return true;
}

bool SettingMetaString::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaString&>(other).default_value;
}

bool SettingMetaInt::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaInt&>(other).default_value;
}

bool SettingMetaBool::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaBool&>(other).default_value;
}

bool SettingMetaIntRange::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    const SettingMetaIntRange& meta = static_cast<const SettingMetaIntRange&>(other);

    return this->default_min_value == meta.default_min_value && this->default_max_value == meta.default_max_value;
}

bool SettingMetaFilesystem::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    const SettingMetaFilesystem& meta = static_cast<const SettingMetaFilesystem&>(other);

    return this->default_value == meta.default_value && this->filter == meta.filter;
}

bool SettingMetaEnumeration::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    return this->enum_values == static_cast<const SettingMetaEnumeration&>(other).enum_values;
}

bool SettingMetaEnum::Equal(const SettingMeta& other) const {
    if (!SettingMetaEnumeration::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaEnum&>(other).default_value;
}

bool SettingMetaFlags::Equal(const SettingMeta& other) const {
    if (!SettingMetaEnumeration::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaFlags&>(other).default_value;
}

bool SettingMetaVUIDFilter::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    const SettingMetaVUIDFilter& meta = static_cast<const SettingMetaVUIDFilter&>(other);

    if (this->list != meta.list) return false;

    return this->default_value == meta.default_value;
}

bool operator==(const SettingEnumValue& a, const SettingEnumValue& b) {
    if (a.key != b.key) return false;
    if (a.label != b.label) return false;
    if (a.description != b.description) return false;
    if (a.url != b.url) return false;
    if (a.status != b.status) return false;
    if (a.platform_flags != b.platform_flags) return false;
    return true;
}

SettingMeta& SettingMetaSet::Create(const std::string& key, SettingType type) {
    assert(!key.empty());
    assert(type >= SETTING_FIRST && type <= SETTING_LAST);

    // Find existing setting, avoid duplicated setting
    {
        SettingMeta* setting_data = this->Get(key.c_str());
        if (setting_data != nullptr) {
            assert(setting_data->type == type);
            return *setting_data;
        }
    }

    // Create a new setting
    {
        std::shared_ptr<SettingMeta> setting_data(nullptr);
        switch (type) {
            case SETTING_STRING:
                setting_data.reset(new SettingMetaString(key));
                break;
            case SETTING_INT:
                setting_data.reset(new SettingMetaInt(key));
                break;
            case SETTING_SAVE_FILE:
                setting_data.reset(new SettingMetaFileSave(key));
                break;
            case SETTING_LOAD_FILE:
                setting_data.reset(new SettingMetaFileLoad(key));
                break;
            case SETTING_SAVE_FOLDER:
                setting_data.reset(new SettingMetaFolderSave(key));
                break;
            case SETTING_BOOL:
                setting_data.reset(new SettingMetaBool(key));
                break;
            case SETTING_BOOL_NUMERIC_DEPRECATED:
                setting_data.reset(new SettingMetaBoolNumeric(key));
                break;
            case SETTING_ENUM:
                setting_data.reset(new SettingMetaEnum(key));
                break;
            case SETTING_FLAGS:
                setting_data.reset(new SettingMetaFlags(key));
                break;
            case SETTING_INT_RANGE:
                setting_data.reset(new SettingMetaIntRange(key));
                break;
            case SETTING_VUID_FILTER:
                setting_data.reset(new SettingMetaVUIDFilter(key));
                break;
            default:
                assert(0);
                break;
        }

        this->data.push_back(setting_data);

        return *setting_data.get();
    }
}

SettingMeta* SettingMetaSet::Get(const char* key) {
    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        if (this->data[i]->key == key) return this->data[i].get();
    }

    return nullptr;
}

const SettingMeta* SettingMetaSet::Get(const char* key) const {
    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        if (this->data[i]->key, key) return this->data[i].get();
    }

    return nullptr;
}
