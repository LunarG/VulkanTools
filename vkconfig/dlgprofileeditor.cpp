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

#include "configurator.h"

#include "dlgprofileeditor.h"
#include "ui_dlgprofileeditor.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <QMessageBox>
#include <QComboBox>
#include <QStyle>
#include <QFileDialog>

#ifdef _WIN32
///////////////////////////////////////////////////////////////////////////
// I totally just stole this from Stack Overflow.
#define MKPTR(p1, p2) ((DWORD_PTR)(p1) + (DWORD_PTR)(p2))

typedef enum _pe_architecture {
    PE_ARCHITECTURE_UNKNOWN = 0x0000,
    PE_ARCHITECTURE_ANYCPU = 0x0001,
    PE_ARCHITECTURE_X86 = 0x010B,
    PE_ARCHITECTURE_x64 = 0x020B
} PE_ARCHITECTURE;

LPVOID GetOffsetFromRva(IMAGE_DOS_HEADER *pDos, IMAGE_NT_HEADERS *pNt, DWORD rva) {
    IMAGE_SECTION_HEADER *pSecHd = IMAGE_FIRST_SECTION(pNt);
    for (unsigned long i = 0; i < pNt->FileHeader.NumberOfSections; ++i, ++pSecHd) {
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
    IMAGE_DOS_HEADER *pDosHd = (IMAGE_DOS_HEADER *)pImageBase;
    if (IsBadReadPtr(pDosHd, sizeof(pDosHd->e_magic)) || pDosHd->e_magic != IMAGE_DOS_SIGNATURE) return PE_ARCHITECTURE_UNKNOWN;

    // Parse and validate the NT header
    IMAGE_NT_HEADERS *pNtHd = (IMAGE_NT_HEADERS *)MKPTR(pDosHd, pDosHd->e_lfanew);
    if (IsBadReadPtr(pNtHd, sizeof(pNtHd->Signature)) || pNtHd->Signature != IMAGE_NT_SIGNATURE) return PE_ARCHITECTURE_UNKNOWN;

    // First, naive, check based on the 'Magic' number in the Optional Header.
    PE_ARCHITECTURE architecture = (PE_ARCHITECTURE)pNtHd->OptionalHeader.Magic;

    return architecture;
}

///////// End Stack Overflow
//////////////////////////////////////////////////////////////////////////////
#endif

///////////////////////////////////////////////////////////////
/// Utility function to see if the file is 32-bit
bool IsDLL32Bit(QString full_path) {
#ifndef _WIN32
    (void)full_path;
    return false;
#else
    if (full_path.isEmpty()) return false;

    QFile file(full_path);
    if (!file.open(QIODevice::ReadOnly)) return false;  // punt...

    // Not gonna lie, just guessed 1024 and it was enough.
    // This is the minimum page size on any OS (I might be wrong,
    // it could be 4096), so going lower is not really helpful.
    // Changed to 4096 on a whim; looking for crashing bug I can't debug
    void *header = file.map(0, 4096, QFileDevice::MapPrivateOption);

    // Another punt as we may not be able to map the file
    if (header == nullptr) return false;

    PE_ARCHITECTURE arch = GetImageArchitecture(header);

    file.unmap((uchar *)header);
    file.close();

    return (PE_ARCHITECTURE_X86 == arch);
#endif
}

// We need a way when we get a tree widget item out, to know
// what layer it references. Use this so that all tree widget
// items contain a pointer to the actual layer.
class QTreeWidgetItemWithLayer : public QTreeWidgetItem {
   public:
    LayerFile *layer_file;
};

#define LAYER_APP_CONTROLLED 0
#define LAYER_FORCED_ON 1
#define LAYER_FORCED_OFF 2

///////////////////////////////////////////////////////////////////////////////
/// Edit a profile. There are three cases of profiles passed in through pProfileToEdit.
/// Case 1: New Profile: A blank profile with no layers activated and all layer settings set to their
///         defaults.
///         The file name is blank. User must save the profile.
/// Case 2: Edit: An exisitng profile that contains specifically activated layers and
///         contains settings that have already been specified and previously saved.
///         The file name is not blank. User can save or abandon the changes.
///////////////////////////////////////////////////////////////////////////////
dlgProfileEditor::dlgProfileEditor(QWidget *parent, Configuration *configuration) : QDialog(parent), ui_(new Ui::dlgProfileEditor) {
    ui_->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    configuration_ = configuration;

    // We never edit a profile directly, we only edit a copy of it.
    setWindowTitle("Creating New Configuration");

    // Case 1: New profile (easiest case)
    if (configuration->name.isEmpty()) {
        setWindowTitle(tr("Create new layer configuration"));
    } else {
        // We are editing an exisitng profile. Make a copy of it
        configuration_ = configuration->DuplicateConfiguration();

        QString title = tr("Select Vulkan Layers to Override and Execution Order");

        // We now have a profile ready for editing, but only the layers that
        // are actually used are attached. Now, we need to add the remaining layers
        // in their default states
        // Loop through all the available layers. If the layer is not use for the profile
        // attach it for editing.
        AddMissingLayers(configuration_);
        setWindowTitle(title);
    }

    ui_->lineEditName->setText(configuration_->name);
    //ui_->lineEditDesc->setText(configuration_->description);

    QTreeWidgetItem *header_item = ui_->layerTree->headerItem();

    header_item->setText(0, "Layers");
    header_item->setText(1, "Usage");

    connect(ui_->layerTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(currentLayerChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui_->treeWidget, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this,
            SLOT(customTreeItemActivated(QTreeWidgetItem *, int)));
    connect(ui_->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(customTreeItemActivated(QTreeWidgetItem *, int)));

    LoadLayerDisplay(0);  // Load/Reload the layer editor
    PopulateCustomTree();
}

void dlgProfileEditor::AddMissingLayers(Configuration *configuration) {
    int nRank = configuration->layers.size();  // Next rank starts here

    Configurator &configurator = Configurator::Get();

    for (int layer_index = 0, n = configurator.available_Layers.size(); layer_index < n; layer_index++) {
        LayerFile *searched_layer_file = configurator.available_Layers[layer_index];

        // Look for through all layers
        LayerFile *found_layer_file = configuration->FindLayer(searched_layer_file->name, searched_layer_file->layer_path);
        if (found_layer_file != nullptr)  // It's in the list already
            continue;

        // Nope, add it to the end
        LayerFile *next_layer_file = new LayerFile();
        searched_layer_file->CopyLayer(next_layer_file);

        // Add default settings to the layer...
        configurator.LoadDefaultSettings(next_layer_file);

        next_layer_file->rank = nRank++;
        next_layer_file->enabled = false;  // Layers read from file are already active

        // Check the blacklist
        if (configuration->excluded_layers.contains(next_layer_file->name)) next_layer_file->disabled = true;

        configuration->layers.push_back(next_layer_file);
    }
}

dlgProfileEditor::~dlgProfileEditor() { delete ui_; }

////////////////////////////////////////////////////////////
/// Add a custom layer path, and update everything accordingly
void dlgProfileEditor::on_pushButtonAddLayers_clicked() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    QString custom_path = dialog.getExistingDirectory(this, tr("Add Custom Layer Folder"), "");

    if (custom_path.isEmpty()) return;
    custom_path = QDir::toNativeSeparators(custom_path);

    Configurator::Get().AppendCustomLayersPath(custom_path);

    configuration_->CollapseConfiguration();
    AddMissingLayers(configuration_);
    LoadLayerDisplay();
    PopulateCustomTree();
}

////////////////////////////////////////////////////////////////
// Remove the selected layer path and update everything accordingly
void dlgProfileEditor::on_pushButtonRemoveLayers_clicked() {
    QTreeWidgetItem *selected_item = ui_->layerTree->currentItem();

    int path_index = ui_->layerTree->indexOfTopLevelItem(selected_item);
    Configurator::Get().RemoveCustomLayersPath(path_index);

    configuration_->CollapseConfiguration();
    AddMissingLayers(configuration_);
    LoadLayerDisplay();
    PopulateCustomTree();
}

void dlgProfileEditor::customTreeItemActivated(QTreeWidgetItem *item, int column_index) {
    (void)column_index;
    (void)item;
    ui_->pushButtonRemoveLayers->setEnabled(true);
}

////////////////////////////////////////////////////////////
// Custom layer paths and the layers found therein
void dlgProfileEditor::PopulateCustomTree() {
    ui_->treeWidget->clear();

    Configurator &configurator = Configurator::Get();

    // Populate the tree
    for (int path_index = 0; path_index < configurator.GetCustomLayersPathSize(); ++path_index) {
        const QString custom_path = configurator.GetCustomLayersPath(path_index);
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, custom_path);
        ui_->treeWidget->addTopLevelItem(item);

        // Look for layers that are in this folder. If any are found, add them to the tree
        QVector<LayerFile *> custom_layers;
        configurator.LoadLayersFromPath(custom_path, custom_layers);

        for (int layer_index = 0; layer_index < custom_layers.size(); ++layer_index) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, custom_layers[layer_index]->name);
            item->addChild(child);
        }

        // Free the dynamic memory, the rest passes out of scope
        qDeleteAll(custom_layers.begin(), custom_layers.end());
    }

    ui_->pushButtonRemoveLayers->setEnabled(false);
}

