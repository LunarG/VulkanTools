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

#include "setting_type.h"
#include "setting_set.h"
#include "util.h"
#include "path.h"

#include <string>
#include <vector>
#include <memory>

struct SettingData {
    SettingData& operator=(const SettingData& other);

    bool operator==(const SettingData& other) const { return this->Equal(other); }
    bool operator!=(const SettingData& other) const { return !this->Equal(other); }

    const std::string key;
    const SettingType type;

   protected:
    SettingData(const std::string& key, const SettingType type);
    virtual ~SettingData() {}

    virtual bool Equal(const SettingData& other) const;
    virtual SettingData& Assign(const SettingData& other) = 0;
};

struct SettingDataGroup : public SettingData {
    SettingDataGroup(const std::string& key) : SettingData(key, SETTING_GROUP) {}
    virtual ~SettingDataGroup() {}

    virtual SettingData& Assign(const SettingData& other);
};

struct SettingDataBool : public SettingData {
    SettingDataBool(const std::string& key) : SettingData(key, SETTING_BOOL), value(false) {}
    virtual ~SettingDataBool() {}

    bool value;

   protected:
    SettingDataBool(const std::string& key, const SettingType type) : SettingData(key, type), value(false) {}

    virtual bool Equal(const SettingData& other) const;
    virtual SettingData& Assign(const SettingData& other);
};

struct SettingDataBoolNumeric : public SettingDataBool {
    SettingDataBoolNumeric(const std::string& key) : SettingDataBool(key, SETTING_BOOL_NUMERIC_DEPRECATED) {}
    virtual ~SettingDataBoolNumeric() {}
};

struct SettingDataInt : public SettingData {
    SettingDataInt(const std::string& key) : SettingData(key, SETTING_INT), value(0) {}
    virtual ~SettingDataInt() {}

    int value;

   protected:
    virtual bool Equal(const SettingData& other) const;
    virtual SettingData& Assign(const SettingData& other);
};

struct SettingDataFloat : public SettingData {
    SettingDataFloat(const std::string& key) : SettingData(key, SETTING_FLOAT), value(0) {}
    virtual ~SettingDataFloat() {}

    float value;

   protected:
    virtual bool Equal(const SettingData& other) const;
    virtual SettingData& Assign(const SettingData& other);
};

struct SettingDataString : public SettingData {
    SettingDataString(const std::string& key) : SettingDataString(key, SETTING_STRING) {}
    virtual ~SettingDataString() {}

    std::string value;

   protected:
    SettingDataString(const std::string& key, const SettingType type) : SettingData(key, type) {}

    virtual bool Equal(const SettingData& other) const;
    virtual SettingData& Assign(const SettingData& other);
};

struct SettingDataEnum : public SettingDataString {
    SettingDataEnum(const std::string& key) : SettingDataString(key, SETTING_ENUM) {}
    virtual ~SettingDataEnum() {}
};

struct SettingDataFileLoad : public SettingDataString {
    SettingDataFileLoad(const std::string& key) : SettingDataString(key, SETTING_LOAD_FILE) {}
    virtual ~SettingDataFileLoad() {}
};

struct SettingDataFileSave : public SettingDataString {
    SettingDataFileSave(const std::string& key) : SettingDataString(key, SETTING_SAVE_FILE) {}
    virtual ~SettingDataFileSave() {}
};

struct SettingDataFolderSave : public SettingDataString {
    SettingDataFolderSave(const std::string& key) : SettingDataString(key, SETTING_SAVE_FOLDER) {}
    virtual ~SettingDataFolderSave() {}
};

struct SettingDataFrames : public SettingDataString {
    SettingDataFrames(const std::string& key) : SettingDataString(key, SETTING_FRAMES) {}
    virtual ~SettingDataFrames() {}
};

struct SettingDataList : public SettingData {
    SettingDataList(const std::string& key) : SettingData(key, SETTING_LIST) {}
    virtual ~SettingDataList() {}

    std::vector<EnabledNumberOrString> value;

    virtual bool Equal(const SettingData& other) const;
    virtual SettingData& Assign(const SettingData& other);
};

struct SettingDataFlags : public SettingData {
    SettingDataFlags(const std::string& key) : SettingData(key, SETTING_FLAGS) {}
    virtual ~SettingDataFlags() {}

    std::vector<std::string> value;

    virtual bool Equal(const SettingData& other) const;
    virtual SettingData& Assign(const SettingData& other);
};

typedef SettingSet<SettingData> SettingDataSet;
