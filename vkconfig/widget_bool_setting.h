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

#include "../vkconfig_core/layer.h"

#include <QObject>
#include <QWidget>
#include <QCheckBox>
#include <QString>

class BoolSettingWidget : public QCheckBox {
    Q_OBJECT

   public:
    explicit BoolSettingWidget(LayerSetting& layer_setting, SettingType setting_type);

   public Q_SLOTS:
    void itemToggled();

   Q_SIGNALS:
    void itemChanged();

   private:
    BoolSettingWidget(const BoolSettingWidget&) = delete;
    BoolSettingWidget& operator=(const BoolSettingWidget&) = delete;

    enum Mode { BOOL_NUMERIC, BOOL_STRING };

    SettingValue GetToken(bool state, SettingType type) const;
    const SettingValue _true_token;
    const SettingValue _false_token;

    LayerSetting& _layer_setting;
};
