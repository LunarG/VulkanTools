/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

#include "configurator.h"

#include "../vkconfig_core/alert.h"
#include "../vkconfig_core/platform.h"
#include "../vkconfig_core/util.h"
#include "../vkconfig_core/doc.h"

#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
#include <windows.h>
#endif

#include <QMessageBox>
#include <QStyle>
#include <QFileDialog>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDesktopServices>

#include <cassert>

#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
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
static bool IsDLL32Bit(const std::string full_path) {
#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
    if (full_path.empty()) return false;

    QFile file(full_path.c_str());
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
    : QDialog(parent), configuration(configuration), ui(new Ui::dialog_layers) {
    assert(&configuration);

    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    Configurator &configurator = Configurator::Get();
    configurator.configurations.SaveAllConfigurations(configurator.layers.available_layers);

    ui->lineEditName->setText(configuration.key.c_str());
    ui->lineEditDescription->setText(configuration.description.c_str());
    ui->buttonBox->setEnabled(!configurator.layers.Empty());
    ui->pushButtonRemove->setEnabled(false);

    Environment &environment = configurator.environment;
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

    this->Reinit();
}

LayersDialog::~LayersDialog() {
    Environment &environment = Configurator::Get().environment;

    environment.Set(LAYOUT_LAYER_SPLITTER, ui->splitter->saveState());
    environment.Set(LAYOUT_LAYER_GEOMETRY, saveGeometry());
}

void LayersDialog::Reinit() {
    this->BuildParameters();

    this->LoadAvailableLayersUI();
    this->LoadSortedLayersUI();
    this->LoadUserDefinedPaths();

    this->UpdateUI();
}

struct ParameterState {
    std::string key;
    LayerState state;
};

static std::vector<ParameterState> StoreParameterStates(const std::vector<Parameter> &parameters) {
    std::vector<ParameterState> ParameterStates;
    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        ParameterState state;
        state.key = parameters[i].key;
        state.state = parameters[i].state;
        ParameterStates.push_back(state);
    }
    return ParameterStates;
}

static void RestoreParameterStates(std::vector<Parameter> &Parameters, const std::vector<ParameterState> &states) {
    for (std::size_t i = 0, n = states.size(); i < n; ++i) {
        Parameter *saved_parameter = FindByKey(Parameters, states[i].key.c_str());
        if (saved_parameter == nullptr) continue;

        saved_parameter->state = states[i].state;
    }
}

void LayersDialog::Reload() {
    std::string configuration_name = this->configuration.key;
    const std::vector<ParameterState> &ParameterStates = StoreParameterStates(this->configuration.parameters);
    std::vector<std::string> user_defined_paths = this->configuration.user_defined_paths;

    Configurator &configurator = Configurator::Get();
    configurator.configurations.available_configurations.clear();
    configurator.environment.SetPerConfigUserDefinedLayersPaths(this->configuration.user_defined_paths);

    configurator.layers.LoadAllInstalledLayers();
    configurator.configurations.LoadAllConfigurations(configurator.layers.available_layers);

    Configuration *saved_configuration =
        FindByKey(configurator.configurations.available_configurations, configuration_name.c_str());
    if (saved_configuration != nullptr) {
        this->configuration = *saved_configuration;
        this->BuildParameters();
    }

    RestoreParameterStates(this->configuration.parameters, ParameterStates);
    this->configuration.user_defined_paths = user_defined_paths;

    configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);
    configurator.request_vulkan_status = true;
}

void LayersDialog::UpdateUI() {
    this->UpdateButtons();

    if (!selected_available_layer_name.empty()) {
        for (int i = 0, n = ui->layerTree->topLevelItemCount(); i < n; ++i) {
            TreeWidgetItemParameter *layer_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTree->topLevelItem(i));
            if (layer_item->layer_name != selected_available_layer_name) continue;

            ui->layerTree->setCurrentItem(layer_item);
        }
    } else if (ui->layerTree->topLevelItemCount() > 0) {
        ui->layerTree->setCurrentItem(ui->layerTree->topLevelItem(0));
    }

    if (!selected_sorted_layer_name.empty() && ui->layerTreeSorted->topLevelItemCount() > 1) {
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

    this->selected_available_layer_name = this->selected_sorted_layer_name = selected_sorted_item->layer_name;

    UpdateUI();
}

