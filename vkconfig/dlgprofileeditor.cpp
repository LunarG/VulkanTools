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
#include "alert.h"

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

///////////////////////////////////////////////////////////////////////////////
/// Edit a configuration. There are three cases of profiles passed in through pProfileToEdit.
/// Case 1: New configuration: A blank configuration with no layers activated and all layer settings set to their
///         defaults.
///         The file name is blank. User must save the profile.
/// Case 2: Edit: An exisitng configuration that contains specifically activated layers and
///         contains settings that have already been specified and previously saved.
///         The file name is not blank. User can save or abandon the changes.
///////////////////////////////////////////////////////////////////////////////
dlgProfileEditor::dlgProfileEditor(QWidget *parent, const Configuration &configuration)
    : QDialog(parent), ui(new Ui::dlgProfileEditor), configuration(configuration) {
    assert(parent);
    assert(&configuration);

    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->lineEditName->setText(configuration._name);
    ui->lineEditDescription->setText(configuration._description);

    QTreeWidgetItem *header_item = ui->layerTree->headerItem();

    header_item->setText(0, "Layers");
    header_item->setText(1, "Usage");

    connect(ui->layerTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(currentLayerChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui->treeWidget, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this,
            SLOT(customTreeItemActivated(QTreeWidgetItem *, int)));
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(customTreeItemActivated(QTreeWidgetItem *, int)));

    LoadLayerDisplay();  // Load/Reload the layer editor
    PopulateCustomTree();
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

void dlgProfileEditor::AddLayerItem(const Parameter &parameter) {
    assert(!parameter.name.isEmpty());

    Configurator &configurator = Configurator::Get();

    const Layer *layer = configurator.FindLayerNamed(parameter.name);

    QString decorated_name = parameter.name;

    bool is_implicit_layer = false;
    if (layer != nullptr) {
        if (IsDLL32Bit(layer->_layer_path)) decorated_name += " (32-bit)";

        if (layer->_layer_type == LAYER_TYPE_IMPLICIT) {
            is_implicit_layer = true;
            decorated_name += QString(" (") + GetLayerTypeLabel(layer->_layer_type) + ")";
        }
    } else {
        decorated_name += " (Missing)";
    }

    TreeWidgetItemParameter *item = new TreeWidgetItemParameter(parameter);

    item->setText(0, decorated_name);
    item->setFlags(item->flags() | Qt::ItemIsSelectable);

    // Add the top level item
    ui->layerTree->addTopLevelItem(item);

    // Add a combo box. Default has gray background which looks hidious
    TreeFriendlyComboBoxWidget *widget = new TreeFriendlyComboBoxWidget(item);
    ui->layerTree->setItemWidget(item, 1, widget);

    const QFontMetrics fm = ui->layerTree->fontMetrics();
    const QSize combo_size = fm.size(Qt::TextSingleLine, "Application-Controlled") * 1.6;
    item->setSizeHint(1, combo_size);

    if (is_implicit_layer)
        widget->addItem("Implicitly On");
    else
        widget->addItem("Application-Controlled");
    widget->addItem("Overridden / Forced On");
    widget->addItem("Excluded / Forced Off");
    widget->setCurrentIndex(item->parameter.state);

    connect(widget, SIGNAL(selectionMade(QTreeWidgetItem *, int)), this, SLOT(layerUseChanged(QTreeWidgetItem *, int)));
}

//////////////////////////////////////////////////////////////////////////////
/// Load all the available layers and initialize their settings
void dlgProfileEditor::LoadLayerDisplay() {
    ui->layerTree->clear();

    Configurator &configurator = Configurator::Get();

    // Loop through the layers. They are expected to be in order
    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        const Parameter &parameter = configuration.parameters[i];
        assert(!parameter.name.isEmpty());

        AddLayerItem(parameter);
    }

    QVector<Layer *> &available_layers = Configurator::Get()._available_Layers;
    for (int i = 0, n = available_layers.size(); i < n; ++i) {
        const Layer &layer = *available_layers[i];

        // The layer is already in the layer tree
        if (configuration.FindParameter(layer._name)) continue;

        Parameter parameter;
        parameter.name = layer._name;
        parameter.state = LAYER_STATE_APPLICATION_CONTROLLED;

        const LayerSettingsDefaults *defaults = configurator.FindLayerSettings(layer._name);
        if (defaults) parameter.settings = defaults->settings;

        if (layer._name == "VK_LAYER_KHRONOS_validation") {
            configuration._preset = GetValidationPreset(ui->lineEditName->text());
        }

        AddLayerItem(parameter);
    }

    resizeEvent(nullptr);

    if (ui->layerTree->topLevelItemCount() > 0) ui->layerTree->setCurrentItem(ui->layerTree->topLevelItem(0));
    ui->layerTree->update();
}

QString dlgProfileEditor::GetConfigurationName() const {
    assert(!ui->lineEditName->text().isEmpty());
    return ui->lineEditName->text();
}

// The only way to catch the resize from the layouts
// (which is screwing up the spacing with the combo boxes)
void dlgProfileEditor::showEvent(QShowEvent *event) {
    (void)event;

    resizeEvent(nullptr);
}

