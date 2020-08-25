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

#include "../vkconfig/configurator.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <QMessageBox>
#include <QComboBox>
#include <QStyle>
#include <QFileDialog>

#include <cassert>

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
    QTreeWidgetItemWithLayer(OverriddenLayer &overridden_layer) : overridden_layer(overridden_layer) { assert(&overridden_layer); }

    OverriddenLayer &overridden_layer;
};

///////////////////////////////////////////////////////////////////////////////
/// Edit a configuration. There are three cases of profiles passed in through pProfileToEdit.
/// Case 1: New configuration: A blank configuration with no layers activated and all layer settings set to their
///         defaults.
///         The file name is blank. User must save the profile.
/// Case 2: Edit: An exisitng configuration that contains specifically activated layers and
///         contains settings that have already been specified and previously saved.
///         The file name is not blank. User can save or abandon the changes.
///////////////////////////////////////////////////////////////////////////////
dlgProfileEditor::dlgProfileEditor(QWidget *parent, Configuration *configuration) : QDialog(parent), ui(new Ui::dlgProfileEditor) {
    assert(parent);
    assert(configuration);

    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    _configuration = configuration;

    // Default, but may not be entirely true
    setWindowTitle("Create a new layers configuration");

    // Case 1: New profile (easiest case)
    if (!_configuration->_name.isEmpty()) {
        // We are editing an exisitng profile. Make a copy of it
        _configuration = configuration->DuplicateConfiguration();

        QString title = tr("Select Vulkan Layers");

        // We now have a profile ready for editing, but only the layers that
        // are actually used are attached. Now, we need to add the remaining layers
        // in their default states
        // Loop through all the available layers. If the layer is not use for the profile
        // attach it for editing.
        AddMissingLayers(_configuration);
        setWindowTitle(title);
    }

    ui->lineEditName->setText(_configuration->_name);
    ui->lineEditDescription->setText(_configuration->_description);

    QTreeWidgetItem *header_item = ui->layerTree->headerItem();

    header_item->setText(0, "Layers");
    header_item->setText(1, "Usage");

    connect(ui->layerTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(currentLayerChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui->treeWidget, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this,
            SLOT(customTreeItemActivated(QTreeWidgetItem *, int)));
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(customTreeItemActivated(QTreeWidgetItem *, int)));

    LoadLayerDisplay(0);  // Load/Reload the layer editor
    PopulateCustomTree();
}

void dlgProfileEditor::AddMissingLayers(Configuration *configuration) {
    assert(configuration);

    int rank = configuration->_overridden_layers.size();  // Next rank starts here

    Configurator &configurator = Configurator::Get();

    for (int layer_index = 0, layer_count = configurator._available_Layers.size(); layer_index < layer_count; layer_index++) {
        const Layer &searched_layer = *configurator._available_Layers[layer_index];

        // Look for through all layers
        OverriddenLayer *layer = configuration->FindOverriddenLayer(searched_layer._name, searched_layer._layer_path);
        if (layer != nullptr)  // It's in the list already
            continue;

        OverriddenLayer overridden_layer;
        overridden_layer.name = searched_layer._name;
        overridden_layer.path = searched_layer._layer_path;
        overridden_layer.type = searched_layer._layer_type;
        overridden_layer.rank = rank++;
        configurator.LoadDefaultSettings(overridden_layer.name, overridden_layer.settings);

        // Check the excluded list
        const bool is_excluded = configuration->_excluded_layers.contains(overridden_layer.name);
        overridden_layer.state = is_excluded ? LAYER_STATE_EXCLUDED : LAYER_STATE_APPLICATION_CONTROLLED;

        configuration->_overridden_layers.push_back(overridden_layer);
    }
}

dlgProfileEditor::~dlgProfileEditor() { delete ui; }

////////////////////////////////////////////////////////////
/// Add a custom layer path, and update everything accordingly
void dlgProfileEditor::on_pushButtonAddLayers_clicked() {
    QString custom_path =
        QFileDialog::getExistingDirectory(this, tr("Add Custom Layer Folder"), "", QFileDialog::DontUseNativeDialog);

    if (custom_path.isEmpty()) return;
    custom_path = QDir::toNativeSeparators(custom_path);

    Configurator::Get().AppendCustomLayersPath(custom_path);

    _configuration->CollapseConfiguration();
    AddMissingLayers(_configuration);
    LoadLayerDisplay();
    PopulateCustomTree();
}