void LayersDialog::AddLayerItem(const Parameter &parameter) {
    assert(!parameter.key.empty());

    std::vector<Layer> &available_layers = Configurator::Get().layers.available_layers;

    const Layer *layer = FindByKey(available_layers, parameter.key.c_str());

    std::string decorated_name(parameter.key);

    bool is_implicit_layer = false;
    if (layer != nullptr) {
        if (layer->status != STATUS_STABLE) {
            decorated_name += format(" (%s)", GetToken(layer->status));
        }

        decorated_name += format(" - %s", layer->api_version.str().c_str());

        if (IsDLL32Bit(layer->manifest_path)) {
            decorated_name += " (32-bit)";
        }

        if (layer->type == LAYER_TYPE_IMPLICIT) {
            is_implicit_layer = true;
            decorated_name += format(" - %s layer", GetLayerTypeLabel(layer->type));
        }
    } else {
        // A layers configuration may have excluded layer that are misssing because they are not available on this platform
        // We simply hide these layers to avoid confusing the Vulkan developers
        if (parameter.state == LAYER_STATE_EXCLUDED) return;

        decorated_name += " (Missing)";
    }

    TreeWidgetItemParameter *item = new TreeWidgetItemParameter(parameter.key.c_str());

    item->setText(0, decorated_name.c_str());
    if (layer != nullptr) item->setToolTip(0, layer->manifest_path.c_str());
    item->setFlags(item->flags() | Qt::ItemIsSelectable);
    item->setDisabled(layer == nullptr);

    // Add the top level item
    ui->layerTree->addTopLevelItem(item);

    // Add a combo box. Default has gray background which looks hidious
    WidgetTreeFriendlyComboBox *widget = new WidgetTreeFriendlyComboBox(item);
    ui->layerTree->setItemWidget(item, 1, widget);

    const QFontMetrics fm = ui->layerTree->fontMetrics();
    const QSize combo_size = fm.size(Qt::TextSingleLine, "Application-Controlled") * 1.6;
    item->setSizeHint(1, combo_size);

    widget->addItem(is_implicit_layer ? "Implicitly On" : "Application-Controlled");
    widget->addItem("Overridden / Forced On");
    widget->addItem("Excluded / Forced Off");
    widget->setCurrentIndex(parameter.state);

    connect(widget, SIGNAL(selectionMade(QTreeWidgetItem *, int)), this, SLOT(layerUseChanged(QTreeWidgetItem *, int)));
}

void LayersDialog::LoadAvailableLayersUI() {
    ui->layerTree->clear();

    for (std::size_t i = 0, n = this->configuration.parameters.size(); i < n; ++i) {
        AddLayerItem(this->configuration.parameters[i]);
    }

    resizeEvent(nullptr);

    ui->layerTree->update();
}

void LayersDialog::LoadSortedLayersUI() {
    ui->layerTreeSorted->clear();

    for (std::size_t i = 0, n = this->configuration.parameters.size(); i < n; ++i) {
        if (this->configuration.parameters[i].state != LAYER_STATE_OVERRIDDEN) continue;

        TreeWidgetItemParameter *sorted_layer_item = new TreeWidgetItemParameter(this->configuration.parameters[i].key.c_str());
        sorted_layer_item->setText(0, this->configuration.parameters[i].key.c_str());
        sorted_layer_item->setFlags(sorted_layer_item->flags() | Qt::ItemIsSelectable);

        ui->layerTreeSorted->addTopLevelItem(sorted_layer_item);
    }
}

