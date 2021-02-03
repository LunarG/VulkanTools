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
   public:
    SettingData(const std::string& key) : key(key) {}

    const char* GetKey() const { return this->key.c_str(); }

    virtual SettingType GetType() const = 0;

    virtual SettingData& operator=(const SettingData& setting_data) = 0;
    virtual bool operator==(const SettingData& setting_data) const = 0;
    bool operator!=(const SettingData& setting_data) const { return !(*this == setting_data); }

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

    bool value;
};

struct SettingDataBoolNumeric : public SettingDataBool {
    SettingDataBoolNumeric(const std::string& key) : SettingDataBool(key) {}

    virtual SettingType GetType() const { return SETTING_BOOL_NUMERIC_DEPRECATED; }
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

    std::string value;
};

struct SettingDataEnum : public SettingDataString {
    SettingDataEnum(const std::string& key) : SettingDataString(key) {}

    virtual SettingType GetType() const { return SETTING_ENUM; }
};

struct SettingDataFilesystem : public SettingDataString {
    SettingDataFilesystem(const std::string& key) : SettingDataString(key) {}
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
    SettingData& Create(const std::string& key, SettingType type);
    SettingData* Get(const char* key);
    const SettingData* Get(const char* key) const;
    bool Empty() const { return this->data.empty(); }

    std::vector<std::shared_ptr<SettingData> > data;
};
