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

#include <QVariant>

#include <vector>

class SettingValue {
   public:
    bool Load(const char* key, const SettingType type, const QJsonObject& json_object);
    bool Save(const char* key, const SettingType type, QJsonObject& json_object) const;

    template <typename T>
    void push_back(const T& value) {
        this->data.push_back(value);
    }

    bool empty() const { return this->data.empty(); }

    std::size_t size() const { return this->data.size(); }

    template <typename T>
    bool remove(const T& value) {
        if (this->data.empty()) return false;

        std::vector<QVariant> new_data;
        new_data.reserve(this->data.size() - 1);

        bool result = false;
        for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
            if (this->data[i] == value) {
                result = true;
                continue;
            }

            new_data.push_back(this->data[i]);
        }

        if (result) std::swap(this->data, new_data);

        return result;
    }

    QVariant& operator[](std::size_t i) { return this->data[i]; }
    const QVariant& operator[](std::size_t i) const { return this->data[i]; }

   private:
    std::vector<QVariant> data;
};

bool operator==(const SettingValue& l, const SettingValue& r);
bool operator!=(const SettingValue& l, const SettingValue& r);
