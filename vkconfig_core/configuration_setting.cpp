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

#include "configuration_setting.h"
#include "layer_setting.h"

#include <QJsonArray>

#include <cassert>

ConfigurationSetting::ConfigurationSetting(const LayerSetting& layer_setting)
    : key(layer_setting.key), value(layer_setting.default_value) {}

bool ConfigurationSetting::Load(const SettingType type, const QJsonObject& json_setting) {
    const QJsonValue& json_key = json_setting.value("key");
    assert(json_key != QJsonValue::Undefined);
    this->key = json_key.toString();

    return this->value.Load("value", type, json_setting);
}

bool ConfigurationSetting::Save(const SettingType type, QJsonObject& json_setting) const {
    json_setting.insert("key", this->key);

    return this->value.Save("value", type, json_setting);
}

bool operator==(const ConfigurationSetting& l, const ConfigurationSetting& r) {
    if (l.key != r.key) return false;

    if (l.value != r.value) return false;

    return true;
}

bool operator!=(const ConfigurationSetting& l, const ConfigurationSetting& r) { return !(l == r); }
