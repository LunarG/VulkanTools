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

#include <memory>
#include <vector>
#include <cassert>

#include <QJsonObject>

enum SettingDependenceMode { SETTING_DEPENDENCE_DISABLE = 0, SETTING_DEPENDENCE_HIDE, SETTING_DEPENDENCE_ENABLE };

enum SettingInputError {
    SETTING_INPUT_NO_ERROR = 0,
    SETTING_INPUT_ERROR_EMPTY,
    SETTING_INPUT_ERROR_SYNTAX,
    SETTING_INPUT_ERROR_SEMENTICS
};

enum SettingType {  // Enum value can't be changed
    SETTING_STRING = 0,
    SETTING_INT,
    SETTING_FLOAT,
    SETTING_GROUP,
    SETTING_SAVE_FILE,
    SETTING_LOAD_FILE,
    SETTING_SAVE_FOLDER,
    SETTING_BOOL,
    SETTING_BOOL_NUMERIC_DEPRECATED,  // Deprecated
    SETTING_ENUM,
    SETTING_FLAGS,
    SETTING_FRAMES,
    SETTING_LIST,

    SETTING_FIRST = SETTING_STRING,
    SETTING_LAST = SETTING_LIST
};

enum { SETTING_COUNT = SETTING_LAST - SETTING_FIRST + 1 };

SettingType GetSettingType(const char* token);
const char* GetToken(SettingType type);

inline bool IsEnum(SettingType type) {
    assert(type >= SETTING_FIRST && type <= SETTING_LAST);

    return type == SETTING_ENUM;
}

inline bool IsFlags(SettingType type) {
    assert(type >= SETTING_FIRST && type <= SETTING_LAST);

    return type == SETTING_FLAGS;
}

enum DependenceMode {
    DEPENDENCE_NONE = 0,
    DEPENDENCE_ALL,
    DEPENDENCE_ANY,

    DEPENDENCE_FIRST = DEPENDENCE_NONE,
    DEPENDENCE_LAST = DEPENDENCE_ANY
};

enum { DEPENDENCE_COUNT = DEPENDENCE_LAST - DEPENDENCE_FIRST + 1 };

const char* GetToken(DependenceMode type);
DependenceMode GetDependenceMode(const char* token);

enum ExportMode {
    EXPORT_MODE_OVERRIDE = 0,
    EXPORT_MODE_DOC,

    EXPORT_MODE_FIRST = EXPORT_MODE_OVERRIDE,
    EXPORT_MODE_LAST = EXPORT_MODE_DOC
};

enum { EXPORT_MODE_COUNT = EXPORT_MODE_LAST - EXPORT_MODE_FIRST + 1 };

enum ParseSource { PARSE_SETTING = 0, PARSE_ENV_VAR };

const char* GetToken(ParseSource type);

class Layer;
struct SettingMeta;
struct SettingData;

typedef std::vector<SettingMeta*> SettingMetaSet;
typedef std::vector<SettingData*> SettingDataSet;
typedef std::vector<const SettingData*> SettingDataSetConst;

struct SettingMeta : public Header {
    SettingMeta(Layer& layer, const std::string& key, const SettingType type);
    virtual ~SettingMeta();

    bool operator==(const SettingMeta& other) const { return this->Equal(other); }
    bool operator!=(const SettingMeta& other) const { return !this->Equal(other); }

    virtual SettingData* Instantiate() = 0;

    virtual bool Load(const QJsonObject& json_setting) = 0;
    virtual std::string Export(ExportMode export_mode) const = 0;

    const std::string key;
    const SettingType type;
    std::string env;
    SettingMetaSet children;
    SettingDataSet dependence;
    DependenceMode dependence_mode;

   protected:
    virtual bool Equal(const SettingMeta& other) const;
    Layer& layer;

    std::vector<SettingData*> instances;
};

struct SettingData {
    virtual ~SettingData() {}

    bool operator==(const SettingData& other) const { return this->Equal(other); }
    bool operator!=(const SettingData& other) const { return !this->Equal(other); }

    virtual void Reset() = 0;
    virtual void Copy(const SettingData* data) = 0;
    virtual bool Load(const QJsonObject& json_setting) = 0;
    virtual bool Save(QJsonObject& json_setting) const = 0;
    virtual std::string Export(ExportMode export_mode) const = 0;

    virtual bool IsValid() const { return true; };

    const std::string key;
    const SettingType type;

   protected:
    SettingData(const std::string& key, const SettingType& type);

    virtual bool Equal(const SettingData& other) const;

   private:
    SettingData& operator=(const SettingData& other) = delete;
};

bool IsSupported(const SettingMeta* meta);

SettingMeta* FindSetting(SettingMetaSet& settings, const char* key);

template <typename T>
T* FindSetting(SettingMetaSet& settings, const char* key) {
    return static_cast<T*>(FindSetting(settings, key));
}

const SettingMeta* FindSetting(const SettingMetaSet& settings, const char* key);

template <typename T>
const T* FindSetting(const SettingMetaSet& settings, const char* key) {
    return static_cast<const T*>(FindSetting(settings, key));
}

SettingData* FindSetting(SettingDataSet& settings, const char* key);

const SettingData* FindSetting(SettingDataSetConst& settings, const char* key);

template <typename T>
T* FindSetting(SettingDataSet& settings, const char* key) {
    return static_cast<T*>(FindSetting(settings, key));
}

const SettingData* FindSetting(const SettingDataSet& settings, const char* key);

template <typename T>
const T* FindSetting(const SettingDataSet& settings, const char* key) {
    return static_cast<const T*>(FindSetting(settings, key));
}

std::size_t CountSettings(const SettingMetaSet& settings);

bool CheckSettingOverridden(const SettingMeta& meta);

std::string GetSettingOverride(const SettingMeta& meta);

SettingDependenceMode CheckDependence(const SettingMeta& meta, const SettingDataSet& data_set);

template <typename SETTING_DATA>
inline SETTING_DATA* Instantiate(SettingMeta* meta) {
    SETTING_DATA* data = static_cast<SETTING_DATA*>(meta->Instantiate());
    assert(data->type == meta->type);
    return data;
}
