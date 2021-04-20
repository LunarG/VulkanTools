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

#include "widget_setting.h"

#include <QString>
#include <QTreeWidgetItem>
#include <QLineEdit>
#include <QResizeEvent>
#include <QTimer>

class WidgetSettingFloat : public QWidget {
    Q_OBJECT

   public:
    WidgetSettingFloat(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaFloat& meta, SettingDataSet& data_set);
    virtual ~WidgetSettingFloat();

   public Q_SLOTS:
    void OnTextEdited(const QString& value);
    void OnErrorValue();
    void OnValidValue();

   Q_SIGNALS:
    void itemChanged();

   protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

   private:
    WidgetSettingFloat(const WidgetSettingFloat&) = delete;
    WidgetSettingFloat& operator=(const WidgetSettingFloat&) = delete;

    void Resize();
    SettingInputError ProcessInputValue();

    QTreeWidget* tree;
    QTreeWidgetItem* item;
    const SettingDataSet& data_set;
    SettingDataFloat& data;
    const SettingMetaFloat& meta;

    std::string value_buffer;
    QLineEdit* field;
    QTimer* timer_error;
    QTimer* timer_valid;
    QSize resize;
    QPalette default_palette;
};
