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

#include "../vkconfig_core/platform.h"
#include "../vkconfig_core/util.h"

#if PLATFORM_WINDOWS
#include <windows.h>
#endif

#include <QMessageBox>
#include <QComboBox>
#include <QStyle>
#include <QFileDialog>

#include <cassert>

#if PLATFORM_WINDOWS
// From Stack Overflow.
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

// End Stack Overflow
#endif

///////////////////////////////////////////////////////////////
/// Utility function to see if the file is 32-bit
bool IsDLL32Bit(QString full_path) {
#if PLATFORM_WINDOWS
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
class TreeWidgetItemParameter : public QTreeWidgetItem {
   public:
    TreeWidgetItemParameter(Parameter &parameter) : parameter(parameter) { assert(&parameter); }

    Parameter &parameter;
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
    setWindowTitle("Select Vulkan Layers");

    _configuration = configuration->Duplicate();

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
/*
void dlgProfileEditor::AddMissingLayers(Configuration *configuration) {
    assert(configuration);

    int rank = configuration->_layers.size();  // Next rank starts here

    Configurator &configurator = Configurator::Get();

    for (int layer_index = 0, layer_count = configurator._available_Layers.size(); layer_index < layer_count; layer_index++) {
        Layer *searched_layer = configurator._available_Layers[layer_index];

        // Look for through all layers
        Layer *found_layer = configuration->FindParameter(searched_layer->_name, searched_layer->_layer_path);
        if (found_layer != nullptr)  // It's in the list already
            continue;

        // Nope, add it to the end
        Layer *next_layer = new Layer(*searched_layer);

        // Add default settings to the layer...
        configurator.LoadDefaultSettings(next_layer);

        next_layer->_rank = rank++;

        // Check the excluded list
        const bool is_excluded = configuration->_excluded_layers.contains(next_layer->_name);
        next_layer->_state = is_excluded ? LAYER_STATE_EXCLUDED : LAYER_STATE_APPLICATION_CONTROLLED;

        assert(next_layer->IsValid());
        configuration->_layers.push_back(next_layer);
    }
}
*/
dlgProfileEditor::~dlgProfileEditor() { delete ui; }

////////////////////////////////////////////////////////////
/// Add a custom layer path, and update everything accordingly
void dlgProfileEditor::on_pushButtonAddLayers_clicked() {
    Configurator &configurator = Configurator::Get();
    const QString custom_path = configurator.path.SelectPath(this, PATH_CUSTOM_LAYER_PATH);

    if (custom_path.isEmpty()) {
        return;
    }

    if (configurator.environment.AppendCustomLayerPath(custom_path)) {
        configurator.LoadAllInstalledLayers();
        configurator.LoadAllConfigurations();
    }

    //_configuration->Collapse();
    // AddMissingLayers(_configuration);
    LoadLayerDisplay();
    PopulateCustomTree();
}

////////////////////////////////////////////////////////////////
// Remove the selected layer path and update everything accordingly
void dlgProfileEditor::on_pushButtonRemoveLayers_clicked() {
    QTreeWidgetItem *selected_item = ui->treeWidget->currentItem();
    assert(selected_item);

    Configurator &configurator = Configurator::Get();

    if (configurator.environment.RemoveCustomLayerPath(selected_item->text(0))) {
        configurator.LoadAllInstalledLayers();
        configurator.LoadAllConfigurations();
    }

    //_configuration->Collapse();
    // AddMissingLayers(_configuration);
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
    assert((selection >= 0 && selection < _configuration->parameters.size()) || selection == 0);

    // Clear out the old
    ui->layerTree->clear();

    // We need some infor for sizing the column with the combo box
    QFontMetrics fm = ui->layerTree->fontMetrics();
    int combo_width = (fm.size(Qt::TextSingleLine, "App Controlled").width() * 2.2);
    int combo_height = fm.size(Qt::TextSingleLine, "App Controlled").height() * 1.6;

    // Loop through the layers. They are expected to be in order
    for (std::size_t i = 0, n = _configuration->parameters.size(); i < n; i++) {
        Parameter &parameter = _configuration->parameters[i];

        TreeWidgetItemParameter *item = new TreeWidgetItemParameter(parameter);

        // Add (32-bit) to the name if it is a 32-bit DLL
        const QFileInfo path(parameter.path);
        const QString layerPath = path.path() + "/" + parameter.path;

        QString decorated_name = parameter.name;
        if (IsDLL32Bit(layerPath)) decorated_name += " (32-bit)";

        const Layer *layer = Configurator::Get().FindLayerNamed(parameter.name);

        bool is_implicit_layer = false;
        if (layer != nullptr) {
            // Add implicit or custom to the name
            if (layer->_layer_type == LAYER_TYPE_IMPLICIT) {
                is_implicit_layer = true;
                decorated_name += QString(" (") + GetLayerTypeLabel(layer->_layer_type) + ")";
            }
        } else {
            decorated_name += " (Missing)";
        }

        item->setText(0, decorated_name);
        item->setFlags(item->flags() | Qt::ItemIsSelectable);

        // Add the top level item
        ui->layerTree->addTopLevelItem(item);
        if (i == selection) ui->layerTree->setCurrentItem(item);

        // Add a combo box. Default has gray background which looks hidious
        TreeFriendlyComboBoxWidget *widget = new TreeFriendlyComboBoxWidget(item);
        ui->layerTree->setItemWidget(item, 1, widget);
        item->setSizeHint(1, QSize(combo_width, combo_height));

        if (is_implicit_layer)
            widget->addItem("Implicitly On");
        else
            widget->addItem("Application-Controlled");
        widget->addItem("Overridden / Forced On");
        widget->addItem("Excluded / Forced Off");
        widget->setCurrentIndex(parameter.state);

        connect(widget, SIGNAL(selectionMade(QTreeWidgetItem *, int)), this, SLOT(layerUseChanged(QTreeWidgetItem *, int)));

        ///////////////////////////////////////////////////
        // Now for the children, which is just supplimental
        // information. These are NOT QTreeWidgetItemWithLayer
        // because they don't link back to a layer, you have to
        // go up the tree
    }

    resizeEvent(nullptr);
    ui->layerTree->update();
}

QString dlgProfileEditor::GetConfigurationName() const {
    assert(_configuration);
    return _configuration->_name;
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
    TreeWidgetItemParameter *layer_item = dynamic_cast<TreeWidgetItemParameter *>(current);
    if (layer_item == nullptr) {
        ui->labelLayerDetails->setText("");
        return;
    }

    /////////////////////////////////////////////////////////////////////
    // Populate the side label
    assert(&layer_item->parameter);

    const Layer *layer = Configurator::Get().FindLayerNamed(layer_item->parameter.name);
    if (layer) {
        QString detailsText = layer->_description;
        detailsText += "\n";
        detailsText += QString("(") + GetLayerTypeLabel(layer->_layer_type) + ")\n\n";

        detailsText += layer_item->parameter.path;
        detailsText += "\n\n";

        detailsText += "API Version: ";
        detailsText += layer->_api_version.str().c_str();
        detailsText += "\n";

        detailsText += "Implementation Version: ";
        detailsText += layer->_implementation_version;
        detailsText += "\n\n";

        detailsText += "File format: ";
        detailsText += layer->_file_format_version.str().c_str();
        detailsText += "\n\n";

        detailsText += "Full path: ";
        detailsText += layer_item->parameter.path;

        ui->labelLayerDetails->setText(detailsText);
    } else {
        ui->labelLayerDetails->setText("Missing layer");
    }
}

///////////////////////////////////////////////////////////////////////////
/// use, don't use, black list...
void dlgProfileEditor::layerUseChanged(QTreeWidgetItem *item, int selection) {
    // Combo box changed. We first need to know which tree item was selected.
    // Fortunatly, changing the combo box also changes the selected item.
    assert(item);
    assert(selection >= LAYER_STATE_FIRST && selection <= LAYER_STATE_LAST);

    TreeWidgetItemParameter *tree_layer_item = dynamic_cast<TreeWidgetItemParameter *>(item);
    assert(tree_layer_item != nullptr);

    Parameter &parameter = tree_layer_item->parameter;
    assert(&parameter);

    LayerState layer_state = static_cast<LayerState>(selection);

    if (layer_state == LAYER_STATE_EXCLUDED) {
        const Layer *layer = Configurator::Get().FindLayerNamed(parameter.name);

        if (layer) {
            if (layer->_layer_type == LAYER_TYPE_IMPLICIT) {
                QMessageBox alert;
                alert.setWindowTitle("Implicit layer excluded...");
                alert.setText(
                    format("%s was excluded but it an implicit layer. This may cause undefined behavior, including crashes. ",
                           parameter.name.toUtf8().constData())
                        .c_str());
                alert.setInformativeText("Do you want to continue?");
                alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                alert.setIcon(QMessageBox::Warning);
                if (alert.exec() == QMessageBox::No) {
                    layer_state = LAYER_STATE_APPLICATION_CONTROLLED;
                }
            }
        }
    }

    // tree_layer_item->setCurrentIndex(layer_state);
    parameter.state = layer_state;
}

///////////////////////////////////////////////////////////////////////////////
/// This is actually the save button.
/// We are either saving an exisitng profile, or creating a new one.
void dlgProfileEditor::accept() {
    _configuration->_name = ui->lineEditName->text();
    _configuration->_description = ui->lineEditDescription->text();

    // Hard Fail: Name must not be blank
    if (ui->lineEditName->text().isEmpty()) {
        QMessageBox alert;
        alert.setWindowTitle("The configuration name is empty...");
        alert.setText("The configuration name is required.");
        alert.setStandardButtons(QMessageBox::Ok);
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
        return;
    }

    for (std::size_t i = 0, n = _configuration->parameters.size(); i < n; ++i) {
        const Parameter &parameter = _configuration->parameters[i];

        const Layer *layer = Configurator::Get().FindLayerNamed(parameter.name);
        if (layer) {
            if (parameter.state != LAYER_STATE_EXCLUDED) continue;
            if (layer->_layer_type != LAYER_TYPE_IMPLICIT) continue;

            // Warn about excluded implicit layers
            QMessageBox alert;
            alert.setWindowTitle("Implicit layer excluded...");
            alert.setText(format("%s was excluded but it an implicit layer. This may cause undefined behavior, including crashes. ",
                                 parameter.name.toUtf8().constData())
                              .c_str());
            alert.setInformativeText("Do you want to continue?");
            alert.setStandardButtons(QMessageBox::Ok);
            alert.setIcon(QMessageBox::Warning);
            alert.exec();
        } else {
            // Warn about missing layers
            QMessageBox alert;
            alert.setWindowTitle("Missing layer...");
            alert.setText(format("%s couldn't be found", parameter.name.toUtf8().constData()).c_str());
            alert.setInformativeText("The layer won't be overridden");
            alert.setStandardButtons(QMessageBox::Ok);
            alert.setIcon(QMessageBox::Warning);
            alert.exec();
        }
    }

    // Prepare... get fully qualified file name, and double check if overwriting
    const QString save_path = Configurator::Get().path.GetFullPath(PATH_CONFIGURATION, _configuration->_name);

    const bool result = _configuration->Save(save_path);
    assert(result);

    QDialog::accept();
}

/*
////////////////////////////////////////////////////////////////////////////////
/// Move the selected layer up in the list
void dlgProfileEditor::on_toolButtonUp_clicked() {
    // Get the item, see if it's a top level item suitable for movement
    TreeWidgetItemParameter *selected_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTree->currentItem());
    if (selected_item == nullptr) return;

    // Make sure I'm not already the first one
    if (selected_item->layer._rank == 0) return;

    // This item goes up by one. The one before it goes down by one
    int bumped = selected_item->layer._rank;
    selected_item->layer._rank--;
    TreeWidgetItemParameter *previous_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTree->itemAbove(selected_item));
    if (previous_item != nullptr) previous_item->layer._rank++;

    // The two rank positons should also by their location in the QVector layers. Swap them
    std::swap(_configuration->_layers[bumped], _configuration->_layers[bumped - 1]);

    LoadLayerDisplay(bumped - 1);
}

//////////////////////////////////////////////////////////////////////////////
/// Move the selected layer down in the list
void dlgProfileEditor::on_toolButtonDown_clicked() {
    // Get the item, see if it's a top level item suitable for movement
    TreeWidgetItemParameter *selected_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTree->currentItem());
    if (selected_item == nullptr) return;

    // Make sure I'm not already the last one
    if (selected_item->layer._rank == (_configuration->_layers.size() - 1)) return;

    // This item goes down by one. The one after it goes up by one
    int bumped = selected_item->layer._rank;
    selected_item->layer._rank++;
    TreeWidgetItemParameter *previous_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTree->itemBelow(selected_item));
    if (previous_item != nullptr) previous_item->layer._rank--;

    // The two rank positons should also by their location in the QVector layers. Swap them
    std::swap(_configuration->_layers[bumped], _configuration->_layers[bumped + 1]);

    LoadLayerDisplay(bumped + 1);
}
*/
