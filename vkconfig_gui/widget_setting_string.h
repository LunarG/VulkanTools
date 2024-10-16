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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "../vkconfig_core/setting_string.h"

#include "widget_setting.h"

#include <QResizeEvent>
#include <QLineEdit>

class WidgetSettingString : public WidgetSettingBase {
    Q_OBJECT

   public:
    WidgetSettingString(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaString& meta, SettingDataSet& data_set);

    void Refresh(RefreshAreas refresh_areas) override;

   public Q_SLOTS:
    void OnTextEdited(const QString& value);

   Q_SIGNALS:
    void itemChanged();

   protected:
    void resizeEvent(QResizeEvent* event) override;

   private:
    void Resize();
    SettingDataString& data();

    const SettingMetaString& meta;
    SettingDataSet& data_set;

    QLineEdit* field;
    QSize resize;
};
