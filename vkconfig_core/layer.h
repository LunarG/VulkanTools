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
 * The vkConfig2 program monitors and adjusts the Vulkan configuration
 * environment. These settings are wrapped in this class, which serves
 * as the "model" of the system.
 *
 * Authors:
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "setting_meta.h"
#include "layer_preset.h"
#include "layer_type.h"
#include "version.h"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include <vector>
#include <string>

class Layer {
   public:
    static const char* NO_PRESET;

    Layer();
    Layer(const std::string& key, const LayerType layer_type);
    Layer(const std::string& key, const LayerType layer_type, const Version& file_format_version, const Version& api_version,
          const std::string& implementation_version, const std::string& library_path, const std::string& type);

    bool IsValid() const;

    std::string FindPresetLabel(const SettingDataSet& settings) const;

   public:
    std::string key;
    Version file_format_version;
    std::string _library_path;  // This is a relative path, straight out of the json
    Version _api_version;
    std::string _implementation_version;
    StatusType status;
    std::string description;
    std::string url;

    SettingMetaSet settings;
    std::vector<LayerPreset> presets;

    std::string _layer_path;  // Actual path to the folder that contains the layer (this is important!)
    LayerType _layer_type;

    // File based layers
    bool Load(const std::string& full_path_to_file, LayerType layer_type);

    void AddSettings(SettingMetaSet& settings, const QJsonValue& json_settings_value);
};

void InitSettingDefaultValue(SettingData& setting_data, const SettingMeta& setting_meta);
SettingDataSet CollectDefaultSettingData(const SettingMetaSet& layer_settings);
