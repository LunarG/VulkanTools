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

#include "dlgprofileeditor.h"
#include "ui_dlgprofileeditor.h"
#include "dlglayeroutput.h"

class QTreeWidgetItemWithLayer : public QTreeWidgetItem
    {
    public:
        TLayerRepresentations *pLayerRepBackPointer;
    };



dlgProfileEditor::dlgProfileEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgProfileEditor)
    {
    ui->setupUi(this);

    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();
    pVulkanConfig->reLoadLayerConfiguration();
    reloadLayersShown();

    }

dlgProfileEditor::~dlgProfileEditor()
    {
    qDeleteAll(layers.begin(), layers.end());
    layers.clear();

    delete ui;
    }

void dlgProfileEditor::reloadLayersShown(void)
    {
    ui->layerTree->clear();
    loadAllFoundLayers(pVulkanConfig->explicitLayers);
    loadAllFoundLayers(pVulkanConfig->implicitLayers);
    loadAllFoundLayers(pVulkanConfig->customLayers);
    }

///////////////////////////////////////////////////////////////////////////////
// Get a list of profiles from the GUI editor
void dlgProfileEditor::createProfileList(QVector <CLayerFile*> &layerFiles)
    {
    // Easy as that... just add all the layers that are flagged active
    for(int i = 0; i < layers.size(); i++) {
        if(layers[i]->bUse)
            layerFiles.push_back(layers[i]->pLayerFileInfo);
       }
    }


void dlgProfileEditor::on_pushButtonResetLayers_clicked(void)
    {
    reloadLayersShown();
    }


///////////////////////////////////////////////////////////////////////////////
void dlgProfileEditor::loadAllFoundLayers(QVector <CLayerFile*> &layerFile)
    {
    QTreeWidgetItem *pHeader = ui->layerTree->headerItem();

    pHeader->setText(0, "Layer Name");
    pHeader->setText(1, "Active");
    pHeader->setText(2, "Implicit?");
    pHeader->setText(3, "Disabled");
    pHeader->setText(4, "Custom Paths?");


    QString out;
    for(int i = 0; i < layerFile.size(); i++)
       {
       TLayerRepresentations *pLayer = new TLayerRepresentations;
       pLayer->pLayerFileInfo = layerFile[i];
       pLayer->bUse = false;
       pLayer->bDisabled = false;
       pLayer->bExplicit = true;
       pLayer->pTreeItem = new QTreeWidgetItem();
       layers.push_back(pLayer);

       pLayer->pTreeItem->setText(0, layerFile[i]->name);
       pLayer->pTreeItem->setFlags(pLayer->pTreeItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
       pLayer->pTreeItem->setTextAlignment(1, Qt::AlignCenter);
       pLayer->pTreeItem->setCheckState(1, Qt::Unchecked);

       pLayer->pTreeItem->setTextAlignment(2, Qt::AlignCenter);
       if(layerFile[i]->layerType == LAYER_TYPE_EXPLICIT)
           pLayer->pTreeItem->setText(2, "No");
       else
        pLayer->pTreeItem->setText(2, "Yes");

       pLayer->pTreeItem->setTextAlignment(3, Qt::AlignCenter);
       pLayer->pTreeItem->setCheckState(3, Qt::Unchecked);
       ui->layerTree->addTopLevelItem(pLayer->pTreeItem);

       pLayer->pTreeItem->setTextAlignment(4, Qt::AlignCenter);
       if(layerFile[i]->layerType == LAYER_TYPE_CUSTOM)
           pLayer->pTreeItem->setText(4, "Yes");
       else
           pLayer->pTreeItem->setText(4, "No");


       QTreeWidgetItemWithLayer *pChild = new QTreeWidgetItemWithLayer();
       pChild->pLayerRepBackPointer = pLayer;
       pChild->setText(0, layerFile[i]->description);
       pLayer->pTreeItem->addChild(pChild);

       pChild = new QTreeWidgetItemWithLayer();
       pChild->pLayerRepBackPointer = pLayer;
       out.sprintf("Path: %s", layerFile[i]->library_path.toUtf8().constData());
       pChild->setText(0, out);
       pLayer->pTreeItem->addChild(pChild);

       pChild = new QTreeWidgetItemWithLayer();
       pChild->pLayerRepBackPointer = pLayer;
       out.sprintf("API Version: %s", layerFile[i]->api_version.toUtf8().constData());
       pChild->setText(0, out);
       pLayer->pTreeItem->addChild(pChild);

       pChild = new QTreeWidgetItemWithLayer();
       pChild->pLayerRepBackPointer = pLayer;
       out.sprintf("Implementation Version: %s", layerFile[i]->implementation_version.toUtf8().constData());
       pChild->setText(0, out);
       pLayer->pTreeItem->addChild(pChild);

       pChild = new QTreeWidgetItemWithLayer();
       pChild->pLayerRepBackPointer = pLayer;
       out.sprintf("File format: %s", layerFile[i]->file_format_version.toUtf8().constData());
       pChild->setText(0, out);
       pLayer->pTreeItem->addChild(pChild);
       }


    ui->layerTree->resizeColumnToContents(0);
    ui->layerTree->resizeColumnToContents(1);
    ui->layerTree->resizeColumnToContents(2);
    }

/////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_layerTree_itemSelectionChanged
/// Tree selection has been made. Display the appropriate editor at the bottom of the window
void dlgProfileEditor::on_layerTree_itemSelectionChanged(void)
    {
    // Which one is selected? We need the top item too
    QTreeWidgetItem *pSelected = ui->layerTree->currentItem();
    while(pSelected->parent() != nullptr)
        pSelected = pSelected->parent();

    QTreeWidgetItemWithLayer *pLayerItem = dynamic_cast<QTreeWidgetItemWithLayer *>(pSelected);
    //pLayerItem->pLayerRepBackPointer->pLayerFileInfo->layerSettings
    if(pLayerItem == nullptr) return;


    // Get the name of the selected layer
    QString csLayer = pSelected->text(0);

    settingsEditor.CleanupGUI();
    settingsEditor.CreateGUI(ui->scrollArea, pLayerItem->pLayerRepBackPointer->pLayerFileInfo->layerSettings);

    }


//////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::on_pushButtonLaunchTest_clicked
/// Test environment
void dlgProfileEditor::on_pushButtonLaunchTest_clicked()
    {
    dlgLayerOutput dlg(this);
    dlg.exec();
    }

