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
 * - Richard S. Wright Jr.
 * - Christophe Riccio
 */

#include "widget_bool_setting.h"

#include <cassert>

BoolSettingWidget::BoolSettingWidget(LayerSetting& layer_setting, SettingType setting_type)
    : _true_token(GetToken(true, setting_type)), _false_token(GetToken(false, setting_type)), _layer_setting(layer_setting) {
    assert(&layer_setting);
    assert(setting_type >= SETTING_FIRST && setting_type <= SETTING_LAST);

    setText(layer_setting.label);
    setToolTip(layer_setting.description);
    setChecked(layer_setting.value == QString("TRUE"));
    connect(this, SIGNAL(clicked()), this, SLOT(itemToggled()));
}

void BoolSettingWidget::itemToggled() {
    _layer_setting.value = isChecked() ? _true_token : _false_token;

    emit itemChanged();
}

QString BoolSettingWidget::GetToken(bool state, SettingType setting_type) const {
    assert(setting_type >= SETTING_FIRST && setting_type <= SETTING_LAST);

    switch (setting_type) {
        case SETTING_BOOL:
            return state ? QString("TRUE") : QString("FALSE");
        case SETTING_BOOL_NUMERIC:
            return state ? QString("1") : QString("0");
        default:
            assert(0);
            return static_cast<Mode>(-1);
    }
}
