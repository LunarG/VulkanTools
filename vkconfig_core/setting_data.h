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

#include "setting_type.h"
#include "json.h"
#include "util.h"
#include "path.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>

#include <string>
#include <vector>
#include <memory>
#include <cstdio>

const char* GetValueToken(const QJsonObject& json_setting_object);

class SettingData {
   public:
    SettingData(const std::string& key) : key(key) {}

    const char* GetKey() const { return this->key.c_str(); }

    virtual SettingType GetType() const = 0;

    virtual SettingData& operator=(const SettingData& setting_data) = 0;
    virtual bool operator==(const SettingData& setting_data) const = 0;
    bool operator!=(const SettingData& setting_data) const { return !(*this == setting_data); }

    virtual bool Load(const QJsonObject& json_setting_object) {
        // If there is a type check the types are the same otherwise they are the same.
        if (json_setting_object.value("type") != QJsonValue::Undefined) {
            const SettingType type = GetSettingType(ReadStringValue(json_setting_object, "type").c_str());
            if (type != this->GetType()) {
                return false;
            }
        }

        return true;
    }

    // For layers configuration files
    virtual void Save(QJsonObject& json_setting) const = 0;

    // For layer settings override files
    virtual void Save(QTextStream& stream) const = 0;

   private:
    const std::string key;
};

struct SettingDataBool : public SettingData {
    SettingDataBool(const std::string& key) : SettingData(key), value(false) {}

    SettingDataBool(const std::string& key, const bool& value) : SettingData(key), value(value) {}

    virtual SettingType GetType() const { return SETTING_BOOL; }

    virtual SettingData& operator=(const SettingData& setting_data) {
        assert(this->GetType() == setting_data.GetType());

        const SettingDataBool& setting_data_typed = static_cast<const SettingDataBool&>(setting_data);
        this->value = setting_data_typed.value;
        return *this;
    }

    virtual bool operator==(const SettingData& setting_data) const {
        if (this->GetType() != setting_data.GetType()) return false;

        return this->value == static_cast<const SettingDataBool&>(setting_data).value;
    }

    virtual bool Load(const QJsonObject& json_setting_object) {
        if (!SettingData::Load(json_setting_object)) {
            return false;
        }

        const char* token = GetValueToken(json_setting_object);

        if (SUPPORT_VKCONFIG_2_1_0 && json_setting_object.value(token).isString()) {
            value = ReadStringValue(json_setting_object, token) == "1" || ReadStringValue(json_setting_object, token) == "TRUE";
        } else {
            value = ReadBoolValue(json_setting_object, token);
        }

        return true;
    }

    virtual void Save(QJsonObject& json_setting_object) const { json_setting_object.insert("value", this->value); }

    virtual void Save(QTextStream& stream) const { stream << (this->value ? "TRUE" : "FALSE"); }

    bool value;
};

struct SettingDataBoolNumeric : public SettingDataBool {
    SettingDataBoolNumeric(const std::string& key) : SettingDataBool(key) {}

    virtual SettingType GetType() const { return SETTING_BOOL_NUMERIC_DEPRECATED; }

    virtual void Save(QTextStream& stream) const { stream << (this->value ? "1" : "0"); }
};

struct SettingDataInt : public SettingData {
    SettingDataInt(const std::string& key) : SettingData(key), value(-1) {}

    SettingDataInt(const std::string& key, const int& value) : SettingData(key), value(value) {}

    virtual SettingType GetType() const { return SETTING_INT; }

    virtual SettingData& operator=(const SettingData& setting_data) {
        assert(this->GetType() == setting_data.GetType());

        const SettingDataInt& setting_data_typed = static_cast<const SettingDataInt&>(setting_data);
        this->value = setting_data_typed.value;
        return *this;
    }

    virtual bool operator==(const SettingData& setting_data) const {
        if (this->GetType() != setting_data.GetType()) return false;

        return this->value == static_cast<const SettingDataInt&>(setting_data).value;
    }