//////////////////////////////////////////////////////////////////////////////
/// Load all the available layers and initialize their settings
void dlgProfileEditor::LoadLayerDisplay(int nSelection) {
    // Clear out the old
    ui_->layerTree->clear();

    // We need some infor for sizing the column with the combo box
    QFontMetrics fm = ui_->layerTree->fontMetrics();
    int comboWidth = (fm.size(Qt::TextSingleLine, "App Controlled").width() * 2.2);
    int comboHeight = fm.size(Qt::TextSingleLine, "App Controlled").height() * 1.6;

    // Loop through the layers. They are expected to be in order
    for (int layer_index = 0; layer_index < configuration_->layers.size(); layer_index++) {
        QTreeWidgetItemWithLayer *item = new QTreeWidgetItemWithLayer();
        item->layer_file = configuration_->layers[layer_index];

        QString decoratedName = item->layer_file->name;

        // Add (32-bit) to the name if it is a 32-bit DLL
        QFileInfo path(item->layer_file->layer_path);
        QString layerPath = path.path();  // hee hee...

        layerPath += "/";
        layerPath += item->layer_file->library_path;
        if (IsDLL32Bit(layerPath)) decoratedName += " (32-bit)";

        // Add implicit or custom to the name
        if (item->layer_file->layer_type == LAYER_TYPE_IMPLICIT) decoratedName += tr(" (Implicit)");

        if (item->layer_file->layer_type == LAYER_TYPE_CUSTOM) decoratedName += tr(" (Custom)");

        item->setText(0, decoratedName);
        item->setFlags(item->flags() | Qt::ItemIsSelectable);
        //        item->setText(1, "App Controlled");  // Fake out for width of column (leaves artifacts on macOS... testing on
        //        other platforms)

        // Add the top level item
        ui_->layerTree->addTopLevelItem(item);
        if (layer_index == nSelection) ui_->layerTree->setCurrentItem(item);

        // Add a combo box. Default has gray background which looks hidious
        TreeFriendlyComboBox *pUse = new TreeFriendlyComboBox(item);
        ui_->layerTree->setItemWidget(item, 1, pUse);
        item->setSizeHint(1, QSize(comboWidth, comboHeight));

        pUse->addItem("Application-Controlled");
        pUse->addItem("Overridden / Forced On");
        pUse->addItem("Excluded / Forced Off");

        if (item->layer_file->enabled) pUse->setCurrentIndex(1);

        if (item->layer_file->disabled) pUse->setCurrentIndex(2);

        connect(pUse, SIGNAL(selectionMade(QTreeWidgetItem *, int)), this, SLOT(layerUseChanged(QTreeWidgetItem *, int)));

        ///////////////////////////////////////////////////
        // Now for the children, which is just supplimental
        // information. These are NOT QTreeWidgetItemWithLayer
        // because they don't link back to a layer, you have to
        // go up the tree
    }

    resizeEvent(nullptr);
    ui_->layerTree->update();
    //    int width = ui->layerTree->width() - comboWidth;
    //    ui->layerTree->setColumnWidth(0, width);
}

