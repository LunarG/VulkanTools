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

#include "../vkconfig_core/setting_data.h"
#include "../vkconfig_core/setting_meta.h"

#include <QWidget>
#include <QCheckBox>
#include <QTreeWidgetItem>

class WidgetSettingFlags : public QCheckBox {
    Q_OBJECT
   public:
    explicit WidgetSettingFlags(const SettingMetaFlags& setting_meta, SettingDataFlags& setting_data,
                                const std::string& setting_value);

   public Q_SLOTS:
    void itemChecked(bool checked);

   Q_SIGNALS:
    void itemChanged();

   private:
    WidgetSettingFlags(const WidgetSettingFlags&) = delete;
    WidgetSettingFlags& operator=(const WidgetSettingFlags&) = delete;

    const SettingMetaFlags& setting_meta;
    SettingDataFlags& setting_data;
    std::string setting_enum_value;
};
