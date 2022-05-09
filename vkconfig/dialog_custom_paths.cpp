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

#include "dialog_custom_paths.h"

#include "configurator.h"

#include "../vkconfig_core/alert.h"

#include <QFileDialog>

UserDefinedPathsDialog::UserDefinedPathsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::dialog_custom_paths) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    Configurator &configurator = Configurator::Get();
    this->layers_paths_saved = this->layers_paths =
        configurator.environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_GUI);
    this->RepopulateTree();

    ui->buttonBox->setEnabled(!configurator.layers.Empty());
}

void UserDefinedPathsDialog::Reload() {
    Configurator &configurator = Configurator::Get();
    configurator.configurations.SaveAllConfigurations(configurator.layers.available_layers);

    configurator.layers.LoadAllInstalledLayers();
    configurator.configurations.LoadAllConfigurations(configurator.layers.available_layers);
    configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);
    configurator.request_vulkan_status = true;
}

void UserDefinedPathsDialog::SaveLayersPaths(const std::vector<std::string> &layers_paths) {
    Configurator &configurator = Configurator::Get();
    configurator.environment.ClearCustomLayerPath();

    for (std::size_t i = 0, n = layers_paths.size(); i < n; ++i) {
        configurator.environment.AppendCustomLayerPath(layers_paths[i]);
    }
}

void UserDefinedPathsDialog::accept() {
    Configurator &configurator = Configurator::Get();
    Configuration *active_configuration = configurator.configurations.GetActiveConfiguration();

    if (active_configuration != nullptr) {
        Version loader_version;
        if (!configurator.SupportDifferentLayerVersions(&loader_version)) {
            std::string log_versions;
            if (!configurator.configurations.CheckApiVersions(configurator.layers.available_layers, active_configuration,
                                                              log_versions)) {
                if (Alert::LayerNewerVersions(log_versions.c_str()) == QMessageBox::No) {
                    return;
                }
            }

            log_versions.clear();
            if (!configurator.configurations.CheckLayersVersions(configurator.layers.available_layers, active_configuration,
                                                                 log_versions)) {
                if (Alert::LayerIncompatibleVersions(log_versions.c_str(), loader_version) == QMessageBox::No) {
                    return;
                }
            }
        }
    }

    QDialog::accept();
}

void UserDefinedPathsDialog::reject() {
    this->SaveLayersPaths(this->layers_paths_saved);
    this->Reload();

    QDialog::reject();
}

// Load the tree widget with the current list
void UserDefinedPathsDialog::RepopulateTree() {
    Configurator &configurator = Configurator::Get();

    // Populate the tree
    ui->treeWidget->clear();

    // Building the list is not obvious. Each custom path may have multiple layers and there
    // could be duplicates, which are not allowed. The layer paths are traversed in order, and
    // layers are used on a first occurance basis. So we can't just show the layers that are
    // present in the folder (because they may not be used). We have to list the custom layer paths
    // and then look for layers that are already loaded that are from that path.

    for (std::size_t custom_path_index = 0, count = this->layers_paths.size(); custom_path_index < count; ++custom_path_index) {
        const std::string user_defined_path(ConvertNativeSeparators(this->layers_paths[custom_path_index]));

        QTreeWidgetItem *item = new QTreeWidgetItem();
        ui->treeWidget->addTopLevelItem(item);
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

            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, layer.key.c_str());
            item->addChild(child);
        }
    }
}

void UserDefinedPathsDialog::on_pushButtonAdd_clicked() {
    Configurator &configurator = Configurator::Get();
    const std::string custom_path = configurator.path.SelectPath(this, PATH_USER_DEFINED_LAYERS_GUI);

    if (!custom_path.empty()) {
        if (std::find(layers_paths.begin(), layers_paths.end(), custom_path) == layers_paths.end()) {
            this->layers_paths.push_back(custom_path);

            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, custom_path.c_str());
            ui->treeWidget->addTopLevelItem(item);

            this->SaveLayersPaths(this->layers_paths);
            this->Reload();
            this->RepopulateTree();
        }
    }

    ui->buttonBox->setEnabled(!configurator.layers.Empty());
}

/// Don't make remove button accessable unless an item has been selected
void UserDefinedPathsDialog::on_treeWidget_itemSelectionChanged() { ui->pushButtonRemove->setEnabled(true); }

/// Remove the selected user-defined search path
void UserDefinedPathsDialog::on_pushButtonRemove_clicked() {
    // Which one is selected? We need the top item too
    QTreeWidgetItem *selected = ui->treeWidget->currentItem();
    if (selected == nullptr) {
        ui->pushButtonRemove->setEnabled(false);
        return;
    }

    while (selected->parent() != nullptr) selected = selected->parent();

    RemoveString(this->layers_paths, selected->text(0).toStdString());

    this->SaveLayersPaths(this->layers_paths);
    this->Reload();
    this->RepopulateTree();

    // Nothing is selected, so disable remove button
    ui->buttonBox->setEnabled(!Configurator::Get().layers.Empty());
}
