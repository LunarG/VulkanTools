/*
 * Copyright (c) 2018 Valve Corporation
 * Copyright (c) 2018 LunarG, Inc.
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
 * Author: Lenny Komow <lenny@lunarg.com>
 */

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QTreeWidget>

#include "layer_locations_widget.h"

LayerLocationsWidget::LayerLocationsWidget(QWidget *parent)
    : QGroupBox(tr("Layer Locations"), parent)
{
    default_layer_locations = {
#if defined(_WIN32)
        QPair<QString, LayerType>("HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ExplicitLayers", LayerType::Explicit),
        QPair<QString, LayerType>("HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ImplicitLayers", LayerType::Implicit),
        QPair<QString, LayerType>("HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ExplicitLayers", LayerType::Explicit),
        QPair<QString, LayerType>("HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ImplicitLayers", LayerType::Implicit),
        QPair<QString, LayerType>("HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanExplicitLayers", LayerType::Explicit),
        QPair<QString, LayerType>("HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanImplicitLayers", LayerType::Implicit),
#else
        QPair<QString, LayerType>("/usr/local/etc/vulkan/explicit_layer.d", LayerType::Explicit),
        QPair<QString, LayerType>("/usr/local/etc/vulkan/implicit_layer.d", LayerType::Implicit),
        QPair<QString, LayerType>("/usr/local/share/vulkan/explicit_layer.d", LayerType::Explicit),
        QPair<QString, LayerType>("/usr/local/share/vulkan/implicit_layer.d", LayerType::Implicit),
        QPair<QString, LayerType>("/etc/vulkan/explicit_layer.d", LayerType::Explicit),
        QPair<QString, LayerType>("/etc/vulkan/implicit_layer.d", LayerType::Implicit),
        QPair<QString, LayerType>("/usr/share/vulkan/explicit_layer.d", LayerType::Explicit),
        QPair<QString, LayerType>("/usr/share/vulkan/implicit_layer.d", LayerType::Implicit),
        QPair<QString, LayerType>(QDir::homePath() + "/.local/share/vulkan/explicit_layer.d", LayerType::Explicit),
        QPair<QString, LayerType>(QDir::homePath() + "/.local/share/vulkan/implicit_layer.d", LayerType::Implicit),
#endif
    };

    layer_icons[LayerType::Explicit] = QIcon(":/layermgr/icons/explicit.png");
    layer_icons[LayerType::Implicit] = QIcon(":/layermgr/icons/implicit.png");

    QVBoxLayout *location_layout = new QVBoxLayout();
    custom_path_box = new QCheckBox(tr("Use custom layer paths"));
    connect(custom_path_box, &QCheckBox::stateChanged, this, &LayerLocationsWidget::selectCustomLayerPaths);
    location_layout->addWidget(custom_path_box);
    QHBoxLayout *layer_list_layout = new QHBoxLayout();
    path_list = new QListWidget();
    layer_list_layout->addWidget(path_list);
    QVBoxLayout *layer_button_layout = new QVBoxLayout();
    search_path_button = new QPushButton(tr("Search..."));
    search_path_button->setToolTip(tr("Search path"));
    search_path_button->setEnabled(false);
    connect(search_path_button, &QPushButton::clicked, this, &LayerLocationsWidget::searchPath);
    layer_button_layout->addWidget(search_path_button);
    add_path_button = new QPushButton(tr("Add..."));
    add_path_button->setToolTip(tr("Add new"));
    add_path_button->setEnabled(false);
    connect(add_path_button, &QPushButton::clicked, this, &LayerLocationsWidget::addLayerPath);
    layer_button_layout->addWidget(add_path_button);
    remove_path_button = new QPushButton(tr("Remove"));
    remove_path_button->setToolTip(tr("Remove selected"));
    remove_path_button->setEnabled(false);
    connect(remove_path_button, &QPushButton::clicked, this, &LayerLocationsWidget::removeSelectedLayerPath);
    layer_button_layout->addWidget(remove_path_button);
    clear_paths_button = new QPushButton("Clear");
    clear_paths_button->setToolTip(tr("Clear all"));
    clear_paths_button->setEnabled(false);
    connect(clear_paths_button, &QPushButton::clicked, this, &LayerLocationsWidget::clearLayerPaths);
    layer_button_layout->addWidget(clear_paths_button);
    layer_button_layout->addStretch();
    layer_list_layout->addLayout(layer_button_layout);
    location_layout->addLayout(layer_list_layout);
    setLayout(location_layout);

    loadLayerPaths();
}

void LayerLocationsWidget::addLayerPath()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Layer Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly);
    if (path == "") {
        return;
    }

