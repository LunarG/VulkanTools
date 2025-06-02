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
#include "setting_flags.h"

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
        if (setting_data->type == SETTING_FLAGS && condition.setting->type == SETTING_FLAGS) {
            const SettingDataFlags* settings_flags = static_cast<const SettingDataFlags*>(setting_data);
            const SettingDataFlags* condition_flags = static_cast<const SettingDataFlags*>(condition.setting);

            if (condition.op == CONDITION_OPERATOR_NOT) {
                bool all_not_found = true;

                for (std::size_t i = 0, n = condition_flags->value.size(); i < n; ++i) {
                    bool found = std::find(settings_flags->value.begin(), settings_flags->value.end(), condition_flags->value[i]) !=
                                 settings_flags->value.end();

                    if (found) {
                        all_not_found = false;
                        break;
                    }
                }

                if (!all_not_found) {
                    result = false;
                    break;
                }
            } else {
                bool all_found = true;

                for (std::size_t i = 0, n = condition_flags->value.size(); i < n; ++i) {
                    bool found = std::find(settings_flags->value.begin(), settings_flags->value.end(), condition_flags->value[i]) !=
                                 settings_flags->value.end();

                    if (!found) {
                        all_found = false;
                        break;
                    }
                }

                if (!all_found) {
                    result = false;
                    break;
                }
            }
        } else {
            if (condition.op == CONDITION_OPERATOR_NOT) {
                if (*condition.setting == *setting_data) {
                    result = false;
                    break;
                }
            } else {
                if (*condition.setting != *setting_data) {
                    result = false;
                    break;
                }
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

        const std::vector<Action>& actions = this->actions[i].actions;
        for (std::size_t j = 0, o = actions.size(); j < o; ++j) {
            const Action& action = actions[j];

            SettingData* setting_data = ::FindSetting<SettingData>(data_set, action.setting->key.c_str());
            const ActionOperatorType op = setting_data->type == SETTING_FLAGS ? action.op : ACTION_OPERATOR_SET;
            switch (action.op) {
                default:
                case ACTION_OPERATOR_SET:
                    if (*action.setting != *setting_data) {
                        setting_data->Copy(action.setting);
                    }
                    break;
                case ACTION_OPERATOR_APPEND: {
                    SettingDataFlags* flags = static_cast<SettingDataFlags*>(setting_data);
                    flags->Append(static_cast<SettingDataFlags*>(action.setting));
                } break;
                case ACTION_OPERATOR_REMOVE: {
                    SettingDataFlags* flags = static_cast<SettingDataFlags*>(setting_data);
                    flags->Remove(static_cast<SettingDataFlags*>(action.setting));
                } break;
            }
        }
    }
}
