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

#include "../vkconfig_core/layer_setting_meta.h"
#include "../vkconfig_core/layer_setting_data.h"

#include <QObject>
#include <QWidget>
#include <QCheckBox>

#include <string>

class BoolSettingWidget : public QCheckBox {
    Q_OBJECT

   public:
    explicit BoolSettingWidget(const LayerSettingMeta& layer_setting_meta, LayerSettingData& layer_setting_data);

   public Q_SLOTS:
    void itemToggled();

   Q_SIGNALS:
    void itemChanged();

   private:
    BoolSettingWidget(const BoolSettingWidget&) = delete;
    BoolSettingWidget& operator=(const BoolSettingWidget&) = delete;

    enum Mode { BOOL_NUMERIC, BOOL_STRING };

    std::string GetToken(bool state, SettingType type) const;
    const std::string true_token;
    const std::string false_token;

    LayerSettingData& layer_setting_data;
};
