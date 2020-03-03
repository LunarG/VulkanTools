/*
 * Copyright (c) 2018-2020 Valve Corporation
 * Copyright (c) 2018-2020 LunarG, Inc.
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

#pragma once
#include <QVector>

#include <QMainWindow>
#include <QTreeWidgetItem>

#include <vulkanconfiguration.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


// For the layer editor, we need a way to keep track of the layers
// that are displayed, and associate them with an actual layer
struct TLayerRepresentations {
    const CLayerFile*   pLayerFileInfo;    // All the details abou the layer. We do not OWN this pointer...
    bool                bDisabled;         // Do we want this layer disabled
    bool                bUse;              // Do we want to use this layer
    bool                bExplicit;         // true for explicit, false for implicit
    QTreeWidgetItem*    pTreeItem;         // Address the layer item in the GUI
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected:
    CVulkanConfiguration*    pVulkanConfig;

    QVector <TLayerRepresentations*> layers;

    void loadAllFoundLayers(QVector <CLayerFile*> &layerFile);

public Q_SLOTS:
    void fileExit(bool bChecked);
    void fileHistory(bool bChecked);

    void aboutVkConfig(bool bChecked);
    void toolsVulkanInfo(bool bChecked);
    void toolsVulkanInstallation(bool bChecked);
    void toolsVulkanTestApp(bool bChecked);

    void on_pushButtonLaunch_clicked();
    void on_pushButtonAddAssociation_clicked();
    void on_checkBoxHideUnused_clicked();
    void on_pushButtonActivateProfile_clicked();
    void on_pushButtonCreateProfile_clicked();
    void on_pushButtonCustomPaths_clicked();

private:
    Ui::MainWindow *ui;
};

