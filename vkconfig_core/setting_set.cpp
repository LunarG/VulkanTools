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

#include "setting_set.h"
#include "setting_data.h"
#include "setting_meta.h"

template <>
std::shared_ptr<SettingData> SettingSet<SettingData>::AllocSetting(const std::string& key, SettingType type) const {
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
        case SETTING_FRAMES:
            return std::shared_ptr<SettingData>(new SettingDataFrames(key));
        case SETTING_LIST:
            return std::shared_ptr<SettingData>(new SettingDataList(key));
        default:
            assert(0);
            return std::shared_ptr<SettingData>();
    }
}

template <>
std::shared_ptr<SettingMeta> SettingSet<SettingMeta>::AllocSetting(const std::string& key, SettingType type) const {
    assert(!key.empty());

    switch (type) {
        case SETTING_STRING:
            return std::shared_ptr<SettingMeta>(new SettingMetaString(key));
        case SETTING_INT:
            return std::shared_ptr<SettingMeta>(new SettingMetaInt(key));
        case SETTING_SAVE_FILE:
            return std::shared_ptr<SettingMeta>(new SettingMetaFileSave(key));
        case SETTING_LOAD_FILE:
            return std::shared_ptr<SettingMeta>(new SettingMetaFileLoad(key));
        case SETTING_SAVE_FOLDER:
            return std::shared_ptr<SettingMeta>(new SettingMetaFolderSave(key));
        case SETTING_BOOL:
            return std::shared_ptr<SettingMeta>(new SettingMetaBool(key));
        case SETTING_BOOL_NUMERIC_DEPRECATED:
            return std::shared_ptr<SettingMeta>(new SettingMetaBoolNumeric(key));
        case SETTING_ENUM:
            return std::shared_ptr<SettingMeta>(new SettingMetaEnum(key));
        case SETTING_FLAGS:
            return std::shared_ptr<SettingMeta>(new SettingMetaFlags(key));
        case SETTING_FRAMES:
            return std::shared_ptr<SettingMeta>(new SettingMetaFrames(key));
        case SETTING_LIST:
            return std::shared_ptr<SettingMeta>(new SettingMetaList(key));
        default:
            assert(0);
            return std::shared_ptr<SettingMeta>();
    }
}
