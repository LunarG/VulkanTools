/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

#include "../vkconfig_core/setting_data.h"
#include "../vkconfig_core/setting_meta.h"

#include <QString>
#include <QTreeWidgetItem>
#include <QLineEdit>

class WidgetSettingFrames : public QLineEdit {
    Q_OBJECT

   public:
    WidgetSettingFrames(QTreeWidgetItem* item, const SettingMetaFrames& setting_meta, SettingDataFrames& setting_data);

   public Q_SLOTS:
    void itemEdited(const QString& value);
    void FieldEditedCheck();

   Q_SIGNALS:
    void itemChanged();

   private:
    WidgetSettingFrames(const WidgetSettingFrames&) = delete;
    WidgetSettingFrames& operator=(const WidgetSettingFrames&) = delete;

    const SettingMetaFrames& setting_meta;
    SettingDataFrames& setting_data;
};