#if defined(LAYERMGR_CUSTOM_IMPLICIT)
    QMessageBox popup(this);
    popup.setText(tr("Is the directory \"%1\" an explicit or implicit layer path?").arg(path));
    popup.setStandardButtons(0);
    QPushButton *explicit_button = popup.addButton(tr("Explicit"), QMessageBox::YesRole);
    QPushButton *implicit_button = popup.addButton(tr("Implicit"), QMessageBox::NoRole);
    QPushButton *cancel_button = popup.addButton(QMessageBox::Cancel);
    popup.exec();

    if (popup.clickedButton() == cancel_button) {
        return;
    }
    LayerType type = popup.clickedButton() == explicit_button ? LayerType::Explicit : LayerType::Implicit;
#else
    LayerType type = LayerType::Explicit;
#endif

    // Check if the layer is already present in the list
    QDir new_path(path);
    for (const auto& location : custom_layer_locations) {
        if (QDir(location.first) == new_path) {
            return;
        }
    }

    custom_layer_locations.append(QPair<QString, LayerType>(path, type));
    QListWidgetItem *item = new QListWidgetItem();
    item->setText(path);
    item->setIcon(layer_icons[type]);
    path_list->addItem(item);

    emit pathsChanged(currentLayerPaths(), custom_path_box->isChecked());
}

void LayerLocationsWidget::clearLayerPaths()
{
    custom_layer_locations.clear();
    while (path_list->count() > 0) {
        delete path_list->takeItem(0);
    }

    emit pathsChanged(currentLayerPaths(), custom_path_box->isChecked());
}

void LayerLocationsWidget::removeSelectedLayerPath()
{
    for(auto item : path_list->selectedItems()) {
        for (int i = 0; i < custom_layer_locations.count(); ++i) {
            if (custom_layer_locations[i].first == item->text()) {
                custom_layer_locations.removeAt(i);
            }
        }
        delete path_list->takeItem(path_list->row(item));
    }

    emit pathsChanged(currentLayerPaths(), custom_path_box->isChecked());
}

void LayerLocationsWidget::selectCustomLayerPaths(int state)
{
    bool is_custom = (state == Qt::Checked);

    search_path_button->setEnabled(is_custom);
    add_path_button->setEnabled(is_custom);
    remove_path_button->setEnabled(is_custom);
    clear_paths_button->setEnabled(is_custom);

    loadLayerPaths();
}

void LayerLocationsWidget::searchPath()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Search Path"), QDir::homePath(), QFileDialog::ShowDirsOnly);
    if (path == "") {
        return;
    }

    QList<LayerManifest> layer_list = LayerManifest::LoadDirectory(QDir(path), LayerType::Explicit, true);
    QHash<QString, QList<LayerManifest>> layers;
    for (LayerManifest &manifest : layer_list) {
        QFileInfo file(manifest.file_path);
        QString dir = file.dir().absolutePath();
        if (!layers.contains(dir)) {
            layers[dir] = QList<LayerManifest>();
        }
        layers[dir].append(manifest);
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("Found Layer Paths"));
    QVBoxLayout *dialog_layout = new QVBoxLayout();

    QLabel *label = new QLabel(tr("Layers were found in the following locations, and contain the layers shown below. Uncheck any "
        "locations that you do not wish to add."));
    dialog_layout->addWidget(label);

    QTreeWidget *tree = new QTreeWidget();
    tree->header()->close();
    QList<QString> keys = layers.keys();
    qSort(keys);
    for (QString &path : keys) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, path);
        item->setCheckState(0, Qt::Checked);
        QFont font = item->font(0);
        font.setBold(true);
        item->setFont(0, font);

        for (LayerManifest &manifest : layers[path]) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, manifest.PrettyName());
            item->addChild(child);
        }
        tree->addTopLevelItem(item);
    }
    if (layer_list.count() < 16) {
        tree->expandAll();
    }
    dialog_layout->addWidget(tree);

    QDialogButtonBox *button_box = new QDialogButtonBox();
    button_box->addButton(QDialogButtonBox::Ok);
    button_box->addButton(QDialogButtonBox::Cancel);
    connect(button_box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog_layout->addWidget(button_box);

    dialog.setLayout(dialog_layout);
    dialog.exec();

    if (dialog.result() == QDialog::Accepted) {
        for(int i = 0; i < tree->topLevelItemCount(); ++i) {
            QTreeWidgetItem *item = tree->topLevelItem(i);
            if (item->checkState(0) == Qt::Checked) {
                custom_layer_locations.append(QPair<QString, LayerType>(item->text(0), LayerType::Explicit));
            }
        }
        loadLayerPaths();
    }
}

void LayerLocationsWidget::loadLayerPaths()
{
    path_list->clear();

    for (QPair<QString, LayerType> &pair : currentLayerPaths()) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(pair.first);
        item->setIcon(layer_icons[pair.second]);
        path_list->addItem(item);
    }

    emit pathsChanged(currentLayerPaths(), custom_path_box->isChecked());
}
