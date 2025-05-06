/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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

#include "message.h"
#include "setting.h"

QMessageBox::StandardButtons Message::GetStandardButtons() const {
    QMessageBox::StandardButtons buttons = QMessageBox::NoButton;

    for (std::size_t i = 0, n = this->actions.size(); i < n; ++i) {
        buttons |= GetStandardButton(this->actions[i].type);
    }

    return buttons;
}

bool Message::Triggered(const SettingDataSet& data_set) const {
    bool result = true;

    for (std::size_t i = 0, n = this->conditions.size(); i < n; ++i) {
        const Condition& condition = this->conditions[i];

        const SettingData* setting_data = ::FindSetting<SettingData>(data_set, condition.setting->key.c_str());
        if (condition.op == CONDITION_OPERATOR_NOT) {
            if (*condition.setting == *setting_data) {
                result = false;
            }
        } else {
            if (*condition.setting != *setting_data) {
                result = false;
            }
        }
    }

    return result;
}

void Message::Apply(SettingDataSet& data_set, QMessageBox::StandardButtons button) const {
    for (std::size_t i = 0, n = this->actions.size(); i < n; ++i) {
        if (GetStandardButton(this->actions[i].type) != button) {
            continue;
        }

        const SettingDataSet& settings = this->actions[i].settings;
        for (std::size_t j = 0, o = settings.size(); j < o; ++j) {
            const SettingData* actions_setting = settings[j];

            SettingData* setting_data = ::FindSetting<SettingData>(data_set, actions_setting->key.c_str());
            if (*actions_setting != *setting_data) {
                setting_data->Copy(actions_setting);
            }
        }
    }
}
