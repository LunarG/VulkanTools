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

#pragma once

#include "setting_type.h"

#include <string>
#include <memory>
#include <vector>

template <typename T>
class SettingSet {
   public:
    T& Create(const std::string& key, SettingType type) { return this->Alloc(key, type); }

    template <typename CONTRETE>
    CONTRETE& Create(const std::string& key, SettingType type) {
        return static_cast<CONTRETE&>(this->Alloc(key, type));
    }

    T* Get(const char* key) { return this->Access(key); }

    template <typename CONTRETE>
    CONTRETE* Get(const char* key) {
        return static_cast<CONTRETE*>(this->Access(key));
    }

    const T* Get(const char* key) const { return this->Access(key); }

    template <typename CONTRETE>
    const CONTRETE* Get(const char* key) const {
        return static_cast<const CONTRETE*>(this->Access(key));
    }

    bool Empty() const { return this->data.empty(); }

    std::size_t Size() const { return this->data.size(); }

    const T& operator[](std::size_t index) const {
        assert(index < data.size());

        return *data[index];
    }

   private:
    T& Alloc(const std::string& key, SettingType type) {
        assert(!key.empty());
        assert(type >= SETTING_FIRST && type <= SETTING_LAST);

        // Find existing setting, avoid duplicated setting
        {
            T* setting = this->Get(key.c_str());
            if (setting != nullptr) {
                if (setting->type == type) return *setting;
            }
        }

        // Create a new setting
        {
            this->data.push_back(AllocSetting(key, type));
            T* setting = this->Get(key.c_str());
            assert(setting != nullptr);
            return *setting;
        }
    }

    T* Access(const char* key) {
        for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
            if (this->data[i]->key == key) return this->data[i].get();
        }

        return nullptr;
    }

    const T* Access(const char* key) const {
        for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
            if (this->data[i]->key == key) return this->data[i].get();
        }

        return nullptr;
    }

    void Remove(const char* key) {
        std::vector<std::shared_ptr<T> > new_data;
        for (std::size_t i = 0, n = data.size(); i < n; ++i) {
            if (data[i]->key == key) continue;
            new_data.push_back(data[i]);
        }

        std::swap(new_data, data);
    }

    std::shared_ptr<T> AllocSetting(const std::string& key, SettingType type) const;

    std::vector<std::shared_ptr<T> > data;
};
