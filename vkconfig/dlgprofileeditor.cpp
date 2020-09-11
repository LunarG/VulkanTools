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

#if PLATFORM_WINDOWS
#include <windows.h>
#endif

#include <QMessageBox>
#include <QComboBox>
#include <QStyle>
#include <QFileDialog>

#include <cassert>

#if PLATFORM_WINDOWS
// From Stack Overflow:
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
#endif  // PLATFORM_WINDOWS

///////////////////////////////////////////////////////////////
/// Utility function to see if the file is 32-bit
bool IsDLL32Bit(QString full_path) {
#if PLATFORM_WINDOWS
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
#else
    (void)full_path;
    return false;
#endif
}

// We need a way when we get a tree widget item out, to know
// what layer it references. Use this so that all tree widget
// items contain a pointer to the actual layer.
class TreeWidgetLayerItem : public QTreeWidgetItem {
   public:
    TreeWidgetLayerItem() {}

    QString layer_name;
    QString label;
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
    // assert(!_configuration->_name.isEmpty());

    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    _configuration = configuration->Duplicate();

    // Default, but may not be entirely true
    setWindowTitle("Select Vulkan Layers");

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

    LoadLayerTree(0);  // Load/Reload the layer editor
    PopulateCustomTree();
}

void dlgProfileEditor::AddMissingLayers(Configuration *configuration) {
    assert(configuration);

    int rank = configuration->_layers.size();  // Next rank starts here

    Configurator &configurator = Configurator::Get();

    for (int layer_index = 0, layer_count = configurator._available_layers.size(); layer_index < layer_count; layer_index++) {
        Layer *searched_layer = configurator._available_layers[layer_index];

        // Look for through all layers
        Layer *found_layer = configuration->FindLayer(searched_layer->_name, searched_layer->_layer_path);
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

    _configuration->Collapse();
    AddMissingLayers(_configuration);
    LoadLayerTree();
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

    _configuration->Collapse();
    AddMissingLayers(_configuration);
    LoadLayerTree();
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
void dlgProfileEditor::LoadLayerTree(int selection) {
    const QVector<Layer *> &available_layers = Configurator::Get()._available_layers;

    assert((selection >= 0 && selection < available_layers.size()) || selection == -1);

    // Clear out the old
    ui->layerTree->clear();

    // We need some infor for sizing the column with the combo box
    QFontMetrics fm = ui->layerTree->fontMetrics();
    int combo_width = (fm.size(Qt::TextSingleLine, "Application-Controlled").width() * 2.2);
    int combo_height = fm.size(Qt::TextSingleLine, "Application-Controlled").height() * 1.6;

    const std::vector<LayerParameter> &parameters = _configuration->_layer_parameters;
    assert(!parameters.empty());

    for (int layer_index = 0, layer_count = parameters.size(); layer_index < layer_count; layer_index++) {
        const LayerParameter &parameter = parameters[layer_index];

        TreeWidgetLayerItem *item = new TreeWidgetLayerItem;
        ui->layerTree->addTopLevelItem(item);

        TreeFriendlyComboBoxWidget *widget = new TreeFriendlyComboBoxWidget(item);
        ui->layerTree->setItemWidget(item, 1, widget);

        const Layer *layer = Configurator::Get().FindLayerNamed(parameter.name);

        bool is_implicit_layer = false;
        QString decorated_name = parameter.name;

        if (layer == nullptr) {  // The layer is missing!
            decorated_name += " (Missing)";
        } else {
            // Add (32-bit) to the name if it is a 32-bit DLL
            QFileInfo path(layer->_layer_path);
            const QString layer_path = path.path() + "/" + layer->_library_path;  // hee hee...

            if (IsDLL32Bit(layer_path)) decorated_name += " (32-bit)";

            is_implicit_layer = layer->_layer_type == LAYER_TYPE_IMPLICIT;

            // Add implicit or custom to the name
            if (is_implicit_layer) {
                decorated_name += QString(" (") + GetLayerTypeLabel(layer->_layer_type) + ")";
            }

            item->setFlags(item->flags() | Qt::ItemIsSelectable);
        }

        item->layer_name = parameter.name;
        item->setSizeHint(1, QSize(combo_width, combo_height));
        item->setText(0, decorated_name);

        if (is_implicit_layer)
            widget->addItem("Implicitly On");
        else
            widget->addItem("Application-Controlled");
        widget->addItem("Overridden / Forced On");
        widget->addItem("Excluded / Forced Off");
        widget->setCurrentIndex(parameter.state);

        connect(widget, SIGNAL(selectionMade(QTreeWidgetItem *, int)), this, SLOT(layerUseChanged(QTreeWidgetItem *, int)));
    }

    /*
        // Loop through the layers. They are expected to be in order
        for (int layer_index = 0, layer_count = available_layers.size(); layer_index < layer_count; layer_index++) {
            Layer& layer = *available_layers[layer_index];
            assert(layer.IsValid());

            TreeWidgetLayerItem *item = new TreeWidgetLayerItem;

            // Add (32-bit) to the name if it is a 32-bit DLL
            QFileInfo path(layer._layer_path);
            const QString layer_path = path.path() + "/" + layer._library_path;  // hee hee...

            QString decorated_name = layer._name;
            if (IsDLL32Bit(layer_path)) decorated_name += " (32-bit)";

            // Add implicit or custom to the name
            if (layer._layer_type != LAYER_TYPE_EXPLICIT) {
                decorated_name += QString(" (") + GetLayerTypeLabel(layer._layer_type) + ")";
            }

            item->setText(0, decorated_name);
            item->setFlags(item->flags() | Qt::ItemIsSelectable);

            // Add the top level item
            ui->layerTree->addTopLevelItem(item);
            if (layer_index == selection) ui->layerTree->setCurrentItem(item);

            // Add a combo box. Default has gray background which looks hidious
            TreeFriendlyComboBoxWidget *use = new TreeFriendlyComboBoxWidget(item);
            ui->layerTree->setItemWidget(item, 1, use);
        }
    */
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
    _configuration->Reset();

    LoadLayerTree();
}

/////////////////////////////////////////////////////////////////////
/// The currently selected layer has changed.
void dlgProfileEditor::currentLayerChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    (void)previous;
    /*
        // New settings
        QTreeWidgetItemWithLayer *layer_item = dynamic_cast<QTreeWidgetItemWithLayer *>(current);
        if (layer_item == nullptr) {
            ui->labelLayerDetails->setText("");
            return;
        }

        /////////////////////////////////////////////////////////////////////
        // Populate the side label
        assert(&layer_item->layer);
        assert(layer_item->layer.IsValid());

        QString detailsText = layer_item->layer._description;
        detailsText += "\n";
        detailsText += QString("(") + GetLayerTypeLabel(layer_item->layer._layer_type) + ")\n\n";

        detailsText += layer_item->layer._library_path;
        detailsText += "\n\n";

        detailsText += "API Version: ";
        detailsText += layer_item->layer._api_version.str().c_str();
        detailsText += "\n";

        detailsText += "Implementation Version: ";
        detailsText += layer_item->layer._implementation_version;
        detailsText += "\n\n";

        detailsText += "File format: ";
        detailsText += layer_item->layer._file_format_version.str().c_str();
        detailsText += "\n\n";

        detailsText += "Full path: ";
        detailsText += layer_item->layer._layer_path;

        ui->labelLayerDetails->setText(detailsText);
    */
}

///////////////////////////////////////////////////////////////////////////
/// use, don't use, black list...
void dlgProfileEditor::layerUseChanged(QTreeWidgetItem *item, int selection) {
    // Combo box changed. We first need to know which tree item was selected.
    // Fortunatly, changing the combo box also changes the selected item.
    assert(item);
    assert(selection >= LAYER_STATE_FIRST && selection <= LAYER_STATE_LAST);

    TreeWidgetLayerItem *tree_layer_item = dynamic_cast<TreeWidgetLayerItem *>(item);
    assert(tree_layer_item != nullptr);

    std::vector<LayerParameter> &parameters = _configuration->_layer_parameters;
    assert(!parameters.empty());

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        if (parameters[i].name == tree_layer_item->layer_name) parameters[i].state = static_cast<LayerState>(selection);
    }

    // Layer &layer = tree_layer_item->layer;
    // assert(&layer);

    // layer._state = static_cast<LayerState>(selection);
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

    //////////////////////////////////////
    // Warn about blacklisting implicit layers
    bool show_warning_imcplit = false;
    for (int i = 0; i < _configuration->_layers.size(); i++)
        if (_configuration->_layers[i]->_state == LAYER_STATE_EXCLUDED &&
            _configuration->_layers[i]->_layer_type == LAYER_TYPE_IMPLICIT) {
            show_warning_imcplit = true;
            break;
        }

    if (show_warning_imcplit) {
        QMessageBox alert;
        alert.setWindowTitle("Implicit layer disabled...");
        alert.setText("Disabling an implicit layer may cause undefined behavior, including crashes.");
        alert.setInformativeText("Do you want to continue?");
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setIcon(QMessageBox::Warning);
        if (alert.exec() == QMessageBox::No) return;
    }

    // Prepare... get fully qualified file name, and double check if overwriting
    const QString save_path = Configurator::Get().path.GetFullPath(PATH_CONFIGURATION, _configuration->_name);

    // Collapse the profile and remove unused layers and write
    _configuration->Collapse();
    const bool result = _configuration->Save(save_path);
    assert(result);

    QDialog::accept();
}