void LayersDialog::LoadUserDefinedPaths() {
    Configurator &configurator = Configurator::Get();

    // Populate the tree
    ui->layerTreePath->clear();

    // Building the list is not obvious. Each custom path may have multiple layers and there
    // could be duplicates, which are not allowed. The layer paths are traversed in order, and
    // layers are used on a first occurance basis. So we can't just show the layers that are
    // present in the folder (because they may not be used). We have to list the custom layer paths
    // and then look for layers that are already loaded that are from that path.

    for (std::size_t path_index = 0, count = this->configuration.user_defined_paths.size(); path_index < count; ++path_index) {
        const std::string user_defined_path(ConvertNativeSeparators(this->configuration.user_defined_paths[path_index]));

        QTreeWidgetItem *item = new QTreeWidgetItem();
        ui->layerTreePath->addTopLevelItem(item);
        item->setText(0, user_defined_path.c_str());
        item->setExpanded(true);

        // Look for layers that are loaded that are also from this folder
        for (std::size_t i = 0, n = configurator.layers.available_layers.size(); i < n; ++i) {
            const Layer &layer = configurator.layers.available_layers[i];

            const QFileInfo file_info(layer.manifest_path.c_str());
            const std::string path(ConvertNativeSeparators(file_info.path().toStdString()));
            if (path != user_defined_path) {
                continue;
            }

            std::string decorated_name(layer.key);
            if (layer.status != STATUS_STABLE) {
                decorated_name += format(" (%s)", GetToken(layer.status));
            }
            decorated_name += format(" - %s", layer.api_version.str().c_str());

            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, decorated_name.c_str());
            item->addChild(child);
        }
    }
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

void LayersDialog::on_button_properties_clicked() {
    const std::vector<Layer> &available_layers = Configurator::Get().layers.available_layers;
    const Layer *layer = FindByKey(available_layers, this->selected_available_layer_name.c_str());

    Alert::LayerProperties(layer);
}

void LayersDialog::on_button_doc_clicked() {
    const std::vector<Layer> &available_layers = Configurator::Get().layers.available_layers;
    const Layer *layer = FindByKey(available_layers, this->selected_available_layer_name.c_str());
    const std::string path = format("%s/%s.html", GetPath(BUILTIN_PATH_APPDATA).c_str(), layer->key.c_str());
    ExportHtmlDoc(*layer, path);
    QDesktopServices::openUrl(QUrl(("file:///" + path).c_str()));
}

void LayersDialog::on_button_website_clicked() {
    const std::vector<Layer> &available_layers = Configurator::Get().layers.available_layers;
    const Layer *layer = FindByKey(available_layers, this->selected_available_layer_name.c_str());
    QDesktopServices::openUrl(QUrl(layer->url.c_str()));
}

void LayersDialog::on_button_reset_clicked() {
    Configurator &configurator = Configurator::Get();

    QMessageBox alert;
    alert.QDialog::setWindowTitle(format("Resetting *%s* configuration...", this->configuration.key.c_str()).c_str());
    alert.setText(format("Are you sure you want to reset the *%s* configuration?", this->configuration.key.c_str()).c_str());
    if (this->configuration.IsBuiltIn())
        alert.setInformativeText(
            format("The configuration layers and settings will be restored to default built-in *%s* configuration.",
                   configuration.key.c_str())
                .c_str());
    else if (configurator.configurations.HasFile(this->configuration))
        alert.setInformativeText(
            format("The configuration layers and settings will be reloaded using the *%s* saved file from previous %s run.",
                   configuration.key.c_str(), VKCONFIG_NAME)
                .c_str());
    else
        alert.setInformativeText("The configuration layers and settings will be reset to zero.");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::Yes);
    alert.setIcon(QMessageBox::Warning);
    if (alert.exec() == QMessageBox::No) return;

    selected_available_layer_name.clear();
    selected_sorted_layer_name.clear();

    configuration.Reset(configurator.layers.available_layers, configurator.path);

    this->Reinit();

    ui->button_reset->setEnabled(false);
}

void LayersDialog::OverrideOrder(const std::string layer_name, const TreeWidgetItemParameter *below,
                                 const TreeWidgetItemParameter *above) {
    assert(below != nullptr);
    assert(below != above);

    Parameter *below_parameter = FindByKey(configuration.parameters, below->layer_name.c_str());
    assert(below_parameter != nullptr);
    Parameter *above_parameter = FindByKey(configuration.parameters, above->layer_name.c_str());
    assert(above_parameter != nullptr);

    this->selected_sorted_layer_name = this->selected_available_layer_name = layer_name;

    std::swap(below_parameter->overridden_rank, above_parameter->overridden_rank);

    OrderParameter(configuration.parameters, Configurator::Get().layers.available_layers);
    LoadAvailableLayersUI();
    LoadSortedLayersUI();
    LoadUserDefinedPaths();

    UpdateUI();
}

