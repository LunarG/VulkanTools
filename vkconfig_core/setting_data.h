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

#include <QTextStream>

#include <string>
#include <vector>
#include <memory>
#include <cstdio>

class SettingData {
   protected:
    SettingData(const std::string& key, const SettingType type);

   public:
    virtual SettingData& operator=(const SettingData& setting_data) = 0;

    virtual bool operator==(const SettingData& setting_data) const = 0;
    bool operator!=(const SettingData& setting_data) const { return !(*this == setting_data); }

    const std::string key;
    const SettingType type;
};

struct SettingDataBool : public SettingData {
   protected:
    SettingDataBool(const std::string& key, const SettingType type) : SettingData(key, type), value(false) {}

   public:
    SettingDataBool(const std::string& key) : SettingData(key, SETTING_BOOL), value(false) {}
    SettingDataBool(const std::string& key, const bool& value) : SettingData(key, SETTING_BOOL), value(value) {}

    virtual SettingData& operator=(const SettingData& setting_data) {
        assert(this->type == setting_data.type);

        const SettingDataBool& setting_data_typed = static_cast<const SettingDataBool&>(setting_data);
        this->value = setting_data_typed.value;
        return *this;
    }

    virtual bool operator==(const SettingData& setting_data) const {
        if (this->type != setting_data.type) return false;

        return this->value == static_cast<const SettingDataBool&>(setting_data).value;
    }

    bool value;
};

struct SettingDataBoolNumeric : public SettingDataBool {
    SettingDataBoolNumeric(const std::string& key) : SettingDataBool(key, SETTING_BOOL_NUMERIC_DEPRECATED) {}
};

struct SettingDataInt : public SettingData {
    SettingDataInt(const std::string& key) : SettingData(key, SETTING_INT), value(-1) {}
    SettingDataInt(const std::string& key, const int& value) : SettingData(key, SETTING_INT), value(value) {}

    virtual SettingData& operator=(const SettingData& setting_data) {
        assert(this->type == setting_data.type);

        const SettingDataInt& setting_data_typed = static_cast<const SettingDataInt&>(setting_data);
        this->value = setting_data_typed.value;
        return *this;
    }

    virtual bool operator==(const SettingData& setting_data) const {
        if (this->type != setting_data.type) return false;

        return this->value == static_cast<const SettingDataInt&>(setting_data).value;
    }

    int value;
};

struct SettingDataString : public SettingData {
   protected:
    SettingDataString(const std::string& key, const SettingType type) : SettingData(key, type) {}

   public:
    SettingDataString(const std::string& key, const std::string& value) : SettingData(key, SETTING_STRING), value(value) {}
    SettingDataString(const std::string& key) : SettingDataString(key, SETTING_STRING) {}

    virtual SettingData& operator=(const SettingData& setting_data) {
        assert(this->type == setting_data.type);

        const SettingDataString& setting_data_typed = static_cast<const SettingDataString&>(setting_data);
        this->value = setting_data_typed.value;
        return *this;
    }

    virtual bool operator==(const SettingData& setting_data) const {
        if (this->type != setting_data.type) return false;

        return this->value == static_cast<const SettingDataString&>(setting_data).value;
    }

    std::string value;
};

struct SettingDataEnum : public SettingDataString {
    SettingDataEnum(const std::string& key) : SettingDataString(key, SETTING_ENUM) {}
};

struct SettingDataFileLoad : public SettingDataString {
    SettingDataFileLoad(const std::string& key) : SettingDataString(key, SETTING_LOAD_FILE) {}
};

struct SettingDataFileSave : public SettingDataString {
    SettingDataFileSave(const std::string& key) : SettingDataString(key, SETTING_SAVE_FILE) {}
};

struct SettingDataFolderSave : public SettingDataString {
    SettingDataFolderSave(const std::string& key) : SettingDataString(key, SETTING_SAVE_FOLDER) {}
};

class SettingDataIntRange : public SettingData {
   public:
    SettingDataIntRange(const std::string& key) : SettingData(key, SETTING_INT_RANGE) {}

    virtual SettingData& operator=(const SettingData& setting_data) {
        assert(this->type == setting_data.type);

        const SettingDataIntRange& setting_data_typed = static_cast<const SettingDataIntRange&>(setting_data);
        this->min_value = setting_data_typed.min_value;
        this->max_value = setting_data_typed.max_value;
        return *this;
    }

    virtual bool operator==(const SettingData& setting_data) const {
        if (this->type != setting_data.type) return false;

        const SettingDataIntRange& setting_data_range = static_cast<const SettingDataIntRange&>(setting_data);
        if (this->min_value != setting_data_range.min_value) return false;
        if (this->max_value != setting_data_range.max_value) return false;

        return true;
    }

    int min_value;
    int max_value;
};

class SettingDataVector : public SettingData {
   protected:
    SettingDataVector(const std::string& key, const SettingType type) : SettingData(key, type) {}

   public:
    virtual SettingData& operator=(const SettingData& setting_data) {
        assert(this->type == setting_data.type);

        const SettingDataVector& setting_data_vector = static_cast<const SettingDataVector&>(setting_data);
        this->value = setting_data_vector.value;
        return *this;
    }

    virtual bool operator==(const SettingData& setting_data) const {
        if (this->type != setting_data.type) return false;

        const SettingDataVector& setting_data_vector = static_cast<const SettingDataVector&>(setting_data);

        if (this->value.size() != setting_data_vector.value.size()) return false;

        for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
            if (std::find(setting_data_vector.value.begin(), setting_data_vector.value.end(), this->value[i].c_str()) ==
                setting_data_vector.value.end())
                return false;
        }

        return true;
    }

    std::vector<std::string> value;
};

struct SettingDataVUIDFilter : public SettingDataVector {
    SettingDataVUIDFilter(const std::string& key) : SettingDataVector(key, SETTING_VUID_FILTER) {}
};

struct SettingDataFlags : public SettingDataVector {
    SettingDataFlags(const std::string& key) : SettingDataVector(key, SETTING_FLAGS) {}
};

class SettingDataSet {
   public:
    SettingData& Create(const std::string& key, SettingType type);
    SettingData* Get(const char* key);
    const SettingData* Get(const char* key) const;
    bool Empty() const { return this->data.empty(); }

    std::vector<std::shared_ptr<SettingData> > data;
};
