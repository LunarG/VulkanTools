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
#include "../vkconfig_core/type_hide_message.h"
#include "../vkconfig_core/is_dll_32.h"

#include <QFileDialog>
#include <QMessageBox>

TabLayers::TabLayers(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_LAYERS, window, ui) {
    Configurator &configurator = Configurator::Get();

    this->ui->layers_browse_button->setText("");
    this->ui->layers_browse_button->setIcon(QIcon(":/resourcefiles/folder_append.png"));
    this->ui->layers_reload_button->setText("");
    this->ui->layers_reload_button->setIcon(QIcon(":/resourcefiles/reload.png"));
    this->ui->layers_progress->setValue(0);
    this->ui->layers_progress->setVisible(false);
    this->ui->layers_path_lineedit->setVisible(true);
    this->ui->layers_path_lineedit->setText(configurator.layers.last_layers_path.RelativePath().c_str());
    this->ui->layers_validate_checkBox->setChecked(configurator.layers.validate_manifests);

    this->connect(this->ui->layers_browse_button, SIGNAL(pressed()), this, SLOT(on_layers_browse_pressed()));
    this->connect(this->ui->layers_reload_button, SIGNAL(pressed()), this, SLOT(on_layers_reload_pressed()));
    this->connect(this->ui->layers_path_lineedit, SIGNAL(returnPressed()), this, SLOT(on_layers_append_pressed()));
    this->connect(this->ui->layers_validate_checkBox, SIGNAL(toggled(bool)), this, SLOT(on_layers_validate_checkBox_toggled(bool)));
}

TabLayers::~TabLayers() {}

void TabLayers::UpdateUI_LayersPaths(UpdateUIMode ui_update_mode) {
    Configurator &configurator = Configurator::Get();

    this->ui->layers_paths_tree->blockSignals(true);
    this->ui->layers_paths_tree->clear();

    for (std::size_t group_index = configurator.layers.paths.size(); group_index > 0; --group_index) {
        const LayersPaths group_path = static_cast<LayersPaths>(group_index - 1);

        std::vector<LayersPathInfo> &paths_group = configurator.layers.paths[group_path];
        for (std::size_t i = 0, n = paths_group.size(); i < n; ++i) {
            QTreeWidgetItem *item_state = new QTreeWidgetItem;
            item_state->setFlags(item_state->flags() | Qt::ItemIsSelectable);
            item_state->setSizeHint(0, QSize(0, 32));
            LayersPathWidget *layer_path_widget = new LayersPathWidget(&paths_group[i], group_path);
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

                std::string label = layer->key;

                if (layer->is_32bits) {
                    label += " (32 bits)";
                }

                label += " - " + layer->api_version.str();

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

void TabLayers::on_layers_validate_checkBox_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();
    configurator.layers.validate_manifests = checked;
}

void TabLayers::on_layers_append_pressed() {
    this->ui->layers_path_lineedit->setVisible(false);
    this->ui->layers_browse_button->setVisible(false);
    this->ui->layers_progress->setVisible(true);

    this->LoadLayersManifest(this->ui->layers_path_lineedit->text());
}

void TabLayers::on_layers_browse_pressed() {
    Configurator &configurator = Configurator::Get();

    this->ui->layers_path_lineedit->setVisible(false);
    this->ui->layers_browse_button->setVisible(false);
    this->ui->layers_progress->setVisible(true);

    const QString selected_path =
        QFileDialog::getExistingDirectory(this->ui->layers_browse_button, "Select Layer Manifests Folder...",
                                          configurator.layers.last_layers_path.AbsolutePath().c_str());

    this->LoadLayersManifest(selected_path);
}

void TabLayers::on_layers_reload_pressed() {}

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

        int loaded = 0;

        for (std::size_t i = 0, n = layers_paths.size(); i < n; ++i) {
            const char *label = configurator.layers.validate_manifests ? "Validating" : "Loading";
            this->ui->layers_progress->setFormat(
                format("%s %s... - %d/%d", label, layers_paths[i].AbsolutePath().c_str(), i, layers_paths.size()).c_str());
            this->ui->layers_progress->setValue(i);

            if (configurator.layers.LoadLayer(layers_paths[i])) {
                ++loaded;
            }
            this->UpdateUI_LayersPaths(UPDATE_REBUILD_UI);
        }

        configurator.layers.UpdatePathEnabled(info);
        this->UpdateUI_LayersPaths(UPDATE_REBUILD_UI);

        this->ui->layers_progress->setValue(layers_paths.size());

        if (loaded > 0) {
            const char *label_result = configurator.layers.validate_manifests ? "validated" : "loaded or reloaded";
            const std::string text = format("%d layers %s!", loaded, label_result);
            this->ui->layers_progress->setFormat(text.c_str());

            if (!configurator.Get(HIDE_MESSAGE_NOTIFICATION_LAYERS_LOADED)) {
                const std::string text =
                    format("%d layers %s from '%s' folder.", loaded, label_result, info.path.AbsolutePath().c_str());

                QMessageBox message;
                message.setIcon(QMessageBox::Information);
                message.setWindowTitle("Loading Layers Successful!");
                message.setText(text.c_str());
                message.setCheckBox(new QCheckBox("Do not show again."));
                message.exec();
                if (message.checkBox()->isChecked()) {
                    configurator.Set(HIDE_MESSAGE_NOTIFICATION_LAYERS_LOADED);
                }
            }
        } else {
            const std::string text = "No Layer Found...";
            this->ui->layers_progress->setFormat(text.c_str());

            if (!configurator.Get(HIDE_MESSAGE_WARN_NO_LAYER_FOUND)) {
                QMessageBox message;
                message.setIcon(QMessageBox::Warning);
                message.setWindowTitle(text.c_str());
                message.setText(format("No layer found in '%s' folder.", info.path.AbsolutePath().c_str()).c_str());
                message.setCheckBox(new QCheckBox("Do not show again."));
                message.exec();
                if (message.checkBox()->isChecked()) {
                    configurator.Set(HIDE_MESSAGE_WARN_NO_LAYER_FOUND);
                }
            }
        }
    }

    this->ui->layers_path_lineedit->setVisible(true);
    this->ui->layers_browse_button->setVisible(true);
    this->ui->layers_progress->setVisible(false);
}