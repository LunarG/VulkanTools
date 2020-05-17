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
#ifdef _WIN32
#include <windows.h>
#endif

#include <QMessageBox>
#include <QComboBox>
#include <QStyle>

#include "dlgprofileeditor.h"
#include "ui_dlgprofileeditor.h"
#include "dlglayeroutput.h"
#include "dlgcustompaths.h"


#ifdef _WIN32
///////////////////////////////////////////////////////////////////////////
// I totally just stole this from Stack Overflow.
#define MKPTR(p1,p2) ((DWORD_PTR)(p1) + (DWORD_PTR)(p2))

typedef enum _pe_architecture {
    PE_ARCHITECTURE_UNKNOWN = 0x0000,
    PE_ARCHITECTURE_ANYCPU  = 0x0001,
    PE_ARCHITECTURE_X86     = 0x010B,
    PE_ARCHITECTURE_x64     = 0x020B
} PE_ARCHITECTURE;

LPVOID GetOffsetFromRva(IMAGE_DOS_HEADER *pDos, IMAGE_NT_HEADERS *pNt, DWORD rva) {
    IMAGE_SECTION_HEADER *pSecHd = IMAGE_FIRST_SECTION(pNt);
    for(unsigned long i = 0; i < pNt->FileHeader.NumberOfSections; ++i, ++pSecHd) {
        // Lookup which section contains this RVA so we can translate the VA to a file offset
        if (rva >= pSecHd->VirtualAddress && rva < (pSecHd->VirtualAddress + pSecHd->Misc.VirtualSize)) {
            DWORD delta = pSecHd->VirtualAddress - pSecHd->PointerToRawData;
            return (LPVOID)MKPTR(pDos, rva - delta);
        }
    }
    return NULL;
}

PE_ARCHITECTURE GetImageArchitecture(void *pImageBase) {
    // Parse and validate the DOS header
    IMAGE_DOS_HEADER *pDosHd = (IMAGE_DOS_HEADER*)pImageBase;
    if (IsBadReadPtr(pDosHd, sizeof(pDosHd->e_magic)) || pDosHd->e_magic != IMAGE_DOS_SIGNATURE)
        return PE_ARCHITECTURE_UNKNOWN;

    // Parse and validate the NT header
    IMAGE_NT_HEADERS *pNtHd = (IMAGE_NT_HEADERS*)MKPTR(pDosHd, pDosHd->e_lfanew);
    if (IsBadReadPtr(pNtHd, sizeof(pNtHd->Signature)) || pNtHd->Signature != IMAGE_NT_SIGNATURE)
        return PE_ARCHITECTURE_UNKNOWN;

    // First, naive, check based on the 'Magic' number in the Optional Header.
    PE_ARCHITECTURE architecture = (PE_ARCHITECTURE)pNtHd->OptionalHeader.Magic;

    return architecture;
}


///////// End Stack Overflow
//////////////////////////////////////////////////////////////////////////////
#endif

