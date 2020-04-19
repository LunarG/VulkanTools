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

#include <QMessageBox>

#include "dlgprofileeditor.h"
#include "ui_dlgprofileeditor.h"
#include "dlglayeroutput.h"
#include "dlgcustompaths.h"
#include "dlgcreateprofile.h"


// We need a way when we get a tree widget item out, to know
// what layer it references. Use this so that all tree widget
// items contain a pointer to the actual layer.
class QTreeWidgetItemWithLayer : public QTreeWidgetItem
    {
    public:
        CLayerFile *pLayer;
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

    // We never edit a profile directly, we only edit a copy of it.
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();
    setWindowTitle("Create New Profile");


    // Case 1: New profile (easiest case)
    if(pProfileToEdit == nullptr)
        pThisProfile = pVulkanConfig->CreateEmptyProfile();
    else {
        // We are editing an exisitng profile. Make a copy of it
        pThisProfile = pProfileToEdit->duplicateProfile();

        // IF this was a canned profile, the first layer has read only settings
        // since we copied the profile, we need to reset these to editable,
        // AND we need to clear the name since we are making a copy
        if(pThisProfile->bContainsReadOnlyFields) {
            pThisProfile->bContainsReadOnlyFields = false;
            pThisProfile->qsFileName = "";
            pThisProfile->qsProfileName = "";
            for(int i = 0; i < pThisProfile->layers[0]->layerSettings.size(); i++)
                pThisProfile->layers[0]->layerSettings[i]->readOnly = false;
            }
        else // Editing an exisitng profile
            {
            QString title = "Clone Profile (" + pProfileToEdit->qsProfileName + ")";
            setWindowTitle(title);
            }

        // We now have a profile ready for editing, but only the layers that
        // are actually used are attached. Now, we need to add the remaining layers
        // in their default states
        // Loop through all the available layers. If the layer is not use for the profile
        // attach it for editing.
        int nRank = pThisProfile->layers.size();    // Next rank starts here
        for(int iAvailable = 0; iAvailable < pVulkanConfig->allLayers.size(); iAvailable++) {
            CLayerFile *pLayerThatMightBeMissing = pVulkanConfig->allLayers[iAvailable];

            // Look for through all layers
            CLayerFile *pAreYouAlreadyThere = pThisProfile->findLayer(pLayerThatMightBeMissing->name);
            if(pAreYouAlreadyThere != nullptr) // It's in the list already
                continue;

            // Nope, add it to the end
            CLayerFile *pNextLayer = new CLayerFile();
            pLayerThatMightBeMissing->CopyLayer(pNextLayer);

            // Add default settings to the layer...
            pVulkanConfig->LoadDefaultSettings(pNextLayer);

            pNextLayer->nRank = nRank++;
            pNextLayer->bActive = false;    // Layers read from file are already active

            // Check the blacklist
            if(pThisProfile->blacklistedLayers.contains(pNextLayer->name))
                pNextLayer->bDisabled = true;

            pThisProfile->layers.push_back(pNextLayer);
            }
        }


    QTreeWidgetItem *pHeader = ui->layerTree->headerItem();
    pHeader->setText(0, "Available Layers");
    pHeader->setText(1, "Use");
    pHeader->setText(2, "Blacklist");
    pHeader->setText(3, "Implicit?");
    pHeader->setText(4, "Custom Path?");

    connect(ui->layerTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this,
                SLOT(currentLayerChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

    connect(ui->layerTree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(layerItemChanged(QTreeWidgetItem*, int)));

    LoadLayerDisplay(0); // Load/Reload the layer editor
    }

dlgProfileEditor::~dlgProfileEditor()
    {
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
void dlgProfileEditor::LoadLayerDisplay(int nSelection)
    {
    // Clear out the old
    ui->layerTree->clear();

    // Loop through the layers. They are expected to be in order
    for(int iLayer = 0; iLayer < pThisProfile->layers.size(); iLayer++)
       {
       QTreeWidgetItemWithLayer *pItem = new QTreeWidgetItemWithLayer();
       pItem->pLayer = pThisProfile->layers[iLayer];

       pItem->setText(0, pItem->pLayer->name);
       pItem->setFlags(pItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);

       // Active or not?
       pItem->setTextAlignment(1, Qt::AlignCenter);
       if(pItem->pLayer->bActive)
           pItem->setCheckState(1, Qt::Checked);
       else
           pItem->setCheckState(1, Qt::Unchecked);

       // Black listed?
       pItem->setTextAlignment(2, Qt::AlignCenter);
       if(pItem->pLayer->bDisabled)
           pItem->setCheckState(2, Qt::Checked);
       else
           pItem->setCheckState(2, Qt::Unchecked);

       // Let me know if it's an implicit layer
       pItem->setTextAlignment(3, Qt::AlignCenter);
       if(pItem->pLayer->layerType == LAYER_TYPE_EXPLICIT)
           pItem->setText(3, "No");
       else
           pItem->setText(3, "Yes");

       // Is it a custom layer? If so, where is it from...
       if(pItem->pLayer->layerType == LAYER_TYPE_CUSTOM)
           pItem->setText(4, pItem->pLayer->qsCustomLayerPath);
       else
           pItem->setText(4, "SDK Supplied");

       // Add the top level item
       ui->layerTree->addTopLevelItem(pItem);
       if(iLayer == nSelection)
           ui->layerTree->setCurrentItem(pItem);

       ///////////////////////////////////////////////////
       // Now for the children, which is just supplimental
       // information. These are NOT QTreeWidgetItemWithLayer
       // because they don't link back to a layer, you have to
       // go up the tree

       QTreeWidgetItem *pChild = new QTreeWidgetItem();
       pChild->setText(0, pThisProfile->layers[iLayer]->description);
       pChild->setFlags(pChild->flags() & ~Qt::ItemIsSelectable);
       pItem->addChild(pChild);

       pChild = new QTreeWidgetItem();
       QString outText;
       outText.sprintf("Path: %s", pThisProfile->layers[iLayer]->library_path.toUtf8().constData());
       pChild->setText(0, outText);
       pChild->setFlags(pChild->flags() & ~Qt::ItemIsSelectable);
       pItem->addChild(pChild);

       pChild = new QTreeWidgetItem();
       outText.sprintf("API Version: %s", pThisProfile->layers[iLayer]->api_version.toUtf8().constData());
       pChild->setText(0, outText);
       pChild->setFlags(pChild->flags() & ~Qt::ItemIsSelectable);
       pItem->addChild(pChild);

       pChild = new QTreeWidgetItem();
       outText.sprintf("Implementation Version: %s", pThisProfile->layers[iLayer]->implementation_version.toUtf8().constData());
       pChild->setText(0, outText);
       pChild->setFlags(pChild->flags() & ~Qt::ItemIsSelectable);
       pItem->addChild(pChild);

       pChild = new QTreeWidgetItem();
       outText.sprintf("File format: %s", pThisProfile->layers[iLayer]->file_format_version.toUtf8().constData());
       pChild->setText(0, outText);
       pChild->setFlags(pChild->flags() & ~Qt::ItemIsSelectable);
       pItem->addChild(pChild);
       }

    ui->layerTree->resizeColumnToContents(0);
    ui->layerTree->resizeColumnToContents(1);
    ui->layerTree->resizeColumnToContents(2);
    ui->layerTree->resizeColumnToContents(3);
    ui->layerTree->resizeColumnToContents(4);
    }


//////////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::on_pushButtonResetLayers_clicked
/// This button clears the display. Basically, we delete the profile and
/// start over.
void dlgProfileEditor::on_pushButtonResetLayers_clicked(void)
    {
    // TBD, needs to reset which layers are active, settings, etc.
    ui->groupBoxSettings->setTitle(tr("Layer Settings"));
    delete pThisProfile;
    pThisProfile = pVulkanConfig->CreateEmptyProfile();
    settingsEditor.CleanupGUI();
    LoadLayerDisplay();
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
    // These are always safe to call
    settingsEditor.CollectSettings();
    settingsEditor.CleanupGUI();

    // New settings
    QTreeWidgetItemWithLayer *pLayerItem = dynamic_cast<QTreeWidgetItemWithLayer *>(pCurrent);
    if(pLayerItem == nullptr) {
        ui->groupBoxSettings->setTitle(tr("Layer Settings"));
        return;
        }

    // Get the name of the selected layer
    QString qsTitle = "Layer Settings (" + pCurrent->text(0);
    qsTitle += ")";
    ui->groupBoxSettings->setTitle(qsTitle);
    settingsEditor.CreateGUI(ui->scrollArea, pLayerItem->pLayer->layerSettings);
    }


///////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::layerItemChanged
/// \param item
/// Something has been checked or unchecked on the tree
void dlgProfileEditor::layerItemChanged(QTreeWidgetItem *item, int nColumn)
    {
    // Get the layer this is pointing to
    QTreeWidgetItemWithLayer *pLayerItem = dynamic_cast<QTreeWidgetItemWithLayer *>(item);
    if(pLayerItem == nullptr) return;
    CLayerFile *pLayer = pLayerItem->pLayer;

    // Column 2 disables/blacklists the layer
    if(nColumn == 2) {
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
            item->setCheckState(2, Qt::Unchecked);
            pLayer->bActive = true;
            return;
            }
        else
            pLayer->bActive = false;
        }
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::accept()
/// This is actually the save button.
/// We are either saving an exisitng profile, or creating a new one.
void dlgProfileEditor::accept()
    {
    // Get the path where the profiles are saved
    QString savePath = pVulkanConfig->getProfilePath();

    // Collect any remaining GUI edits
    settingsEditor.CollectSettings();

    // If we are editing a profile, just save it and be done.
    if(!pThisProfile->qsFileName.isEmpty()) {
        savePath += "/" + pThisProfile->qsFileName;
        pThisProfile->CollapseProfile();
        pVulkanConfig->SaveProfile(pThisProfile);
        QDialog::accept();
        return;
        }

    // Creating a user defined profile. A bit more work.
    dlgcreateprofile dlg(this);
    if(QDialog::Accepted != dlg.exec()) // If we cancel, just return
        return;

    pThisProfile->qsProfileName = dlg.profileName;
    pThisProfile->qsDescription = dlg.profileDescription;

    // Name must not be blank
    if(dlg.profileName.isEmpty()) {
        QMessageBox msg;
        msg.setInformativeText(tr("Profile must have a name."));
        msg.setText(tr("Name your new profile"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
        }

    // Collapse the profile and remove unused layers
    pThisProfile->CollapseProfile();

    pThisProfile->qsFileName = dlg.profileName + ".profile";
    savePath += pThisProfile->qsFileName;
    pVulkanConfig->SaveProfile(pThisProfile);
    pVulkanConfig->profileList.push_back(pThisProfile);
    QDialog::accept();
    }


////////////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::on_toolButtonUp_clicked
/// Move the selected layer up in the list
void dlgProfileEditor::on_toolButtonUp_clicked()
    {
    // Get the item, see if it's a top level item suitable for movement
    QTreeWidgetItemWithLayer *pLayerItem = dynamic_cast<QTreeWidgetItemWithLayer*>(ui->layerTree->currentItem());
    if(pLayerItem == nullptr)
        return;

    // Make sure I'm not already the first one
    if(pLayerItem->pLayer->nRank == 0)
        return;

    // This item goes up by one. The one before it goes down by one
    int nBumped = pLayerItem->pLayer->nRank;
    pLayerItem->pLayer->nRank--;
    QTreeWidgetItemWithLayer *pPreviousItem = dynamic_cast<QTreeWidgetItemWithLayer*>(ui->layerTree->itemAbove(pLayerItem));
    if(pPreviousItem != nullptr)
        pPreviousItem->pLayer->nRank++;

    // The two rank positons should also by their location in the QVector layers. Swap them
    CLayerFile *pTemp = pThisProfile->layers[nBumped];
    pThisProfile->layers[nBumped] = pThisProfile->layers[nBumped-1];
    pThisProfile->layers[nBumped-1] = pTemp;

    LoadLayerDisplay(nBumped-1);
    }

//////////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::on_toolButtonDown_clicked
/// Move the selected layer down in the list
void dlgProfileEditor::on_toolButtonDown_clicked()
    {
    // Get the item, see if it's a top level item suitable for movement
    QTreeWidgetItemWithLayer *pLayerItem = dynamic_cast<QTreeWidgetItemWithLayer*>(ui->layerTree->currentItem());
    if(pLayerItem == nullptr)
        return;

    // Make sure I'm not already the last one
    if(pLayerItem->pLayer->nRank == (pThisProfile->layers.size()-1))
        return;

    // This item goes down by one. The one after it goes up by one
    int nBumped = pLayerItem->pLayer->nRank;
    pLayerItem->pLayer->nRank++;
    QTreeWidgetItemWithLayer *pPreviousItem = dynamic_cast<QTreeWidgetItemWithLayer*>(ui->layerTree->itemBelow(pLayerItem));
    if(pPreviousItem != nullptr)
        pPreviousItem->pLayer->nRank--;

    // The two rank positons should also by their location in the QVector layers. Swap them
    CLayerFile *pTemp = pThisProfile->layers[nBumped];
    pThisProfile->layers[nBumped] = pThisProfile->layers[nBumped+1];
    pThisProfile->layers[nBumped+1] = pTemp;

    LoadLayerDisplay(nBumped+1);
    }

