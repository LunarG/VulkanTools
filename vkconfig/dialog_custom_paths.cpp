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

#include "dialog_custom_paths.h"

#include "configurator.h"

#include <QFileDialog>
#include <QMessageBox>

CustomPathsDialog::CustomPathsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::dialog_custom_paths) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->treeWidget->headerItem()->setText(0, "User-Defined Layers Paths");

    RepopulateTree();
    ui->buttonBox->setEnabled(!Configurator::Get().layers.Empty());
    Configurator::Get().request_vulkan_status = true;
}

CustomPathsDialog::~CustomPathsDialog() {}

// Load the tree widget with the current list
void CustomPathsDialog::RepopulateTree() {
    Configurator &configurator = Configurator::Get();

    // Populate the tree
    ui->treeWidget->clear();

    // Building the list is not obvious. Each custom path may have multiple layers and there
    // could be duplicates, which are not allowed. The layer paths are traversed in order, and
    // layers are used on a first occurance basis. So we can't just show the layers that are
    // present in the folder (because they may not be used). We have to list the custom layer paths
    // and then look for layers that are already loaded that are from that path.
    const std::vector<std::string> &user_defined_layers_paths =
        configurator.environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_GUI);
    for (std::size_t custom_path_index = 0, n = user_defined_layers_paths.size(); custom_path_index < n; ++custom_path_index) {
        // Custom path is the parent tree item
        const std::string custom_path(ConvertNativeSeparators(user_defined_layers_paths[custom_path_index]));

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, custom_path.c_str());
        ui->treeWidget->addTopLevelItem(item);

        // Look for layers that are loaded that are also from this folder
        for (std::size_t i = 0, n = configurator.layers.available_layers.size(); i < n; i++) {
            const Layer &layer = configurator.layers.available_layers[i];

            const QFileInfo file_info(layer._layer_path.c_str());
            const std::string path(ConvertNativeSeparators(file_info.path().toStdString()));
            if (path != custom_path) continue;

            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, layer.key.c_str());
            item->addChild(child);
        }
        item->setExpanded(true);
    }
}

void CustomPathsDialog::on_pushButtonAdd_clicked() {
    Configurator &configurator = Configurator::Get();
    const std::string custom_path = configurator.path.SelectPath(this, PATH_USER_DEFINED_LAYERS_PATHS_GUI);

    if (!custom_path.empty()) {
        if (configurator.environment.AppendCustomLayerPath(custom_path)) {
            configurator.layers.LoadAllInstalledLayers();
            configurator.configurations.LoadAllConfigurations(configurator.layers.available_layers, configurator.path,
                                                              configurator.environment);
        }

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, custom_path.c_str());
        ui->treeWidget->addTopLevelItem(item);

        RepopulateTree();
    }

    ui->buttonBox->setEnabled(!Configurator::Get().layers.Empty());
}

/// Don't make remove button accessable unless an item has been selected
void CustomPathsDialog::on_treeWidget_itemSelectionChanged() { ui->pushButtonRemove->setEnabled(true); }

/// Remove the selected custom search path
void CustomPathsDialog::on_pushButtonRemove_clicked() {
    // Which one is selected? We need the top item too
    QTreeWidgetItem *selected = ui->treeWidget->currentItem();
    if (selected == nullptr) {
        ui->pushButtonRemove->setEnabled(false);
        return;
    }

    while (selected->parent() != nullptr) selected = selected->parent();

    Configurator &configurator = Configurator::Get();

    // Now actually remove it.
    if (configurator.environment.RemoveCustomLayerPath(selected->text(0).toStdString())) {
        configurator.layers.LoadAllInstalledLayers();
        configurator.configurations.LoadAllConfigurations(configurator.layers.available_layers, configurator.path,
                                                          configurator.environment);
    }

    // Update GUI and save
    RepopulateTree();

    // Nothing is selected, so disable remove button
    ui->buttonBox->setEnabled(false);
}
