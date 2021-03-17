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

#include "setting_data.h"

#include <cassert>

SettingData::SettingData(const std::string& key, const SettingType type) : key(key), type(type) { assert(!this->key.empty()); }

SettingData& SettingData::operator=(const SettingData& other) {
    this->Assign(other);
    return *this;
}

bool SettingData::Equal(const SettingData& other) const {
    if (this->key != other.key)
        return false;
    else if (this->type != other.type)
        return false;
    return true;
}

bool SettingDataBool::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    return this->value == static_cast<const SettingDataBool&>(other).value;
}

SettingData& SettingDataBool::Assign(const SettingData& other) {
    assert(this->type == other.type);

    this->value = static_cast<const SettingDataBool&>(other).value;
    return *this;
}

bool SettingDataInt::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    return this->value == static_cast<const SettingDataInt&>(other).value;
}

SettingData& SettingDataInt::Assign(const SettingData& other) {
    assert(this->type == other.type);

    this->value = static_cast<const SettingDataInt&>(other).value;
    return *this;
}

bool SettingDataString::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    return this->value == static_cast<const SettingDataString&>(other).value;
}

SettingData& SettingDataString::Assign(const SettingData& other) {
    assert(this->type == other.type);

    this->value = static_cast<const SettingDataString&>(other).value;
    return *this;
}

bool SettingDataList::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    const SettingDataList& data = static_cast<const SettingDataList&>(other);

    if (this->values.size() != data.values.size()) return false;

    for (std::size_t i = 0, n = this->values.size(); i < n; ++i) {
        const EnabledString& value = this->values[i];

        auto it = std::find(data.values.begin(), data.values.end(), value);
        if (it == data.values.end()) return false;
    }

    return true;
}

SettingData& SettingDataList::Assign(const SettingData& other) {
    assert(this->type == other.type);

    const SettingDataList& data = static_cast<const SettingDataList&>(other);

    this->values = data.values;
    return *this;
}

bool SettingDataFlags::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    const SettingDataFlags& data = static_cast<const SettingDataFlags&>(other);

    if (this->value.size() != data.value.size()) return false;

    for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
        const std::string& value = this->value[i];

        auto it = std::find(data.value.begin(), data.value.end(), value);
        if (it == data.value.end()) return false;
    }

    return true;
}

SettingData& SettingDataFlags::Assign(const SettingData& other) {
    assert(this->type == other.type);

    this->value = static_cast<const SettingDataFlags&>(other).value;
    return *this;
}
