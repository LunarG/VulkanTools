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

class TreeWidgetItemParameter : public QTreeWidgetItem {
   public:
    TreeWidgetItemParameter(const char *layer_name) : widget(nullptr), layer_name(layer_name) { assert(layer_name != nullptr); }

    std::string layer_name;
    QWidget *widget;

   private:
    TreeWidgetItemParameter(const TreeWidgetItemParameter &) = delete;
    TreeWidgetItemParameter &operator=(const TreeWidgetItemParameter &) = delete;
};

TabLayers::TabLayers(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_LAYERS, window, ui) {}

TabLayers::~TabLayers() {}

void TabLayers::UpdateUI_LayersPaths(UpdateUIMode ui_update_mode) {
    Configurator &configurator = Configurator::Get();

    ui->layers_paths_tree->blockSignals(true);
    ui->layers_paths_tree->clear();

    for (std::size_t group_index = 0, group_count = configurator.layers.paths.size(); group_index < group_count; ++group_index) {
        const LayerType layer_type = ::GetLayerType(static_cast<LayersPaths>(group_index));

        const std::vector<Path> &paths_group = configurator.layers.paths[group_index];
        for (std::size_t i = 0, n = paths_group.size(); i < n; ++i) {
            TreeWidgetItemParameter *item_state = new TreeWidgetItemParameter(paths_group[i].RelativePath().c_str());

            item_state->setFlags(item_state->flags() | Qt::ItemIsSelectable);
            item_state->setSizeHint(0, QSize(0, ITEM_HEIGHT));
            LayersPathWidget *layer_path_widget = new LayersPathWidget(paths_group[i].RelativePath());

            layer_path_widget->setChecked(true);

            item_state->widget = layer_path_widget;

            ui->layers_paths_tree->addTopLevelItem(item_state);
            ui->layers_paths_tree->setItemWidget(item_state, 0, layer_path_widget);

            const std::string &layer_path = paths_group[i].AbsolutePath();
            const std::vector<Path> &manifest_paths = CollectFilePaths(layer_path);

            for (std::size_t manifest_index = 0, manifest_count = manifest_paths.size(); manifest_index < manifest_count;
                 ++manifest_index) {
                const Layer *layer = configurator.layers.FindFromManifest(manifest_paths[manifest_index]);

                QTreeWidgetItem *item = new QTreeWidgetItem;

                std::string label =
                    layer->key + " - " + layer->api_version.str() + " - " + format("%s layer", GetToken(layer->type));
                if (layer->status != STATUS_STABLE) {
                    label += format(" (%s)", GetToken(layer->status));
                }
                item->setText(0, label.c_str());
                item->setToolTip(0, layer->manifest_path.AbsolutePath().c_str());
                item_state->addChild(item);
            }
        }
    }

    ui->layers_paths_tree->expandAll();
    ui->layers_paths_tree->blockSignals(false);
}

void TabLayers::UpdateUI(UpdateUIMode ui_update_mode) { this->UpdateUI_LayersPaths(ui_update_mode); }

void TabLayers::CleanUI() {}

bool TabLayers::EventFilter(QObject *target, QEvent *event) { return false; }
