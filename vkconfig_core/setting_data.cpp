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

bool SettingDataIntRange::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    const SettingDataIntRange& data = static_cast<const SettingDataIntRange&>(other);

    return this->min_value == data.min_value && this->max_value == data.max_value;
}

SettingData& SettingDataIntRange::Assign(const SettingData& other) {
    assert(this->type == other.type);

    this->min_value = static_cast<const SettingDataIntRange&>(other).min_value;
    this->max_value = static_cast<const SettingDataIntRange&>(other).max_value;
    return *this;
}

bool SettingDataVector::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    const SettingDataVector& data = static_cast<const SettingDataVector&>(other);

    if (this->value.size() != data.value.size()) return false;

    for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
        if (std::find(data.value.begin(), data.value.end(), this->value[i].c_str()) == data.value.end()) return false;
    }

    return true;
}

SettingData& SettingDataVector::Assign(const SettingData& other) {
    assert(this->type == other.type);

    this->value = static_cast<const SettingDataVector&>(other).value;
    return *this;
}