////////////////////////////////////////////////////////////////
// Remove the selected layer path and update everything accordingly
void dlgProfileEditor::on_pushButtonRemoveLayers_clicked() {
    QTreeWidgetItem *selected_item = ui->treeWidget->currentItem();
    assert(selected_item);

    Configurator::Get().RemoveCustomLayersPath(selected_item->text(0));

    _configuration->CollapseConfiguration();
    AddMissingLayers(_configuration);
    LoadLayerDisplay();
    PopulateCustomTree();
}

void dlgProfileEditor::customTreeItemActivated(QTreeWidgetItem *item, int column_index) {
    (void)column_index;
    (void)item;
    ui->pushButtonRemoveLayers->setEnabled(true);
}

////////////////////////////////////////////////////////////
// Custom layer paths and the layers found therein
void dlgProfileEditor::PopulateCustomTree() {
    Configurator::Get().BuildCustomLayerTree(ui->treeWidget);

    ui->pushButtonRemoveLayers->setEnabled(false);
}

//////////////////////////////////////////////////////////////////////////////
/// Load all the available layers and initialize their settings
void dlgProfileEditor::LoadLayerDisplay(int selection) {
    assert((selection >= 0 && selection <= _configuration->_overridden_layers.size()) || selection == -1);

    // Clear out the old
    ui->layerTree->clear();

    // We need some infor for sizing the column with the combo box
    QFontMetrics fm = ui->layerTree->fontMetrics();
    int comboWidth = (fm.size(Qt::TextSingleLine, "App Controlled").width() * 2.2);
    int comboHeight = fm.size(Qt::TextSingleLine, "App Controlled").height() * 1.6;

    // Loop through the layers. They are expected to be in order
    for (std::size_t layer_index = 0; layer_index < _configuration->_overridden_layers.size(); layer_index++) {
        OverriddenLayer &overridden_layer = _configuration->_overridden_layers[layer_index];

        QTreeWidgetItemWithLayer *item = new QTreeWidgetItemWithLayer(overridden_layer);

        QString decoratedName = overridden_layer.name;

        // Add (32-bit) to the name if it is a 32-bit DLL
        QFileInfo path(overridden_layer.path);
        QString layer_path = path.path() + "/";

        const Layer &layer = *Configurator::Get().FindLayer(overridden_layer.name);
        layer_path += layer._library_path;
        if (IsDLL32Bit(layer_path)) decoratedName += " (32-bit)";

        // Add implicit or custom to the name
        if (overridden_layer.type != LAYER_TYPE_EXPLICIT) {
            decoratedName += QString(" (") + GetLayerTypeLabel(overridden_layer.type) + ")";
        }

        item->setText(0, decoratedName);
        item->setFlags(item->flags() | Qt::ItemIsSelectable);
        //        item->setText(1, "App Controlled");  // Fake out for width of column (leaves artifacts on macOS... testing on
        //        other platforms)

        // Add the top level item
        ui->layerTree->addTopLevelItem(item);
        if (layer_index == selection) ui->layerTree->setCurrentItem(item);

        // Add a combo box. Default has gray background which looks hidious
        TreeFriendlyComboBoxWidget *use = new TreeFriendlyComboBoxWidget(item);
        ui->layerTree->setItemWidget(item, 1, use);
        item->setSizeHint(1, QSize(comboWidth, comboHeight));

        use->addItem("Application-Controlled");
        use->addItem("Overridden / Forced On");
        use->addItem("Excluded / Forced Off");
        use->setCurrentIndex(overridden_layer.state);

        connect(use, SIGNAL(selectionMade(QTreeWidgetItem *, int)), this, SLOT(layerUseChanged(QTreeWidgetItem *, int)));

        ///////////////////////////////////////////////////
        // Now for the children, which is just supplimental
        // information. These are NOT QTreeWidgetItemWithLayer
        // because they don't link back to a layer, you have to
        // go up the tree
    }

    resizeEvent(nullptr);
    ui->layerTree->update();
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

    QFontMetrics fm = ui->layerTree->fontMetrics();
    int comboWidth = (fm.size(Qt::TextSingleLine, "App Controlled").width() * 2.2);
    int width = ui->layerTree->width() - comboWidth;
    ui->layerTree->setColumnWidth(0, width);
}