void LayersDialog::on_pushButtonUp_clicked() {
    TreeWidgetItemParameter *selected_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTreeSorted->currentItem());
    assert(selected_item);
    TreeWidgetItemParameter *above_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTreeSorted->itemAbove(selected_item));
    assert(above_item);

    OverrideOrder(selected_item->layer_name, selected_item, above_item);

    ui->button_reset->setEnabled(true);
    this->UpdateButtons();
}

void LayersDialog::on_pushButtonDown_clicked() {
    TreeWidgetItemParameter *selected_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTreeSorted->currentItem());
    assert(selected_item);
    TreeWidgetItemParameter *below_item = dynamic_cast<TreeWidgetItemParameter *>(ui->layerTreeSorted->itemBelow(selected_item));
    assert(below_item);

    OverrideOrder(selected_item->layer_name, below_item, selected_item);

    ui->button_reset->setEnabled(true);
    this->UpdateButtons();
}

void LayersDialog::currentLayerChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    (void)previous;

    TreeWidgetItemParameter *layer_item = dynamic_cast<TreeWidgetItemParameter *>(current);

    if (layer_item == nullptr) {
        this->selected_available_layer_name.clear();
        return;
    }

    assert(!layer_item->layer_name.empty());

    this->selected_available_layer_name = layer_item->layer_name.c_str();

    ui->button_reset->setEnabled(true);
    this->UpdateButtons();
}

void LayersDialog::UpdateButtons() {
    const bool enabled = !selected_available_layer_name.empty();

    bool enabled_url = false;

    if (enabled) {
        std::vector<Layer> &available_layers = Configurator::Get().layers.available_layers;
        const Layer *layer = FindByKey(available_layers, selected_available_layer_name.c_str());
        assert(layer != nullptr);
        if (!layer->url.empty()) enabled_url = true;
    }

    ui->button_properties->setEnabled(enabled);
    ui->button_doc->setEnabled(enabled);
    ui->button_website->setEnabled(enabled && enabled_url);
}

