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

#include "dialog_layers.h"
#include "dialog_custom_paths.h"

#include "configurator.h"
#include "alert.h"

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

LayersDialog::LayersDialog(QWidget *parent, const Configuration &configuration)
    : QDialog(parent), ui(new Ui::dialog_layers), configuration(configuration) {
    assert(parent);
    assert(&configuration);

    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->lineEditName->setText(configuration.name);
    ui->lineEditDescription->setText(configuration._description);

    Environment &environment = Configurator::Get().environment;
    restoreGeometry(environment.Get(LAYOUT_LAYER_GEOMETRY));

    const QByteArray &restore_splitter_state = environment.Get(LAYOUT_LAYER_SPLITTER);
    if (restore_splitter_state.isEmpty()) {
        ui->splitter->setSizes(QList<int>({300, 30}));
    } else {
        ui->splitter->restoreState(environment.Get(LAYOUT_LAYER_SPLITTER));
    }

    QTreeWidgetItem *header_item = ui->layerTree->headerItem();

    header_item->setText(0, "Layers");
    header_item->setText(1, "Usage");

    connect(ui->layerTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(currentLayerChanged(QTreeWidgetItem *, QTreeWidgetItem *)));

    connect(ui->layerTreeSorted, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnLayerTreeSortedClicked(QTreeWidgetItem *, int)));

    BuildParameters();
    LoadAvailableLayersUI();
    LoadSortedLayersUI();

    UpdateUI();
}

LayersDialog::~LayersDialog() {
    Environment &environment = Configurator::Get().environment;

    environment.Set(LAYOUT_LAYER_SPLITTER, ui->splitter->saveState());
    environment.Set(LAYOUT_LAYER_GEOMETRY, saveGeometry());
}

void LayersDialog::UpdateUI() {
    if (!selected_available_layer_name.isEmpty()) {
        for (int i = 0, n = ui->layerTree->topLevelItemCount(); i < n; ++i) {
            TreeWidgetItemParameter *layer_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTree->topLevelItem(i));
            if (layer_item->layer_name != selected_available_layer_name) continue;

            ui->layerTree->setCurrentItem(layer_item);
        }
    } else if (ui->layerTree->topLevelItemCount() > 0) {
        ui->layerTree->setCurrentItem(ui->layerTree->topLevelItem(0));
    }

    if (!selected_sorted_layer_name.isEmpty() && ui->layerTreeSorted->topLevelItemCount() > 1) {
        for (int i = 0, n = ui->layerTreeSorted->topLevelItemCount(); i < n; ++i) {
            TreeWidgetItemParameter *layer_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTreeSorted->topLevelItem(i));
            if (layer_item->layer_name != selected_sorted_layer_name) continue;

            ui->layerTreeSorted->setCurrentItem(layer_item);
        }
    } else if (ui->layerTreeSorted->topLevelItemCount() > 1) {
        ui->layerTreeSorted->setCurrentItem(ui->layerTreeSorted->topLevelItem(0));
    }

    ui->layerTreeSorted->setEnabled(ui->layerTreeSorted->topLevelItemCount() > 1);

    QTreeWidgetItem *current_sorted_layers = ui->layerTreeSorted->currentItem();
    const bool has_selected_sorted_item = current_sorted_layers != nullptr;

    const bool has_above_sorted_item = ui->layerTreeSorted->itemAbove(current_sorted_layers) != nullptr;
    ui->pushButtonUp->setEnabled(has_selected_sorted_item && has_above_sorted_item);

    const bool has_below_sorted_item = ui->layerTreeSorted->itemBelow(current_sorted_layers) != nullptr;
    ui->pushButtonDown->setEnabled(has_selected_sorted_item && has_below_sorted_item);
}

void LayersDialog::OnLayerTreeSortedClicked(QTreeWidgetItem *item, int column) {
    (void)column;

    TreeWidgetItemParameter *selected_sorted_item = dynamic_cast<TreeWidgetItemParameter *>(item);
    assert(selected_sorted_item);

    selected_available_layer_name = selected_sorted_layer_name = selected_sorted_item->layer_name;

    UpdateUI();
}

void LayersDialog::AddLayerItem(const Parameter &parameter) {
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

    TreeWidgetItemParameter *item = new TreeWidgetItemParameter(parameter.name);

    item->setText(0, decorated_name);
    item->setFlags(item->flags() | Qt::ItemIsSelectable);
    item->setDisabled(layer == nullptr);

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
    widget->setCurrentIndex(parameter.state);

    connect(widget, SIGNAL(selectionMade(QTreeWidgetItem *, int)), this, SLOT(layerUseChanged(QTreeWidgetItem *, int)));
}

void LayersDialog::LoadAvailableLayersUI() {
    ui->layerTree->clear();

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        AddLayerItem(parameters[i]);
    }

    resizeEvent(nullptr);

    ui->layerTree->update();
}

