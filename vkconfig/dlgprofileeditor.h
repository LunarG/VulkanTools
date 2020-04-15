#ifndef DLGPROFILEEDITOR_H
#define DLGPROFILEEDITOR_H
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
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#include <QDialog>
#include <QTreeWidgetItem>

#include "profiledef.h"
#include <vulkanconfiguration.h>
#include <settingseditor.h>

class QTreeWidgetItemWithLayer;

// For the layer editor, we need a way to keep track of the layers
// that are displayed, and associate them with an actual layer
struct TLayerRepresentations {
    CLayerFile*                     pLayerFileInfo;    // All the details abou the layer. We do not OWN this pointer...
    bool                            bDisabled;         // Do we want this layer disabled
    bool                            bUse;              // Do we want to use this layer
    bool                            bExplicit;         // true for explicit, false for implicit
    QTreeWidgetItemWithLayer*       pTreeItem;         // Address the layer item in the GUI
};


namespace Ui {
class dlgProfileEditor;
}

class dlgProfileEditor : public QDialog
{
    Q_OBJECT

public:
    explicit dlgProfileEditor(QWidget *parent, CProfileDef* pProfileToEdit);
    ~dlgProfileEditor();

    QVector <TLayerRepresentations*> layers;

    // Load all layers found in all search folders
    void loadAllFoundLayers(QVector <CLayerFile*> &layerFile);

    // Get a list of profiles from the GUI editor
    void createProfileList(QVector <CLayerFile*> &layerFiles);

    // Load all found layers and set their settings to the default
    void refreshLayers();


private:
    Ui::dlgProfileEditor *ui;

    CVulkanConfiguration*    pVulkanConfig;
    CSettingsEditor          settingsEditor;

    CProfileDef             *pThisProfile;

    QVector <CLayerFile *>   layerList;

public Q_SLOTS:
    void on_pushButtonResetLayers_clicked();
    void on_pushButtonLaunchTest_clicked();
    void on_layerTree_itemSelectionChanged();
    void on_pushButtonAddLayers_clicked();

};

#endif // DLGPROFILEEDITOR_H
