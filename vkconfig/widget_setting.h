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

#include "../vkconfig_core/setting_meta.h"
#include "../vkconfig_core/setting_data.h"

#include <QTreeWidgetItem>
#include <QWidget>

class WidgetSetting : public QWidget {
   public:
    explicit WidgetSetting(QTreeWidget* tree, QTreeWidgetItem* parent, const SettingMeta& setting_meta);

    QTreeWidgetItem* GetItem() const { return this->item; }

    virtual void Enable(bool enable) = 0;

   protected:
    QTreeWidget* tree;
    QTreeWidgetItem* item;

   private:
    WidgetSetting(const WidgetSetting&) = delete;
    WidgetSetting& operator=(const WidgetSetting&) = delete;
};

void EnableItem(QTreeWidgetItem* item, bool enable);
