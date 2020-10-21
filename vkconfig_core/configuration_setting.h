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

#include <QJsonObject>

#include <string>
#include <vector>

struct ConfigurationSetting {
    std::string key;

    virtual SettingType GetType() const = 0;
    virtual bool Load(const QJsonObject& json_setting) = 0;
    virtual bool Save(QJsonObject& json_setting) const = 0;
};

template <SettingType SETTING_TYPE, typename T>
struct ConfigurationSettingValue : public ConfigurationSetting {
    T value;

    virtual SettingType GetType() const { return SETTING_TYPE; }

    virtual bool Load(const QJsonObject& json_setting);
    virtual bool Save(QJsonObject& json_setting) const;
};

template <SettingType SETTING_TYPE, typename T>
inline bool ConfigurationSettingValue<SETTING_TYPE, T>::Load(const QJsonObject& json_setting) {
    return true;
}

template <SettingType SETTING_TYPE, typename T>
inline bool ConfigurationSettingValue<SETTING_TYPE, T>::Save(QJsonObject& json_setting) const {
    json_setting.insert("key", this->key.c_str());
    json_setting.insert("value", this->value);

    return true;
}

template <SettingType SETTING_TYPE, typename T>
inline bool operator==(const ConfigurationSettingValue<SETTING_TYPE, T>& l, const ConfigurationSettingValue<SETTING_TYPE, T>& r) {
    if (l.key != r.key)
        return false;
    else if (l.value != r.value)
        return false;
    return true;
}

template <SettingType SETTING_TYPE, typename T>
inline bool operator!=(const ConfigurationSettingValue<SETTING_TYPE, T>& l, const ConfigurationSettingValue<SETTING_TYPE, T>& r) {
    return !(l == r);
}

template <SettingType SETTING_TYPE, typename T>
struct ConfigurationSettingRange : public ConfigurationSetting {
    T min_value;
    T max_value;

    virtual SettingType GetType() const { return SETTING_TYPE; }

    virtual bool Load(const QJsonObject& json_setting);
    virtual bool Save(QJsonObject& json_setting) const;
};

template <SettingType SETTING_TYPE, typename T>
bool ConfigurationSettingRange<SETTING_TYPE, T>::Save(QJsonObject& json_setting) const {
    json_setting.insert("key", this->key.c_str());
    json_setting.insert("min_value", this->min_value);
    json_setting.insert("max_value", this->max_value);

    return true;
}

template <SettingType SETTING_TYPE, typename T>
inline bool operator==(const ConfigurationSettingRange<SETTING_TYPE, T>& l, const ConfigurationSettingRange<SETTING_TYPE, T>& r) {
    if (l.key != r.key)
        return false;
    else if (l.min_value != r.min_value)
        return false;
    else if (l.max_value != r.max_value)
        return false;
    return true;
}

template <SettingType SETTING_TYPE, typename T>
inline bool operator!=(const ConfigurationSettingRange<SETTING_TYPE, T>& l, const ConfigurationSettingRange<SETTING_TYPE, T>& r) {
    return !(l == r);
}

template <SettingType SETTING_TYPE, typename T>
struct ConfigurationSettingVector : public ConfigurationSetting {
    std::vector<T> values;

    virtual SettingType GetType() const { return SETTING_TYPE; }

    virtual bool Load(const QJsonObject& json_setting);
    virtual bool Save(QJsonObject& json_setting) const;
};

template <SettingType SETTING_TYPE, typename T>
bool ConfigurationSettingVector<SETTING_TYPE, T>::Save(QJsonObject& json_setting) const {
    json_setting.insert("key", this->key.c_str());

    QJsonArray json_values;
    for (std::size_t i = 0, n = values.size(); i < n; ++i) {
        json_values.append(this->values[i]);
    }

    json_setting.insert("values", json_values);

    return true;
}

template <SettingType SETTING_TYPE, typename T>
inline bool operator==(const ConfigurationSettingVector<SETTING_TYPE, T>& l, const ConfigurationSettingVector<SETTING_TYPE, T>& r) {
    if (l.key != r.key) return false;
    if (l.values.size() != r.values.size()) return false;
    for (std::size_t i = 0, n = l.values.size(); i < n; ++i) {
        if (l.values[i] != r.values[i]) return false;
    }

    return true;
}

template <SettingType SETTING_TYPE, typename T>
inline bool operator!=(const ConfigurationSettingVector<SETTING_TYPE, T>& l, const ConfigurationSettingVector<SETTING_TYPE, T>& r) {
    return !(l == r);
}