void LayersDialog::OverrideAllExplicitLayers() {
    for (auto it = this->configuration.parameters.begin(); it != this->configuration.parameters.end(); ++it) {
        if (it->state != LAYER_STATE_APPLICATION_CONTROLLED) continue;

        Configurator &configurator = Configurator::Get();
        std::vector<Layer> &available_layers = configurator.layers.available_layers;

        const Layer *layer = FindByKey(available_layers, it->key.c_str());
        if (layer == nullptr) continue;

        if (layer->type == LAYER_TYPE_IMPLICIT) continue;

        if (it->key == "VK_LAYER_KHRONOS_validation")
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
    Parameter *current_parameter = FindByKey(configuration.parameters, tree_layer_item->layer_name.c_str());
    assert(current_parameter != nullptr);

    LayerState layer_state = static_cast<LayerState>(selection);

    if (layer_state == LAYER_STATE_OVERRIDDEN && current_parameter->key == "VK_LAYER_KHRONOS_profiles") {
        if (Alert::LayerProfiles() == QMessageBox::Yes) {
            OverrideAllExplicitLayers();
        }
    } else if (layer_state == LAYER_STATE_EXCLUDED) {
        const std::vector<Layer> &available_layers = Configurator::Get().layers.available_layers;
        const Layer *layer = FindByKey(available_layers, tree_layer_item->layer_name.c_str());

        if (layer != nullptr) {
            if (layer->type == LAYER_TYPE_IMPLICIT) {
                if (Alert::LayerImplicitExcluded(tree_layer_item->layer_name.c_str()) == QMessageBox::No) {
                    current_parameter->state = LAYER_STATE_APPLICATION_CONTROLLED;
                }
            }
        }
    }

    current_parameter->state = layer_state;
    current_parameter->overridden_rank = Parameter::NO_RANK;

    OrderParameter(this->configuration.parameters, Configurator::Get().layers.available_layers);

    ui->button_reset->setEnabled(true);

    LoadAvailableLayersUI();
    LoadSortedLayersUI();
    LoadUserDefinedPaths();

    UpdateUI();
}

void LayersDialog::on_layerTreePath_itemSelectionChanged() { ui->pushButtonRemove->setEnabled(true); }

void LayersDialog::on_pushButtonAdd_clicked() {
    Configurator &configurator = Configurator::Get();
    const std::string custom_path = configurator.path.SelectPath(this, PATH_USER_DEFINED_LAYERS_GUI);

    if (!custom_path.empty()) {
        std::vector<std::string> &user_defined_paths = this->configuration.user_defined_paths;

        if (std::find(user_defined_paths.begin(), user_defined_paths.end(), custom_path) == user_defined_paths.end()) {
            user_defined_paths.push_back(custom_path);

            this->Reload();
            this->Reinit();
        }
    }

    ui->buttonBox->setEnabled(!configurator.layers.Empty());
}

void LayersDialog::on_pushButtonRemove_clicked() {
    // Which one is selected? We need the top item too
    QTreeWidgetItem *selected = ui->layerTreePath->currentItem();
    if (selected == nullptr) {
        ui->pushButtonRemove->setEnabled(false);
        return;
    }

    while (selected->parent() != nullptr) selected = selected->parent();

    RemoveString(this->configuration.user_defined_paths, selected->text(0).toStdString());

    this->Reload();
    this->Reinit();

    // Nothing is selected, so disable remove button
    ui->pushButtonRemove->setEnabled(false);
}

void LayersDialog::accept() {
    if (ui->lineEditName->text().isEmpty()) {
        Alert::ConfigurationNameEmpty();
        return;
    }

    if (!IsPortableFilename(ui->lineEditName->text().toStdString())) {
        Alert::ConfigurationNameInvalid();
        return;
    }

    Configurator &configurator = Configurator::Get();
    if (this->configuration.key != ui->lineEditName->text().toStdString() &&
        IsFound(configurator.configurations.available_configurations, ui->lineEditName->text().toStdString().c_str())) {
        Alert::ConfigurationRenamingFailed();
        return;
    }

    Version loader_version;
    if (!configurator.SupportDifferentLayerVersions(&loader_version)) {
        std::string log_versions;
        if (!configurator.configurations.CheckLayersVersions(configurator.layers.available_layers, &this->configuration,
                                                             log_versions)) {
            Alert::LayerIncompatibleVersions(log_versions.c_str(), loader_version);
            return;
        }
    }

    FilterParameters(this->configuration.parameters, LAYER_STATE_APPLICATION_CONTROLLED);

    Configuration *saved_configuration =
        FindByKey(configurator.configurations.available_configurations, this->configuration.key.c_str());
    assert(saved_configuration != nullptr);

    if (saved_configuration->key != ui->lineEditName->text().toStdString()) {
        configurator.configurations.RemoveConfigurationFile(saved_configuration->key);
    }

    const std::string active_configuration_name = saved_configuration->key = ui->lineEditName->text().toStdString();
    saved_configuration->description = ui->lineEditDescription->text().toStdString();
    saved_configuration->parameters = this->configuration.parameters;
    saved_configuration->user_defined_paths = this->configuration.user_defined_paths;
    saved_configuration->setting_tree_state.clear();

    configurator.configurations.SaveAllConfigurations(configurator.layers.available_layers);
    configurator.configurations.LoadAllConfigurations(configurator.layers.available_layers);

    configurator.configurations.SetActiveConfiguration(configurator.layers.available_layers, active_configuration_name.c_str());
    QDialog::accept();
}

void LayersDialog::reject() {
    Configurator &configurator = Configurator::Get();
    Configuration *saved_configuration =
        FindByKey(configurator.configurations.available_configurations, this->configuration.key.c_str());
    assert(saved_configuration != nullptr);

    if (saved_configuration->user_defined_paths != this->configuration.user_defined_paths) {
        configurator.environment.SetPerConfigUserDefinedLayersPaths(saved_configuration->user_defined_paths);
        // Restore layers
        this->Reload();
    }

    QDialog::reject();
}

void LayersDialog::BuildParameters() {
    Configurator &configurator = Configurator::Get();
    this->configuration.parameters = GatherParameters(this->configuration.parameters, configurator.layers.available_layers);
}
