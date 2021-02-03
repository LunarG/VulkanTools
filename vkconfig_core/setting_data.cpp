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

#include "setting_data.h"

std::shared_ptr<SettingData> CreateSettingData(const std::string& key, SettingType type) {
    assert(!key.empty());

    switch (type) {
        case SETTING_STRING:
            return std::shared_ptr<SettingData>(new SettingDataString(key));
        case SETTING_INT:
            return std::shared_ptr<SettingData>(new SettingDataInt(key));
        case SETTING_SAVE_FILE:
            return std::shared_ptr<SettingData>(new SettingDataFileSave(key));
        case SETTING_LOAD_FILE:
            return std::shared_ptr<SettingData>(new SettingDataFileLoad(key));
        case SETTING_SAVE_FOLDER:
            return std::shared_ptr<SettingData>(new SettingDataFolderSave(key));
        case SETTING_BOOL:
            return std::shared_ptr<SettingData>(new SettingDataBool(key));
        case SETTING_BOOL_NUMERIC_DEPRECATED:
            return std::shared_ptr<SettingData>(new SettingDataBoolNumeric(key));
        case SETTING_ENUM:
            return std::shared_ptr<SettingData>(new SettingDataEnum(key));
        case SETTING_FLAGS:
            return std::shared_ptr<SettingData>(new SettingDataFlags(key));
        case SETTING_INT_RANGE:
            return std::shared_ptr<SettingData>(new SettingDataIntRange(key));
        case SETTING_VUID_FILTER:
            return std::shared_ptr<SettingData>(new SettingDataVUIDFilter(key));
        default:
            assert(0);
            return std::shared_ptr<SettingData>();
    }
}

SettingData& SettingDataSet::Create(const std::string& key, SettingType type) {
    assert(!key.empty());
    assert(type >= SETTING_FIRST && type <= SETTING_LAST);

    // Find existing setting, avoid duplicated setting
    {
        SettingData* setting_data = this->Get(key.c_str());
        if (setting_data != nullptr) {
            return *setting_data;
        }
    }

    // Create a new setting
    {
        this->data.push_back(CreateSettingData(key, type));
        SettingData* setting_data = this->Get(key.c_str());
        assert(setting_data != nullptr);
        return *setting_data;
    }
}

SettingData* SettingDataSet::Get(const char* key) {
    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        if (std::strcmp(this->data[i]->GetKey(), key) == 0) return this->data[i].get();
    }

    return nullptr;
}

const SettingData* SettingDataSet::Get(const char* key) const {
    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        if (std::strcmp(this->data[i]->GetKey(), key) == 0) return this->data[i].get();
    }

    return nullptr;
}
