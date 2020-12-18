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

    ui->treeWidget->headerItem()->setText(0, "Custom Search Paths & Layers");

    RepopulateTree();
    ui->buttonBox->setEnabled(Configurator::Get().HasLayers());
    Configurator::Get().request_vulkan_status = true;
}

CustomPathsDialog::~CustomPathsDialog() {}

// Load the tree widget with the current list
void CustomPathsDialog::RepopulateTree() {
    Configurator &configurator = Configurator::Get();
    configurator.BuildCustomLayerTree(ui->treeWidget);
}

void CustomPathsDialog::on_pushButtonAdd_clicked() {
    Configurator &configurator = Configurator::Get();
    const QString custom_path = configurator.path.SelectPath(this, PATH_CUSTOM_LAYER_PATH);

    if (!custom_path.isEmpty()) {
        if (configurator.environment.AppendCustomLayerPath(custom_path)) {
            configurator.layers.LoadAllInstalledLayers();
            configurator.LoadAllConfigurations();
        }

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, custom_path);
        ui->treeWidget->addTopLevelItem(item);

        RepopulateTree();
    }

    ui->buttonBox->setEnabled(configurator.HasLayers());
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
    if (configurator.environment.RemoveCustomLayerPath(selected->text(0))) {
        configurator.layers.LoadAllInstalledLayers();
        configurator.LoadAllConfigurations();
    }

    // Update GUI and save
    RepopulateTree();

    // Nothing is selected, so disable remove button
    ui->buttonBox->setEnabled(false);
}
