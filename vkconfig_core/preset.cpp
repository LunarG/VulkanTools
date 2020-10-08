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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "preset.h"

#include <QJsonArray>
#include <QJsonObject>

static int GetPlatformFlags(QString value) {
    // TODO
    return 0;
}

bool LoadLayerPresets(const QJsonValue& json_layer_presets, std::vector<Preset>& presets) {
    assert(json_layer_presets.isArray());

    const QJsonArray& json_presets = json_layer_presets.toArray();

    for (int i = 0, n = json_presets.size(); i < n; ++i) {
        Preset preset;

        const QJsonObject& json_preset = json_presets[i].toObject();

        {
            const QJsonValue& json_value = json_preset.value("index");
            assert(json_value != QJsonValue::Undefined && json_value.isString());
            preset.index = json_value.toInt();
        }
        {
            const QJsonValue& json_value = json_preset.value("label");
            assert(json_value != QJsonValue::Undefined && json_value.isString());
            preset.label = json_value.toString();
        }
        {
            const QJsonValue& json_value = json_preset.value("platform");
            assert(json_value != QJsonValue::Undefined && json_value.isArray());
            preset.platforms_flags = GetPlatformFlags(json_value.toString());
        }
        {
            const QJsonValue& json_value = json_preset.value("status");
            assert(json_value != QJsonValue::Undefined && json_value.isString());
            preset.status = GetStatusType(json_value.toString().toStdString().c_str());
        }
        {
            const QJsonValue& json_value = json_preset.value("description");
            assert(json_value != QJsonValue::Undefined && json_value.isString());
            preset.description = json_value.toString();
        }
        {
            const QJsonValue& json_value = json_preset.value("settings");
            assert(json_value != QJsonValue::Undefined && json_value.isArray());
            const QJsonArray& json_array_settings = json_value.toArray();
            for (int j = 0, m = json_array_settings.size(); j < m; ++j) {
                PresetSetting preset_setting;

                const QJsonObject& json_setting = json_array_settings[j].toObject();
                {
                    const QJsonValue& json_value = json_setting.value("key");
                    assert(json_value != QJsonValue::Undefined && json_value.isString());
                    preset_setting.key = json_value.toString();
                }
                {
                    const QJsonValue& json_value = json_setting.value("values");
                    assert(json_value != QJsonValue::Undefined && json_value.isArray());
                    // TODO
                    // preset_setting.value = json_value.toString();
                }

                preset.settings.push_back(preset_setting);
            }
        }

        presets.push_back(preset);
    }

    return true;
}