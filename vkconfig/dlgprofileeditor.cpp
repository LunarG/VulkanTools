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
#include "dlgcustompaths.h"

class QTreeWidgetItemWithLayer : public QTreeWidgetItem
    {
    public:
        TLayerRepresentations *pLayerRepBackPointer;
    };


///////////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::dlgProfileEditor
/// \param parent
/// \param pProfileToEdit
/// Edit a profile. There are three cases of profiles passed in through pProfileToEdit.
/// Case 1: A blank profile with no layers activated and all layer settings set to their
///         defaults.
///         The file name is blank. User must save the profile.
/// Case 2: An exisitng profile that contains specifically activated layers and
///         contains settings that have already been specified and previously saved.
///         The file name is not blank. User can save or abandon the changes.
/// Case 3: A canned profile was "cloned". It contains all the availabe layers, but
///         only the Khronos validation layer is active. All the settings are copied
///         over, but stripped of their read only flags.
///         The file name is blank. User must save the profile.
///////////////////////////////////////////////////////////////////////////////
dlgProfileEditor::dlgProfileEditor(QWidget *parent, CProfileDef* pProfileToEdit) :
    QDialog(parent),
    ui(new Ui::dlgProfileEditor)
    {
    ui->setupUi(this);

    // We will need this
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();

    // Case 1: New profile
    if(pProfileToEdit == nullptr) {
        setWindowTitle("Create New Profile");
        ui->pushButtonSaveProfileAs->setText(tr("Save..."));
        pThisProfile = pVulkanConfig->CreateEmptyProfile();
        }
    else {
        QString title;
        if(pProfileToEdit->bContainsReadOnlyFields) {
            QString title = "Clone Profile (" + pProfileToEdit->qsProfileName + ")";
            setWindowTitle(title);
            ui->pushButtonSaveProfileAs->setText(tr("Save profile as..."));
            pThisProfile = pVulkanConfig->CreateEmptyProfile();

            // Replace the first layer with the one we are cloning
            // I could assume it's the first layer... but that seems fragile
            for(int i = 0; i < pThisProfile->layers.size(); i++) {
                if(pProfileToEdit->layers[0]->name == pThisProfile->layers[i]->name) {
                    pThisProfile->layers[i]->CopyLayer(pProfileToEdit->layers[0]);
                    pThisProfile->layers[i]->nRank = 0;
                    pThisProfile->layers[i]->bActive = true;

                    // Clear out read only settings. Layer one is always khronos validation
                    for(int j = 0; j < pThisProfile->layers[i]->layerSettings.size(); j++)
                        pThisProfile->layers[i]->layerSettings[j]->readOnly = false;
                    break;
                    }
                }
            }
        else {
            QString title = "Edit Profile (" + pProfileToEdit->qsProfileName + ")";
            setWindowTitle(title);
            ui->pushButtonSaveProfileAs->setText(tr("Save..."));
            pThisProfile = pProfileToEdit;
            }
        }

    QTreeWidgetItem *pHeader = ui->layerTree->headerItem();
    pHeader->setText(0, "Layer Name");
    pHeader->setText(1, "Active");
    pHeader->setText(2, "Implicit?");
    pHeader->setText(3, "Disabled");
    pHeader->setText(4, "Custom Path?");

    connect(ui->layerTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this,
                SLOT(currentLayerChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

    connect(ui->layerTree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(layerItemChanged(QTreeWidgetItem*, int)));

    resetLayerDisplay(); // Load all available layers
    }

dlgProfileEditor::~dlgProfileEditor()
    {
    qDeleteAll(layers.begin(), layers.end());
    layers.clear();

    delete ui;
    }

void dlgProfileEditor::on_pushButtonAddLayers_clicked()
    {
    dlgCustomPaths dlg(this);
    dlg.exec();
    }

//////////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::refreshLayers
/// Load all the available layers and initialize their settings
void dlgProfileEditor::resetLayerDisplay(void)
    {
    // Clear out the old
    ui->layerTree->clear();

    QString out;
    for(int i = 0; i < pThisProfile->layers.size(); i++)
       {
       TLayerRepresentations *pLayer = new TLayerRepresentations;
       pLayer->pLayerFileInfo = pThisProfile->layers[i];
       pLayer->bUse = false;
       pLayer->bDisabled = false;
       pLayer->bExplicit = true;
       pLayer->pTreeItem = new QTreeWidgetItemWithLayer();
       pLayer->pTreeItem->pLayerRepBackPointer = pLayer;
       layers.push_back(pLayer);

       pLayer->pTreeItem->setText(0, pThisProfile->layers[i]->name);
       pLayer->pTreeItem->setFlags(pLayer->pTreeItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
       pLayer->pTreeItem->setTextAlignment(1, Qt::AlignCenter);
       pLayer->pTreeItem->setCheckState(1, Qt::Unchecked);

       pLayer->pTreeItem->setTextAlignment(2, Qt::AlignCenter);
       if(pThisProfile->layers[i]->layerType == LAYER_TYPE_EXPLICIT)
           pLayer->pTreeItem->setText(2, "No");
       else
        pLayer->pTreeItem->setText(2, "Yes");

       pLayer->pTreeItem->setTextAlignment(3, Qt::AlignCenter);
       pLayer->pTreeItem->setCheckState(3, Qt::Unchecked);
       ui->layerTree->addTopLevelItem(pLayer->pTreeItem);

       if(pThisProfile->layers[i]->layerType == LAYER_TYPE_CUSTOM)
           pLayer->pTreeItem->setText(4, "Yes");
       else
           pLayer->pTreeItem->setText(4, "No");


       QTreeWidgetItemWithLayer *pChild = new QTreeWidgetItemWithLayer();
       pChild->pLayerRepBackPointer = pLayer;
       pChild->setText(0, pThisProfile->layers[i]->description);
       pLayer->pTreeItem->addChild(pChild);

       pChild = new QTreeWidgetItemWithLayer();
       pChild->pLayerRepBackPointer = pLayer;
       out.sprintf("Path: %s", pThisProfile->layers[i]->library_path.toUtf8().constData());
       pChild->setText(0, out);
       pLayer->pTreeItem->addChild(pChild);

       pChild = new QTreeWidgetItemWithLayer();
       pChild->pLayerRepBackPointer = pLayer;
       out.sprintf("API Version: %s", pThisProfile->layers[i]->api_version.toUtf8().constData());
       pChild->setText(0, out);
       pLayer->pTreeItem->addChild(pChild);

       pChild = new QTreeWidgetItemWithLayer();
       pChild->pLayerRepBackPointer = pLayer;
       out.sprintf("Implementation Version: %s", pThisProfile->layers[i]->implementation_version.toUtf8().constData());
       pChild->setText(0, out);
       pLayer->pTreeItem->addChild(pChild);

       pChild = new QTreeWidgetItemWithLayer();
       pChild->pLayerRepBackPointer = pLayer;
       out.sprintf("File format: %s", pThisProfile->layers[i]->file_format_version.toUtf8().constData());
       pChild->setText(0, out);
       pLayer->pTreeItem->addChild(pChild);
       }

    ui->layerTree->resizeColumnToContents(0);
    ui->layerTree->resizeColumnToContents(1);
    ui->layerTree->resizeColumnToContents(2);
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
    // TBD, needs to reset which layers are active, settings, etc.
    ui->groupBoxSettings->setTitle(tr("Layer Settings"));
    settingsEditor.CleanupGUI();
    resetLayerDisplay();
    }



//////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::on_pushButtonLaunchTest_clicked
/// Test environment
void dlgProfileEditor::on_pushButtonLaunchTest_clicked()
    {
    dlgLayerOutput dlg(this);
    dlg.exec();
    }

/////////////////////////////////////////////////////////////////////
/// \brief currentLayerChanged
/// \param pCurrent
/// \param pPrevious
/// The currently selected layer has changed.
void dlgProfileEditor::currentLayerChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious)
    {
    // If something was previously selected, update
    // the settings data for it's layer.
    if(pPrevious != nullptr)
        settingsEditor.CollectSettings();

    // New settings
    QTreeWidgetItemWithLayer *pLayerItem = dynamic_cast<QTreeWidgetItemWithLayer *>(pCurrent);
    //pLayerItem->pLayerRepBackPointer->pLayerFileInfo->layerSettings
    if(pLayerItem == nullptr) return;


    // Get the name of the selected layer
    QString qsTitle = "Layer Settings (" + pCurrent->text(0);
    qsTitle += ")";
    ui->groupBoxSettings->setTitle(qsTitle);

    settingsEditor.CleanupGUI();
    settingsEditor.CreateGUI(ui->scrollArea, pLayerItem->pLayerRepBackPointer->pLayerFileInfo->layerSettings);
    }

/////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_layerTree_itemSelectionChanged
/// Tree selection has been made. Display the appropriate editor at the bottom of the window
/*void dlgProfileEditor::on_layerTree_itemSelectionChanged(void)
    {
    // Which one is selected? We need the top item too
    QTreeWidgetItem *pSelected = ui->layerTree->currentItem();
    while(pSelected->parent() != nullptr)
        pSelected = pSelected->parent();

    QTreeWidgetItemWithLayer *pLayerItem = dynamic_cast<QTreeWidgetItemWithLayer *>(pSelected);
    //pLayerItem->pLayerRepBackPointer->pLayerFileInfo->layerSettings
    if(pLayerItem == nullptr) return;


    // Get the name of the selected layer
    QString qsTitle = "Layer Settings (" + pSelected->text(0);
    qsTitle += ")";
    ui->groupBoxSettings->setTitle(qsTitle);

    settingsEditor.CleanupGUI();
    settingsEditor.CreateGUI(ui->scrollArea, pLayerItem->pLayerRepBackPointer->pLayerFileInfo->layerSettings);
    }
*/


///////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::layerItemChanged
/// \param item
/// Something has been checked or unchecked on the tree
void dlgProfileEditor::layerItemChanged(QTreeWidgetItem *item, int nColumn)
    {
    // Get the layer this is pointing to
    QTreeWidgetItemWithLayer *pLayerItem = dynamic_cast<QTreeWidgetItemWithLayer *>(item);
    if(pLayerItem == nullptr) return;
    CLayerFile *pLayer = pLayerItem->pLayerRepBackPointer->pLayerFileInfo;

    // Column 3 disables/blacklists the layer
    if(nColumn == 3) {
        if(item->checkState(nColumn) == Qt::Checked) {
            pLayer->bDisabled = true;
            pLayer->bActive = false;
            item->setCheckState(1, Qt::Unchecked);
            return;
            }
        else {
            pLayer->bDisabled = false;
            return;
            }
        }

    // Is it active?
    if(nColumn == 1) {
        if(item->checkState(nColumn) == Qt::Checked) {
            pLayer->bDisabled = false;
            item->setCheckState(3, Qt::Unchecked);
            pLayer->bActive = true;
            return;
            }
        }
    }


