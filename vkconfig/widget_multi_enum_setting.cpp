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

#include "widget_multi_enum_setting.h"

#include "../vkconfig_core/layer.h"

#include <cassert>

MultiEnumSettingWidget::MultiEnumSettingWidget(LayerSetting& layer_setting, QString setting_name)
    : _layer_setting(layer_setting), _setting_name(setting_name) {
    assert(&layer_setting);
    assert(!setting_name.isEmpty());

    if (_layer_setting.value.contains(setting_name)) this->setChecked(true);

    connect(this, SIGNAL(clicked(bool)), this, SLOT(itemChecked(bool)));
}

void MultiEnumSettingWidget::itemChecked(bool checked) {
    if (checked)
        AppendString(_layer_setting.value, _setting_name);
    else
        RemoveString(_layer_setting.value, _setting_name);

    emit itemChanged();
}