//////////////////////////////////////////////////////////////////////////////
/// This button clears the display. Basically, we delete the profile and
/// start over.
void dlgProfileEditor::on_pushButtonResetLayers_clicked() {
    // TBD, needs to reset which layers are active, settings, etc.
    delete _configuration;
    _configuration = Configurator::Get().CreateEmptyConfiguration();
    //    settingsEditor.CleanupGUI();
    LoadLayerDisplay();
}

/////////////////////////////////////////////////////////////////////
/// The currently selected layer has changed.
void dlgProfileEditor::currentLayerChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    (void)previous;

    // New settings
    QTreeWidgetItemWithLayer *layer_item = dynamic_cast<QTreeWidgetItemWithLayer *>(current);
    if (layer_item == nullptr) {
        ui->labelLayerDetails->setText("");
        return;
    }

    /////////////////////////////////////////////////////////////////////
    // Populate the side label
    assert(&layer_item->overridden_layer);

    const Layer &layer = *Configurator::Get().FindLayer(layer_item->overridden_layer.name);
    assert(layer.IsValid());

    QString detailsText = layer._description;
    detailsText += "\n";
    detailsText += QString("(") + GetLayerTypeLabel(layer_item->overridden_layer.type) + ")\n\n";

    detailsText += layer._library_path;
    detailsText += "\n\n";

    detailsText += "API Version: ";
    detailsText += layer._api_version;
    detailsText += "\n";

    detailsText += "Implementation Version: ";
    detailsText += layer._implementation_version;
    detailsText += "\n\n";

    detailsText += "File format: ";
    detailsText += layer._file_format_version;
    detailsText += "\n\n";

    detailsText += "Full path: ";
    detailsText += layer._layer_path;

    ui->labelLayerDetails->setText(detailsText);
}

///////////////////////////////////////////////////////////////////////////
/// use, don't use, black list...
void dlgProfileEditor::layerUseChanged(QTreeWidgetItem *item, int selection) {
    // Combo box changed. We first need to know which tree item was selected.
    // Fortunatly, changing the combo box also changes the selected item.
    assert(item);
    assert(selection >= LAYER_STATE_FIRST && selection <= LAYER_STATE_LAST);

    QTreeWidgetItemWithLayer *tree_layer_item = dynamic_cast<QTreeWidgetItemWithLayer *>(item);
    assert(tree_layer_item != nullptr);

    OverriddenLayer &layer = tree_layer_item->overridden_layer;
    assert(&layer);

    layer.state = static_cast<LayerState>(selection);
}

