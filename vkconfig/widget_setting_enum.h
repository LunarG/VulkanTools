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

#include "../vkconfig_core/setting_data.h"
#include "../vkconfig_core/setting_meta.h"

#include <QTreeWidgetItem>
#include <QResizeEvent>
#include <QComboBox>

class WidgetSettingEnum : public QWidget {
    Q_OBJECT

   public:
    explicit WidgetSettingEnum(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaEnum& meta, SettingDataSet& data_set);

   protected:
    void paintEvent(QPaintEvent* event) override;

   public Q_SLOTS:
    void indexChanged(int index);

   Q_SIGNALS:
    void itemChanged();

   private:
    WidgetSettingEnum(const WidgetSettingEnum&) = delete;
    WidgetSettingEnum& operator=(const WidgetSettingEnum&) = delete;

    void resizeEvent(QResizeEvent* event) override;

    QTreeWidget* tree;
    QTreeWidgetItem* item;
    const SettingDataSet& data_set;
    SettingDataEnum& data;
    const SettingMetaEnum& meta;
    QComboBox* field;
    std::vector<std::size_t> enum_indexes;
};