///////////////////////////////////////////////////////////////
/// \brief isDLL32Bit
/// \param qsFileAndPath
/// \return
/// Utility function to see if the file is 32-bit
bool isDLL32Bit(QString qsFileAndPath)
    {
#ifndef _WIN32
    return false;
#else
    if(qsFileAndPath.isEmpty())
        return false;

    QFile file(qsFileAndPath);
    if(!file.open(QIODevice::ReadOnly))
        return false; // punt...

    // Not gonna lie, just guessed 1024 and it was enough.
    // This is the minimum page size on any OS (I might be wrong,
    // it could be 4096), so going lower is not really helpful.
    // Changed to 4096 on a whim; looking for crashing bug I can't debug
    void *pHeader = file.map(0, 4096, QFileDevice::MapPrivateOption);

    // Another punt as we may not be able to map the file
    if(pHeader == nullptr)
        return false;

    PE_ARCHITECTURE arch = GetImageArchitecture(pHeader);

    file.unmap((uchar *)pHeader);
    file.close();

    return (PE_ARCHITECTURE_X86 == arch);
#endif
    }



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
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    // We never edit a profile directly, we only edit a copy of it.
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();
    setWindowTitle("Creating New Profile");

    // Case 1: New profile (easiest case)
    if(pProfileToEdit == nullptr) {
        pThisProfile = pVulkanConfig->CreateEmptyProfile();
        setWindowTitle(tr("Create new layer configuration"));
        }
    else {
        QString title;

        // We are editing an exisitng profile. Make a copy of it
        pThisProfile = pProfileToEdit->DuplicateProfile();

        // IF this was a fixed profile, clear that setting
        // AND we need to modify the name since we are making a copy
        if(pProfileToEdit->bFixedProfile) {
            pThisProfile->qsFileName = "";
            pThisProfile->qsProfileName += "-Duplicate";
            title = tr("Save Duplicate Configuration");
            }
        else
            title = tr("Edit existing configuration");

        // We now have a profile ready for editing, but only the layers that
        // are actually used are attached. Now, we need to add the remaining layers
        // in their default states
        // Loop through all the available layers. If the layer is not use for the profile
        // attach it for editing.
        addMissingLayers(pThisProfile);
        setWindowTitle(title);
        }

    ui->lineEditName->setText(pThisProfile->qsProfileName);
    ui->lineEditDesc->setText(pThisProfile->qsDescription);

    QTreeWidgetItem *pHeader = ui->layerTree->headerItem();

    pHeader->setText(0, "Layers");
    pHeader->setText(1, "Usage");

    connect(ui->lineEditName, SIGNAL(textChanged(const QString&)), this, SLOT(profileNameChanged(const QString&)));
    connect(ui->layerTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this,
                SLOT(currentLayerChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

    LoadLayerDisplay(0); // Load/Reload the layer editor
    }

void dlgProfileEditor::addMissingLayers(CProfileDef *pProfile)
    {
    int nRank = pProfile->layers.size();    // Next rank starts here
    for(int iAvailable = 0; iAvailable < pVulkanConfig->allLayers.size(); iAvailable++) {
        CLayerFile *pLayerThatMightBeMissing = pVulkanConfig->allLayers[iAvailable];

        // Look for through all layers
        CLayerFile *pAreYouAlreadyThere = pProfile->FindLayer(pLayerThatMightBeMissing->name, pLayerThatMightBeMissing->qsLayerPath);
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
        if(pProfile->blacklistedLayers.contains(pNextLayer->name))
            pNextLayer->bDisabled = true;

        pProfile->layers.push_back(pNextLayer);
        }
    }



dlgProfileEditor::~dlgProfileEditor()
    {
    delete ui;
    }

void dlgProfileEditor::on_pushButtonAddLayers_clicked()
    {
    dlgCustomPaths dlg(this);
    dlg.exec();
    pThisProfile->CollapseProfile();
    addMissingLayers(pThisProfile);
    LoadLayerDisplay();
    }


//////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::profileNameChanged
/// \param qsName
/// Monitor the name of the profile and do not allow saving
/// of a profile with the same name.
void dlgProfileEditor::profileNameChanged(const QString& qsName)
    {
    // Compare the edit field with the list of canned profile names
    for(int i = 0; i < 10; i+=2)
        if(szCannedProfiles[i] == qsName) {
            ui->buttonBox->button(ui->buttonBox->Save)->setEnabled(false);
            return;
            }

    ui->buttonBox->button(ui->buttonBox->Save)->setEnabled(true);
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

       // Add (32-bit) to the name if it is a 32-bit DLL
       QFileInfo path(pItem->pLayer->qsLayerPath);
       QString layerPath = path.path(); // hee hee...

       layerPath += "/";
       layerPath += pItem->pLayer->library_path;
       if(isDLL32Bit(layerPath))
           decoratedName += " (32-bit)";

       // Add implicit or custom to the name
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
    // Push the current working environment on the stack
    pVulkanConfig->pushProfile(pThisProfile);

    dlgLayerOutput dlg(this);
    dlg.bTempEnvironment = true;
    dlg.exec();

    // Pop the current working environmetn off the stack
    pVulkanConfig->popProfile();
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
    if(pLayerItem->pLayer->name == QString("VK_LAYER_KHRONOS_validation"))
        settingsEditor.CreateGUI(ui->scrollArea, pLayerItem->pLayer->layerSettings, EDITOR_TYPE_kHRONOS_ADVANCED, "");
    else
        settingsEditor.CreateGUI(ui->scrollArea, pLayerItem->pLayer->layerSettings, EDITOR_TYPE_GENERAL, "");

    // Is this layer Force on?
    settingsEditor.SetEnabled(pLayerItem->pLayer->bActive);

    /////////////////////////////////////////////////////////////////////
    // Populate the side label
    QString detailsText = pLayerItem->pLayer->description;
    detailsText += "\n";
    switch(pLayerItem->pLayer->layerType) {
        case LAYER_TYPE_EXPLICIT:
        detailsText += "(Explicit Layer)\n\n";
        break;
        case LAYER_TYPE_IMPLICIT:
        detailsText += "(Implicit Layer)\n\n";
        break;
        case LAYER_TYPE_CUSTOM:
        detailsText += "(Custom Layer Path)\n\n";
        break;
        }

    detailsText += pLayerItem->pLayer->library_path;
    detailsText += "\n\n";

    detailsText += "API Version: ";
    detailsText += pLayerItem->pLayer->api_version;
    detailsText += "\n";

    detailsText += "Implementation Version: ";
    detailsText += pLayerItem->pLayer->implementation_version;
    detailsText += "\n\n";

    detailsText += "File format: ";
    detailsText += pLayerItem->pLayer->file_format_version;
    detailsText += "\n\n";

    detailsText += "Full path: ";
    detailsText += pLayerItem->pLayer->qsLayerPath;

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
            settingsEditor.SetEnabled(false);
        break;

        case LAYER_FORCED_ON:
            pLayer->bActive = true;
            pLayer->bDisabled = false;
            settingsEditor.SetEnabled(true);
        break;

        case LAYER_FORCED_OFF:
            pLayer->bActive = false;
            pLayer->bDisabled = true;
            settingsEditor.SetEnabled(false);
        }
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief dlgProfileEditor::accept()
/// This is actually the save button.
/// We are either saving an exisitng profile, or creating a new one.
void dlgProfileEditor::accept()
    {
    // Collect any remaining GUI edits
    settingsEditor.CollectSettings();

    pThisProfile->qsProfileName = ui->lineEditName->text();
    pThisProfile->qsDescription = ui->lineEditDesc->text();


    // Hard Fail: Cannot use two layers with the same name
    bool bSameName = false;
    for(int i = 0; i < pThisProfile->layers.size()-1; i++)
        for(int j = i+1; j < pThisProfile->layers.size(); j++)
            // Layers active OR blacklisted cannot appear more than once
            if((pThisProfile->layers[i]->bActive || pThisProfile->layers[i]->bDisabled) &&
               (pThisProfile->layers[j]->bActive || pThisProfile->layers[j]->bDisabled))
                if(pThisProfile->layers[i]->name == pThisProfile->layers[j]->name) {
                    bSameName = true;
                    break;
                    }

    if(bSameName)
        {
        QMessageBox msg;
        msg.setInformativeText(tr("You cannot use two layers with the same name."));
        msg.setText(tr("Invalid Configuration!"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
        }

    // Hard Fail: Name must not be blank
    if(pThisProfile->qsProfileName.isEmpty()) {
        QMessageBox msg;
        msg.setInformativeText(tr("Configuration must have a name."));
        msg.setText(tr("Name your new config!"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
        }


    //////////////////////////////////////
    // Warn about blacklisting implicit layers
    bool bWarn = false;
    for(int i = 0; i < pThisProfile->layers.size(); i++)
        if(pThisProfile->layers[i]->bDisabled && pThisProfile->layers[i]->layerType == LAYER_TYPE_IMPLICIT) {
            bWarn = true;
            break;
            }

    if(bWarn) {
        QMessageBox warning;
        warning.setInformativeText(tr("You are saving a configuration that disables an implicit layer. Disabling an implicit layer may cause undefined behavior."));
        warning.setText(tr("Disable an implicit layer?"));
        warning.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(warning.exec() == QMessageBox::No)
            return;
        }

    // Prepare... get fully qualified file name, and double check if overwriting
    pThisProfile->qsFileName = pThisProfile->qsProfileName + ".json";
    QString savePath = pVulkanConfig->GetProfilePath();
    savePath += "/" + pThisProfile->qsFileName;

    if(QDir().exists(savePath)) {
        QMessageBox warning;
        warning.setInformativeText(tr("Are you sure you want to overwrite this configuration?"));
        warning.setText(pThisProfile->qsProfileName);
        warning.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        warning.setDefaultButton(QMessageBox::No);
        if(QMessageBox::No == warning.exec())
            return; // No harm, no foul
        }


    // Collapse the profile and remove unused layers and write
    pThisProfile->CollapseProfile();
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

