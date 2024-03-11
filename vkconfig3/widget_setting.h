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

#include "../vkconfig_core/setting.h"

#include <QTreeWidgetItem>
#include <QWidget>
#include <QFontMetrics>
#include <QString>

#include <cassert>

static const int MIN_BUTTON_SIZE = 24;
static const int ITEM_HEIGHT = 24;

enum RefreshAreas { REFRESH_ENABLE_AND_STATE = 0, REFRESH_ENABLE_ONLY };

class WidgetSettingBase : public QWidget {
    Q_OBJECT

   public:
    WidgetSettingBase(QTreeWidget* tree, QTreeWidgetItem* item);

    virtual void Refresh(RefreshAreas refresh_areas) = 0;

   private:
    WidgetSettingBase(const WidgetSettingBase&) = delete;
    WidgetSettingBase& operator=(const WidgetSettingBase&) = delete;

   protected:
    void DisplayOverride(QWidget* widget, const SettingMeta& meta) const;

    QTreeWidget* tree;
    QTreeWidgetItem* item;
};

int HorizontalAdvance(const QFontMetrics& fm, const QString& string);

std::string GetLabel(const SettingMeta& meta);
