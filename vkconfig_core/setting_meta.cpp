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

#include "setting_meta.h"

SettingMeta::SettingMeta(const std::string& key, const SettingType type) : key(key), type(type) {
    assert(!this->key.empty());
    assert(type >= SETTING_FIRST && type <= SETTING_LAST);
}

bool SettingMeta::Equal(const SettingMeta& other) const {
    if (this->key != other.key)
        return false;
    else if (this->type != other.type)
        return false;
    else if (this->view != other.view)
        return false;
    else if (this->env != other.env)
        return false;
    else if (this->label != other.label)
        return false;
    else if (this->description != other.description)
        return false;
    else if (this->url != other.url)
        return false;
    else if (this->status != other.status)
        return false;
    else if (this->platform_flags != other.platform_flags)
        return false;
    else if (this->children.Size() != other.children.Size())
        return false;
    else {
        for (std::size_t i = 0, n = this->children.Size(); i < n; ++i) {
            if (this->children[i] != other.children[i]) return false;
        }
    }
    return true;
}

bool SettingMetaString::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaString&>(other).default_value;
}

bool SettingMetaFloat::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    const SettingMetaFloat& meta = static_cast<const SettingMetaFloat&>(other);

    if (std::abs(this->min_value - meta.min_value) < std::numeric_limits<float>::epsilon()) return false;
    if (std::abs(this->max_value - meta.max_value) < std::numeric_limits<float>::epsilon()) return false;
    if (this->precision != meta.precision) return false;
    if (this->width != meta.width) return false;
    if (this->unit != meta.unit) return false;

    return std::abs(this->default_value - meta.default_value) < std::numeric_limits<float>::epsilon();
}

bool SettingMetaInt::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    const SettingMetaInt& meta = static_cast<const SettingMetaInt&>(other);

    if (this->min_value != meta.min_value) return false;
    if (this->max_value != meta.max_value) return false;
    if (this->unit != meta.unit) return false;

    return this->default_value == meta.default_value;
}

bool SettingMetaBool::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaBool&>(other).default_value;
}

bool SettingMetaFilesystem::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    const SettingMetaFilesystem& meta = static_cast<const SettingMetaFilesystem&>(other);

    return this->default_value == meta.default_value && this->filter == meta.filter;
}

bool SettingMetaEnumeration::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    return this->enum_values == static_cast<const SettingMetaEnumeration&>(other).enum_values;
}

bool SettingMetaEnum::Equal(const SettingMeta& other) const {
    if (!SettingMetaEnumeration::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaEnum&>(other).default_value;
}

bool SettingMetaFlags::Equal(const SettingMeta& other) const {
    if (!SettingMetaEnumeration::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaFlags&>(other).default_value;
}

bool SettingMetaList::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    const SettingMetaList& meta = static_cast<const SettingMetaList&>(other);

    if (this->list != meta.list) {
        return false;
    }

    if (this->default_value != meta.default_value) {
        return false;
    }

    return true;
}

bool operator==(const SettingEnumValue& a, const SettingEnumValue& b) {
    if (a.key != b.key) return false;
    if (a.label != b.label) return false;
    if (a.description != b.description) return false;
    if (a.url != b.url) return false;
    if (a.status != b.status) return false;
    if (a.view != b.view) return false;
    if (a.platform_flags != b.platform_flags) return false;
    return true;
}

const SettingMeta* FindSettingMeta(const SettingMetaSet& settings, const char* key) {
    const SettingMeta* setting_meta = settings.Get(key);
    if (setting_meta != nullptr) return setting_meta;

    for (std::size_t i = 0, n = settings.Size(); i < n; ++i) {
        const SettingMeta* child = FindSettingMeta(settings[i].children, key);

        if (child != nullptr) return child;
    }

    return nullptr;
}

std::size_t CountSettings(const SettingMetaSet& settings) {
    std::size_t count = settings.Size();

    for (std::size_t i = 0, n = settings.Size(); i < n; ++i) {
        count += CountSettings(settings[i].children);
    }

    return count;
}
