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
#include <QResizeEvent>

class WidgetSettingInt : public QWidget {
    Q_OBJECT

   public:
    WidgetSettingInt(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaInt& setting_meta, SettingDataInt& setting_data);

    void Enable(bool enable);
    QLineEdit* GetField() const { return this->field; }

   public Q_SLOTS:
    void OnTextEdited(const QString& newString);
    void FieldEditedCheck();

   Q_SIGNALS:
    void itemChanged();

   private:
    WidgetSettingInt(const WidgetSettingInt&) = delete;
    WidgetSettingInt& operator=(const WidgetSettingInt&) = delete;

    void resizeEvent(QResizeEvent* event) override;

    void Resize();

    const SettingMetaInt& setting_meta;
    SettingDataInt& setting_data;
    QLineEdit* field;
    QSize resize;
};
