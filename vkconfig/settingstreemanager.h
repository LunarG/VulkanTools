#ifndef CSETTINGSTREEMANAGER_H
#define CSETTINGSTREEMANAGER_H
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
 * This class takes a pointer to a treewidget and a profile
 * and creates a gui for displaying and editing those settings.
 *
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#include <QObject>
#include <QTreeWidget>
#include <QComboBox>

#include "profiledef.h"
#include "khronossettingsadvanced.h"
#include "filenamesettingwidget.h"
#include "boolsettingwidget.h"
#include "enumsettingwidget.h"
#include "stringsettingwidget.h"
#include "foldersettingwidget.h"
#include "multienumsetting.h"


class CSettingsTreeManager : QObject
{
    Q_OBJECT
public:
    CSettingsTreeManager();

    void CreateGUI(QTreeWidget *pBuildTree, CProfileDef *pProfileDef);
    void CleanupGUI(void);

    void GetTreeState(QByteArray &byteArray, QTreeWidgetItem *pTopItem);
    int SetTreeState(QByteArray &byteArray, int nIndex, QTreeWidgetItem *pTopItem);

protected:
    QTreeWidget *pEditorTree;
    CProfileDef *pProfile;
    QVector <QTreeWidgetItem*> fileWidgets; // These have special cleanup requirements

    void BuildKhronosTree();
    void BuildGenericTree(QTreeWidgetItem* pParent, CLayerFile *pLayer);

    QVector <QTreeWidgetItem*> layerItems; // These parallel the  profiles layers

    QComboBox *pKhronosPresets;
    CLayerFile *pKhronosLayer;
    QTreeWidgetItem *pKhronosTree;
    QTreeWidgetItem *pKhronosFileItem;
    QTreeWidgetItem *pKhronosPresetItem;
    QTreeWidgetItem *pKhronosLogFileItem;
    CFilenameSettingWidget* pKhronosLogFileWidget;
    CEnumSettingWidget *pKhronosDebugAction;
    KhronosSettingsAdvanced *pAdvancedKhronosEditor;

public Q_SLOTS:
//    void itemCollapsed(QTreeWidgetItem *);
//    void itemExpanded(QTreeWidgetItem *);

    void khronosDebugChanged(int nIndex);
    void khronosPresetChanged(int nIndex); // Okay, is this a custom guy HERE, or do we move it out
                                           // It really forces a reload of the entire branch of this tree
                                           // Reset layer defaults for the profile, and then call BuildKhronosTree again
    void khronosPresetEdited(void);         // The user has changed something from a preset, and we are now a custom setting
};

#endif // CSETTINGSTREEMANAGER_H
