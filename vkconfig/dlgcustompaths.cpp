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

#include "dlgcustompaths.h"
#include "ui_dlgcustompaths.h"

#include <QFileDialog>
#include <QMessageBox>

dlgCustomPaths::dlgCustomPaths(QWidget *parent) : QDialog(parent), ui(new Ui::dlgCustomPaths) {
    _paths_changed = false;
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->treeWidget->headerItem()->setText(0, tr("Custom Search Paths & Layers"));

    RepopulateTree();
    ui->buttonBox->setEnabled(Configurator::Get().HasLayers());
}

dlgCustomPaths::~dlgCustomPaths() { delete ui; }

void dlgCustomPaths::RepopulateTree() {
    ui->treeWidget->clear();

    Configurator &configurator = Configurator::Get();

    // Populate the tree
    for (int custom_path_index = 0, n = configurator.GetCustomLayersPathSize(); custom_path_index < n; ++custom_path_index) {
        const QString &custom_path = configurator.GetCustomLayersPath(custom_path_index);

        QTreeWidgetItem *pItem = new QTreeWidgetItem();
        pItem->setText(0, custom_path);
        ui->treeWidget->addTopLevelItem(pItem);

        // Look for layers that are in this folder. If any are found, add them to the tree
        QVector<LayerFile *> custom_layers;
        configurator.LoadLayersFromPath(custom_path, custom_layers);

        for (int j = 0; j < custom_layers.size(); j++) {
            QTreeWidgetItem *pChild = new QTreeWidgetItem();
            pChild->setText(0, custom_layers[j]->_name);
            pItem->addChild(pChild);
        }

        // Free the dynamic memory, the rest passes out of scope
        qDeleteAll(custom_layers.begin(), custom_layers.end());
    }
}

void dlgCustomPaths::on_pushButtonAdd_clicked() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    QString custom_folder = dialog.getExistingDirectory(this, tr("Add Custom Layer Folder"), "");

    Configurator &configurator = Configurator::Get();

    if (!custom_folder.isEmpty()) {
        configurator.AppendCustomLayersPath(custom_folder);
        QTreeWidgetItem *pItem = new QTreeWidgetItem();
        pItem->setText(0, custom_folder);
        ui->treeWidget->addTopLevelItem(pItem);

        _paths_changed = true;
        RepopulateTree();
    }

    ui->buttonBox->setEnabled(configurator.HasLayers());
}

//////////////////////////////////////////////////////////////////////////////
/// Don't make remove button accessable unless an item has been selected
void dlgCustomPaths::on_treeWidget_itemSelectionChanged() { ui->pushButtonRemove->setEnabled(true); }

//////////////////////////////////////////////////////////////////////////////
/// Remove the selected custom search path
void dlgCustomPaths::on_pushButtonRemove_clicked() {
    // Which one is selected? We need the top item too
    QTreeWidgetItem *selected = ui->treeWidget->currentItem();
    while (selected->parent() != nullptr) selected = selected->parent();

    Configurator &configurator = Configurator::Get();

    // Now actually remove it.
    configurator.RemoveCustomLayersPath(selected->text(0));

    // Update GUI and save
    RepopulateTree();
    ui->buttonBox->setEnabled(configurator.HasLayers());
    _paths_changed = true;
}
