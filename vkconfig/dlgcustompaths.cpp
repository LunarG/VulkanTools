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

#include <QFileDialog>
#include <QMessageBox>

#include "dlgcustompaths.h"
#include "ui_dlgcustompaths.h"

#include "configurator.h"

dlgCustomPaths::dlgCustomPaths(QWidget *parent) : QDialog(parent), ui(new Ui::dlgCustomPaths) {
    bPathsChanged = false;
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->treeWidget->headerItem()->setText(0, tr("Custom Search Paths & Layers"));

    RepopulateTree();
    ui->buttonBox->setEnabled(Configurator::Get().HasLayers());
}

dlgCustomPaths::~dlgCustomPaths() { delete ui; }

void dlgCustomPaths::RepopulateTree(void) {
    ui->treeWidget->clear();

    Configurator &configurator = Configurator::Get();

    // Populate the tree
    for (int i = 0; i < configurator.additionalSearchPaths.size(); i++) {
        QTreeWidgetItem *pItem = new QTreeWidgetItem();
        pItem->setText(0, configurator.additionalSearchPaths[i]);
        ui->treeWidget->addTopLevelItem(pItem);

        // Look for layers that are in this folder. If any are found, add them to the tree
        QVector<LayerFile *> customLayers;
        configurator.LoadLayersFromPath(configurator.additionalSearchPaths[i], customLayers, LAYER_TYPE_CUSTOM);

        for (int j = 0; j < customLayers.size(); j++) {
            QTreeWidgetItem *pChild = new QTreeWidgetItem();
            pChild->setText(0, customLayers[j]->name);
            pItem->addChild(pChild);
        }

        // Free the dynamic memory, the rest passes out of scope
        qDeleteAll(customLayers.begin(), customLayers.end());
    }
}

void dlgCustomPaths::on_pushButtonAdd_clicked() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    QString customFolder = dialog.getExistingDirectory(this, tr("Add Custom Layer Folder"), "");

    Configurator &configurator = Configurator::Get();

    if (!customFolder.isEmpty()) {
        configurator.additionalSearchPaths.append(customFolder);
        QTreeWidgetItem *pItem = new QTreeWidgetItem();
        pItem->setText(0, customFolder);
        ui->treeWidget->addTopLevelItem(pItem);

        configurator.SaveAdditionalSearchPaths();
        configurator.FindAllInstalledLayers();
        configurator.LoadAllProfiles();
        bPathsChanged = true;
        RepopulateTree();
    }

    ui->buttonBox->setEnabled(configurator.HasLayers());
}

//////////////////////////////////////////////////////////////////////////////
/// \brief dlgCustomPaths::on_treeWidget_itemSelectionChanged
/// Don't make remove button accessable unless an item has been selected
void dlgCustomPaths::on_treeWidget_itemSelectionChanged() { ui->pushButtonRemove->setEnabled(true); }

//////////////////////////////////////////////////////////////////////////////
/// \brief dlgCustomPaths::on_pushButtonRemove_clicked
/// Remove the selected custom search path
void dlgCustomPaths::on_pushButtonRemove_clicked() {
    // Which one is selected? We need the top item too
    QTreeWidgetItem *pSelected = ui->treeWidget->currentItem();
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
    for (int i = 0; i < configurator.additionalSearchPaths.size(); i++) {
        if (configurator.additionalSearchPaths[i] == pSelected->text(0)) {
            configurator.additionalSearchPaths.removeAt(i);
            break;
        }
    }

    // Update GUI and save
    RepopulateTree();
    configurator.SaveAdditionalSearchPaths();
    configurator.FindAllInstalledLayers();
    configurator.LoadAllProfiles();
    ui->buttonBox->setEnabled(configurator.HasLayers());
    bPathsChanged = true;
}