// The only way to catch the resize from the layouts
// (which is screwing up the spacing with the combo boxes)
void dlgProfileEditor::showEvent(QShowEvent *event) {
    (void)event;
    resizeEvent(nullptr);
}

void dlgProfileEditor::resizeEvent(QResizeEvent *event) {
    (void)event;
    QFontMetrics fm = ui_->layerTree->fontMetrics();
    int comboWidth = (fm.size(Qt::TextSingleLine, "App Controlled").width() * 2.2);
    int width = ui_->layerTree->width() - comboWidth;
    ui_->layerTree->setColumnWidth(0, width);
}

//////////////////////////////////////////////////////////////////////////////
/// This button clears the display. Basically, we delete the profile and
/// start over.
void dlgProfileEditor::on_pushButtonResetLayers_clicked() {
    // TBD, needs to reset which layers are active, settings, etc.
    delete configuration_;
    configuration_ = Configurator::Get().CreateEmptyConfiguration();
    //    settingsEditor.CleanupGUI();
    LoadLayerDisplay();
}

/////////////////////////////////////////////////////////////////////
/// The currently selected layer has changed.
void dlgProfileEditor::currentLayerChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious) {
    (void)pPrevious;

    // New settings
    QTreeWidgetItemWithLayer *pLayerItem = dynamic_cast<QTreeWidgetItemWithLayer *>(pCurrent);
    if (pLayerItem == nullptr) {
        ui_->labelLayerDetails->setText("");
        return;
    }

    //    // Get the name of the selected layer
    //    QString qsTitle = "Layer Settings (" + pCurrent->text(0);
    //    qsTitle += ")";
    //    ui->groupBoxSettings->setTitle(qsTitle);
    //    if(pLayerItem->pLayer->name == QString("VK_LAYER_KHRONOS_validation"))
    //        settingsEditor.CreateGUI(ui->scrollArea, pLayerItem->pLayer->layerSettings, EDITOR_TYPE_kHRONOS_ADVANCED, "");
    //    else
    //        settingsEditor.CreateGUI(ui->scrollArea, pLayerItem->pLayer->layerSettings, EDITOR_TYPE_GENERAL, "");

    //    // Is this layer Force on?
    //    settingsEditor.SetEnabled(pLayerItem->pLayer->bActive);

    //ui_->lineEditDesc->setText(pLayerItem->layer_file->description);

    /////////////////////////////////////////////////////////////////////
    // Populate the side label
    QString detailsText = pLayerItem->layer_file->description;
    detailsText += "\n";
    switch (pLayerItem->layer_file->layer_type) {
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

    detailsText += pLayerItem->layer_file->library_path;
    detailsText += "\n\n";

    detailsText += "API Version: ";
    detailsText += pLayerItem->layer_file->api_version;
    detailsText += "\n";

    detailsText += "Implementation Version: ";
    detailsText += pLayerItem->layer_file->implementation_version;
    detailsText += "\n\n";

    detailsText += "File format: ";
    detailsText += pLayerItem->layer_file->file_format_version;
    detailsText += "\n\n";

    detailsText += "Full path: ";
    detailsText += pLayerItem->layer_file->layer_path;

    ui_->labelLayerDetails->setText(detailsText);
}

///////////////////////////////////////////////////////////////////////////
/// use, don't use, black list...
void dlgProfileEditor::layerUseChanged(QTreeWidgetItem *item, int selection) {
    // Combo box changed. We first need to know which tree item was selected.
    // Fortunatly, changing the combo box also changes the selected item.

    QTreeWidgetItemWithLayer *tree_layer_item = dynamic_cast<QTreeWidgetItemWithLayer *>(item);
    Q_ASSERT(tree_layer_item != nullptr);

    LayerFile *layer_file = tree_layer_item->layer_file;
    Q_ASSERT(layer_file != nullptr);

    // Okay, easy now, just set the flags appropriately
    switch (selection) {
        case LAYER_APP_CONTROLLED:
            layer_file->enabled = false;
            layer_file->disabled = false;
            //            settingsEditor.SetEnabled(false);
            break;

        case LAYER_FORCED_ON:
            layer_file->enabled = true;
            layer_file->disabled = false;
            //            settingsEditor.SetEnabled(true);
            break;

        case LAYER_FORCED_OFF:
            layer_file->enabled = false;
            layer_file->disabled = true;
            //            settingsEditor.SetEnabled(false);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// This is actually the save button.
/// We are either saving an exisitng profile, or creating a new one.
void dlgProfileEditor::accept() {
    configuration_->name = ui_->lineEditName->text();
    //configuration_->description = ui_->lineEditDesc->text();

    // Hard Fail: Cannot use two layers with the same name
    bool bSameName = false;
    for (int i = 0; i < configuration_->layers.size() - 1; i++)
        for (int j = i + 1; j < configuration_->layers.size(); j++)
            // Layers active OR blacklisted cannot appear more than once
            if ((configuration_->layers[i]->enabled || configuration_->layers[i]->disabled) &&
                (configuration_->layers[j]->enabled || configuration_->layers[j]->disabled))
                if (configuration_->layers[i]->name == configuration_->layers[j]->name) {
                    bSameName = true;
                    break;
                }

    if (bSameName) {
        QMessageBox msg;
        msg.setInformativeText(tr("You cannot use two layers with the same name."));
        msg.setText(tr("Invalid Configuration!"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }

    // Hard Fail: Name must not be blank
    if (configuration_->name.isEmpty()) {
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
    for (int i = 0; i < configuration_->layers.size(); i++)
        if (configuration_->layers[i]->disabled && configuration_->layers[i]->layer_type == LAYER_TYPE_IMPLICIT) {
            bWarn = true;
            break;
        }

    if (bWarn) {
        QMessageBox warning;
        warning.setInformativeText(
            tr("You are saving a configuration that disables an implicit layer. Disabling an implicit layer may cause undefined "
               "behavior."));
        warning.setText(tr("Disable an implicit layer?"));
        warning.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if (warning.exec() == QMessageBox::No) return;
    }

    // Prepare... get fully qualified file name, and double check if overwriting
    configuration_->file = configuration_->name + ".json";
    const QString save_path = Configurator::Get().GetPath(Configurator::ConfigurationPath) + "/" + configuration_->file;

    if (QDir().exists(save_path)) {
        QMessageBox warning;
        warning.setInformativeText(tr("Are you sure you want to overwrite this configuration?"));
        warning.setText(configuration_->name);
        warning.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        warning.setDefaultButton(QMessageBox::No);
        if (QMessageBox::No == warning.exec()) return;  // No harm, no foul
    }

    // Collapse the profile and remove unused layers and write
    configuration_->CollapseConfiguration();
    if (!Configurator::Get().SaveConfiguration(configuration_)) {
        AddMissingLayers(configuration_);
        LoadLayerDisplay(0);
        return;
    }

    QDialog::accept();
}

////////////////////////////////////////////////////////////////////////////////
/// Move the selected layer up in the list
void dlgProfileEditor::on_toolButtonUp_clicked() {
    // Get the item, see if it's a top level item suitable for movement
    QTreeWidgetItemWithLayer *selected_item = dynamic_cast<QTreeWidgetItemWithLayer *>(ui_->layerTree->currentItem());
    if (selected_item == nullptr) return;

    // Make sure I'm not already the first one
    if (selected_item->layer_file->rank == 0) return;

    // This item goes up by one. The one before it goes down by one
    int nBumped = selected_item->layer_file->rank;
    selected_item->layer_file->rank--;
    QTreeWidgetItemWithLayer *previous_item = dynamic_cast<QTreeWidgetItemWithLayer *>(ui_->layerTree->itemAbove(selected_item));
    if (previous_item != nullptr) previous_item->layer_file->rank++;

    // The two rank positons should also by their location in the QVector layers. Swap them
    LayerFile *temp_layer_file = configuration_->layers[nBumped];
    configuration_->layers[nBumped] = configuration_->layers[nBumped - 1];
    configuration_->layers[nBumped - 1] = temp_layer_file;

    LoadLayerDisplay(nBumped - 1);
}

//////////////////////////////////////////////////////////////////////////////
/// Move the selected layer down in the list
void dlgProfileEditor::on_toolButtonDown_clicked() {
    // Get the item, see if it's a top level item suitable for movement
    QTreeWidgetItemWithLayer *selected_item = dynamic_cast<QTreeWidgetItemWithLayer *>(ui_->layerTree->currentItem());
    if (selected_item == nullptr) return;

    // Make sure I'm not already the last one
    if (selected_item->layer_file->rank == (configuration_->layers.size() - 1)) return;

    // This item goes down by one. The one after it goes up by one
    int nBumped = selected_item->layer_file->rank;
    selected_item->layer_file->rank++;
    QTreeWidgetItemWithLayer *previous_item = dynamic_cast<QTreeWidgetItemWithLayer *>(ui_->layerTree->itemBelow(selected_item));
    if (previous_item != nullptr) previous_item->layer_file->rank--;

    // The two rank positons should also by their location in the QVector layers. Swap them
    LayerFile *temp_layer_file = configuration_->layers[nBumped];
    configuration_->layers[nBumped] = configuration_->layers[nBumped + 1];
    configuration_->layers[nBumped + 1] = temp_layer_file;

    LoadLayerDisplay(nBumped + 1);
}
