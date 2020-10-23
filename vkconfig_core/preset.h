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

#include "platform.h"

#include <QString>
#include <QVariant>
#include <QJsonValue>

#include <vector>
#include <cassert>

enum { PRESET_INDEX_USER_DEFINED = -1 };

struct PresetSetting {
    QString key;
    std::vector<QVariant> value;
};

struct Preset {
    Preset();

    int index;
    QString label;
    int platforms_flags;  // PlatformFlags
    StatusType status;
    QString description;
    std::vector<PresetSetting> settings;
};

bool LoadLayerPresets(const QJsonValue& json_layer_presets, std::vector<Preset>& presets);

inline typename std::vector<PresetSetting>::iterator Find(std::vector<PresetSetting>& container, const QString& key) {
    assert(!key.isEmpty());

    for (auto it = container.begin(), end = container.end(); it != end; ++it)
        if (it->key == key) return it;
    return container.end();
}

inline typename std::vector<PresetSetting>::const_iterator Find(const std::vector<PresetSetting>& container, const QString& key) {
    assert(!key.isEmpty());

    for (auto it = container.begin(), end = container.end(); it != end; ++it) {
        if (it->key == key) return it;
    }

    return container.end();
}
