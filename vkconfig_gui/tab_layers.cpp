/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "widget_tab_layers_path.h"
#include "tab_layers.h"
#include "mainwindow.h"

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/type_layer_path_view.h"

#include <QFileDialog>

TabLayers::TabLayers(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_LAYERS, window, ui) {
    Configurator &configurator = Configurator::Get();

    this->ui->layers_progress->setValue(0);
    this->ui->layers_progress->setVisible(false);
    this->ui->layers_path_lineedit->setVisible(true);
    this->ui->layers_path_lineedit->setText(configurator.layers.last_layers_path.RelativePath().c_str());

    this->ui->layers_paths_view_comboBox->setCurrentIndex(configurator.layers.paths_view);

    this->connect(this->ui->layers_browse_pushButton, SIGNAL(pressed()), this, SLOT(on_layers_browse_pushButton_pressed()));
    this->connect(this->ui->layers_path_lineedit, SIGNAL(returnPressed()), this, SLOT(on_layers_add_pushButton_pressed()));
    this->connect(this->ui->layers_paths_view_comboBox, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_layers_paths_view_comboBox_currentIndexChanged(int)));
}

TabLayers::~TabLayers() {}

void TabLayers::UpdateUI_LayersPaths(UpdateUIMode ui_update_mode) {
    Configurator &configurator = Configurator::Get();

    this->ui->layers_paths_tree->blockSignals(true);
    this->ui->layers_paths_tree->clear();

    LayersPathViewType view = static_cast<LayersPathViewType>(this->ui->layers_paths_view_comboBox->currentIndex());

    for (std::size_t group_index = 0, group_count = configurator.layers.paths.size(); group_index < group_count; ++group_index) {
        if (view == LAYERS_PATH_ONLY_USER_DEFINED) {
            if (group_index != LAYERS_PATHS_GUI) {
                continue;
            }
        } else if (view == LAYERS_PATH_ONLY_ACTIVE_SDK) {
            if (group_index != LAYERS_PATHS_SDK) {
                continue;
            }
        }

        const LayerType layer_type = ::GetLayerType(static_cast<LayersPaths>(group_index));

        std::vector<LayersPathInfo> &paths_group = configurator.layers.paths[group_index];
        for (std::size_t i = 0, n = paths_group.size(); i < n; ++i) {
            QTreeWidgetItem *item_state = new QTreeWidgetItem;
            item_state->setFlags(item_state->flags() | Qt::ItemIsSelectable);
            item_state->setSizeHint(0, QSize(0, ITEM_HEIGHT));
            LayersPathWidget *layer_path_widget = new LayersPathWidget(&paths_group[i], static_cast<LayersPaths>(group_index));
            this->connect(layer_path_widget, SIGNAL(toggled(bool)), this, SLOT(on_check_box_paths_toggled(bool)));

            ui->layers_paths_tree->addTopLevelItem(item_state);
            ui->layers_paths_tree->setItemWidget(item_state, 0, layer_path_widget);

            const std::string &layer_path = paths_group[i].path.AbsolutePath();
            const std::vector<Path> &manifest_paths = CollectFilePaths(layer_path);

            for (std::size_t manifest_index = 0, manifest_count = manifest_paths.size(); manifest_index < manifest_count;
                 ++manifest_index) {
                const Layer *layer = configurator.layers.FindFromManifest(manifest_paths[manifest_index]);
                if (layer == nullptr) {
                    continue;  // When the directory has JSON files that are not layer manifest
                }

                std::string label = layer->key + " - " + layer->api_version.str();
                if (layer->status != STATUS_STABLE) {
                    label += format(" (%s)", GetToken(layer->status));
                }

                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, label.c_str());
                item->setToolTip(0, layer->manifest_path.AbsolutePath().c_str());
                item->setDisabled(!layer->enabled);
                item_state->addChild(item);
            }
        }
    }

    this->ui->layers_paths_tree->expandAll();
    this->ui->layers_paths_tree->blockSignals(false);
}

void TabLayers::UpdateUI(UpdateUIMode ui_update_mode) {
    this->ui->layers_progress->resetFormat();
    this->ui->layers_progress->setValue(0);
    this->UpdateUI_LayersPaths(ui_update_mode);
}

void TabLayers::CleanUI() {}

bool TabLayers::EventFilter(QObject *target, QEvent *event) { return false; }

void TabLayers::on_check_box_paths_toggled(bool checked) {
    (void)checked;

    this->UpdateUI_LayersPaths(UPDATE_REBUILD_UI);
}

void TabLayers::on_layers_browse_pushButton_pressed() {
    Configurator &configurator = Configurator::Get();

    this->ui->layers_paths_view_comboBox->setVisible(false);
    this->ui->layers_path_lineedit->setVisible(false);
    this->ui->layers_browse_pushButton->setVisible(false);
    this->ui->layers_progress->setVisible(true);

    const QString selected_path =
        QFileDialog::getExistingDirectory(this->ui->layers_browse_pushButton, "Select Layers Manifest Folder...",
                                          configurator.layers.last_layers_path.AbsolutePath().c_str());

    this->LoadLayersManifest(selected_path);
}

void TabLayers::on_layers_add_pushButton_pressed() {
    this->ui->layers_paths_view_comboBox->setVisible(false);
    this->ui->layers_path_lineedit->setVisible(false);
    this->ui->layers_browse_pushButton->setVisible(false);
    this->ui->layers_progress->setVisible(true);

    this->LoadLayersManifest(this->ui->layers_path_lineedit->text());
}

void TabLayers::on_layers_paths_view_comboBox_currentIndexChanged(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.layers.paths_view = static_cast<LayersPathViewType>(index);

    this->UpdateUI_LayersPaths(UPDATE_REBUILD_UI);
}

void TabLayers::LoadLayersManifest(const QString &selected_path) {
    Configurator &configurator = Configurator::Get();

    if (!selected_path.isEmpty()) {
        configurator.layers.last_layers_path = selected_path.toStdString();
        this->ui->layers_path_lineedit->setText(configurator.layers.last_layers_path.AbsolutePath().c_str());

        LayersPathInfo info;
        info.path = selected_path.toStdString();

        const std::vector<Path> &layers_paths = CollectFilePaths(info.path);

        this->ui->layers_progress->setMaximum(layers_paths.size());
        this->ui->layers_progress->setValue(0);

        configurator.layers.AppendPath(info);

        for (std::size_t i = 0, n = layers_paths.size(); i < n; ++i) {
            this->ui->layers_progress->setFormat(
                format("Validating %s... - %d/%d", layers_paths[i].AbsolutePath().c_str(), i, layers_paths.size()).c_str());
            this->ui->layers_progress->setValue(i);
            configurator.layers.LoadLayer(layers_paths[i]);
            this->UpdateUI_LayersPaths(UPDATE_REBUILD_UI);
        }

        configurator.layers.UpdatePathEnabled(info);
        this->UpdateUI_LayersPaths(UPDATE_REBUILD_UI);

        this->ui->layers_progress->setFormat("Layers validated!");
        this->ui->layers_progress->setValue(layers_paths.size());
    }

    this->ui->layers_paths_view_comboBox->setVisible(true);
    this->ui->layers_path_lineedit->setVisible(true);
    this->ui->layers_browse_pushButton->setVisible(true);
    this->ui->layers_progress->setVisible(false);
}