void LayersDialog::LoadSortedLayersUI() {
    ui->layerTreeSorted->clear();

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        if (parameters[i].state != LAYER_STATE_OVERRIDDEN) continue;

        TreeWidgetItemParameter *sorted_layer_item = new TreeWidgetItemParameter(parameters[i].name);
        sorted_layer_item->setText(0, parameters[i].name);
        sorted_layer_item->setFlags(sorted_layer_item->flags() | Qt::ItemIsSelectable);

        ui->layerTreeSorted->addTopLevelItem(sorted_layer_item);
    }
}

QString LayersDialog::GetConfigurationName() const {
    assert(!ui->lineEditName->text().isEmpty());
    return ui->lineEditName->text();
}

// The only way to catch the resize from the layouts
// (which is screwing up the spacing with the combo boxes)
void LayersDialog::showEvent(QShowEvent *event) {
    (void)event;

    resizeEvent(nullptr);
}

void LayersDialog::resizeEvent(QResizeEvent *event) {
    (void)event;

    const QFontMetrics fm = ui->layerTree->fontMetrics();
    const int combo_width = (fm.size(Qt::TextSingleLine, "Application-Controlled").width() * 1.6);
    const int width = ui->layerTree->width() - combo_width;
    ui->layerTree->setColumnWidth(0, width);
}

void LayersDialog::on_pushButtonResetLayers_clicked() {
    configuration._preset = ValidationPresetNone;
    selected_available_layer_name.clear();
    selected_sorted_layer_name.clear();

    for (auto it = parameters.begin(); it != parameters.end(); ++it) {
        it->state = LAYER_STATE_APPLICATION_CONTROLLED;
        it->overridden_rank = Parameter::UNRANKED;

        const LayerSettingsDefaults *defaults = Configurator::Get().FindLayerSettings(it->name);
        if (defaults) {
            it->settings = defaults->settings;

            if (it->name == "VK_LAYER_KHRONOS_validation") configuration._preset = ValidationPresetStandard;
        }
    }

    OrderParameter(parameters, Configurator::Get().available_layers);
    LoadAvailableLayersUI();
    UpdateUI();
}

void LayersDialog::on_pushButtonCustomLayers_clicked() {
    CustomPathsDialog dlg(this);
    dlg.exec();

    BuildParameters();
    LoadAvailableLayersUI();
    LoadSortedLayersUI();
    UpdateUI();
}

void LayersDialog::OverrideOrder(const QString layer_name, const TreeWidgetItemParameter *below,
                                 const TreeWidgetItemParameter *above) {
    auto below_parameter = FindParameter(parameters, below->layer_name);
    assert(below_parameter != parameters.end());
    auto above_parameter = FindParameter(parameters, above->layer_name);
    assert(above_parameter != parameters.end());

    std::swap(below_parameter->overridden_rank, above_parameter->overridden_rank);
    std::swap(*below_parameter, *above_parameter);

    OrderParameter(parameters, Configurator::Get().available_layers);
    LoadAvailableLayersUI();
    LoadSortedLayersUI();

    selected_sorted_layer_name = selected_available_layer_name = layer_name;
    UpdateUI();
}

void LayersDialog::on_pushButtonUp_clicked() {
    TreeWidgetItemParameter *selected_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTreeSorted->currentItem());
    assert(selected_item);
    TreeWidgetItemParameter *above_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTreeSorted->itemAbove(selected_item));
    assert(above_item);

    OverrideOrder(selected_item->layer_name, selected_item, above_item);
}

void LayersDialog::on_pushButtonDown_clicked() {
    TreeWidgetItemParameter *selected_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTreeSorted->currentItem());
    assert(selected_item);
    TreeWidgetItemParameter *below_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTreeSorted->itemBelow(selected_item));
    assert(below_item);

    OverrideOrder(selected_item->layer_name, below_item, selected_item);
}

void LayersDialog::currentLayerChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    (void)previous;

    // New settings
    TreeWidgetItemParameter *layer_item = dynamic_cast<TreeWidgetItemParameter *>(current);
    if (layer_item == nullptr) {
        ui->labelLayerDetails->setText("");
        return;
    }

    // Populate the side label
    assert(!layer_item->layer_name.isEmpty());

    selected_available_layer_name = layer_item->layer_name;

    const Layer *layer = Configurator::Get().FindLayerNamed(layer_item->layer_name);
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

void LayersDialog::OverrideAllExplicitLayers() {
    Configurator &configurator = Configurator::Get();

    for (auto it = parameters.begin(); it != parameters.end(); ++it) {
        if (it->state != LAYER_STATE_APPLICATION_CONTROLLED) continue;

        Layer *layer = configurator.FindLayerNamed(it->name);
        if (layer == nullptr) continue;

        if (layer->_layer_type == LAYER_TYPE_IMPLICIT) continue;

        if (it->name == "VK_LAYER_KHRONOS_validation")
            it->state = LAYER_STATE_OVERRIDDEN;
        else
            it->state = LAYER_STATE_EXCLUDED;
    }
}

