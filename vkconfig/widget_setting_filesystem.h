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

#include "../vkconfig_core/setting_filesystem.h"

#include "widget_setting.h"

#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>

class WidgetSettingFilesystem : public WidgetSettingBase {
    Q_OBJECT

   public:
    explicit WidgetSettingFilesystem(QTreeWidget *tree, QTreeWidgetItem *item, const SettingMetaFilesystem &meta,
                                     SettingDataSet &data_set);

    void Refresh(RefreshAreas refresh_areas) override;

   public Q_SLOTS:
    void browseButtonClicked();
    void textFieldChanged(const QString &value);

   Q_SIGNALS:
    void itemChanged();

   protected:
    void resizeEvent(QResizeEvent *event) override;

   private:
    SettingDataString &data();
    void LoadFile(const std::string& path);

    const SettingMetaFilesystem &meta;
    SettingDataSet &data_set;

    QTreeWidgetItem *item_child;
    QLineEdit *field;
    QPushButton *button;
};
