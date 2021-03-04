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

#include "settings_validation_areas.h"
#include "widget_setting_flags.h"
#include "widget_setting_filesystem.h"
#include "widget_preset.h"

#include "../vkconfig_core/configuration.h"

#include <QObject>
#include <QTreeWidget>
#include <QComboBox>

#include <vector>

class SettingsTreeManager : QObject {
    Q_OBJECT

   public:
    SettingsTreeManager();

    void CreateGUI(QTreeWidget *build_tree);
    void CleanupGUI();

    void GetTreeState(QByteArray &byte_array, QTreeWidgetItem *top_item);
    int SetTreeState(QByteArray &byte_array, int index, QTreeWidgetItem *top_item);

   public Q_SLOTS:
    void khronosDebugChanged(int index);
    void OnPresetChanged(int index);  // Okay, is this a custom guy HERE, or do we move it out
                                      // It really forces a reload of the entire branch of this tree
                                      // Reset layer defaults for the profile, and then call BuildKhronosTree again
    void OnSettingChanged();          // The profile has been edited and should be saved

   private:
    SettingsTreeManager(const SettingsTreeManager &) = delete;
    SettingsTreeManager &operator=(const SettingsTreeManager &) = delete;

    void BuildValidationTree(QTreeWidgetItem *parent, Parameter &parameter);
    void BuildGenericTree(QTreeWidgetItem *parent, Parameter &parameter);

    QTreeWidget *_settings_tree;
    std::vector<WidgetPreset *> _presets_comboboxes;

    QTreeWidgetItem *_validation_log_file_item;
    WidgetSettingFilesystem *_validation_log_file_widget;
    WidgetSettingFlag *_validation_debug_action;
    SettingsValidationAreas *_validation_areas;
};
