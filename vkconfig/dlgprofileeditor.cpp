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
#include <QComboBox>
#include <QStyle>

#include "dlgprofileeditor.h"
#include "ui_dlgprofileeditor.h"
#include "dlglayeroutput.h"
#include "dlgcustompaths.h"


// We need a way when we get a tree widget item out, to know
// what layer it references. Use this so that all tree widget
// items contain a pointer to the actual layer.
class QTreeWidgetItemWithLayer : public QTreeWidgetItem
    {
    public:
        CLayerFile  *pLayer;
    };

#define     LAYER_APP_CONTROLLED    0
#define     LAYER_FORCED_ON         1
#define     LAYER_FORCED_OFF        2

///////////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::dlgProfileEditor
/// \param parent
/// \param pProfileToEdit
/// Edit a profile. There are three cases of profiles passed in through pProfileToEdit.
/// Case 1: New Profile: A blank profile with no layers activated and all layer settings set to their
///         defaults.
///         The file name is blank. User must save the profile.
/// Case 2: Edit: An exisitng profile that contains specifically activated layers and
///         contains settings that have already been specified and previously saved.
///         The file name is not blank. User can save or abandon the changes.
/// Case 3: Clone: Same as edit, but the name of the profile is cleared, and if it was
///         a fixed profile, that flag is cleared.
///         The file name is blank. User must save the profile.
///////////////////////////////////////////////////////////////////////////////
dlgProfileEditor::dlgProfileEditor(QWidget *parent, CProfileDef* pProfileToEdit) :
    QDialog(parent),
    ui(new Ui::dlgProfileEditor)
    {
    ui->setupUi(this);

    // We never edit a profile directly, we only edit a copy of it.
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();
    setWindowTitle("Creating New Profile");

    // Case 1: New profile (easiest case)
    if(pProfileToEdit == nullptr)
        pThisProfile = pVulkanConfig->CreateEmptyProfile();
    else {
        // We are editing an exisitng profile. Make a copy of it
        pThisProfile = pProfileToEdit->duplicateProfile();

        // IF this was a fixed profile, clear that setting
        // AND we need to clear the name since we are making a copy
        if(pProfileToEdit->bFixedProfile) {
            pThisProfile->qsFileName = "";
            pThisProfile->qsProfileName = "";
            pThisProfile->qsDescription = "";
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

    ui->lineEditName->setText(pThisProfile->qsProfileName);
    ui->lineEditDesc->setText(pThisProfile->qsDescription);

    QTreeWidgetItem *pHeader = ui->layerTree->headerItem();

    pHeader->setText(0, "Layers");
    pHeader->setText(1, "Usage");
//    pHeader->setText(2, "Blacklist");
//    pHeader->setText(3, "Implicit?");
//    pHeader->setText(4, "Custom Path?");
//    pHeader->setFirstColumnSpanned(true);

    connect(ui->layerTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this,
                SLOT(currentLayerChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

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
    LoadLayerDisplay();
    }


//////////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::refreshLayers
/// Load all the available layers and initialize their settings
void dlgProfileEditor::LoadLayerDisplay(int nSelection)
    {
    // Clear out the old
    ui->layerTree->clear();

    // We need some infor for sizing the column with the combo box
    QFontMetrics fm = ui->layerTree->fontMetrics();
    int comboWidth = (fm.size(Qt::TextSingleLine, "App Controlled").width() * 1.6);
    int comboHeight = fm.size(Qt::TextSingleLine, "App Controlled").height() * 1.6;

    // Loop through the layers. They are expected to be in order
    for(int iLayer = 0; iLayer < pThisProfile->layers.size(); iLayer++)
       {
       QTreeWidgetItemWithLayer *pItem = new QTreeWidgetItemWithLayer();
       pItem->pLayer = pThisProfile->layers[iLayer];

       QString decoratedName = pItem->pLayer->name;
       if(pItem->pLayer->layerType == LAYER_TYPE_IMPLICIT)
           decoratedName += tr(" (Implicit)");

       if(pItem->pLayer->layerType == LAYER_TYPE_CUSTOM)
           decoratedName += tr(" (Custom)");

       pItem->setText(0, decoratedName);
       pItem->setFlags(pItem->flags() | Qt::ItemIsSelectable);
       pItem->setText(1, "App Controlled"); // Fake out for width of column

       // Add the top level item
       ui->layerTree->addTopLevelItem(pItem);
       if(iLayer == nSelection)
           ui->layerTree->setCurrentItem(pItem);

       // Add a combo box. Default has gray background which looks hidious
       QComboBox *pUse = new QComboBox();

       const char *style =
//               "QComboBox { background-color: white;"
//                           "down-arrow {"
//                                 "width:0px;"
//                                 "height:0px;"
//                               "}}";


                          // "}"
                           "QComboBox::down-arrow {"
                                 "width:0px;"
                                 "height:0px;"
                                 "padding-right:0px;"
                               "}";


//      pUse->setStyleSheet(style);
       ui->layerTree->setItemWidget(pItem, 1, pUse);
       pItem->setSizeHint(1, QSize(comboWidth, comboHeight));

       pUse->addItem("App Controlled");
       pUse->addItem("Force On");
       pUse->addItem("Force Off");

       if(pItem->pLayer->bActive)
           pUse->setCurrentIndex(1);

       if(pItem->pLayer->bDisabled)
           pUse->setCurrentIndex(2);

       connect(pUse, SIGNAL(currentIndexChanged(int)), this, SLOT(layerUseChanged(int)));


       ///////////////////////////////////////////////////
       // Now for the children, which is just supplimental
       // information. These are NOT QTreeWidgetItemWithLayer
       // because they don't link back to a layer, you have to
       // go up the tree
/*
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

       pChild = new QTreeWidgetItem();
       pChild->setText(0, pThisProfile->layers[iLayer]->qsLayerPath);
       pChild->setFlags(pChild->flags() & ~Qt::ItemIsSelectable);
       pItem->addChild(pChild);
*/
       }

    resizeEvent(nullptr);
//    int width = ui->layerTree->width() - comboWidth;
//    ui->layerTree->setColumnWidth(0, width);
    }

// The only way to catch the resize from the layouts
// (which is screwing up the spacing with the combo boxes)
void dlgProfileEditor::showEvent(QShowEvent *event)
    {
    (void)event;
    resizeEvent(nullptr);
    }

void dlgProfileEditor::resizeEvent(QResizeEvent *event)
    {
    (void)event;
    QFontMetrics fm = ui->layerTree->fontMetrics();
    int comboWidth = (fm.size(Qt::TextSingleLine, "App Controlled").width() * 1.6);
    int width = ui->layerTree->width() - comboWidth;
    ui->layerTree->setColumnWidth(0, width);
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
    (void)pPrevious;
    // These are always safe to call
    settingsEditor.CollectSettings();
    settingsEditor.CleanupGUI();

    // New settings
    QTreeWidgetItemWithLayer *pLayerItem = dynamic_cast<QTreeWidgetItemWithLayer *>(pCurrent);
    if(pLayerItem == nullptr) {
        ui->groupBoxSettings->setTitle(tr("Layer Settings"));
        ui->labelLayerDetails->setText("");
        return;
        }

    // Get the name of the selected layer
    QString qsTitle = "Layer Settings (" + pCurrent->text(0);
    qsTitle += ")";
    ui->groupBoxSettings->setTitle(qsTitle);
    settingsEditor.CreateGUI(ui->scrollArea, pLayerItem->pLayer->layerSettings, false);

    // Populate the side label
    QString detailsText = pLayerItem->pLayer->description;
    detailsText += "\n";
    switch(pLayerItem->pLayer->layerType) {
        case LAYER_TYPE_EXPLICIT:
        detailsText += "Explicit Layer\n";
        break;
        case LAYER_TYPE_IMPLICIT:
        detailsText += "Implicit Layer\n";
        break;
        case LAYER_TYPE_CUSTOM:
        detailsText += "Custom Layer Path\n";
        break;
        }

    detailsText += pLayerItem->pLayer->library_path;
    detailsText += "\n";

    detailsText += "API Version: ";
    detailsText += pLayerItem->pLayer->api_version;
    detailsText += "\n";

    detailsText += "Implementation Version: ";
    detailsText += pLayerItem->pLayer->implementation_version;
    detailsText += "\n";

    detailsText += "File format: ";
    detailsText += pLayerItem->pLayer->file_format_version;

    ui->labelLayerDetails->setText(detailsText);
    }


///////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::layerUseChanged
/// \param item
/// use, don't use, black list...
void dlgProfileEditor::layerUseChanged(int nSelection)
    {
    // Combo box changed. We first need to know which tree item was selected.
    // Fortunatly, changing the combo box also changes the selected item.

    QTreeWidgetItemWithLayer *pLayerItem = dynamic_cast<QTreeWidgetItemWithLayer *>(ui->layerTree->currentItem());
    Q_ASSERT(pLayerItem != nullptr);

    CLayerFile *pLayer = pLayerItem->pLayer;
    Q_ASSERT(pLayer != nullptr);

    // Okay, easy now, just set the flags appropriately
    switch(nSelection) {
        case LAYER_APP_CONTROLLED:
            pLayer->bActive = false;
            pLayer->bDisabled = false;
        break;

        case LAYER_FORCED_ON:
            pLayer->bActive = true;
            pLayer->bDisabled = false;
        break;

        case LAYER_FORCED_OFF:
            pLayer->bActive = false;
            pLayer->bDisabled = true;
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

    pThisProfile->qsProfileName = ui->lineEditName->text();
    pThisProfile->qsDescription = ui->lineEditDesc->text();

    // Name must not be blank
    if(pThisProfile->qsProfileName.isEmpty()) {
        QMessageBox msg;
        msg.setInformativeText(tr("Configuration must have a name."));
        msg.setText(tr("Name your new config!"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
        }

    // Collapse the profile and remove unused layers
    pThisProfile->CollapseProfile();

    // Wa-wa-wait... no profiles without any layers buster
    if(pThisProfile->layers.size() == 0)
        {
        QMessageBox msg;
        msg.setInformativeText(tr("You cannot have a named configuration without any layers."));
        msg.setText(tr("Add some layers!"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
        }


    pThisProfile->qsFileName = pThisProfile->qsProfileName + ".json";
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

