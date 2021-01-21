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

#pragma once

#include "platform.h"
#include "setting_data.h"

#include <QStringList>
#include <QVariant>
#include <QJsonObject>

#include <string>
#include <memory>

struct SettingMeta {
    SettingMeta(const std::string& key, const SettingType& setting_type)
        : status(STATUS_STABLE), platform_flags(PLATFORM_ALL_BIT), default_value(CreateSettingData(key, setting_type)) {
        assert(this->default_value);
        assert(this->default_value->GetKey() == key);
    }

    const char* GetKey() const {
        assert(this->default_value);
        return this->default_value->GetKey();
    }

    SettingType GetType() const {
        assert(this->default_value);
        return this->default_value->GetType();
    }

    virtual bool operator==(const SettingMeta& setting_meta) const {
        if (std::strcmp(this->GetKey(), setting_meta.GetKey()) != 0)
            return false;
        else if (this->GetType() != setting_meta.GetType())
            return false;
        else if (this->label != setting_meta.label)
            return false;
        else if (this->description != setting_meta.description)
            return false;
        else if (this->url != setting_meta.url)
            return false;
        else if (this->status != setting_meta.status)
            return false;
        else if (this->platform_flags != setting_meta.platform_flags)
            return false;
        else if (this->default_value != setting_meta.default_value)
            return false;
        return true;
    }

    bool operator!=(const SettingMeta& setting_meta) const { return !(*this == setting_meta); }

    virtual bool Load(const QJsonObject& json_setting_object) {
        assert(this->default_value);

        this->label = ReadStringValue(json_setting_object, "label");
        this->description = ReadStringValue(json_setting_object, "description");
        if (json_setting_object.value("url") != QJsonValue::Undefined) {
            this->url = ReadStringValue(json_setting_object, "url");
        }
        if (json_setting_object.value("status") != QJsonValue::Undefined) {
            this->status = GetStatusType(ReadStringValue(json_setting_object, "status").c_str());
        }
        if (json_setting_object.value("platforms") != QJsonValue::Undefined) {
            this->platform_flags = GetPlatformFlags(ReadStringArray(json_setting_object, "platforms"));
        }

        this->default_value->Load(json_setting_object);

        return true;
    }

    std::string label;
    std::string description;
    std::string url;
    StatusType status;
    int platform_flags;
    std::shared_ptr<SettingData> default_value;
};

struct SettingMetaString : public SettingMeta {
    SettingMetaString(const std::string& key) : SettingMeta(key, SETTING_STRING) {}
};

struct SettingMetaInt : public SettingMeta {
    SettingMetaInt(const std::string& key) : SettingMeta(key, SETTING_INT) {}
};

struct SettingMetaBool : public SettingMeta {
    SettingMetaBool(const std::string& key) : SettingMeta(key, SETTING_BOOL) {}
};

struct SettingMetaBoolNumeric : public SettingMeta {
    SettingMetaBoolNumeric(const std::string& key) : SettingMeta(key, SETTING_BOOL_NUMERIC_DEPRECATED) {}
};

struct SettingMetaIntRange : public SettingMeta {
    SettingMetaIntRange(const std::string& key) : SettingMeta(key, SETTING_INT_RANGE) {}
};

struct SettingMetaFilesystem : public SettingMeta {
    SettingMetaFilesystem(const std::string& key, const SettingType& setting_type) : SettingMeta(key, setting_type) {}

    virtual bool Load(const QJsonObject& json_setting_object) {
        if (!SettingMeta::Load(json_setting_object)) {
            return false;
        }

        if (json_setting_object.value("filter") != QJsonValue::Undefined && this->GetType() != SETTING_SAVE_FOLDER) {
            this->filter = ReadStringValue(json_setting_object, "filter");
        }

        return true;
    }

    std::string filter;
};

struct SettingMetaFileLoad : public SettingMetaFilesystem {
    SettingMetaFileLoad(const std::string& key) : SettingMetaFilesystem(key, SETTING_LOAD_FILE) {}
};

struct SettingMetaFileSave : public SettingMetaFilesystem {
    SettingMetaFileSave(const std::string& key) : SettingMetaFilesystem(key, SETTING_SAVE_FILE) {}
};

struct SettingMetaFolderSave : public SettingMetaFilesystem {
    SettingMetaFolderSave(const std::string& key) : SettingMetaFilesystem(key, SETTING_SAVE_FOLDER) {}
};

