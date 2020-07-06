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
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#include "configurator.h"

#include "dlgcustompaths.h"
#include "ui_dlgcustompaths.h"

#include <QFileDialog>
#include <QMessageBox>

dlgCustomPaths::dlgCustomPaths(QWidget *parent) : QDialog(parent), ui_(new Ui::dlgCustomPaths) {
    paths_changed_ = false;
    ui_->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui_->treeWidget->headerItem()->setText(0, tr("Custom Search Paths & Layers"));

    RepopulateTree();
    ui_->buttonBox->setEnabled(Configurator::Get().HasLayers());
}

dlgCustomPaths::~dlgCustomPaths() { delete ui_; }

void dlgCustomPaths::RepopulateTree(void) {
    ui_->treeWidget->clear();

    Configurator &configurator = Configurator::Get();

    // Populate the tree
    for (int custom_path_index = 0, n = configurator.GetCustomLayersPathSize(); custom_path_index < n; ++custom_path_index) {
        const QString &custom_path = configurator.GetCustomLayersPath(custom_path_index);

        QTreeWidgetItem *pItem = new QTreeWidgetItem();
        pItem->setText(0, custom_path);
        ui_->treeWidget->addTopLevelItem(pItem);

        // Look for layers that are in this folder. If any are found, add them to the tree
        QVector<LayerFile *> custom_layers;
        configurator.LoadLayersFromPath(custom_path, custom_layers, LAYER_TYPE_CUSTOM);

        for (int j = 0; j < custom_layers.size(); j++) {
            QTreeWidgetItem *pChild = new QTreeWidgetItem();
            pChild->setText(0, custom_layers[j]->name);
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
        ui_->treeWidget->addTopLevelItem(pItem);

        paths_changed_ = true;
        RepopulateTree();
    }

    ui_->buttonBox->setEnabled(configurator.HasLayers());
}

//////////////////////////////////////////////////////////////////////////////
/// \brief dlgCustomPaths::on_treeWidget_itemSelectionChanged
/// Don't make remove button accessable unless an item has been selected
void dlgCustomPaths::on_treeWidget_itemSelectionChanged() { ui_->pushButtonRemove->setEnabled(true); }

//////////////////////////////////////////////////////////////////////////////
/// \brief dlgCustomPaths::on_pushButtonRemove_clicked
/// Remove the selected custom search path
void dlgCustomPaths::on_pushButtonRemove_clicked() {
    // Which one is selected? We need the top item too
    QTreeWidgetItem *pSelected = ui_->treeWidget->currentItem();
    while (pSelected->parent() != nullptr) pSelected = pSelected->parent();

    // Confirm?
    QMessageBox msg;
    msg.setText(pSelected->text(0));
    msg.setInformativeText(tr("Delete this custom path?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    if (msg.exec() == QMessageBox::No) return;

    Configurator &configurator = Configurator::Get();

    // Now actually remove it.
    configurator.RemoveCustomLayersPath(pSelected->text(0));

    // Update GUI and save
    RepopulateTree();
    ui_->buttonBox->setEnabled(configurator.HasLayers());
    paths_changed_ = true;
}