///////////////////////////////////////////////////////////////////////////////
/// This is actually the save button.
/// We are either saving an exisitng profile, or creating a new one.
void dlgProfileEditor::accept() {
    _configuration->_name = ui->lineEditName->text();
    _configuration->_description = ui->lineEditDescription->text();

    // Hard Fail: Name must not be blank
    if (_configuration->_name.isEmpty()) {
        QMessageBox msg;
        msg.setInformativeText(tr("Configuration must have a name."));
        msg.setText(tr("Name your new config!"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }

    // Hard Fail: Cannot use two layers with the same name
    bool show_warning_twice = false;
    for (int i = 0; i < _configuration->_overridden_layers.size() - 1; i++)
        for (int j = i + 1; j < _configuration->_overridden_layers.size(); j++)
            // Layers cannot appear more than once
            if ((_configuration->_overridden_layers[i].state != LAYER_STATE_APPLICATION_CONTROLLED) &&
                (_configuration->_overridden_layers[j].state != LAYER_STATE_APPLICATION_CONTROLLED))
                if (_configuration->_overridden_layers[i].name == _configuration->_overridden_layers[j].name) {
                    show_warning_twice = true;
                    break;
                }

    if (show_warning_twice) {
        QMessageBox msg;
        msg.setInformativeText(tr("You cannot use two layers with the same name."));
        msg.setText(tr("Invalid Configuration!"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }

    //////////////////////////////////////
    // Warn about blacklisting implicit layers
    bool show_warning_imcplit = false;
    for (int i = 0; i < _configuration->_overridden_layers.size(); i++)
        if (_configuration->_overridden_layers[i].state == LAYER_STATE_EXCLUDED &&
            _configuration->_overridden_layers[i].type == LAYER_TYPE_IMPLICIT) {
            show_warning_imcplit = true;
            break;
        }

    if (show_warning_imcplit) {
        QMessageBox warning;
        warning.setInformativeText(
            tr("You are saving a configuration that disables an implicit layer. Disabling an implicit layer may cause undefined "
               "behavior."));
        warning.setText(tr("Disable an implicit layer?"));
        warning.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if (warning.exec() == QMessageBox::No) return;
    }

    // Prepare... get fully qualified file name, and double check if overwriting
    const QString &save_path = GetFullPath(DIRECTORY_CONFIGURATION, _configuration->_name);

    if (QDir().exists(save_path)) {
        QMessageBox warning;
        warning.setInformativeText(tr("Are you sure you want to overwrite this configuration?"));
        warning.setText(_configuration->_name);
        warning.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        warning.setDefaultButton(QMessageBox::No);
        if (QMessageBox::No == warning.exec()) return;  // No harm, no foul
    }

    // Collapse the profile and remove unused layers and write
    _configuration->CollapseConfiguration();
    if (!_configuration->Save()) {
        AddMissingLayers(_configuration);
        LoadLayerDisplay(0);
        return;
    }

    QDialog::accept();
}

////////////////////////////////////////////////////////////////////////////////
/// Move the selected layer up in the list
void dlgProfileEditor::on_toolButtonUp_clicked() {
    // Get the item, see if it's a top level item suitable for movement
    QTreeWidgetItemWithLayer *selected_item = dynamic_cast<QTreeWidgetItemWithLayer *>(ui->layerTree->currentItem());
    if (selected_item == nullptr) return;

    // Make sure I'm not already the first one
    if (selected_item->overridden_layer.rank == 0) return;

    // This item goes up by one. The one before it goes down by one
    int bumped = selected_item->overridden_layer.rank;
    selected_item->overridden_layer.rank--;
    QTreeWidgetItemWithLayer *previous_item = dynamic_cast<QTreeWidgetItemWithLayer *>(ui->layerTree->itemAbove(selected_item));
    if (previous_item != nullptr) previous_item->overridden_layer.rank++;

    // The two rank positons should also by their location in the QVector layers. Swap them
    std::swap(_configuration->_overridden_layers[bumped], _configuration->_overridden_layers[bumped - 1]);

    LoadLayerDisplay(bumped - 1);
}

//////////////////////////////////////////////////////////////////////////////
/// Move the selected layer down in the list
void dlgProfileEditor::on_toolButtonDown_clicked() {
    // Get the item, see if it's a top level item suitable for movement
    QTreeWidgetItemWithLayer *selected_item = dynamic_cast<QTreeWidgetItemWithLayer *>(ui->layerTree->currentItem());
    if (selected_item == nullptr) return;

    // Make sure I'm not already the last one
    if (selected_item->overridden_layer.rank == (_configuration->_overridden_layers.size() - 1)) return;

    // This item goes down by one. The one after it goes up by one
    int bumped = selected_item->overridden_layer.rank;
    selected_item->overridden_layer.rank++;
    QTreeWidgetItemWithLayer *previous_item = dynamic_cast<QTreeWidgetItemWithLayer *>(ui->layerTree->itemBelow(selected_item));
    if (previous_item != nullptr) previous_item->overridden_layer.rank--;

    // The two rank positons should also by their location in the QVector layers. Swap them
    std::swap(_configuration->_overridden_layers[bumped], _configuration->_overridden_layers[bumped + 1]);

    LoadLayerDisplay(bumped + 1);
}