struct SettingEnumValue {
    SettingEnumValue() : status(STATUS_STABLE), platform_flags(PLATFORM_ALL_BIT) {}

    std::string key;
    std::string label;
    std::string description;
    std::string url;
    StatusType status;
    int platform_flags;
};

bool operator==(const SettingEnumValue& a, const SettingEnumValue& b);
inline bool operator!=(const SettingEnumValue& a, const SettingEnumValue& b) { return !(a == b); }

struct SettingMetaEnumeration : public SettingMeta {
    SettingMetaEnumeration(const std::string& key, const SettingType& setting_type) : SettingMeta(key, setting_type) {}

    virtual bool operator==(const SettingMeta& setting_meta) const {
        if (SettingMeta::operator!=(setting_meta)) return false;

        const SettingMetaEnumeration& setting_meta_enum = static_cast<const SettingMetaEnumeration&>(setting_meta);

        if (this->enum_values.size() != setting_meta_enum.enum_values.size()) return false;
        if (this->enum_values != setting_meta_enum.enum_values) return false;

        return true;
    }

    virtual bool Load(const QJsonObject& json_setting_object) {
        if (!SettingMeta::Load(json_setting_object)) {
            return false;
        }

        const QJsonArray& json_array = ReadArray(json_setting_object, GetTypeToken(json_setting_object));
        for (int i = 0, n = json_array.size(); i < n; ++i) {
            const QJsonObject& json_object = json_array[i].toObject();

            SettingEnumValue setting_enum_value;
            setting_enum_value.key = ReadStringValue(json_object, "key");
            setting_enum_value.label = ReadStringValue(json_object, "label");
            setting_enum_value.description = ReadStringValue(json_object, "description");
            if (json_object.value("url") != QJsonValue::Undefined) {
                setting_enum_value.url = ReadStringValue(json_object, "url");
            }
            if (json_object.value("status") != QJsonValue::Undefined) {
                setting_enum_value.status = GetStatusType(ReadStringValue(json_object, "status").c_str());
            }
            if (json_object.value("platforms") != QJsonValue::Undefined) {
                setting_enum_value.platform_flags = GetPlatformFlags(ReadStringArray(json_object, "platforms"));
            }

            this->enum_values.push_back(setting_enum_value);
        }

        json_setting_object.value("filter");

        return true;
    }

    std::vector<SettingEnumValue> enum_values;

   private:
    static const char* GetTypeToken(const QJsonObject& json_setting_object) {
        static const char* value = "flags";

        if (json_setting_object.value(value) != QJsonValue::Undefined) {
            return value;
        }

        if (SUPPORT_VKCONFIG_2_1_0) {
            static const char* legacy_value = "multi_enum";
            if (json_setting_object.value(legacy_value) != QJsonValue::Undefined) return legacy_value;
        }

        return nullptr;
    }
};

struct SettingMetaEnum : public SettingMetaEnumeration {
    SettingMetaEnum(const std::string& key) : SettingMetaEnumeration(key, SETTING_ENUM) {}
};

struct SettingMetaFlags : public SettingMetaEnumeration {
    SettingMetaFlags(const std::string& key) : SettingMetaEnumeration(key, SETTING_FLAGS) {}
};

struct SettingMetaVUIDFilter : public SettingMeta {
    SettingMetaVUIDFilter(const std::string& key) : SettingMeta(key, SETTING_VUID_FILTER) {
        assert(this->default_value);
        assert(this->default_value->GetKey() == key);
    }

    virtual bool Load(const QJsonObject& json_setting_object) {
        if (!SettingMeta::Load(json_setting_object)) {
            return false;
        }

        const QJsonArray& json_array = ReadArray(json_setting_object, "list");
        for (int i = 0, n = json_array.size(); i < n; ++i) {
            this->list.push_back(json_array[i].toString().toStdString());
        }

        return true;
    }

    std::vector<std::string> list;
};

class SettingMetaSet {
   public:
    SettingMeta& Create(const std::string& key, SettingType type);
    SettingMeta* Get(const char* key);
    const SettingMeta* Get(const char* key) const;
    bool Empty() const { return this->data.empty(); }

    std::vector<std::shared_ptr<SettingMeta> > data;
};
