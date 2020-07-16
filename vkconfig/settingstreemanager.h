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

#include "configuration.h"
#include "khronossettingsadvanced.h"
#include "filenamesettingwidget.h"
#include "boolsettingwidget.h"
#include "enumsettingwidget.h"
#include "stringsettingwidget.h"
#include "foldersettingwidget.h"
#include "multienumsetting.h"
#include "mutemessagewidget.h"
#include "vuidsearchwidget.h"

#include <QObject>
#include <QTreeWidget>
#include <QComboBox>

class SettingsTreeManager : QObject {
    Q_OBJECT
   public:
    SettingsTreeManager();

    void CreateGUI(QTreeWidget *build_tree, Configuration *configuration);
    void CleanupGUI();

    void GetTreeState(QByteArray &byte_array, QTreeWidgetItem *top_item);
    int SetTreeState(QByteArray &byte_array, int index, QTreeWidgetItem *top_item);

   protected:
    QTreeWidget *configuration_settings_tree_;
    Configuration *configuration_;
    QVector<QTreeWidgetItem *> compound_widgets_;  // These have special cleanup requirements

    void BuildKhronosTree();
    void BuildGenericTree(QTreeWidgetItem *parent, LayerFile *layer_file);

    QVector<QTreeWidgetItem *> layer_items_;  // These parallel the  profiles layers

    QComboBox *validation_presets_combo_box_;
    LayerFile *validation_layer_file_;
    QTreeWidgetItem *validation_tree_item_;
    QTreeWidgetItem *validation_file_item_;
    QTreeWidgetItem *validation_preset_item_;
    QTreeWidgetItem *validation_log_file_item_;
    FilenameSettingWidget *validation_log_file_widget_;
    EnumSettingWidget *validation_debug_action_;
    KhronosSettingsAdvanced *validation_settings_;
    MuteMessageWidget *mute_message_widget_;
    VUIDSearchWidget *vuid_search_widget_;

   public Q_SLOTS:
    //    void itemCollapsed(QTreeWidgetItem *);
    //    void itemExpanded(QTreeWidgetItem *);

    void khronosDebugChanged(int index);
    void khronosPresetChanged(int index);  // Okay, is this a custom guy HERE, or do we move it out
                                           // It really forces a reload of the entire branch of this tree
                                           // Reset layer defaults for the profile, and then call BuildKhronosTree again
    void khronosPresetEdited();            // The user has changed something from a preset, and we are now a custom setting
    void profileEdited();                  // The profile has been edited and should be saved
};
