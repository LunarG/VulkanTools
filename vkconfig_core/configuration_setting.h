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
#include "setting_value.h"

#include <QJsonObject>
#include <QString>
#include <QVariant>

#include <vector>

struct LayerSetting;

struct ConfigurationSetting {
    ConfigurationSetting() {}
    ConfigurationSetting(const LayerSetting& layer_setting);

    QString key;
    SettingValue value;

    bool Load(const SettingType type, const QJsonObject& json_setting);
    bool Save(const SettingType type, QJsonObject& json_setting) const;
};

ConfigurationSetting* Find(std::vector<ConfigurationSetting>& settings, const char* key);

bool operator==(const ConfigurationSetting& l, const ConfigurationSetting& r);
bool operator!=(const ConfigurationSetting& l, const ConfigurationSetting& r);
