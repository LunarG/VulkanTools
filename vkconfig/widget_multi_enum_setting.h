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

#pragma once

#include "../vkconfig_core/layer_setting_data.h"

#include <QWidget>
#include <QCheckBox>
#include <QTreeWidgetItem>

class MultiEnumSettingWidget : public QCheckBox {
    Q_OBJECT
   public:
    explicit MultiEnumSettingWidget(LayerSettingData& setting, const char* setting_value);

   public Q_SLOTS:
    void itemChecked(bool checked);

   Q_SIGNALS:
    void itemChanged();

   private:
    MultiEnumSettingWidget(const MultiEnumSettingWidget&) = delete;
    MultiEnumSettingWidget& operator=(const MultiEnumSettingWidget&) = delete;

    LayerSettingData& setting;
    std::string setting_value;
};
