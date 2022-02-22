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

#include "setting_string.h"

struct SettingMetaFilesystem : public SettingMeta {
    bool Load(const QJsonObject& json_setting) override;
    std::string Export(ExportMode export_mode) const override;

    std::string default_value;
    std::string filter;
    std::string format;

   protected:
    SettingMetaFilesystem(Layer& layer, const std::string& key, const SettingType& type);

    bool Equal(const SettingMeta& other) const override;
};

struct SettingDataFilesystem : public SettingDataString {
    std::string Export(ExportMode export_mode) const override;

   protected:
    SettingDataFilesystem(const std::string& key, const SettingType& type);
};

struct SettingMetaFileLoad : public SettingMetaFilesystem {
    static const SettingType TYPE;

    SettingData* Instantiate() override;

   private:
    SettingMetaFileLoad(Layer& layer, const std::string& key);

    friend class Layer;
};

struct SettingDataFileLoad : public SettingDataFilesystem {
    SettingDataFileLoad(const SettingMetaFileLoad* meta);

    void Copy(const SettingData* data) override;
    bool Load(const QJsonObject& json_setting) override;
    void Reset() override;

    std::vector<std::string> profile_names;

   private:
    const SettingMetaFileLoad* meta;
};

struct SettingMetaFileSave : public SettingMetaFilesystem {
    static const SettingType TYPE;

    SettingData* Instantiate() override;

   private:
    SettingMetaFileSave(Layer& layer, const std::string& key);

    friend class Layer;
};

struct SettingDataFileSave : public SettingDataFilesystem {
    SettingDataFileSave(const SettingMetaFileSave* meta);

    void Copy(const SettingData* data) override;
    void Reset() override;

   private:
    const SettingMetaFileSave* meta;
};

struct SettingMetaFolderSave : public SettingMetaFilesystem {
    static const SettingType TYPE;

    SettingData* Instantiate() override;

   private:
    SettingMetaFolderSave(Layer& layer, const std::string& key);

    friend class Layer;
};

struct SettingDataFolderSave : public SettingDataFilesystem {
    SettingDataFolderSave(const SettingMetaFolderSave* meta);

    void Copy(const SettingData* data) override;
    void Reset() override;

   private:
    const SettingMetaFolderSave* meta;
};
