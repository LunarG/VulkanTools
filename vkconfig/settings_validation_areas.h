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

#include "../vkconfig_core/layer.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QRadioButton>

#include <vector>

class SettingsValidationAreas : public QObject {
    Q_OBJECT

   public:
    explicit SettingsValidationAreas(QTreeWidget *main_tree, QTreeWidgetItem *parent, const Version &version,
                                     const SettingMetaSet &settings_meta, SettingDataSet &settings_data);

    bool CollectSettings();

   private:
    QTreeWidget *_main_tree_widget;
    QTreeWidgetItem *_main_parent;
    QTreeWidgetItem *_core_checks_parent;

    QTreeWidgetItem *_synchronization_box;
    QTreeWidgetItem *_shader_based_box;
    QTreeWidgetItem *_gpu_assisted_box;
    QRadioButton *_gpu_assisted_radio;
    QTreeWidgetItem *_gpu_assisted_reserve_box;
    QTreeWidgetItem *_gpu_assisted_oob_box;
    QTreeWidgetItem *_debug_printf_box;
    QRadioButton *_debug_printf_radio;
    QTreeWidgetItem *_debug_printf_to_stdout;
    QTreeWidgetItem *_debug_printf_verbose;
    QTreeWidgetItem *_debug_printf_buffer_size;

   public Q_SLOTS:
    void itemChanged(QTreeWidgetItem *item, int column);
    void itemClicked(QTreeWidgetItem *item, int column);
    void gpuToggled(bool toggle);
    void printfToggled(bool toggle);

   Q_SIGNALS:
    void settingChanged();

   private:
    SettingsValidationAreas(const SettingsValidationAreas &) = delete;
    SettingsValidationAreas &operator=(const SettingsValidationAreas &) = delete;

    bool HasEnable(const char *token) const;
    bool HasDisable(const char *token) const;

    QTreeWidgetItem *CreateSettingWidget(QTreeWidgetItem *parent, const char *key) const;

    void StoreBoolSetting(QTreeWidgetItem *setting_data, const char *key);

    void EnableSettingWidget(QTreeWidgetItem *setting_data, bool enable);

    const Version version;
    const SettingMetaSet &settings_meta;
    SettingDataSet &settings_data;
};