void dlgProfileEditor::resizeEvent(QResizeEvent *event) {
    (void)event;

    const QFontMetrics fm = ui->layerTree->fontMetrics();
    const int combo_width = (fm.size(Qt::TextSingleLine, "Application-Controlled").width() * 1.6);
    const int width = ui->layerTree->width() - combo_width;
    ui->layerTree->setColumnWidth(0, width);
}

//////////////////////////////////////////////////////////////////////////////
/// This button clears the display. Basically, we delete the profile and
/// start over.
void dlgProfileEditor::on_pushButtonResetLayers_clicked() {
    for (int i = 0, n = ui->layerTree->topLevelItemCount(); i < n; ++i) {
        TreeWidgetItemParameter *layer_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTree->topLevelItem(i));
        assert(layer_item);
        layer_item->parameter.state = LAYER_STATE_APPLICATION_CONTROLLED;

        if (layer_item->parameter.name == "VK_LAYER_KHRONOS_validation") {
            configuration._preset = GetValidationPreset(configuration._name);
        }

        const LayerSettingsDefaults *defaults = Configurator::Get().FindLayerSettings(layer_item->parameter.name);
        if (defaults) layer_item->parameter.settings = defaults->settings;

        TreeFriendlyComboBoxWidget *widget = dynamic_cast<TreeFriendlyComboBoxWidget *>(ui->layerTree->itemWidget(layer_item, 1));
        assert(widget);
        widget->setCurrentIndex(layer_item->parameter.state);
        ui->layerTree->repaint();  // Force update for macOS
    }
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

    // Populate the side label
    assert(!layer_item->parameter.name.isEmpty());

    const Layer *layer = Configurator::Get().FindLayerNamed(layer_item->parameter.name);
    if (layer) {
        QString detailsText = layer->_description;
        detailsText += "\n";
        detailsText += QString("(") + GetLayerTypeLabel(layer->_layer_type) + ")\n";

        detailsText += layer->_library_path;
        detailsText += "\n\n";

        detailsText += "API Version: ";
        detailsText += layer->_api_version.str().c_str();
        detailsText += "\n";

        detailsText += "Implementation Version: ";
        detailsText += layer->_implementation_version;
        detailsText += "\n\n";

        detailsText += layer->_layer_path;
        detailsText += "\n";

        detailsText += "File format: ";
        detailsText += layer->_file_format_version.str().c_str();

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

    LayerState layer_state = static_cast<LayerState>(selection);

    if (layer_state == LAYER_STATE_EXCLUDED) {
        const Layer *layer = Configurator::Get().FindLayerNamed(tree_layer_item->parameter.name);

        if (layer) {
            if (layer->_layer_type == LAYER_TYPE_IMPLICIT) {
                QMessageBox alert;
                alert.setWindowTitle("Implicit layer excluded...");
                alert.setText(
                    format("%s was excluded but it an implicit layer. This may cause undefined behavior, including crashes. ",
                           tree_layer_item->parameter.name.toUtf8().constData())
                        .c_str());
                alert.setInformativeText("Do you want to continue?");
                alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                alert.setIcon(QMessageBox::Warning);
                if (alert.exec() == QMessageBox::No) {
                    layer_state = LAYER_STATE_APPLICATION_CONTROLLED;

                    TreeFriendlyComboBoxWidget *widget =
                        dynamic_cast<TreeFriendlyComboBoxWidget *>(ui->layerTree->itemWidget(tree_layer_item, 1));
                    assert(widget);
                    widget->setCurrentIndex(layer_state);
                }
            }
        }
    }

    tree_layer_item->parameter.state = layer_state;
}

///////////////////////////////////////////////////////////////////////////////
/// This is actually the save button.
/// We are either saving an exisitng profile, or creating a new one.
void dlgProfileEditor::accept() {
    // Hard Fail: Name must not be blank
    if (ui->lineEditName->text().isEmpty()) {
        Alert::ConfigurationNameEmpty();
        return;
    }

    Configurator &configurator = Configurator::Get();
    if (configuration._name != ui->lineEditName->text() && configurator.FindConfiguration(ui->lineEditName->text())) {
        Alert::ConfigurationRenamingFailed();
        return;
    }

    std::vector<Parameter> parameters;

    for (int i = 0, n = ui->layerTree->topLevelItemCount(); i < n; ++i) {
        TreeWidgetItemParameter *layer_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTree->topLevelItem(i));
        assert(layer_item);

        if (layer_item->parameter.state == LAYER_STATE_APPLICATION_CONTROLLED) continue;

        const Layer *layer = Configurator::Get().FindLayerNamed(layer_item->parameter.name);
        if (!layer) {
            // Warn about missing layers
            QMessageBox alert;
            alert.setWindowTitle("Missing layer...");
            alert.setText(format("%s couldn't be found", layer_item->parameter.name.toUtf8().constData()).c_str());
            alert.setInformativeText("The Vulkan layers won't be overridden using this configuration");
            alert.setStandardButtons(QMessageBox::Ok);
            alert.setIcon(QMessageBox::Warning);
            alert.exec();
        }

        parameters.push_back(layer_item->parameter);
    }

    SortLayers(parameters);

    configuration._name = ui->lineEditName->text();
    configuration._description = ui->lineEditDescription->text();
    configuration.parameters = parameters;

    const QString save_path = Configurator::Get().path.GetFullPath(PATH_CONFIGURATION, ui->lineEditName->text());
    const bool result = configuration.Save(save_path);
    assert(result);

    QDialog::accept();
}