    virtual bool Load(const QJsonObject& json_setting_object) {
        if (!SettingData::Load(json_setting_object)) {
            return false;
        }

        const char* token = GetValueToken(json_setting_object);

        if (SUPPORT_VKCONFIG_2_1_0 && json_setting_object.value(token).isString()) {
            const std::string tmp = ReadStringValue(json_setting_object, token);
            assert(!tmp.empty());
            this->value = std::atoi(tmp.c_str());
        } else {
            this->value = ReadIntValue(json_setting_object, token);
        }

        return true;
    }

    virtual void Save(QJsonObject& json_setting_object) const { json_setting_object.insert("value", this->value); }

    virtual void Save(QTextStream& stream) const { stream << this->value; }

    int value;
};

struct SettingDataString : public SettingData {
    SettingDataString(const std::string& key) : SettingData(key) {}

    SettingDataString(const std::string& key, const std::string& value) : SettingData(key), value(value) {}

    virtual SettingType GetType() const { return SETTING_STRING; }

    virtual SettingData& operator=(const SettingData& setting_data) {
        assert(this->GetType() == setting_data.GetType());

        const SettingDataString& setting_data_typed = static_cast<const SettingDataString&>(setting_data);
        this->value = setting_data_typed.value;
        return *this;
    }

    virtual bool operator==(const SettingData& setting_data) const {
        if (this->GetType() != setting_data.GetType()) return false;

        return this->value == static_cast<const SettingDataString&>(setting_data).value;
    }

    virtual bool Load(const QJsonObject& json_setting_object) {
        if (!SettingData::Load(json_setting_object)) {
            return false;
        }

        this->value = ReadString(json_setting_object, GetValueToken(json_setting_object));

        return true;
    }

    virtual void Save(QJsonObject& json_setting_object) const { json_setting_object.insert("value", this->value.c_str()); }

    virtual void Save(QTextStream& stream) const { stream << this->value.c_str(); }

    std::string value;
};

struct SettingDataEnum : public SettingDataString {
    SettingDataEnum(const std::string& key) : SettingDataString(key) {}

    virtual SettingType GetType() const { return SETTING_ENUM; }
};

struct SettingDataFilesystem : public SettingDataString {
    SettingDataFilesystem(const std::string& key) : SettingDataString(key) {}

    virtual void Save(QTextStream& stream) const { stream << ReplaceBuiltInVariable(this->value.c_str()).c_str(); }
};

struct SettingDataFileLoad : public SettingDataFilesystem {
    SettingDataFileLoad(const std::string& key) : SettingDataFilesystem(key) {}

    virtual SettingType GetType() const { return SETTING_LOAD_FILE; }
};

struct SettingDataFileSave : public SettingDataFilesystem {
    SettingDataFileSave(const std::string& key) : SettingDataFilesystem(key) {}

    virtual SettingType GetType() const { return SETTING_SAVE_FILE; }
};

struct SettingDataFolderSave : public SettingDataFilesystem {
    SettingDataFolderSave(const std::string& key) : SettingDataFilesystem(key) {}

    virtual SettingType GetType() const { return SETTING_SAVE_FOLDER; }
};

class SettingDataIntRange : public SettingData {
   public:
    SettingDataIntRange(const std::string& key) : SettingData(key) {}

    virtual SettingType GetType() const { return SETTING_INT_RANGE; }

    virtual SettingData& operator=(const SettingData& setting_data) {
        assert(this->GetType() == setting_data.GetType());

        const SettingDataIntRange& setting_data_typed = static_cast<const SettingDataIntRange&>(setting_data);
        this->min_value = setting_data_typed.min_value;
        this->max_value = setting_data_typed.max_value;
        return *this;
    }

    virtual bool operator==(const SettingData& setting_data) const {
        if (this->GetType() != setting_data.GetType()) return false;

        const SettingDataIntRange& setting_data_range = static_cast<const SettingDataIntRange&>(setting_data);
        if (this->min_value != setting_data_range.min_value) return false;
        if (this->max_value != setting_data_range.max_value) return false;

        return true;
    }

