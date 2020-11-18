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

#include "layer_setting_type.h"

#include <QJsonObject>
#include <QVariant>

#include <string>
#include <vector>

class SettingValue {
   public:
    SettingValue() {}
    SettingValue(const char* key, const QVariant& value);
    SettingValue(const char* key, const std::vector<QVariant>& value);

    bool Load(const char* key, const SettingType type, const QJsonObject& json_object);
    bool Save(const char* key, const SettingType type, QJsonObject& json_object) const;

    const char* Key() const;

    bool Empty() const;

    std::size_t Size() const;

    template <typename T>
    void Append(const T& value) {
        this->setting_value.push_back(value);
    }

    template <typename T>
    bool Remove(const T& value) {
        if (this->setting_value.empty()) return false;

        std::vector<QVariant> new_data;
        new_data.reserve(this->setting_value.size() - 1);

        bool result = false;
        for (std::size_t i = 0, n = this->setting_value.size(); i < n; ++i) {
            if (this->setting_value[i] == value) {
                result = true;
                continue;
            }

            new_data.push_back(this->setting_value[i]);
        }

        if (result) std::swap(this->setting_value, new_data);

        return result;
    }

    template <typename T>
    bool Has(const T& value) {
        for (std::size_t i = 0, n = this->setting_value.size(); i < n; ++i) {
            if (this->setting_value[i] == value) return true;
        }
        return false;
    }

    QVariant& operator[](std::size_t i) { return this->setting_value[i]; }
    const QVariant& operator[](std::size_t i) const { return this->setting_value[i]; }

   private:
    std::string setting_key;
    std::vector<QVariant> setting_value;
};

bool operator==(const SettingValue& l, const SettingValue& r);
bool operator!=(const SettingValue& l, const SettingValue& r);
