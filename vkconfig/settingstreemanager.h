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

#include "khronossettingsadvanced.h"
#include "widget_filesystem_setting.h"
#include "widget_bool_setting.h"
#include "widget_enum_setting.h"
#include "widget_string_setting.h"
#include "widget_multi_enum_setting.h"
#include "widget_vuid_search.h"
#include "widget_mute_message.h"

#include "../vkconfig_core/configuration.h"

#include <QObject>
#include <QTreeWidget>
#include <QComboBox>

#include <vector>

class SettingsTreeManager : QObject {
    Q_OBJECT
   public:
    SettingsTreeManager();

    void CreateGUI(QTreeWidget *build_tree, Configuration *configuration);
    void CleanupGUI();

    void GetTreeState(QByteArray &byte_array, QTreeWidgetItem *top_item);
    int SetTreeState(QByteArray &byte_array, int index, QTreeWidgetItem *top_item);

   public Q_SLOTS:
    void khronosDebugChanged(int index);
    void khronosPresetChanged(int index);  // Okay, is this a custom guy HERE, or do we move it out
                                           // It really forces a reload of the entire branch of this tree
                                           // Reset layer defaults for the profile, and then call BuildKhronosTree again
    void khronosPresetEdited();            // The user has changed something from a preset, and we are now a custom setting
    void profileEdited();                  // The profile has been edited and should be saved

   private:
    SettingsTreeManager(const SettingsTreeManager &) = delete;
    SettingsTreeManager &operator=(const SettingsTreeManager &) = delete;

    void BuildKhronosTree(std::vector<LayerSetting> &settings);
    void BuildGenericTree(QTreeWidgetItem *parent, Parameter &parameter);

    int GetValidationPresentIndex(const ValidationPreset preset) const;

    QTreeWidget *_configuration_settings_tree;
    Configuration *_configuration;
    QVector<QTreeWidgetItem *> _compound_widgets;  // These have special cleanup requirements
    QVector<QTreeWidgetItem *> _layer_items;       // These parallel the profiles layers

    QComboBox *_validation_presets_combo_box;
    std::vector<ValidationPreset> _validation_presets;  // The preset in the combobox

    QTreeWidgetItem *_validation_tree_item;
    QTreeWidgetItem *_validation_file_item;
    QTreeWidgetItem *_validation_preset_item;
    QTreeWidgetItem *_validation_log_file_item;
    FileSystemSettingWidget *_validation_log_file_widget;
    MultiEnumSettingWidget *_validation_debug_action;
    KhronosSettingsAdvanced *_validation_settings;
    MuteMessageWidget *_mute_message_widget;
    VUIDSearchWidget *_vuid_search_widget;
};
