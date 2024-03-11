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

#include "../vkconfig_core/setting_list.h"

#include "widget_setting.h"

#include <QResizeEvent>
#include <QCheckBox>
#include <QPushButton>

class WidgetSettingListElement : public WidgetSettingBase {
    Q_OBJECT
   public:
    explicit WidgetSettingListElement(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaList& meta,
                                      SettingDataSet& data_set, EnabledNumberOrString& element);

    void Refresh(RefreshAreas refresh_areas) override;

   public Q_SLOTS:
    void OnElementRemoved();
    void OnElementChecked(bool checked);

   Q_SIGNALS:
    void itemSelected(const QString& value);
    void itemChanged();

   protected:
    void resizeEvent(QResizeEvent* event) override;

   private:
    SettingDataList& data();

    const SettingMetaList& meta;
    SettingDataSet& data_set;

    EnabledNumberOrString& element;
    QPushButton* button;
    QCheckBox* field;
};
