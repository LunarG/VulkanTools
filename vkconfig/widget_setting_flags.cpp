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

#include "widget_setting_flags.h"

#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/util.h"

#include <cassert>

WidgetSettingFlags::WidgetSettingFlags(const SettingMetaFlags& setting_meta, SettingDataFlags& setting_data,
                                       const std::string& setting_enum_value)
    : setting_meta(setting_meta), setting_data(setting_data), setting_enum_value(setting_enum_value) {
    assert(&setting_data);
    assert(!setting_enum_value.empty());

    if (std::find(setting_data.value.begin(), setting_data.value.end(), setting_enum_value) != setting_data.value.end())
        this->setChecked(true);

    connect(this, SIGNAL(clicked(bool)), this, SLOT(itemChecked(bool)));
}

void WidgetSettingFlags::itemChecked(bool checked) {
    if (checked) {
        AppendString(setting_data.value, setting_enum_value);
    } else {
        RemoveString(setting_data.value, setting_enum_value);
    }

    emit itemChanged();
}
