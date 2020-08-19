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

////////////////////////////////////////////////////////////////////////////
// Load the tree widget with the current list
void dlgCustomPaths::RepopulateTree() {
    Configurator &configurator = Configurator::Get();
    configurator.BuildCustomLayerTree(ui->treeWidget);
}

void dlgCustomPaths::on_pushButtonAdd_clicked() {
    QString custom_folder =
        QFileDialog::getExistingDirectory(this, tr("Add Custom Layer Folder"), "", QFileDialog::DontUseNativeDialog);

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
    if (selected == nullptr) {
        ui->pushButtonRemove->setEnabled(false);
        return;
    }

    while (selected->parent() != nullptr) selected = selected->parent();

    Configurator &configurator = Configurator::Get();

    // Now actually remove it.
    configurator.RemoveCustomLayersPath(selected->text(0));

    // Update GUI and save
    RepopulateTree();

    // Nothing is selected, so disable remove button
    ui->buttonBox->setEnabled(false);
    _paths_changed = true;
}