    virtual bool Load(const QJsonObject& json_setting_object) {
        if (!SettingData::Load(json_setting_object)) {
            return false;
        }

        const char* token = GetValueToken(json_setting_object);
        if (SUPPORT_VKCONFIG_2_1_0 && json_setting_object.value(token).isString()) {
            const std::string value = ReadStringValue(json_setting_object, token);
            std::sscanf(value.c_str(), "%d-%d", &this->min_value, &this->max_value);
        } else {
            const QJsonObject& json_range_object = ReadObject(json_setting_object, token);

            this->min_value = ReadIntValue(json_range_object, "min");
            this->max_value = ReadIntValue(json_range_object, "max");
        }

        return true;
    }

    virtual void Save(QJsonObject& json_setting_object) const {
        QJsonObject json_value_object;
        json_value_object.insert("min", this->min_value);
        json_value_object.insert("max", this->max_value);

        json_setting_object.insert("value", json_value_object);
    }

    virtual void Save(QTextStream& stream) const {
        if (this->min_value < this->max_value) {
            stream << format("%d-%d", this->min_value, this->max_value).c_str();
        }
    }

    int min_value;
    int max_value;
};

class SettingDataVector : public SettingData {
   public:
    SettingDataVector(const std::string& key) : SettingData(key) {}

    virtual SettingType GetType() const = 0;

    virtual SettingData& operator=(const SettingData& setting_data) {
        assert(this->GetType() == setting_data.GetType());

        const SettingDataVector& setting_data_vector = static_cast<const SettingDataVector&>(setting_data);
        this->value = setting_data_vector.value;
        return *this;
    }

    virtual bool operator==(const SettingData& setting_data) const {
        if (this->GetType() != setting_data.GetType()) return false;

        const SettingDataVector& setting_data_vector = static_cast<const SettingDataVector&>(setting_data);

        if (this->value.size() != setting_data_vector.value.size()) return false;

        for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
            if (std::find(setting_data_vector.value.begin(), setting_data_vector.value.end(), this->value[i].c_str()) ==
                setting_data_vector.value.end())
                return false;
        }

        return true;
    }

    virtual bool Load(const QJsonObject& json_setting_object) {
        if (!SettingData::Load(json_setting_object)) {
            return false;
        }

        if (SUPPORT_VKCONFIG_2_1_0 && json_setting_object.value(GetValueToken(json_setting_object)).isString()) {
            const QString tmp = ReadStringValue(json_setting_object, GetValueToken(json_setting_object)).c_str();
            const QStringList list = tmp.split(",");
            for (int i = 0, n = list.size(); i < n; ++i) {
                this->value.push_back(list[i].toStdString());
            }
        } else {
            this->value = ReadStringArray(json_setting_object, GetValueToken(json_setting_object));
        }

        return true;
    }

    virtual void Save(QJsonObject& json_setting_object) const {
        QJsonArray json_array;

        for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
            json_array.append(this->value[i].c_str());
        }

        json_setting_object.insert("value", json_array);
    }

    virtual void Save(QTextStream& stream) const {
        for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
            stream << this->value[i].c_str();
            if (i < n - 1) stream << ",";
        }
    }

    std::vector<std::string> value;
};

struct SettingDataVUIDFilter : public SettingDataVector {
    SettingDataVUIDFilter(const std::string& key) : SettingDataVector(key) {}

    virtual SettingType GetType() const { return SETTING_VUID_FILTER; };
};

struct SettingDataFlags : public SettingDataVector {
    SettingDataFlags(const std::string& key) : SettingDataVector(key) {}

    virtual SettingType GetType() const { return SETTING_FLAGS; };
};

class SettingDataSet {
   public:
    SettingData* Create(const std::string& key, SettingType type);
    SettingData* Get(const char* key);
    const SettingData* Get(const char* key) const;
    bool Empty() const { return this->data.empty(); }

    std::vector<std::shared_ptr<SettingData> > data;
};

std::shared_ptr<SettingData> CreateSettingData(const std::string& key, SettingType type);
