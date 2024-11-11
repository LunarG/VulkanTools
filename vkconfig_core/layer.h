/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "setting.h"
#include "layer_preset.h"
#include "version.h"
#include "path.h"
#include "type_layer_type.h"
#include "type_layer_control.h"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include <vector>
#include <string>

struct LayersPathInfo {
    Path path;
    LayerType type = LAYER_TYPE_EXPLICIT;
    bool enabled = true;
};

bool operator<(const LayersPathInfo& a, const LayersPathInfo& b);

bool Found(const std::vector<LayersPathInfo>& data, const Path& path);

enum LayerLoadStatus {
    LAYER_LOAD_ADDED = 0,
    LAYER_LOAD_RELOADED,
    LAYER_LOAD_UNMODIFIED,
    LAYER_LOAD_FAILED,
    LAYER_LOAD_INVALID,
    LAYER_LOAD_IGNORED,

    LAYER_LOAD_FIRST = LAYER_LOAD_ADDED,
    LAYER_LOAD_LAST = LAYER_LOAD_IGNORED,
};

enum { LAYER_LOAD_COUNT = LAYER_LOAD_LAST - LAYER_LOAD_FIRST + 1 };

class Layer {
   public:
    static const char* NO_PRESET;

    Layer();
    Layer(const std::string& key);
    Layer(const std::string& key, const Version& file_format_version, const Version& api_version,
          const std::string& implementation_version, const std::string& library_path);

    bool IsValid() const;

    LayerControl GetActualControl() const;
    std::string GetActualControlTooltip() const;

    std::string FindPresetLabel(const SettingDataSet& settings) const;

    SettingMeta* Instantiate(SettingMetaSet& meta_set, const std::string& key, const SettingType type);

    void AddSettingData(SettingDataSet& data_set, const QJsonValue& json_setting_value);

    void AddSettingsSet(SettingMetaSet& meta_set, const SettingMeta* parent, const QJsonValue& json_settings_value);

   public:
    std::string key;
    Version file_format_version;
    Path binary_path;
    Version api_version;
    std::string implementation_version;
    std::string validated_last_modified;
    StatusType status;
    std::string description;
    std::string introduction;
    std::string url;
    int platforms;
    Path manifest_path;
    LayerType type = LAYER_TYPE_EXPLICIT;
    QJsonDocument profile;
    std::string disable_env;
    std::string enable_env;
    std::string enable_value;
    bool is_32bits = false;
    bool enabled = true;

    std::vector<SettingMeta*> settings;
    std::vector<LayerPreset> presets;

    LayerLoadStatus Load(const Path& full_path_to_file, LayerType type, bool request_validate_manifest,
                         const std::map<Path, std::string>& layers_validated);

   private:
    Layer& operator=(const Layer&) = delete;

    bool IsBuiltIn() const;

    std::vector<std::shared_ptr<SettingMeta> > memory;  // Settings are deleted when all layers instances are deleted.
};

void CollectDefaultSettingData(const SettingMetaSet& meta_set, SettingDataSet& data_set);
