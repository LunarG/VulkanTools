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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "boolsettingwidget.h"

BoolSettingWidget::BoolSettingWidget(LayerSettings *layer_settings, bool numeric) {
    _numeric_output = numeric;
    _layer_settings = layer_settings;
    setText(layer_settings->settings_prompt);
    setToolTip(layer_settings->settings_desc);
    setChecked(layer_settings->settings_value == QString("TRUE"));
    connect(this, SIGNAL(clicked()), this, SLOT(itemToggled()));
}

void BoolSettingWidget::itemToggled() {
    if (_numeric_output) {
        if (isChecked())
            _layer_settings->settings_value = QString("1");
        else
            _layer_settings->settings_value = QString("0");
    } else {
        if (isChecked())
            _layer_settings->settings_value = QString("TRUE");
        else
            _layer_settings->settings_value = QString("FALSE");
    }

    emit itemChanged();
}
