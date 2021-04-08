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

#pragma once

#include "header.h"
#include "setting_data.h"

#include <string>
#include <vector>
#include <memory>

struct SettingMeta;

typedef SettingSet<SettingMeta> SettingMetaSet;

struct SettingMeta : public Header {
    SettingMeta(const std::string& key, const SettingType type);
    virtual ~SettingMeta() {}

    bool operator==(const SettingMeta& other) const { return this->Equal(other); }
    bool operator!=(const SettingMeta& other) const { return !this->Equal(other); }

    const std::string key;
    const SettingType type;
    std::string env;
    SettingMetaSet children;

   protected:
    virtual bool Equal(const SettingMeta& other) const;
};

struct SettingMetaString : public SettingMeta {
    SettingMetaString(const std::string& key) : SettingMetaString(key, SETTING_STRING) {}
    virtual ~SettingMetaString() {}

    std::string default_value;

   protected:
    SettingMetaString(const std::string& key, const SettingType& setting_type) : SettingMeta(key, setting_type) {}

    virtual bool Equal(const SettingMeta& other) const;
};

struct SettingMetaInt : public SettingMeta {
    SettingMetaInt(const std::string& key)
        : SettingMeta(key, SETTING_INT),
          default_value(0),
          min_value(std::numeric_limits<int>::min()),
          max_value(std::numeric_limits<int>::max()) {}
    virtual ~SettingMetaInt() {}

    int default_value;
    int min_value;
    int max_value;
    std::string unit;

    bool IsValid(const SettingDataInt& data) const { return data.value >= this->min_value && data.value <= this->max_value; }

   protected:
    virtual bool Equal(const SettingMeta& other) const;
};

struct SettingMetaFloat : public SettingMeta {
    SettingMetaFloat(const std::string& key)
        : SettingMeta(key, SETTING_FLOAT), default_value(0.0f), min_value(0.0f), max_value(0.0f), precision(0), width(0) {}
    virtual ~SettingMetaFloat() {}

    float default_value;
    float min_value;
    float max_value;
    int precision;
    int width;
    std::string unit;

    bool HasRange() const { return std::abs(this->max_value - this->min_value) > std::numeric_limits<float>::epsilon(); }

    bool HasPrecision() const { return this->precision != 0 || this->width != 0; }

    std::string GetFloatFormat() const {
        if (this->HasPrecision()) {
            return "%" + format("%d.%df", this->width, this->precision);
        } else {
            return "%f";
        }
    }

    bool IsValid(const SettingDataFloat& data) const {
        if (this->HasRange())
            return data.value >= this->min_value && data.value <= this->max_value;
        else
            return true;
    }

   protected:
    virtual bool Equal(const SettingMeta& other) const;
};

struct SettingMetaBool : public SettingMeta {
    SettingMetaBool(const std::string& key) : SettingMeta(key, SETTING_BOOL), default_value(false) {}
    virtual ~SettingMetaBool() {}

    bool default_value;

   protected:
    virtual bool Equal(const SettingMeta& other) const;
};

struct SettingMetaBoolNumeric : public SettingMeta {
    SettingMetaBoolNumeric(const std::string& key) : SettingMeta(key, SETTING_BOOL_NUMERIC_DEPRECATED), default_value(false) {}
    virtual ~SettingMetaBoolNumeric() {}

    bool default_value;
};

struct SettingMetaFrames : public SettingMetaString {
    SettingMetaFrames(const std::string& key) : SettingMetaString(key, SETTING_FRAMES) {}
    virtual ~SettingMetaFrames() {}

    bool IsValid(const SettingDataFrames& data) const { return IsFrames(data.value) || data.value.empty(); }
};

struct SettingMetaFilesystem : public SettingMeta {
    SettingMetaFilesystem(const std::string& key, const SettingType& setting_type) : SettingMeta(key, setting_type) {}
    virtual ~SettingMetaFilesystem() {}

    std::string default_value;
    std::string filter;

   protected:
    virtual bool Equal(const SettingMeta& other) const;
};

struct SettingMetaFileLoad : public SettingMetaFilesystem {
    SettingMetaFileLoad(const std::string& key) : SettingMetaFilesystem(key, SETTING_LOAD_FILE) {}
    virtual ~SettingMetaFileLoad() {}
};

struct SettingMetaFileSave : public SettingMetaFilesystem {
    SettingMetaFileSave(const std::string& key) : SettingMetaFilesystem(key, SETTING_SAVE_FILE) {}
    virtual ~SettingMetaFileSave() {}
};

struct SettingMetaFolderSave : public SettingMetaFilesystem {
    SettingMetaFolderSave(const std::string& key) : SettingMetaFilesystem(key, SETTING_SAVE_FOLDER) {}
    virtual ~SettingMetaFolderSave() {}
};

struct SettingEnumValue : public Header {
    std::string key;
};

bool operator==(const SettingEnumValue& a, const SettingEnumValue& b);
inline bool operator!=(const SettingEnumValue& a, const SettingEnumValue& b) { return !(a == b); }

struct SettingMetaEnumeration : public SettingMeta {
    SettingMetaEnumeration(const std::string& key, const SettingType& setting_type) : SettingMeta(key, setting_type) {}
    virtual ~SettingMetaEnumeration() {}

    std::vector<SettingEnumValue> enum_values;

   protected:
    virtual bool Equal(const SettingMeta& other) const;
};

struct SettingMetaEnum : public SettingMetaEnumeration {
    SettingMetaEnum(const std::string& key) : SettingMetaEnumeration(key, SETTING_ENUM) {}
    virtual ~SettingMetaEnum() {}

    std::string default_value;

   protected:
    virtual bool Equal(const SettingMeta& other) const;
};

struct SettingMetaFlags : public SettingMetaEnumeration {
    SettingMetaFlags(const std::string& key) : SettingMetaEnumeration(key, SETTING_FLAGS) {}
    virtual ~SettingMetaFlags() {}

    std::vector<std::string> default_value;

   protected:
    virtual bool Equal(const SettingMeta& other) const;
};

struct SettingMetaList : public SettingMeta {
    SettingMetaList(const std::string& key) : SettingMeta(key, SETTING_LIST), list_only(false) {}
    virtual ~SettingMetaList() {}

    std::vector<NumberOrString> list;
    std::vector<EnabledNumberOrString> default_value;
    bool list_only;

   protected:
    virtual bool Equal(const SettingMeta& other) const;
};

const SettingMeta* FindSettingMeta(const SettingMetaSet& settings, const char* key);

std::size_t CountSettings(const SettingMetaSet& settings);
