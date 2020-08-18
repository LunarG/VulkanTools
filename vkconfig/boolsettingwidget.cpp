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

#include <cassert>

BoolSettingWidget::BoolSettingWidget(LayerSetting& layer_setting, bool numeric_output) 
    : _layer_setting(layer_setting)
    , _numeric_output(numeric_output) {
    assert(&_layer_setting);

    setText(_layer_setting.label);
    setToolTip(_layer_setting.description);
    setChecked(_layer_setting.value == QString("TRUE"));
    connect(this, SIGNAL(clicked()), this, SLOT(itemToggled()));
}

void BoolSettingWidget::itemToggled() {
    if (_numeric_output) {
        if (isChecked())
            _layer_setting.value = QString("1");
        else
            _layer_setting.value = QString("0");
    } else {
        if (isChecked())
            _layer_setting.value = QString("TRUE");
        else
            _layer_setting.value = QString("FALSE");
    }

    emit itemChanged();
}
