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

#include <QObject>
#include <QTreeWidget>
#include <QComboBox>

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

class SettingsTreeManager : QObject {
    Q_OBJECT
   public:
    SettingsTreeManager();

    void CreateGUI(QTreeWidget *pBuildTree, Configuration *pProfileDef);
    void CleanupGUI();

    void GetTreeState(QByteArray &byteArray, QTreeWidgetItem *pTopItem);
    int SetTreeState(QByteArray &byteArray, int nIndex, QTreeWidgetItem *pTopItem);

   protected:
    QTreeWidget *pEditorTree;
    Configuration *pProfile;
    QVector<QTreeWidgetItem *> compoundWidgets;  // These have special cleanup requirements

    void BuildKhronosTree();
    void BuildGenericTree(QTreeWidgetItem *pParent, LayerFile *pLayer);

    QVector<QTreeWidgetItem *> layerItems;  // These parallel the  profiles layers

    QComboBox *pKhronosPresets;
    LayerFile *pKhronosLayer;
    QTreeWidgetItem *pKhronosTree;
    QTreeWidgetItem *pKhronosFileItem;
    QTreeWidgetItem *pKhronosPresetItem;
    QTreeWidgetItem *pKhronosLogFileItem;
    FilenameSettingWidget *pKhronosLogFileWidget;
    EnumSettingWidget *pKhronosDebugAction;
    KhronosSettingsAdvanced *pAdvancedKhronosEditor;
    MuteMessageWidget *pMuteMessageWidget;
    VUIDSearchWidget *pVUIDSearchWidget;
    QTreeWidgetItem *pMuteMessageSearchItem;

   public Q_SLOTS:
    //    void itemCollapsed(QTreeWidgetItem *);
    //    void itemExpanded(QTreeWidgetItem *);

    void khronosDebugChanged(int nIndex);
    void khronosPresetChanged(int nIndex);  // Okay, is this a custom guy HERE, or do we move it out
                                            // It really forces a reload of the entire branch of this tree
                                            // Reset layer defaults for the profile, and then call BuildKhronosTree again
    void khronosPresetEdited();         // The user has changed something from a preset, and we are now a custom setting
    void profileEdited();               // The profile has been edited and should be saved
};
