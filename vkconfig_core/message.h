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

#pragma once

#include "type_severity.h"
#include "type_button.h"
#include "type_action.h"
#include "type_action_operator.h"
#include "type_condition_operator.h"

#include <string>
#include <vector>
#include <array>
#include <memory>

struct SettingData;
typedef std::vector<SettingData*> SettingDataSet;

struct Condition {
    ConditionOperatorType op = CONDITION_OPERATOR_NONE;
    SettingData* setting;
};

struct Action {
    ActionOperatorType op = ACTION_OPERATOR_SET;
    SettingData* setting;
};

struct Actions {
    ButtonType type = BUTTON_OK;
    std::vector<Action> actions;
};

struct Message {
    std::string key;
    std::string title;
    int version = 1;
    std::string description;
    std::string informative;
    SeverityType severity;
    std::vector<Condition> conditions;
    std::array<Actions, ACTION_COUNT> actions;
    ActionType default_action = ACTION0;

    QMessageBox::StandardButtons GetStandardButtons() const;

    bool Triggered(const SettingDataSet& data_set) const;
    void Apply(SettingDataSet& data_set, QMessageBox::StandardButtons button) const;
};