// Select the layer LayerState
void LayersDialog::layerUseChanged(QTreeWidgetItem *item, int selection) {
    // Combo box changed. We first need to know which tree item was selected.
    // Fortunatly, changing the combo box also changes the selected item.
    assert(item);
    assert(selection >= LAYER_STATE_FIRST && selection <= LAYER_STATE_LAST);

    TreeWidgetItemParameter *tree_layer_item = dynamic_cast<TreeWidgetItemParameter *>(item);
    assert(tree_layer_item != nullptr);
    auto current_parameter = FindParameter(parameters, tree_layer_item->layer_name);
    assert(current_parameter != parameters.end());

    LayerState layer_state = static_cast<LayerState>(selection);

    if (layer_state == LAYER_STATE_OVERRIDDEN && current_parameter->name == "VK_LAYER_LUNARG_device_simulation") {
        QMessageBox alert;
        alert.setWindowTitle("Overridding or excluding ALL explicit layers is recommanded");
        alert.setText(
            "VK_LAYER_LUNARG_device_simulation requires being executed close to the Vulkan drivers. However, "
            "application-controlled layers are executed after Vulkan Configurator overridden layers.");
        alert.setInformativeText("Do you want to override ALL explicit layers too?");
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setIcon(QMessageBox::Warning);
        if (alert.exec() == QMessageBox::Yes) {
            OverrideAllExplicitLayers();
        }
    } else if (layer_state == LAYER_STATE_EXCLUDED) {
        const Layer *layer = Configurator::Get().FindLayerNamed(tree_layer_item->layer_name);

        if (layer) {
            if (layer->_layer_type == LAYER_TYPE_IMPLICIT) {
                QMessageBox alert;
                alert.setWindowTitle("Implicit layer excluded...");
                alert.setText(
                    format("%s was excluded but it is an implicit layer. This may cause undefined behavior, including crashes. ",
                           tree_layer_item->layer_name.toUtf8().constData())
                        .c_str());
                alert.setInformativeText("Do you want to continue?");
                alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                alert.setIcon(QMessageBox::Warning);
                if (alert.exec() == QMessageBox::No) {
                    current_parameter->state = LAYER_STATE_APPLICATION_CONTROLLED;
                }
            }
        }
    }

    current_parameter->state = layer_state;
    current_parameter->overridden_rank = Parameter::UNRANKED;

    OrderParameter(parameters, Configurator::Get().available_layers);

    LoadAvailableLayersUI();
    LoadSortedLayersUI();
    UpdateUI();
}

/// This is actually the save button.
/// We are either saving an exisitng profile, or creating a new one.
void LayersDialog::accept() {
    // Hard Fail: Name must not be blank
    if (ui->lineEditName->text().isEmpty()) {
        Alert::ConfigurationNameEmpty();
        return;
    }

    Configurator &configurator = Configurator::Get();
    if (configuration.name != ui->lineEditName->text() && configurator.FindConfiguration(ui->lineEditName->text())) {
        Alert::ConfigurationRenamingFailed();
        return;
    }

    FilterParameters(parameters, LAYER_STATE_APPLICATION_CONTROLLED);

    configuration.name = ui->lineEditName->text();
    configuration._description = ui->lineEditDescription->text();

    const QString save_path = Configurator::Get().path.GetFullPath(PATH_CONFIGURATION, ui->lineEditName->text());
    const bool result = configuration.Save(save_path);
    assert(result);

    QDialog::accept();
}

void LayersDialog::BuildParameters() {
    Configurator &configurator = Configurator::Get();
    std::vector<Layer> &available_layers = configurator.available_layers;

    parameters.clear();

    for (std::size_t i = 0, n = available_layers.size(); i < n; ++i) {
        const Layer &layer = available_layers[i];

        if (layer._layer_type != LAYER_TYPE_IMPLICIT) continue;

        // The layer is overridden
        if (FindParameter(configuration.parameters, layer._name) != configuration.parameters.end()) continue;

        Parameter parameter;
        parameter.name = layer._name;
        parameter.state = LAYER_STATE_APPLICATION_CONTROLLED;

        parameters.push_back(parameter);
    }

    // Loop through the layers. They are expected to be in order
    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        const Parameter &parameter = configuration.parameters[i];
        assert(!parameter.name.isEmpty());

        parameters.push_back(parameter);
    }

    for (std::size_t i = 0, n = available_layers.size(); i < n; ++i) {
        const Layer &layer = available_layers[i];

        if (layer._layer_type == LAYER_TYPE_IMPLICIT) continue;

        // The layer is already in the layer tree
        if (FindParameter(configuration.parameters, layer._name) != configuration.parameters.end()) continue;

        Parameter parameter;
        parameter.name = layer._name;
        parameter.state = LAYER_STATE_APPLICATION_CONTROLLED;

        const LayerSettingsDefaults *defaults = configurator.FindLayerSettings(layer._name);
        if (defaults) {
            parameter.settings = defaults->settings;

            if (layer._name == "VK_LAYER_KHRONOS_validation") configuration._preset = ValidationPresetStandard;
        }

        parameters.push_back(parameter);
    }

    OrderParameter(parameters, available_layers);
}
